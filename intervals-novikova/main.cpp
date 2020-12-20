#include <iostream>
#include "intervals_c/headers/tests_c.h"
#include "intervals_cpp/headers/cross_tests.h"
#include "intervals_cpp/headers/ServiceTests.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");

    cout << "Running C tests implementing basic logic..." << endl;
    run_tests();
    cout << "...end of C tests\n" << endl;

    cout << "Running C++ cross_tests wrapping basic implementation up..." << endl;
    cross_tests::run_cpp_tests();
    cout << "...end of C++ cross_tests\n" << endl;

    cout << "Running ServiceTests..." << endl;
    ServiceTests::serviceTests();
    cout << "...end of ServiceTests\n" << endl;

    // test for equations in systems_to_solve.csv
    string fileName = "../data/systems_to_solve.csv";
    vector<vector<string>> openData = Service::readCSV(fileName);
    vector<vector<Inequality>> parsedSystems = Service::parseInequalitySystems(openData);
    cout << "System of inequalities from " + fileName + " parsed:" << endl;
    Service::solve(parsedSystems);

    // test with console
    string inputLine;
    vector<string> inequality;
    cout << "please use this quadratic equation variables format: x^2, x" << endl;
    cout << "input 2 quadratic inequalities in format: ax^2 + bx + c <relation> 0,"
            " where <relation> is one of: '<','>','=','<=','>='" << endl;
    for (int i = 0; i < 2; ++i) {
        cin >> inputLine;
        inequality.push_back(inputLine);
    }
    vector<Inequality> tmp = Service::parseInequality(inequality, "x^2", "x");
    vector<vector<Inequality>> systems;
    systems.push_back(tmp);
    Service::solve(systems);

    return 0;
}
