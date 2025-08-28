#pragma once

#include <cstdint>
#include <string>

namespace geo::debug
{

// Find cities by name.
void Search(const std::string& name, const std::string& configFilePath);

// Find cities by position.
void Search(double latitude, double longitude, const std::string& configFilePath);

// Find regions by position.
void Search(double latitude, double longitude, std::uint32_t rangeMeters, std::uint32_t filter,
   const std::string& configFilePath);

// Return weather for position and dates.
void RequestWeather(double latitude, double longitude, const std::string& fromDate, const std::string& toDate,
   const std::string& configFilePath);

}  // namespace geo::debug
