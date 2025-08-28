#include "WebClient.h"

#include <absl/log/log.h>
#include <curl/curl.h>
#include <curl/easy.h>

#include <format>
#include <stdexcept>

namespace
{

// Callback function for CURL to write received data into string buffer
// @param contents Pointer to the delivered data
// @param size Always 1
// @param nmemb Size of the data
// @param userp Pointer to user data (string buffer in our case)
// @return Number of bytes actually taken care of
size_t curlWriteFunction(void* contents, size_t size, size_t nmemb, void* userp)
{
   ((std::string*)userp)->append((char*)contents, size * nmemb);
   return size * nmemb;
}

// Template helper function to set CURL options with error handling
// @param curl CURL handle to set option on
// @param opt CURL option to set
// @param val Value to set for the option
template <typename TCurl, typename TOptionKey, typename TOptionValue>
void setCurlOpt(TCurl curl, TOptionKey opt, TOptionValue val)
{
   const auto res = curl_easy_setopt(curl.get(), opt, val);
   if (res != CURLE_OK)
      throw std::runtime_error(std::format("cURL error: {0}", curl_easy_strerror(res)));
}

// Template helper function to safely execute a function and log errors
// @param f Function to execute safely
// @return true if function executed successfully, false if exception was caught
template <typename TUnsafe>
bool safeCall(TUnsafe f)
{
   try
   {
      f();
   }
   catch (const std::runtime_error& e)
   {
      LOG(ERROR) << e.what();
      return false;
   }
   return true;
}

}  // namespace

namespace geo
{

WebClient::WebClient(std::string url, std::uint64_t writeTimeoutMs)
   : m_url(std::move(url))
   , m_writeTimeoutMs(writeTimeoutMs)
{
}

std::string WebClient::Get(const std::string& request)
{
   if (request.empty())
   {
      LOG(ERROR) << "Empty request passed.";
      return "";
   }

   std::string response;
   auto curl = createCurl(m_url + "?" + request, m_writeTimeoutMs, &response);
   if (!curl)
   {
      LOG(ERROR) << "Cannot create cURL instance. Data is not sent.";
      return "";
   }

#ifdef NDEBUG
   LOG(INFO) << std::format("Starting HTTP GET request to {}", m_url);
#else
   LOG(INFO) << std::format("Starting HTTP GET request to {}, request:\n{}", m_url, request);
#endif

   if (!perform(curl))
   {
      LOG(INFO) << std::format("HTTP GET request to {} finished with error (request = {})", m_url, request);
      return "";
   }

#ifdef NDEBUG
   LOG(INFO) << std::format("HTTP GET request to {} finished", m_url);
#else
   LOG(INFO) << std::format("HTTP GET request to {} finished, response:\n{}", m_url, response);
#endif
   return response;
}

std::string WebClient::Post(const std::string& data)
{
   if (data.empty())
   {
      LOG(ERROR) << "Empty data passed.";
      return "";
   }

   std::string response;
   auto curl = createCurl(m_url, m_writeTimeoutMs, &response);
   if (!curl)
   {
      LOG(ERROR) << "Cannot create cURL instance. Data is not sent.";
      return "";
   }

   if (!safeCall(
          [&]
          {
             setCurlOpt(curl, CURLOPT_POST, 1L);
             setCurlOpt(curl, CURLOPT_POSTFIELDS, data.c_str());
          }))
   {
      return "";
   }

#ifdef NDEBUG
   LOG(INFO) << std::format("Starting HTTP POST request to {}", m_url);
#else
   LOG(INFO) << std::format("Starting HTTP POST request to {}, data:\n{}", m_url, data);
#endif

   if (!perform(curl))
   {
      LOG(INFO) << std::format("HTTP POST request to {} finished with error (data = {})", m_url, data);
      return "";
   }

#ifdef NDEBUG
   LOG(INFO) << std::format("HTTP POST request to {} finished", m_url);
#else
   LOG(INFO) << std::format("HTTP POST request to {} finished, response:\n{}", m_url, response);
#endif
   return response;
}

// Creates and configures a CURL instance with specified URL, timeout, and response buffer
WebClient::CurlPtr WebClient::createCurl(
   const std::string& url, std::uint64_t writeTimeoutMs, std::string* responseBuffer)
{
   auto curl = CurlPtr(curl_easy_init(),
      [](auto p)
      {
         curl_easy_cleanup(p);
      });
   if (!curl)
      return nullptr;

   if (!safeCall(
          [&]
          {
             setCurlOpt(curl, CURLOPT_URL, url.c_str());
             setCurlOpt(curl, CURLOPT_SSL_VERIFYPEER, 0L);  // Disable SSL peer verification
             setCurlOpt(curl, CURLOPT_SSL_VERIFYHOST, 0L);  // Disable SSL host verification
             setCurlOpt(curl, CURLOPT_TIMEOUT_MS, writeTimeoutMs);
             setCurlOpt(curl, CURLOPT_WRITEFUNCTION, curlWriteFunction);
             setCurlOpt(curl, CURLOPT_WRITEDATA, responseBuffer);
             setCurlOpt(curl, CURLOPT_FAILONERROR, 1L);  // Fail on HTTP errors (4xx, 5xx)
             setCurlOpt(curl, CURLOPT_USERAGENT, "geo-service/0.1");
          }))
   {
      return nullptr;
   }

   return curl;
}

// Executes CURL request and handles potential errors
bool WebClient::perform(const CurlPtr& curl)
{
   const auto res = curl_easy_perform(curl.get());
   if (res == CURLE_HTTP_RETURNED_ERROR)
   {
      long httpErrorCode = 0;
      curl_easy_getinfo(curl.get(), CURLINFO_RESPONSE_CODE, &httpErrorCode);
      LOG(ERROR) << std::format("HTTP error code: {}", httpErrorCode);
      return false;
   }
   else if (res != CURLE_OK)
   {
      LOG(ERROR) << std::format("cURL error: {}", curl_easy_strerror(res));
      return false;
   }
   return true;
}

}  // namespace geo
