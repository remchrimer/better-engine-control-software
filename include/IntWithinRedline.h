#ifndef BETTER_ENGINE_CONTROL_SOFTWARE_INTWITHINREDLINE_H
#define BETTER_ENGINE_CONTROL_SOFTWARE_INTWITHINREDLINE_H

#include "generic/IRedline.h"
#include <functional>

/**
 * class to represent IRedline that checks for expected int value within a range
 */
class IntWithinRedline: public IRedline{
public:
    IntWithinRedline(std::string n, std::function<int(const SensorData*)> sFunct, int lBound, int uBound);
    IntWithinRedline(std::string n, std::function<int(const SensorData*)> sFunct, int lBound, int uBound,
                     ECSRedLineResponse r);

    /**
     * tests to make sure if passed sensor data falls into expected value
     * bounds are INCLUSIVE
     * @param data Pointer to sensor data object to read value from
     * @return boolean
     */
    bool testCondition(const SensorData* data) const override;

    /**
     * creates a formatted error message string
     * @param data Pointer to sensor data object to read value from
     * @return string, containing IRedline name, expected value(s), and actual value
     */
    std::string errorMessage(const SensorData* data) const override;

protected:
    std::function<int(const SensorData*)> selector;
    int lowerBound;
    int upperBound;
};

#endif //BETTER_ENGINE_CONTROL_SOFTWARE_INTWITHINREDLINE_H
