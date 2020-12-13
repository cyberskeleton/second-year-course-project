#include "../headers/cross_tests.h"

using namespace std;

void cross_tests::run_cpp_tests() {
    auto *one = new IntervalWrapper(open, closed, -INFINITY, 15.0);
    auto *two = new IntervalWrapper(closed, open, -1000, 15.0);
    auto *three = new IntervalWrapper(open, open, 15.0, INFINITY);

    int byStart = compare_start(one->getInterval(), two->getInterval());
    string byStartString = one->toString() + ((byStart == -1) ? " < " : (byStart == 1) ? " > " : " == ") + two->toString();
    cout << "Comparing left margin: " + byStartString << endl;
    assert(byStartString == "(-inf;15.000000] < [-1000.000000;15.000000)");

    int byEnd = compare_end(one->getInterval(), three->getInterval());
    string byEndString = one->toString() + ((byEnd == -1) ? " < " : (byEnd == 1) ? " > " : " == ") + three->toString();
    cout << "Comparing right margin: " + byEndString << endl;
    assert(byEndString == "(-inf;15.000000] < (15.000000;inf)");

    int startToEnd = compare_start_to_end(three->getInterval(), two->getInterval());
    string startToEndString = three->toString() + ((startToEnd == -1) ? " < " : (startToEnd == 1) ? " > " : " == ") + two->toString();
    cout << "Comparing firstCoefficient start to secondCoefficient end: " + startToEndString << endl;
    assert(startToEndString == "(15.000000;inf) == [-1000.000000;15.000000)");

    auto *source = new SetIntervalWrapper();
    source->addInterval(one);
    source->addInterval(two);
    source->addInterval(three);

    auto *difference = new SetIntervalWrapper(get_difference(one->getInterval(), two->getInterval()));
    string differenceString = one->toString() + " \\ " + two->toString() + " = " + difference->toString();
    cout << differenceString << endl;
    assert(differenceString == "(-inf;15.000000] \\ [-1000.000000;15.000000) = {(-inf;-1000.000000), 15.000000}");

    auto *unionSet = new SetIntervalWrapper(get_union(source->getSetInterval()));
    string unionSetString = one->toString() + " \u222A " + two->toString() + " \u222A " + three->toString() + " = " + unionSet->toString();
    cout << unionSet << endl;
    assert(unionSetString == "(-inf;15.000000] ∪ [-1000.000000;15.000000) ∪ (15.000000;inf) = {(-inf;inf)}");

    auto *intersection = new IntervalWrapper(get_intersection(source->getSetInterval()));
    string intersectionString = one->toString() + " \xE2\x88\xA9 " + two->toString() + " \u2229 " + three->toString() + " = " + intersection->toString();
    cout << intersectionString << endl;
    assert(intersectionString == "(-inf;15.000000] ∩ [-1000.000000;15.000000) ∩ (15.000000;inf) = ∅");
}
