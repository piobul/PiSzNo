#pragma once

#include <stdint.h>

namespace pb {

/**
 *  One measurement value with an optional timestamp
 *  
 *  The class is a small value type: it is created once and then only read
 * 
 *  T number type of the value, for example int32_t (fixed point) or float
*/

template <typename T>

class Measurement {
public:
    // Creates a measurement with value 0 and timestamp 0
    constexpr Measurement = default();

    /**
     *  Creates a measurement with the given value and timestamp 0
     *  value measured value
     */
    constexpr explicit Measurement(T value) : value_(value) {}

    /**
     *  Creates a measurement with the given value and timestamp
     *  value measured value
     *  timestamp time of the measurement
     */
    constexpr Measurement(T value, uint32_t timestamp) : value_(value), timestamp_(timestamp) {}

    //  Returns the measured value
    constexpr T value() const { return value_; }

    //  Returns the timestamp
    constexpr uint32_t timestamp() const { return timestamp_; }

    //  TODO: operators

private:
    T value_ {};
    uint32_t timestamp_ {0};
};

}   // namespace pb