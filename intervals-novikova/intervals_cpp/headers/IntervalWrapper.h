#ifndef INTERVALS_INTERVALWRAPPER_H
#define INTERVALS_INTERVALWRAPPER_H
#include <string>
#include <ostream>
#include "../../intervals_c/headers/interval.h"
using namespace std;

class IntervalWrapper {
private:
    Interval *interval{};
public:
    explicit IntervalWrapper(Interval *interval);

    IntervalWrapper(Interval_type left, Interval_type right, double a, double b);

    Interval *getInterval() {
        return interval;
    };

    string toString();
};

#endif //INTERVALS_INTERVALWRAPPER_H
