#ifndef INTERVALS_SETINTERVALWRAPPER_H
#define INTERVALS_SETINTERVALWRAPPER_H
#include <string>
#include "../../intervals_c/headers/set_interval.h"
#include "IntervalWrapper.h"
using namespace std;

class SetIntervalWrapper {
private:
    SetInterval* setInterval{};

public:
    SetIntervalWrapper();

    explicit SetIntervalWrapper(SetInterval *pInterval);

    SetInterval* getSetInterval();

    void addInterval(IntervalWrapper* interval);

    string toString();
};


#endif //INTERVALS_SETINTERVALWRAPPER_H
