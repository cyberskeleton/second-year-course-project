#include "cross_tests.h"

using namespace std;

void cross_tests::run_cpp_tests() {
    auto *one = new IntervalWrapper(open, closed, -INFINITY, 15.0);
    auto *two = new IntervalWrapper(closed, open, -1000, 15.0);
    auto *three = new IntervalWrapper(open, open, 15.0, INFINITY);

    int byStart = compare_start(one->getInterval(), two->getInterval());
    cout << "Comparing left margin: " + one->toString() + ((byStart == -1) ? " < " : (byStart == 1) ? " > " : " == ") +
            two->toString() << endl;

    int byEnd = compare_end(one->getInterval(), three->getInterval());
    cout << "Comparing right margin: " + one->toString() + ((byEnd == -1) ? " < " : (byEnd == 1) ? " > " : " == ") +
            three->toString() << endl;

    int startToEnd = compare_start_to_end(three->getInterval(), two->getInterval());
    cout << "Comparing first start to second end: " + three->toString() +
            ((startToEnd == -1) ? " < " : (startToEnd == 1) ? " > " : " == ") + two->toString() << endl;

    auto *difference = new SetIntervalWrapper(get_difference(one->getInterval(), two->getInterval()));
    cout << one->toString() + " \\ " + two->toString() + " = " + difference->toString() << endl;

    auto *source = new SetIntervalWrapper();
    source->addInterval(one);
    source->addInterval(two);
    source->addInterval(three);

    auto *unionSet = new SetIntervalWrapper(get_union(source->getSetInterval()));
    cout << one->toString() + " \u222A " + two->toString() + " \u222A " + three->toString() + " = " +
            unionSet->toString() << endl;

    auto *intersection = new IntervalWrapper(get_intersection(source->getSetInterval()));
    cout << one->toString() + " \xE2\x88\xA9 " + two->toString() + " \u2229 " + three->toString() + " = " +
            intersection->toString() << endl;
}
