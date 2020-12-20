#ifndef INTERVALS_INEQUALITY_H
#define INTERVALS_INEQUALITY_H
#include <string>
#include <math.h>
using namespace std;

class Inequality {
private:
    double firstCoefficient;
    double secondCoefficient;
    double freeCoefficient;
    string relation;
public:
    Inequality();

    double getFirstCoefficient() const;

    void setFirstCoefficient(double coefficient);

    double getSecondCoefficient() const;

    void setSecondCoefficient(double coefficient);

    double getFreeCoefficient() const;

    void setFreeCoefficient(double coefficient);

    const string &getRelation() const;

    void setRelation(const string &sign);

    string toString() const;

    string toString(const string &quadraticVariable, const string &variable) const;

    void copyFields(const Inequality source);

    void copyFields(Inequality *source);
};

#endif //INTERVALS_INEQUALITY_H
