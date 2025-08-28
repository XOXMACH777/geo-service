#pragma once

#include <rapidjson/document.h>

#include <cstdint>
#include <string_view>
#include <type_traits>

namespace geo::json
{

// Base case for the Get function: returns the JSON value itself.
// Used when no additional keys are provided to traverse the JSON structure.
template <typename TJsonValue>
const rapidjson::Value& Get(const TJsonValue& v)
{
   static_assert(std::is_same_v<TJsonValue, rapidjson::Value> || std::is_same_v<TJsonValue, rapidjson::Document>,
      "Get function only supports rapidjson::Value and rapidjson::Document");
   return v;
}

// Recursive case for the Get function: traverses the JSON structure using a sequence of keys.
// The first key `t` is used to access the current JSON object, and the remaining keys `args...`
// are used to continue traversing the nested structure.
// This overload is enabled only if the key type `T` is `const char*`.
template <typename TJsonValue, typename T, typename... TArgs,
   typename = typename std::enable_if<std::is_same_v<T, const char*>>::type>
const rapidjson::Value& Get(const TJsonValue& v, T t, TArgs... args)
{
   static_assert(std::is_same_v<TJsonValue, rapidjson::Value> || std::is_same_v<TJsonValue, rapidjson::Document>,
      "Get function only supports rapidjson::Value and rapidjson::Document");
   return Get(v[t], args...);
}

// Base case for the Has function: checks if the JSON value is not null.
template <typename TJsonValue>
bool Has(const TJsonValue& v)
{
   static_assert(std::is_same_v<TJsonValue, rapidjson::Value> || std::is_same_v<TJsonValue, rapidjson::Document>,
      "Has function only supports rapidjson::Value and rapidjson::Document");
   return !v.IsNull();
}

// Checks if a nested key or sequence of keys exists in the JSON structure.
// Returns `true` if all keys exist and the final value is not null, otherwise `false`.
// If the current JSON value is not an object, returns `false`.
template <typename TJsonValue, typename T, typename... TArgs,
   typename = typename std::enable_if<std::is_same_v<T, const char*>>::type>
bool Has(const TJsonValue& v, T t, TArgs... args)
{
   static_assert(std::is_same_v<TJsonValue, rapidjson::Value> || std::is_same_v<TJsonValue, rapidjson::Document>,
      "Has function only supports rapidjson::Value and rapidjson::Document");
   if (!v.IsObject() || !v.HasMember(t))
      return false;
   return Has(v[t], args...);
}

// Retrieves a string value from a JSON value.
// If the JSON value is null, returns an empty string.
// Otherwise, returns the string value using `GetString()`.
template <typename JsonValue>
std::string_view GetString(const JsonValue& v)
{
   static_assert(std::is_same_v<JsonValue, rapidjson::Value> || std::is_same_v<JsonValue, rapidjson::Document>,
      "GetString function only supports rapidjson::Value and rapidjson::Document");
   return v.IsNull() ? "" : v.GetString();
}

// Retrieves a double value from a JSON value.
// If the JSON value is null, returns `0.0`.
// Otherwise, returns the double value using `GetDouble()`.
template <typename JsonValue>
double GetDouble(const JsonValue& v)
{
   static_assert(std::is_same_v<JsonValue, rapidjson::Value> || std::is_same_v<JsonValue, rapidjson::Document>,
      "GetDouble function only supports rapidjson::Value and rapidjson::Document");
   return v.IsNull() ? 0 : v.GetDouble();
}

// Retrieves an int64 value from a JSON value.
// If the JSON value is null, returns `0`.
// Otherwise, returns the int64 value using `GetInt64()`.
template <typename JsonValue>
std::int64_t GetInt64(const JsonValue& v)
{
   static_assert(std::is_same_v<JsonValue, rapidjson::Value> || std::is_same_v<JsonValue, rapidjson::Document>,
      "GetInt64 function only supports rapidjson::Value and rapidjson::Document");
   return v.IsNull() ? 0 : v.GetInt64();
}

}  // namespace geo::json
