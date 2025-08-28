#include "DebugHelpers.h"
#include "GeoServiceImpl.h"
#include "utils/Configuration.h"

#include <absl/flags/commandlineflag.h>
#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <absl/log/globals.h>
#include <absl/log/initialize.h>
#include <absl/log/log.h>
#include <google/protobuf/message_lite.h>
#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>

#include <chrono>
#include <format>
#include <string>
#include <thread>

namespace geo
{

void RunServer(const std::string& configFilePath)
{
   std::string server_address("0.0.0.0:50051");
   Configuration configuration(configFilePath.c_str());
   GeoServiceImpl service(configuration);

   grpc::EnableDefaultHealthCheckService(true);

   grpc::ServerBuilder builder;
   builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
   builder.RegisterService(&service);
   std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

   const int lifetimeSeconds = 300;
   LOG(INFO) << std::format("Server listening on {} for {} seconds", server_address, lifetimeSeconds);
   std::this_thread::sleep_for(std::chrono::seconds(lifetimeSeconds));

   const int rpcShutdownTimeoutSeconds = 1;
   LOG(INFO) << std::format("Shutting down. Shutdown timeout = {} seconds", rpcShutdownTimeoutSeconds);
   server->Shutdown(std::chrono::system_clock::now() + std::chrono::seconds(rpcShutdownTimeoutSeconds));
}

}  // namespace geo

ABSL_FLAG(std::string, config, "", "Configuration file name");
ABSL_FLAG(bool, debug, false, "Debug mode");
ABSL_FLAG(double, lat, NAN, "[Debug] Search for cities near this geographical point (lat)");
ABSL_FLAG(double, lon, NAN, "[Debug] Search for cities near this geographical point (lon)");
ABSL_FLAG(std::uint32_t, dist, 0, "[Debug] Half width (and height) of the square box in kilometers");
ABSL_FLAG(std::uint32_t, filter, 0, "[Debug] Bitmask to specify which features to include ");
ABSL_FLAG(std::string, name, "", "[Debug] Search for cities by name");
ABSL_FLAG(std::string, fromDate, "", "[Debug] Start date for weather request");
ABSL_FLAG(std::string, toDate, "", "[Debug] End date for weather request");

int main(int argc, char** argv)
{
   absl::ParseCommandLine(argc, argv);
   absl::SetStderrThreshold(absl::LogSeverityAtLeast::kInfo);
   absl::InitializeLog();

   const std::string configFilePath = absl::GetFlag(FLAGS_config);
   if (configFilePath.empty())
   {
      return -1;
   }

   if (absl::GetFlag(FLAGS_debug))
   {
      double lat = absl::GetFlag(FLAGS_lat);
      double lon = absl::GetFlag(FLAGS_lon);
      std::uint32_t dist = absl::GetFlag(FLAGS_dist);
      std::uint32_t filter = absl::GetFlag(FLAGS_filter);
      std::string name = absl::GetFlag(FLAGS_name);
      std::string fromDate = absl::GetFlag(FLAGS_fromDate);
      std::string toDate = absl::GetFlag(FLAGS_toDate);

      if (!name.empty())
         geo::debug::Search(name, configFilePath);
      else if (lat != NAN && lon != NAN && !fromDate.empty() && !toDate.empty())
         geo::debug::RequestWeather(lat, lon, fromDate, toDate, configFilePath);
      else if (lat != NAN && lon != NAN && dist != 0 && filter != 0)
         geo::debug::Search(lat, lon, dist, filter, configFilePath);
      else if (lat != NAN && lon != NAN && dist != 0)
         geo::debug::Search(lat, lon, dist, 1, configFilePath);
      else if (lat != NAN && lon != NAN)
         geo::debug::Search(lat, lon, configFilePath);
   }
   else
   {
      geo::RunServer(configFilePath);
      LOG(INFO) << "Exiting";
   }
   return 0;
}
