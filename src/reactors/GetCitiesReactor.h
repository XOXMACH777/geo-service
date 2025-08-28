#pragma once

#include "geo.grpc.pb.h"

#include <absl/log/log.h>
#include <grpc/grpc.h>
#include <grpcpp/support/server_callback.h>

#include <format>

namespace geo
{

class WebClient;
class ISearchEngine;

// Reactor class for handling unary (non-streaming) responses for the GetCities RPC.
// This class is responsible for processing a single request and returning a single response
// containing city data based on the client's query.
class GetCitiesReactor : public grpc::ServerUnaryReactor
{
public:
   // Constructor for the GetCitiesReactor.
   // @param context: Server context.
   // @param request: The incoming CitiesRequest from the client.
   // @param response: The CitiesResponse to be populated and sent back to the client.
   // @param searchEngine: Reference to the search engine used to find cities.
   GetCitiesReactor(grpc::CallbackServerContext* context, const geoproto::CitiesRequest& request,
      geoproto::CitiesResponse& response, ISearchEngine& searchEngine);

private:
   // Called when the RPC is completed. Logs the completion and cleans up the reactor.
   void OnDone() override
   {
      LOG(INFO) << std::format("GetCities() RPC completed");
      delete this;
   }

   // Called when the RPC is cancelled by the client. Logs the cancellation.
   void OnCancel() override { LOG(ERROR) << std::format("GetCities() RPC cancelled"); }
};

}  // namespace geo