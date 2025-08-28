#include "GetRegionsReactor.h"

#include "../search/SearchEngineItf.h"
#include "../utils/GeoUtils.h"
#include "../utils/grpcUtils.h"
#include "RequestValidators.h"

#include <format>
#include <iterator>

namespace geo
{

GetRegionsReactor::GetRegionsReactor(grpc::CallbackServerContext* context, const geoproto::RegionsRequest& request,
   geoproto::RegionsResponse& response, ISearchEngine& searchEngine)
{
   if (auto errorString = ValidateRegionsRequest(request))
   {
      LOG(ERROR) << std::format("Bad request, client-id={}", geo::ExtractClientId(*context));
      Finish(grpc::Status{grpc::StatusCode::INVALID_ARGUMENT, errorString});
      return;
   }

   // Convert protocol buffer properties to search engine preferences
   const ISearchEngine::RegionPreferences::Properties props = {
      request.prefs().properties().begin(), request.prefs().properties().end()};
   ISearchEngine::RegionPreferences prefs{request.prefs().mask(), std::move(props)};

   // Create bounding box around requested position (converting km to meters)
   const auto box =
      CreateBoundingBox(request.position().latitude(), request.position().longitude(), request.distance_km() * 1000);

   // Execute region search and populate response
   auto regions = searchEngine.StartFindRegions()(box, prefs);
   *response.mutable_regions() = {std::make_move_iterator(regions.begin()), std::make_move_iterator(regions.end())};

   // Complete the RPC successfully
   Finish(grpc::Status::OK);
}

}  // namespace geo
