#pragma once

#include "../utils/TimeUtils.h"
#include "../utils/WeatherInfo.h"
#include "../utils/WebClient.h"

#include <chrono>
#include <vector>

namespace geo::openmeteo
{

// Collect historical ranges for given date range for N most recent years.
// @param dateRange: Controls "month and day" dates or resulting ranges.
// @param latestTime: The latest time that is considered "historical".
//                   Resulting ranges are always in the past compared to this time.
// @param numYears: Number of yearly ranges to return.
// @return: N most recent historical ranges corresponding to given dates.
std::vector<DateRange> CollectHistoricalRanges(
   const DateRange& dateRange, const TimePoint& latestTime, std::uint32_t numYears);

// Requests Open Meteo Historical API for given location and date range.
// @param client: WebClient instance to interact with the Open Meteo Historical API.
// @param latitude: The latitude of the location.
// @param longitude: The longitude of the location.
// @param dateRange: The range of dates to request historical weather for.
// @return: A list of weather information for each date in the range.
WeatherInfoVector LoadHistoricalWeather(
   WebClient& client, double latitude, double longitude, const DateRange& dateRange);

}  // namespace geo::openmeteo
