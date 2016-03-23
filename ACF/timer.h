#include "chrono"
using namespace std::chrono;

class Timer {
    high_resolution_clock::time_point t1, t2;
    duration<double> time_span;

public:
    Timer() {}
    void start()
    {
        t1 = high_resolution_clock::now();
    }

    double count()
    {
        t2 = high_resolution_clock::now();
        time_span = duration_cast<duration<double> >(t2 - t1);
        return time_span.count();
    }

    ~Timer() {}
};