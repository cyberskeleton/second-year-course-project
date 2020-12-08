#include "../headers/SetIntervalWrapper.h"

SetIntervalWrapper::SetIntervalWrapper() {
    setInterval = init();
}

SetIntervalWrapper::SetIntervalWrapper(SetInterval *pInterval) {
    setInterval = pInterval;
}

SetInterval* SetIntervalWrapper::getSetInterval() {
    return setInterval;
}

void SetIntervalWrapper::addInterval(IntervalWrapper *interval) {
    add(setInterval, interval->getInterval()->left, interval->getInterval()->right, interval->getInterval()->a, interval->getInterval()->b);
}

std::string SetIntervalWrapper::toString() {
    std::string result = "{";
    for (int i = 0; i < setInterval->counter; ++i) {
        const Interval* interval = setInterval->in_array[i];
        if (i > 0) result += ", ";
        result += (new IntervalWrapper(interval->left, interval->right, interval->a, interval->b))->toString();
    }
    result += "}";

    return result;
}
