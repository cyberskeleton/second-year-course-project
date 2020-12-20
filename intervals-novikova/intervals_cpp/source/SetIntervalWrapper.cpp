#include "../headers/SetIntervalWrapper.h"
using namespace std;

SetIntervalWrapper::SetIntervalWrapper() {
    setInterval = init();
}

SetIntervalWrapper::SetIntervalWrapper(SetInterval *pInterval) {
    setInterval = pInterval;
}

SetInterval* SetIntervalWrapper::getSetInterval() {
    return setInterval;
}

IntervalWrapper* SetIntervalWrapper::getIntervalWrapper(int index) {
    auto* interval = setInterval->in_array[index];
    return new IntervalWrapper(interval->left, interval->right, interval->a, interval->b);
}

void SetIntervalWrapper::copyIntervals(SetIntervalWrapper* source) {
    for (int i = 0; i < source->getSetInterval()->counter; ++i) {
        this->addInterval(source->getIntervalWrapper(i));
    }
}

void SetIntervalWrapper::addInterval(IntervalWrapper *interval) {
    if (interval == nullptr || interval->getInterval() == nullptr) {
        return;
    }
    add(setInterval, interval->getInterval()->left, interval->getInterval()->right, interval->getInterval()->a, interval->getInterval()->b);
}

string SetIntervalWrapper::toString() {
    string result = "{";
    for (int i = 0; i < setInterval->counter; ++i) {
        const Interval* interval = setInterval->in_array[i];
        if (i > 0) result += ", ";
        result += (new IntervalWrapper(interval->left, interval->right, interval->a, interval->b))->toString();
    }
    result += "}";

    return result;
}
