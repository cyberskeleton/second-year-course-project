
#include <cstring>
#include "../headers/Service.h"
#include "../headers/IntervalWrapper.h"
#include "../headers/SetIntervalWrapper.h"

using namespace std;

static const double APPROXIMATION = 0.1;

Service::Service() = default;

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

vector<vector<Inequality>> Service::parseInequality(const vector<vector<string>>& lines) {
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
    if (token.empty()) {
        token = "1";
    }
    if (token == "-") {
        token = "-1";
    }
    double result = stod(token);
    inequalityString = inequalityString.substr(pos + coefficientVariable.size());
    return result;
}

string Service::getInequalityRelation(string &inequalityString) {
    vector<string> allowedSigns = vector<string>({"<", ">", "="});
    for(string sign : allowedSigns) {
        auto found = inequalityString.find_first_of(sign);
        if (found != string::npos) {
            auto toKill = inequalityString.substr(found + 1, 1);
            if (found < inequalityString.length() - 1 && inequalityString.substr(found + 1, 1) == "=") {

                return sign.append("=");
            }
            return sign;
        }
    }
    return string();
}

/**
 * Implementation taken here: https://stackoverflow.com/questions/5891610/how-to-remove-certain-characters-from-a-string-in-c
 * @param s
 * @return
 */
string Service::trim(string &s) {
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    return s;
}

IntervalWrapper* getInterval(double leftMargin, double rightMargin, const string relation) {
    Interval_type left;
    Interval_type right;
    double wa = leftMargin;
    if (leftMargin == -INFINITY && rightMargin == INFINITY) {
        wa = 0.0;
        left = open;
        right = open;
    } else {
        if (leftMargin == -INFINITY) {
            wa = rightMargin - APPROXIMATION;
            left = open;
        } else if (rightMargin == INFINITY) {
            wa = leftMargin + APPROXIMATION;
            right = open;
        } else {
            wa = (leftMargin + rightMargin) / 2;
        }
    }
    if ((wa + leftMargin) * (wa + rightMargin) < 0) {
        if (relation == "<" || relation == "<=") {
            return new IntervalWrapper(left, right, leftMargin, rightMargin);
        }
    }
    if ((wa + leftMargin) * (wa + rightMargin) > 0) {
        if (relation == ">" || relation == ">=") {
            return new IntervalWrapper(left, right, leftMargin, rightMargin);
        }
    }
    if ((wa + leftMargin) * (wa + rightMargin) == 0) {
        if (relation == "<=" || relation == ">=") {
            return new IntervalWrapper(closed, closed, leftMargin, rightMargin);
        }
    }
    return new IntervalWrapper(open, open, 0, 0);
}

SetIntervalWrapper Service::getSolution(Inequality* inequality) {
    double a = inequality->getFirstCoefficient();
    double b = inequality->getSecondCoefficient();
    double c = inequality->getFreeCoefficient();
    double d = b * b - (4 * a * c);
    double root1 = (-b + sqrt(d)) / (2 * a);
    double root2 = (-b - sqrt(d)) / (2 * a);
    // form vector of interval edges
    vector<double> points;
    points.push_back(-INFINITY);
    if (MAX(root1, root2) == root2) {
        points.push_back(root1);
        points.push_back(root2);
    } else if (MAX(root1, root2) == root1) {
        points.push_back(root2);
        points.push_back(root1);
    } else {
        points.push_back(root1);
    }
    points.push_back(INFINITY);
    // add intervals to a set
    SetIntervalWrapper intervals;
    intervals.addInterval(getInterval(points[0], points[1], inequality->getRelation()));
    intervals.addInterval(getInterval(points[1], points[2], inequality->getRelation()));
    intervals.addInterval(getInterval(points[2], points[3], inequality->getRelation()));

    return intervals;
}
