#ifndef INTERVALS_SERVICE_H
#define INTERVALS_SERVICE_H
#include <ostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "Inequality.h"
#include "IntervalWrapper.h"
#include "SetIntervalWrapper.h"

using namespace std;

class Service {
private:

public:
    Service();

    static vector<vector<string>> readCSV(const string& fileName);

    static vector<vector<Inequality>> parseInequalitySystems(const vector<vector<string>>& lines);

    static vector<Inequality> parseInequality(const vector<string>& inequalitiesSet, const string& quadraticVariable, const string& variable);

    static double getCoefficient(string &inequalityString, const string& coefficientVariable);

    static SetIntervalWrapper* getInequalitySolution(Inequality *inequality);

    static SetIntervalWrapper *getSystemSolution(vector<SetIntervalWrapper> inequalitySolutions);

    static void solve(vector<vector<Inequality>> parsedSystems);
};

#endif //INTERVALS_SERVICE_H
