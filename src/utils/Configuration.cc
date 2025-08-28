#include "Configuration.h"

#include "JsonUtils.h"

#include <absl/log/log.h>

#include <format>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace geo
{

Configuration::Configuration(const char* filename)
{
   // Open the configuration file
   std::ifstream file(filename);
   if (!file.is_open())
   {
      LOG(ERROR) << std::format("Failed to open configuration file: {}", std::string(filename));
      throw std::runtime_error("Failed to open configuration file: " + std::string(filename));
   }

   // Read the file content into a string
   std::stringstream buffer;
   buffer << file.rdbuf();
   std::string jsonContent = buffer.str();

   // Parse the JSON content
   m_config.Parse(jsonContent.c_str());
   if (m_config.HasParseError())
   {
      LOG(ERROR) << std::format("Failed to parse configuration file: {}", std::string(filename));
      throw std::runtime_error("Failed to parse configuration file: " + std::string(filename));
   }
}

std::string Configuration::GetString(const char* name) const
{
   // Check if the key exists
   if (!json::Has(m_config, name))
   {
      LOG(ERROR) << std::format("Configuration key not found: {}", std::string(name));
      throw std::runtime_error("Configuration key not found: " + std::string(name));
   }
   // Return the string value
   return std::string(json::GetString(json::Get(m_config, name)));
}

std::int64_t Configuration::GetInt64(const char* name) const
{
   // Check if the key exists
   if (!json::Has(m_config, name))
   {
      LOG(ERROR) << std::format("Configuration key not found: {}", std::string(name));
      throw std::runtime_error("Configuration key not found: " + std::string(name));
   }
   // Return the int64 value
   return json::GetInt64(json::Get(m_config, name));
}

}  // namespace geo
