
#include <cstring>
#include "../headers/Service.h"
#include "../headers/IntervalWrapper.h"
#include "../headers/SetIntervalWrapper.h"

using namespace std;

static const double APPROXIMATION = 0.1;

Service::Service() = default;

string getInequalityRelation(string &inequalityString) {
    vector<string> allowedSigns = vector<string>({"<", ">", "="});
    for(string sign : allowedSigns) {
        auto found = inequalityString.find_first_of(sign);
        if (found != string::npos) {
            if (found < inequalityString.length() - 1 && inequalityString.substr(found + 1, 1) == "=") {
                return sign.append("=");
            }
            return sign;
        }
    }
    return string();
}

string trim(string &s) {
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    return s;
}

vector<vector<string>> Service::readCSV(const string& fileName) {
    string line;
    vector<vector<string>> readCsv;
    ifstream data(fileName);
    while(getline(data,line)) {
        stringstream lineStream(line);
        string cell;
        vector<std::string> readRow;
        while(getline(lineStream,cell,',')) {
            cell = trim(cell);
            readRow.push_back(cell);
        }
        readCsv.push_back(readRow);
    }

    return readCsv;
}

vector<vector<Inequality>> Service::parseInequalitySystems(const vector<vector<string>>& lines) {
    vector<vector<Inequality>> parsed;
    parsed.reserve(lines.size());
    for (const vector<string>& line : lines) {
        parsed.push_back(parseInequality(line, "x^2", "x"));
    }

    return parsed;
}

vector<Inequality> Service::parseInequality(const vector<string>& inequalitiesSet, const string& quadraticVariable, const string& variable) {
    vector<Inequality> parsed;
    for (string entry : inequalitiesSet) {
        Inequality inequality;
        inequality.setFirstCoefficient(getCoefficient(entry, quadraticVariable));
        inequality.setSecondCoefficient(getCoefficient(entry, variable));
        string relationSign = getInequalityRelation(entry);
        string::size_type inequalityRelationSignPosition = entry.find(relationSign);
        inequality.setFreeCoefficient(inequalityRelationSignPosition != 0 && inequalityRelationSignPosition != std::string::npos ? stod(entry.substr()) : 0);
        inequality.setRelation(relationSign);

        parsed.push_back(inequality);
    }

    return parsed;
}

double Service::getCoefficient(string &inequalityString, const string& coefficientVariable) {
    string::size_type pos = inequalityString.find(coefficientVariable);
    if (pos == string::npos) {
        return 0.0;
    }
    auto token = inequalityString.substr(0, pos);
    if (token.empty() || token == "+") {
        token = "1";
    }
    if (token == "-") {
        token = "-1";
    }
    double result = stod(token);
    inequalityString = inequalityString.substr(pos + coefficientVariable.size());
    return result;
}

Interval_type getIntervalType(const string& relation) {
    if (relation == "<=" || relation == ">=" || relation == "=") {
        return closed;
    }
    return open;
}

IntervalWrapper* getInterval(double leftMargin,
                             double rightMargin,
                             const Interval_type leftType,
                             const Interval_type rightType,
                             const string relation,
                             double firstCoefficient,
                             double secondCoefficient,
                             double freeCoefficient) {
    // get point to calculate function value
    double point;
    if (leftMargin == -INFINITY && rightMargin == INFINITY) {
        point = 0.0;
    } else {
        if (leftMargin == -INFINITY) {
            point = rightMargin - APPROXIMATION;
        } else if (rightMargin == INFINITY) {
            point = leftMargin + APPROXIMATION;
        } else {
            point = (leftMargin + rightMargin) / 2;
        }
    }
    // calculate value of the function for the given segment of interval
    double value = firstCoefficient * point * point + secondCoefficient * point + freeCoefficient;
    // compare inequality relation to the sign of function at the given segment & return interval
    if (value < 0 && (relation == "<=" || relation == "<")) {
        return new IntervalWrapper(leftType, rightType, leftMargin, rightMargin);
    }
    if (value > 0 && (relation == ">=" || relation == ">")) {
        return new IntervalWrapper(leftType, rightType, leftMargin, rightMargin);
    }
    if (relation == "=") {
        if (leftMargin == -INFINITY) {
            return new IntervalWrapper(closed, closed, rightMargin, rightMargin);
        }
        if (rightMargin == INFINITY) {
            return new IntervalWrapper(closed, closed, leftMargin, leftMargin);
        }
    }
    return new IntervalWrapper(open, open, INFINITY, INFINITY);
}

void addIfDoesNotExist(SetIntervalWrapper* target, IntervalWrapper* source) {
    int setSize = target->getSetInterval()->counter;
    auto interval = source->getInterval();
    if (interval->left == open && interval->right == open && interval->a == interval->b) {
        // empty
        return;
    }
    if (setSize == 0) {
        target->addInterval(source);
        return;
    }
    for (int i = 0; i < setSize; ++i) {
        if (equals(target->getSetInterval()->in_array[i], interval)) {
            return;
        }
    }
    target->addInterval(source);
}

SetIntervalWrapper* Service::getInequalitySolution(Inequality* inequality) {
    auto* result = new SetIntervalWrapper();
    double a = inequality->getFirstCoefficient();
    double b = inequality->getSecondCoefficient();
    double c = inequality->getFreeCoefficient();
    double d = b * b - (4 * a * c);
    if (d < 0) {
        // no roots
        result->addInterval(new IntervalWrapper(open, open, INFINITY, INFINITY));
        return result;
    }
    Interval_type type = getIntervalType(inequality->getRelation());
    if (d == 0) {
        // one root
        double root = (-b) / (2 * a);
        addIfDoesNotExist(result, getInterval(-INFINITY, root, open, type, inequality->getRelation(), a, b, c));
        addIfDoesNotExist(result, getInterval(root, INFINITY, type, open, inequality->getRelation(), a, b, c));

        return new SetIntervalWrapper(get_union(result->getSetInterval()));
    }
    if (d > 0) {
        // two roots
        double root1 = (-b + sqrt(d)) / (2 * a);
        double root2 = (-b - sqrt(d)) / (2 * a);
        double min = MIN(root1, root2);
        double max = MAX(root1, root2);
        addIfDoesNotExist(result, getInterval(-INFINITY, min, open, type, inequality->getRelation(), a, b, c));
        addIfDoesNotExist(result, getInterval(min, max, type, type, inequality->getRelation(), a, b, c));
        addIfDoesNotExist(result, getInterval(max, INFINITY, type, open, inequality->getRelation(), a, b, c));

        return new SetIntervalWrapper(get_union(result->getSetInterval()));
    }
    // handle error
    return result;
}

SetIntervalWrapper* Service::getSystemSolution(vector<SetIntervalWrapper> inequalitySolutions) {
    auto* systemSolution = new SetIntervalWrapper();
    auto* temp = new SetIntervalWrapper(inequalitySolutions[0].getSetInterval());
    for (int inequalityCounter = 1; inequalityCounter < inequalitySolutions.size(); inequalityCounter++) {
//        cout << " .start merging: " + temp->toString() << ends;
        auto* current = &inequalitySolutions[inequalityCounter];
//        cout << " with: " + current->toString() << endl;
        // for each interval in current solution
        for (int k = 0; k < temp->getSetInterval()->counter; k++) {
            // get intersection with each interval in next solution
            for (int l = 0; l < current->getSetInterval()->counter; l++) {
                auto *toIntersect = new SetIntervalWrapper();
                toIntersect->addInterval(temp->getIntervalWrapper(k));
                toIntersect->addInterval(current->getIntervalWrapper(l));
//                cout << " ..merging: " + toIntersect->toString() << ends;
                auto intersection = get_intersection(toIntersect->getSetInterval());
//                cout << " gives intersection at stage " + to_string(k) + ":" + to_string(l) + ": " + (new IntervalWrapper(intersection))->toString() << endl;
                if (intersection != nullptr) {
                    systemSolution->addInterval(new IntervalWrapper(intersection));
                }
            }
            systemSolution = new SetIntervalWrapper(get_union(systemSolution->getSetInterval()));
        }
//        cout << " .system solution at iteration " + to_string(inequalityCounter) + ": " + systemSolution->toString() << endl;
        temp = new SetIntervalWrapper();
        temp->copyIntervals(systemSolution);
    }
    return systemSolution;
}

void Service::solve(vector<vector<Inequality>> parsedSystems) {
    for (const vector<Inequality>& system : parsedSystems) {
        vector<SetIntervalWrapper> inequalitySolutions;

        for (const Inequality& inequality : system) {
            cout << " Inequality: " + inequality.toString() << ends;
            auto* toSolve = new Inequality();
            toSolve->copyFields(inequality);
            SetIntervalWrapper* solution = Service::getInequalitySolution(toSolve);
            cout << "\n  Solution: " + solution->toString() << endl;
            SetIntervalWrapper copy;
            copy.copyIntervals(solution);
            inequalitySolutions.push_back(copy);
        }
        auto* systemSolution = Service::getSystemSolution(inequalitySolutions);
        cout << "Solution of the system: " + systemSolution->toString() + "\n" << endl;
    }
}
