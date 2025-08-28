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

// Reactor class for handling unary (non-streaming) responses for the GetRegions RPC.
// This class processes a single request and returns region data matching the query.
class GetRegionsReactor : public grpc::ServerUnaryReactor
{
public:
   // Constructor for the GetRegionsReactor.
   // @param context: Server context.
   // @param request: The incoming RegionsRequest containing search parameters.
   // @param response: The RegionsResponse to be populated with results.
   // @param searchEngine: Reference to the search engine used to find regions.
   GetRegionsReactor(grpc::CallbackServerContext* context, const geoproto::RegionsRequest& request,
      geoproto::RegionsResponse& response, ISearchEngine& searchEngine);

private:
   // Called when the RPC is completed. Logs completion and cleans up the reactor.
   void OnDone() override
   {
      LOG(INFO) << "GetRegions() RPC completed";
      delete this;
   }

   // Called when the RPC is cancelled. Logs the cancellation.
   void OnCancel() override { LOG(ERROR) << "GetRegions() RPC cancelled"; }
};

}  // namespace geo
