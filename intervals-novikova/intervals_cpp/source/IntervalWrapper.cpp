#include "../headers/IntervalWrapper.h"
using namespace std;

IntervalWrapper::IntervalWrapper(Interval *pInterval) {
    interval = pInterval;
}

IntervalWrapper::IntervalWrapper(Interval_type left, Interval_type right, double a, double b) {
    interval = new Interval();
    interval->left = left;
    interval->right = right;
    interval->a = a;
    interval->b = b;
}

std::string IntervalWrapper::toString() {
    if (interval == NULL) {
        return "\u2205";
    }
    bool intervalClosed = interval->left == closed && interval->right == closed;
    if (interval->a == interval->b && !intervalClosed) {
        return "\u2205";
    }
    if (interval->a == interval->b && intervalClosed) {
        return to_string(interval->a);
    }
    return (interval->left == open ? "(" : "[") + to_string(interval->a) + ";" + to_string(interval->b) + (interval->right == open ? ")" : "]");
}
