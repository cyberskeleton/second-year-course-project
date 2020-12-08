#ifndef INTERVALS_INTERVALWRAPPER_H
#define INTERVALS_INTERVALWRAPPER_H
#include <string>
#include <ostream>
#include "../intervals_c/interval.h"

class IntervalWrapper {
private:
    Interval *interval{};
public:
    explicit IntervalWrapper(Interval *interval);

    IntervalWrapper(Interval_type left, Interval_type right, double a, double b);

    Interval *getInterval() {
        return interval;
    };

    std::string toString();

    friend std::ostream &operator<<(std::ostream &os, const IntervalWrapper &wrapper);
};

#endif //INTERVALS_INTERVALWRAPPER_H
