#pragma once

#include <rapidjson/document.h>
#include <string>

namespace geo
{

// Configuration class for reading parameters from a JSON configuration file
class Configuration
{
public:
   // Constructor that loads and parses the configuration file
   explicit Configuration(const char* filename);

   // Retrieves a string value from the configuration by key
   std::string GetString(const char* name) const;

   // Retrieves an int64 value from the configuration by key
   std::int64_t GetInt64(const char* name) const;

private:
   rapidjson::Document m_config; // RapidJSON document holding the parsed configuration
};

using ConfigurationPtr = std::unique_ptr<Configuration>;

}  // namespace geo