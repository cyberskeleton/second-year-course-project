#include "../headers/Inequality.h"
using namespace std;

Inequality::Inequality() {}

void Inequality::copyFields(const Inequality source) {
    this->setFirstCoefficient(source.getFirstCoefficient());
    this->setSecondCoefficient(source.getSecondCoefficient());
    this->setFreeCoefficient(source.getFreeCoefficient());
    this->setRelation(source.getRelation());
}

void Inequality::copyFields(Inequality* source) {
    this->setFirstCoefficient(source->getFirstCoefficient());
    this->setSecondCoefficient(source->getSecondCoefficient());
    this->setFreeCoefficient(source->getFreeCoefficient());
    this->setRelation(source->getRelation());
}

double Inequality::getFirstCoefficient() const {
    return firstCoefficient;
}

void Inequality::setFirstCoefficient(double coefficient) {
    Inequality::firstCoefficient = coefficient;
}

double Inequality::getSecondCoefficient() const {
    return secondCoefficient;
}

void Inequality::setSecondCoefficient(double coefficient) {
    Inequality::secondCoefficient = coefficient;
}

double Inequality::getFreeCoefficient() const {
    return freeCoefficient;
}

void Inequality::setFreeCoefficient(double coefficient) {
    Inequality::freeCoefficient = coefficient;
}

const string &Inequality::getRelation() const {
    return relation;
}

void Inequality::setRelation(const string &sign) {
    Inequality::relation = sign;
}

string Inequality::toString() const {
    return toString("x^2", "x");
}

string Inequality::toString(const string& quadraticVariable, const string& variable) const {
    string firstSign = getSecondCoefficient() >= 0 ? "+": "";
    string secondSign = getFreeCoefficient() >= 0 ? "+": "";
    return to_string(getFirstCoefficient()) + quadraticVariable
    + firstSign + to_string(getSecondCoefficient()) + variable
    + secondSign + to_string(getFreeCoefficient())
    + getRelation() + "0";
}

