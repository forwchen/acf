//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include "imResampleMex.hpp"
//#include "rgbConvertMex.hpp"
#include "LoadChannels.hpp"
#include <cstdlib>

// Constants for rgb2luv conversion and lookup table for y-> l conversion
template <class oT>
oT* rgb2luv_setupOld(oT z, oT* mr, oT* mg, oT* mb,
    oT& minu, oT& minv, oT& un, oT& vn)
{
    // set constants for conversion
    const oT y0 = (oT)((6.0 / 29) * (6.0 / 29) * (6.0 / 29));
    const oT a = (oT)((29.0 / 3) * (29.0 / 3) * (29.0 / 3));
    un = (oT)0.197833;
    vn = (oT)0.468331;
    mr[0] = (oT)0.430574 * z;
    mr[1] = (oT)0.222015 * z;
    mr[2] = (oT)0.020183 * z;
    mg[0] = (oT)0.341550 * z;
    mg[1] = (oT)0.706655 * z;
    mg[2] = (oT)0.129553 * z;
    mb[0] = (oT)0.178325 * z;
    mb[1] = (oT)0.071330 * z;
    mb[2] = (oT)0.939180 * z;
    oT maxi = (oT)1.0 / 270;
    minu = -88 * maxi;
    minv = -134 * maxi;
    // build (padded) lookup table for y->l conversion assuming y in [0,1]
    static oT lTable[1064];
    static bool lInit = false;
    if (lInit)
        return lTable;
    oT y, l;
    for (int i = 0; i < 1025; i++) {
        y = (oT)(i / 1024.0);
        l = y > y0 ? 116 * (oT)pow((double)y, 1.0 / 3.0) - 16 : y * a;
        lTable[i] = l * maxi;
    }
    for (int i = 1025; i < 1064; i++)
        lTable[i] = lTable[i - 1];
    lInit = true;
    return lTable;
}

// Convert from rgb to luv
template <class iT, class oT>
void rgb2luvOld(iT* I, oT* J, int n, oT nrm)
{
    oT minu, minv, un, vn, mr[3], mg[3], mb[3];
    oT* lTable = rgb2luv_setupOld(nrm, mr, mg, mb, minu, minv, un, vn);
    oT *L = J, *U = L + n, *V = U + n;
    iT *R = I, *G = R + n, *B = G + n;
    for (int i = 0; i < n; i++) {
        oT r, g, b, x, y, z, l;
        r = (oT)*R++;
        g = (oT)*G++;
        b = (oT)*B++;
        x = mr[0] * r + mg[0] * g + mb[0] * b;
        y = mr[1] * r + mg[1] * g + mb[1] * b;
        z = mr[2] * r + mg[2] * g + mb[2] * b;
        l = lTable[(int)(y * 1024)];
        *(L++) = l;
        z = 1 / (x + 15 * y + 3 * z + (oT)1e-35);
        *(U++) = l * (13 * 4 * x * z - 13 * un) - minu;
        *(V++) = l * (13 * 9 * y * z - 13 * vn) - minv;
    }
}

// Convert from rgb to luv using sse
template <class iT>
void rgb2luv_sseOld(iT* I, float* J, int n, float nrm)
{
    const int k = 256;
    float R[k], G[k], B[k];
    if ((size_t(R) & 15 || size_t(G) & 15 || size_t(B) & 15 || size_t(I) & 15 || size_t(J) & 15)
        || n % 4 > 0) {
        rgb2luvOld(I, J, n, nrm);
        return;
    }
    int i = 0, i1, n1;
    float minu, minv, un, vn, mr[3], mg[3], mb[3];
    float* lTable = rgb2luv_setupOld(nrm, mr, mg, mb, minu, minv, un, vn);
    while (i < n) {
        n1 = i + k;
        if (n1 > n)
            n1 = n;
        float* J1 = J + i;
        float *R1, *G1, *B1;
        // convert to floats (and load input into cache)
        if (typeid(iT) != typeid(float)) {
            R1 = R;
            G1 = G;
            B1 = B;
            iT *Ri = I + i, *Gi = Ri + n, *Bi = Gi + n;
            for (i1 = 0; i1 < (n1 - i); i1++) {
                R1[i1] = (float)*Ri++;
                G1[i1] = (float)*Gi++;
                B1[i1] = (float)*Bi++;
            }
        }
        else {
            R1 = ((float*)I) + i;
            G1 = R1 + n;
            B1 = G1 + n;
        }
        // compute RGB -> XYZ
        for (int j = 0; j < 3; j++) {
            __m128 _mr, _mg, _mb, *_J = (__m128 *)(J1 + j * n);
            __m128 *_R = (__m128 *)R1, *_G = (__m128 *)G1, *_B = (__m128 *)B1;
            _mr = SET(mr[j]);
            _mg = SET(mg[j]);
            _mb = SET(mb[j]);
            for (i1 = i; i1 < n1; i1 += 4)
                *(_J++) = ADD(ADD(MUL(*(_R++), _mr),
                                  MUL(*(_G++), _mg)),
                    MUL(*(_B++), _mb));
        }
        { // compute XZY -> LUV (without doing L lookup/normalization)
            __m128 _c15, _c3, _cEps, _c52, _c117, _c1024, _cun, _cvn;
            _c15 = SET(15.0f);
            _c3 = SET(3.0f);
            _cEps = SET(1e-35f);
            _c52 = SET(52.0f);
            _c117 = SET(117.0f), _c1024 = SET(1024.0f);
            _cun = SET(13 * un);
            _cvn = SET(13 * vn);
            __m128 *_X, *_Y, *_Z, _x, _y, _z;
            _X = (__m128*)J1;
            _Y = (__m128*)(J1 + n);
            _Z = (__m128*)(J1 + 2 * n);
            for (i1 = i; i1 < n1; i1 += 4) {
                _x = *_X;
                _y = *_Y;
                _z = *_Z;
                _z = RCP(ADD(_x, ADD(_cEps, ADD(MUL(_c15, _y), MUL(_c3, _z)))));
                *(_X++) = MUL(_c1024, _y);
                *(_Y++) = SUB(MUL(MUL(_c52, _x), _z), _cun);
                *(_Z++) = SUB(MUL(MUL(_c117, _y), _z), _cvn);
            }
        }
        { // perform lookup for L and finalize computation of U and V
            for (i1 = i; i1 < n1; i1++)
                J[i1] = lTable[(int)J[i1]];
            __m128 *_L, *_U, *_V, _l, _cminu, _cminv;
            _L = (__m128*)J1;
            _U = (__m128*)(J1 + n);
            _V = (__m128*)(J1 + 2 * n);
            _cminu = SET(minu);
            _cminv = SET(minv);
            for (i1 = i; i1 < n1; i1 += 4) {
                _l = *(_L++);
                *(_U++) = SUB(MUL(_l, *_U), _cminu);
                *(_V++) = SUB(MUL(_l, *_V), _cminv);
            }
        }
        i = n1;
    }
}

// Convert from rgb to hsv
template <class iT, class oT>
void rgb2hsvOld(iT* I, oT* J, int n, oT nrm)
{
    oT *H = J, *S = H + n, *V = S + n;
    iT *R = I, *G = R + n, *B = G + n;
    for (int i = 0; i < n; i++) {
        const oT r = (oT) * (R++), g = (oT) * (G++), b = (oT) * (B++);
        oT h, s, v, minv, maxv;
        if (r == g && g == b) {
            *(H++) = 0;
            *(S++) = 0;
            *(V++) = r * nrm;
            continue;
        }
        else if (r >= g && r >= b) {
            maxv = r;
            minv = g < b ? g : b;
            h = (g - b) / (maxv - minv) + 6;
            if (h >= 6)
                h -= 6;
        }
        else if (g >= r && g >= b) {
            maxv = g;
            minv = r < b ? r : b;
            h = (b - r) / (maxv - minv) + 2;
        }
        else {
            maxv = b;
            minv = r < g ? r : g;
            h = (r - g) / (maxv - minv) + 4;
        }
        h *= (oT)(1 / 6.0);
        s = 1 - minv / maxv;
        v = maxv * nrm;
        *(H++) = h;
        *(S++) = s;
        *(V++) = v;
    }
}

// Convert from rgb to gray
template <class iT, class oT>
void rgb2grayOld(iT* I, oT* J, int n, oT nrm)
{
    oT* GR = J;
    iT *R = I, *G = R + n, *B = G + n;
    int i;
    oT mr = (oT).2989360213 * nrm, mg = (oT).5870430745 * nrm, mb = (oT).1140209043 * nrm;
    for (i = 0; i < n; i++)
        *(GR++) = (oT) * (R++) * mr + (oT) * (G++) * mg + (oT) * (B++) * mb;
}

// Convert from rgb (double) to gray (float)
template <>
void rgb2grayOld(double* I, float* J, int n, float nrm)
{
    float* GR = J;
    double *R = I, *G = R + n, *B = G + n;
    int i;
    double mr = .2989360213 * nrm, mg = .5870430745 * nrm, mb = .1140209043 * nrm;
    for (i = 0; i < n; i++)
        *(GR++) = (float)(*(R++) * mr + *(G++) * mg + *(B++) * mb);
}

// Copy and normalize only
template <class iT, class oT>
void normalizeOld(iT* I, oT* J, int n, oT nrm)
{
    for (int i = 0; i < n; i++)
        *(J++) = (oT) * (I++) * nrm;
}

// Convert rgb to various colorspaces
template <class iT, class oT>
oT* rgbConvertOld(iT* I, int n, int d, int flag, oT nrm)
{
    oT* J = (oT*)wrMalloc(n * (flag == 0 ? (d == 1 ? 1 : d / 3) : d) * sizeof(oT));
    int i, n1 = d * (n < 1000 ? n / 10 : 100);
    oT thr = oT(1.001);
    if (flag > 1 && nrm == 1)
        for (i = 0; i < n1; i++)
            if (I[i] > thr)
                wrError("For floats all values in I must be smaller than 1.");
    bool useSse = n % 4 == 0 && typeid(oT) == typeid(float);
    if (flag == 2 && useSse)
        for (i = 0; i < d / 3; i++)
            rgb2luv_sseOld(I + i * n * 3, (float*)(J + i * n * 3), n, (float)nrm);
    else if ((flag == 0 && d == 1) || flag == 1)
        normalizeOld(I, J, n * d, nrm);
    else if (flag == 0)
        for (i = 0; i < d / 3; i++)
            rgb2grayOld(I + i * n * 3, J + i * n * 1, n, nrm);
    else if (flag == 2)
        for (i = 0; i < d / 3; i++)
            rgb2luvOld(I + i * n * 3, J + i * n * 3, n, nrm);
    else if (flag == 3)
        for (i = 0; i < d / 3; i++)
            rgb2hsvOld(I + i * n * 3, J + i * n * 3, n, nrm);
    else
        wrError("Unknown flag.");
    return J;
}

/*!
	Calculate the LUV-features of an image (convertion of image channels). Based on P. Dollar's framework
*/

void LUVChannel::getFeatOfChannel(const cv::Mat& image, cv::vector<cv::Mat>& Feat, cv::vector<cv::Mat>& FeatOriginal, int shrinking = 1)
{
    cv::Mat dst;

    if (image.channels() != 3) {
        std::cerr << "Image should have 3 channels" << image.channels() << std::endl;
        exit(2);
    }

    float* I0 = Convert_ToMatlab(image, 1.0);
    int h1 = image.rows;
    int w1 = image.cols;
    int d = image.channels();
    int rad = 1;
    int sf = sizeof(float);
    float* I1 = (float*)wrCalloc(h1 * w1 * d, sf);
    float* Shrinked = (float*)wrCalloc(h1 * w1 * d, sf);
    float* ShrinkedN = (float*)wrCalloc(h1 * w1 * d, sf);

    float* I2 = rgbConvertOld(I0, h1 * w1, d, 2, 1.0f);
    convTri1(I2, I1, h1, w1, d, 2, 1);

    std::vector<cv::Mat> ch;

    for (int C = 0; C < this->getChannelAmount(); C++)
        ch.push_back(Convert_ToOCV(I1 + C * h1 * w1, 1, w1, h1, 255.0));

    cv::merge(ch, dst);

    FeatOriginal.push_back(dst);

    /*Shrinking*/
    if (shrinking != 1) {

        int h2 = h1 / shrinking;
        int w2 = w1 / shrinking;

        resample(I1, Shrinked, h1, h2, w1, w2, d, 1.0f);
        convTri1(Shrinked, ShrinkedN, h2, w2, d, 2, 1);
        cv::Mat Shrinked_Image;
        std::vector<cv::Mat> ch2;

        for (int C = 0; C < this->getChannelAmount(); C++)
            ch2.push_back(Convert_ToOCV(ShrinkedN + C * h2 * w2, 1, w2, h2, 255.0));

        cv::merge(ch2, Shrinked_Image);

        Feat.push_back(Shrinked_Image);
    }
    else {
        Feat.push_back(dst);
    }

    free(Shrinked);
    free(ShrinkedN);
    free(I0);
    free(I1);
    free(I2);
}

/*!
	Calculate the Gray-features of an image (convertion of image channels). Based on P. Dollar's framework
*/
void GrayChannel::getFeatOfChannel(const cv::Mat& image, cv::vector<cv::Mat>& Feat, cv::vector<cv::Mat>& FeatOriginal, int shrinking = 1)
{
    cv::Mat dst;

    float* I0 = Convert_ToMatlab(image, 1.0);
    int h1 = image.rows;
    int w1 = image.cols;
    int d = image.channels();
    int rad = 2;
    int sf = sizeof(float);
    float* I1 = (float*)wrCalloc(h1 * w1 * d, sf);
    float* I2 = rgbConvertOld(I0, h1 * w1, d, 0, 1.0f);
    convTri(I2, I1, h1, w1, 1, rad, 1);

    std::vector<cv::Mat> ch;
    for (int C = 0; C < this->getChannelAmount(); C++)
        ch.push_back(Convert_ToOCV(I1 + C * h1 * w1, 1, w1, h1, 1.0));
    cv::merge(ch, dst);
    FeatOriginal.push_back(dst);

    /*Shrinking*/
    if (shrinking != 1) {
        float* Shrinked = (float*)wrCalloc(h1 * w1 * d, sf);
        cv::Mat Shrinked_Image;
        int h2 = h1 / shrinking;
        int w2 = w1 / shrinking;

        resample(I1, Shrinked, h1, h2, w1, w2, d, 1.0f);

        std::vector<cv::Mat> ch2;

        for (int C = 0; C < this->getChannelAmount(); C++)
            ch2.push_back(Convert_ToOCV(Shrinked + C * h2 * w2, 1, w2, h2, 255.0));

        cv::merge(ch2, Shrinked_Image);
        Feat.push_back(Shrinked_Image);
        free(Shrinked);
    }
    else {

        Feat.push_back(dst);
    }

    free(I0);
    free(I1);
    free(I2);
}

/*!
	Calculate the HSV-features of an image (convertion of image channels). Based on P. Dollar's framework
*/

void HSVChannel::getFeatOfChannel(const cv::Mat& image, cv::vector<cv::Mat>& Feat, cv::vector<cv::Mat>& FeatOriginal, int shrinking = 1)
{
    //Channel::getFeatOfChannel(image,Feat);

    cv::Mat dst;

    float* I0 = Convert_ToMatlab(image, 1.0);
    int h1 = image.rows;
    int w1 = image.cols;
    int d = image.channels();
    int rad = 2;
    int sf = sizeof(float);
    float* I1 = (float*)wrCalloc(h1 * w1 * d, sf);
    float* I2 = rgbConvertOld(I0, h1 * w1, d, 3, 1.0f);
    convTri1(I2, I1, h1, w1, d, rad, 1);

    std::vector<cv::Mat> ch;

    for (int C = 0; C < this->getChannelAmount(); C++)
        ch.push_back(Convert_ToOCV(I1 + C * h1 * w1, 1, w1, h1, 1.0));

    cv::merge(ch, dst);

    FeatOriginal.push_back(dst);

    /*Shrinking*/
    if (shrinking != 1) {
        float* Shrinked = (float*)wrCalloc(h1 * w1 * d, sf);
        cv::Mat Shrinked_Image;
        int h2 = h1 / shrinking;
        int w2 = w1 / shrinking;

        resample(I1, Shrinked, h1, h2, w1, w2, d, 1.0f);

        std::vector<cv::Mat> ch2;

        for (int C = 0; C < this->getChannelAmount(); C++)
            ch2.push_back(Convert_ToOCV(Shrinked + C * h2 * w2, 1, w2, h2, 255.0));

        cv::merge(ch2, Shrinked_Image);
        Feat.push_back(Shrinked_Image);
        free(Shrinked);
    }
    else {

        Feat.push_back(dst);
    }

    free(I0);
    free(I1);
    free(I2);
}

/*!
	calculate the features based on the gradient maginude of an image. Hereby an earlier channel (LUV) is used.
*/

void GradMagChannelOld::getFeatOfChannel(const cv::Mat& image, cv::vector<cv::Mat>& Feat, cv::vector<cv::Mat>& FeatOriginal, int shrinking = 1)
{
    cv::Mat dst;
    cv::Mat II = FeatOriginal[this->m_dependencies_index[0]];

    float* I0 = Convert_ToMatlab(II, 1.0);

    int h1 = image.rows;
    int w1 = image.cols;
    int d = image.channels();
    int sf = sizeof(float);
    float normConst = 0.0050;

    float* M = (float*)wrCalloc(h1 * w1 * d, sf);
    float* M_smooth = (float*)wrCalloc(h1 * w1 * d, sf);
    //float *M_Norm  = (float*) wrCalloc(h1*w1,sf);
    float* O = (float*)wrCalloc(h1 * w1 * d, sf);

    gradMag(I0, M, O, h1, w1, d, false);

    convTri(M, M_smooth, h1, w1, d, 5, 1);

    gradMagNorm(M, M_smooth, h1, w1, 0.0050);

    std::vector<cv::Mat> ch;

    //for(int C=0;C<this->Amount();C++)
    ch.push_back(Convert_ToOCV(M, 1, w1, h1, 1.0));
    ch.push_back(Convert_ToOCV(O, 1, w1, h1, 1.0));

    cv::merge(ch, dst);
    FeatOriginal.push_back(dst);

    /*Shrinking*/
    if (shrinking != 1) {
        float* ShrinkedM = (float*)wrCalloc(h1 * w1 * d, sf);
        float* ShrinkedO = (float*)wrCalloc(h1 * w1 * d, sf);

        float* ShrinkedMN = (float*)wrCalloc(h1 * w1 * d, sf);
        float* ShrinkedON = (float*)wrCalloc(h1 * w1 * d, sf);

        cv::Mat Shrinked_Image;
        int h2 = h1 / shrinking;
        int w2 = w1 / shrinking;

        resample(M, ShrinkedM, h1, h2, w1, w2, d, 1.0f);
        resample(O, ShrinkedO, h1, h2, w1, w2, d, 1.0f);

        convTri1(ShrinkedM, ShrinkedMN, h2, w2, d, 2, 1);
        convTri1(ShrinkedO, ShrinkedON, h2, w2, d, 2, 1);

        std::vector<cv::Mat> ch2;

        ch2.push_back(Convert_ToOCV(ShrinkedMN, 1, w2, h2, 1.0));
        ch2.push_back(Convert_ToOCV(ShrinkedON, 1, w2, h2, 1.0));

        cv::merge(ch2, Shrinked_Image);
        Feat.push_back(Shrinked_Image);

        free(ShrinkedM);
        free(ShrinkedO);

        free(ShrinkedMN);
        free(ShrinkedON);
    }
    else {

        Feat.push_back(dst);
    }

    free(I0);
    free(M);
    free(M_smooth);
    free(O);
}

/*!
	Calculate the HOG-features of an image. Hereby the gradient magnitude channels are used so should be calculated in advance. Based on P. Dollar's framework
*/
void HogChannel::getFeatOfChannel(const cv::Mat& image, cv::vector<cv::Mat>& Feat, cv::vector<cv::Mat>& FeatOriginal, int shrinking = 1)
{
    //shrinking = 1;

    cv::Mat dst;
    cv::Mat II = FeatOriginal[this->m_dependencies_index[0]];

    cv::vector<cv::Mat> F;
    cv::split(II, F);
    cv::Mat M = F[0];
    cv::Mat O = F[1];

    /* floor to multiple of shrinking*/
    int NWidth = (image.cols / shrinking) * shrinking;
    int NHeight = (image.rows / shrinking) * shrinking;

    int h1 = NHeight;
    int w1 = NWidth;

    cv::Mat NM = M(cv::Rect(0, 0, NWidth, NHeight));
    cv::Mat NO = O(cv::Rect(0, 0, NWidth, NHeight));

    int h2 = h1 / shrinking;
    int w2 = w1 / shrinking;

    int d = 1; //image.channels();
    int sf = sizeof(float);

    float* M0 = Convert_ToMatlab(NM, 1.0);
    float* O0 = Convert_ToMatlab(NO, 1.0);

    float* H = (float*)wrCalloc(h2 * w2 * d * 6, sf);
    float* HN = (float*)wrCalloc(h2 * w2 * d * 6, sf);
    gradHist(M0, O0, H, h1, w1, shrinking, 6, 0, false);
    convTri1(H, HN, h2, w2, 6, 2, 1);

    std::vector<cv::Mat> ch;
    for (int C = 0; C < this->getChannelAmount(); C++)
        ch.push_back(Convert_ToOCV(HN + C * h2 * w2, 1, w2, h2, 1.0));

    cv::merge(ch, dst);

    FeatOriginal.push_back(dst);

    /*Shrinking*/
    if (shrinking != 1) {
        cv::Mat Shrinked_Image;
        Feat.push_back(dst);
    }
    else {
        Feat.push_back(dst);
    }

    free(M0);
    free(O0);
    free(H);
    free(HN);
}
