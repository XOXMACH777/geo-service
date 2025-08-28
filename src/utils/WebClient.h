#pragma once

#include <curl/curl.h>

#include <memory>
#include <string>

namespace geo
{

class WebClient
{
public:
   static const int sc_defaultTimeoutMs = 180'000;  // Default timeout in milliseconds (180 seconds)

public:
   // Constructor taking base URL and optional write timeout in milliseconds
   // @param address The base URL for web requests
   // @param writeTimeoutMs Timeout value for write operations in milliseconds (default: sc_defaultTimeoutMs)
   WebClient(std::string address, std::uint64_t writeTimeoutMs = sc_defaultTimeoutMs);

   // Performs HTTP GET request with provided request string and returns response
   // @param request The request string to append to the base URL
   // @return The server response as string, or empty string on error
   std::string Get(const std::string& request);

   // Performs HTTP POST request with provided data and returns response
   // @param data The data to send in the POST request body
   // @return The server response as string, or empty string on error
   std::string Post(const std::string& data);

private:
   using CurlPtr = std::shared_ptr<CURL>;  // Type alias for shared pointer to CURL handle

private:
   // Creates and configures a CURL instance with given parameters
   // @param url The complete URL for the request
   // @param writeTimeoutMs Timeout value for write operations in milliseconds
   // @param responseBuffer Pointer to string where response will be stored
   // @return Configured CURL handle wrapped in shared_ptr, or nullptr on error
   static CurlPtr createCurl(const std::string& url, std::uint64_t writeTimeoutMs, std::string* responseBuffer);

   // Executes the CURL request and returns success status
   // @param curl Configured CURL handle to perform
   // @return true if request succeeded, false otherwise
   static bool perform(const CurlPtr& curl);

private:
   std::string m_url;               // Base URL for web requests
   std::uint64_t m_writeTimeoutMs;  // Timeout value for write operations in milliseconds
};

}  // namespace geo
