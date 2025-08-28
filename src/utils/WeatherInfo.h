#pragma once

#include "TimeUtils.h"

#include <vector>

namespace geo
{

struct WeatherInfo
{
   Date time;
   double temperatureMin{};
   double temperatureAverage{};
   double temperatureMax{};
};

using WeatherInfoVector = std::vector<WeatherInfo>;

}  // namespace geo
