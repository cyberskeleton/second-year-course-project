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

    vector<vector<string>> readCSV(const string& fileName);

    vector<vector<Inequality>> parseInequality(const vector<vector<string>>& lines);

    vector<Inequality> parseInequality(const vector<string>& inequalitiesSet, const string& quadraticVariable, const string& variable);

    double getCoefficient(string &inequalityString, const string& coefficientVariable);

    string getInequalityRelation(string &inequalityString);

    string trim(string &s);

    SetIntervalWrapper getSolution(Inequality *inequality);
};


#endif //INTERVALS_SERVICE_H
