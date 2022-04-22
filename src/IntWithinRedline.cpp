//
// Created by kevin on 4/11/2022.
//
#include "IntWithinRedline.h"
#include <stdexcept>

IntWithinRedline::IntWithinRedline(std::string n, std::function<int(SensorData*)> sFunct, int lBound, int uBound):
        IntWithinRedline(n, sFunct, lBound, uBound, WARN)
{
}

IntWithinRedline::IntWithinRedline(std::string n, std::function<int(SensorData*)> sFunct, int lBound, int uBound, ECSRedLineResponse r):
        Redline(n),
        selector(sFunct),
        lowerBound(lBound),
        upperBound(uBound),
        response(r)
{
    if (lBound > uBound){
        throw std::invalid_argument("Lower bound cannot be greater than upper bound");
    }
}


bool IntWithinRedline::testCondition(SensorData* data){
    int testNum = this->selector(data);

    return (this->lowerBound <= testNum) and (testNum <= this->upperBound);
}

std::string IntWithinRedline::errorMessage(SensorData* data){
    int testNum = this->selector(data);

    return this->name + " failed " + std::to_string(testNum) + " is not in range " +
        std::to_string(this->lowerBound) + " - " + std::to_string(this->upperBound);
}