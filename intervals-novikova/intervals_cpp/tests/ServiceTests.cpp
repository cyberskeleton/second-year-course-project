#include "../headers/ServiceTests.h"
using namespace std;

void ServiceTests::serviceTests() {
    auto* service = new Service();

    vector<vector<string>> data = service->readCSV("../data/test_do_not_modify.csv");
    for (const vector<string>& line : data) {
        cout << "System read from file:" << endl;
        for (const string & result : line) {
            cout << result << endl;
        }
    }
    vector<vector<Inequality>> in = service->parseInequalitySystems(data);
    vector<string> expectedStrings = vector<string>({"0.000000x^2+4.091000x+0.000000=0",
                                                     "3.200000x^2+4.660000x+20.019000<=0",
                                                     "1.000000x^2-1.000000x-6.330000>0",
                                                     "-1.000000x^2+0.000000x-2.990000>0"});
    vector<string> actualStrings;
    for (const vector<Inequality>& system : in) {
        auto *solutionSet = new SetIntervalWrapper();
        cout << "System parsed:" << endl;
        for (const Inequality& inequality : system) {
            string inq = inequality.toString();
            actualStrings.push_back(inq);
            cout << " Inequality: " + inq << ends;
            auto* toSolve = new Inequality();
            toSolve->copyFields(inequality);
            SetIntervalWrapper* solution = service->getInequalitySolution(toSolve);
            cout << "\n  Solution: " + solution->toString() << endl;

            for (int i = 0; i < solution->getSetInterval()->counter; i++) {
                solutionSet->addInterval(solution->getIntervalWrapper(i));
            }
        }
        auto *unionSet = new SetIntervalWrapper(get_union(solutionSet->getSetInterval()));
        cout << "Union of the solutions: " + unionSet->toString() << endl;

        auto *intersection = new IntervalWrapper(get_intersection(solutionSet->getSetInterval()));
        cout << "Intersection of the solutions: " + intersection->toString() << endl;
    }
    for(const string& current: actualStrings) {
        assert(find(expectedStrings.begin(), expectedStrings.end(), current) != expectedStrings.end());
    }
}