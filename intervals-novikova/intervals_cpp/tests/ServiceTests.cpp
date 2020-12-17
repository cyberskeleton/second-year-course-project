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
    vector<vector<Inequality>> in = service->parseInequality(data);
    vector<string> expectedStrings = vector<string>({"0.000000x^2+4.091000x+0.000000=0",
                                                     "3.200000x^2+4.660000x+20.019000<=0",
                                                     "1.000000x^2-1.000000x-6.330000>0",
                                                     "-1.000000x^2+0.000000x-2.990000>0"});
    vector<string> actualStrings;
    for (const vector<Inequality>& system : in) {
        cout << "System parsed:" << endl;
        for (const Inequality& inequality : system) {
            string result = inequality.toString();
            actualStrings.push_back(result);
            cout << result << endl;
        }
    }
    for(const string& current: actualStrings) {
        assert(find(expectedStrings.begin(), expectedStrings.end(), current) != expectedStrings.end());
    }

    auto* toSolve = new Inequality();
    toSolve->setFirstCoefficient(1);
    toSolve->setSecondCoefficient(1);
    toSolve->setFreeCoefficient(-6);
    toSolve->setRelation(">=");
    cout << "\nInequality: " + toSolve->toString() << endl;
    SetIntervalWrapper solution = service->getSolution(toSolve);
    cout << "Solution: " + solution.toString() << endl;
}