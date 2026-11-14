#include <iostream>

#include <pb/measurement.hpp>

int main() {
    constexpr pb::Measurement<uint32_t> empty;                  // value 0, timestamp 0
    constexpr pb::Measurement<uint32_t> temperature(2137);      // 21.37 degrees as fixed point
    constexpr pb::Measurement<float> pressure(1013.25F, 42U);   // value and timestamp

    static_assert(temperature.value() == 2137, "checked at compile time");

    std::cout << "empty: " << empty.value() << " at " << empty.timestamp() << std::endl;
    std::cout << "temperature: " << temperature.value() << " at " << temperature.timestamp() << std::endl;
    std::cout << "pressure: " << pressure.value() << " at " << pressure.timestamp() << std::endl;

    return 0;
}