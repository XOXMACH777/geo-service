#pragma once

#include "ProtoTypes.h"

#include <chrono>
#include <format>
#include <string>
#include <utility>

namespace geo
{

// Both google::protobuf::Timestamp and std::chrono::system_clock are relative to Unix Time Epoch.
// https://en.cppreference.com/w/cpp/chrono/system_clock
// https://protobuf.dev/reference/php/api-docs/Google/Protobuf/Timestamp.html

using TimePoint = std::chrono::system_clock::time_point;
using Date = std::chrono::year_month_day;

using TimeRange = std::pair<TimePoint, TimePoint>;
using DateRange = std::pair<Date, Date>;

inline TimePoint TimestampToTimePoint(const google::protobuf::Timestamp& timestamp)
{
   return TimePoint{std::chrono::seconds{timestamp.seconds()}};
}

inline google::protobuf::Timestamp TimePointToTimestamp(const TimePoint& time)
{
   google::protobuf::Timestamp result;
   const auto secondsSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch());
   result.set_seconds(secondsSinceEpoch.count());
   return result;
}

inline std::tm TimePointToTm(const TimePoint& time)
{
   const auto timeT = std::chrono::system_clock::to_time_t(time);
   return *std::gmtime(&timeT);
}

inline TimePoint TmToTimePoint(std::tm tm)
{
   const std::time_t timeT = std::mktime(&tm);
   return std::chrono::system_clock::from_time_t(timeT);
}

inline Date TimePointToDate(const TimePoint& time)
{
   const auto days = std::chrono::floor<std::chrono::days>(time);
   return Date{days};
}

inline TimePoint DateToTimePoint(const Date& ymd)
{
   const auto ymdFixed = ymd.ok() ? ymd : (ymd.year() / ymd.month() / std::chrono::last);
   return std::chrono::sys_days{ymdFixed};
}

inline std::string DateToString(const Date& date)
{
   return std::format("{:%F}", date);
}

inline Date StringToDate(std::string_view value)
{
   Date date{};

   std::istringstream ss(std::string{value});
   ss >> std::chrono::parse("%F", date);
   if (ss.fail())
      return Date{};

   return date;
}

}  // namespace geo
