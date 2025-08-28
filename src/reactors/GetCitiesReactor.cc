#include "GetCitiesReactor.h"

#include "../search/SearchEngineItf.h"
#include "../utils/GeoUtils.h"
#include "../utils/grpcUtils.h"
#include "RequestValidators.h"

#include <absl/log/log.h>

#include <format>

namespace geo
{

GetCitiesReactor::GetCitiesReactor(grpc::CallbackServerContext* context, const geoproto::CitiesRequest& request,
   geoproto::CitiesResponse& response, ISearchEngine& searchEngine)
{
   if (auto errorString = ValidateCitiesRequest(request))
   {
      LOG(ERROR) << std::format("Bad request, client-id={}", geo::ExtractClientId(*context));
      Finish(grpc::Status{grpc::StatusCode::INVALID_ARGUMENT, errorString});
      return;
   }

   GeoProtoPlaces cities;  // Container to hold the search results.

   // Check if the request includes a position (latitude/longitude) for the search.
   if (request.has_position())
   {
      // Find cities by their geographic position.
      cities = searchEngine.FindCitiesByPosition(
         request.position().latitude(), request.position().longitude(), request.include_details());
   }
   // Check if the request includes a city name for the search.
   else if (request.has_name())
   {
      // Find cities by their name.
      cities = searchEngine.FindCitiesByName(request.name(), request.include_details());
   }

   // Populate the response with the found cities.
   *response.mutable_cities() = {std::make_move_iterator(cities.begin()), std::make_move_iterator(cities.end())};

   // Finish the RPC with a success status.
   Finish(grpc::Status::OK);
}

}  // namespace geo
