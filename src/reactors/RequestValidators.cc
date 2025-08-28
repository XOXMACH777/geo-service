#include "RequestValidators.h"

#include "../utils/GeoUtils.h"
#include "geo.pb.h"

namespace geo
{

const char* ValidateCitiesRequest(const geoproto::CitiesRequest& request)
{
   // If neither position nor name is provided in the request, it's considered invalid.
   if (!request.has_position() && !request.has_name())
      return "Either position or name must be set in CitiesRequest";

   // If a position is provided, validate the latitude and longitude.
   if (request.has_position())
   {
      if (!geo::IsValidLatitude(request.position().latitude()))
         return "Wrong latitude in CitiesRequest";

      if (!geo::IsValidLongitude(request.position().longitude()))
         return "Wrong longitude in CitiesRequest";
   }

   return nullptr;
}

const char* ValidateRegionsRequest(const geoproto::RegionsRequest& request)
{
   // Check if position is provided in the request.
   if (!request.has_position())
      return "Position must be set in RegionsRequest";

   // Check if preferences are provided in the request.
   if (!request.has_prefs())
      return "Preferences must be set in RegionsRequest";

   // Validate the latitude and longitude of the position.
   if (!geo::IsValidLatitude(request.position().latitude()))
      return "Wrong latitude in RegionsRequest";

   if (!geo::IsValidLongitude(request.position().longitude()))
      return "Wrong longitude in RegionsRequest";

   if (request.distance_km() > 1000)
      return "distance_km is out-of-range";

   if (request.prefs().mask() == geoproto::RegionsRequest::Preferences::GEOGRAPHICAL_FEATURE_UNSPECIFIED)
      return "At least one feature must be specified";

   if (request.prefs().mask() & geoproto::RegionsRequest::Preferences::GEOGRAPHICAL_FEATURE_PEAKS)
      if (request.prefs().properties().find("minPeakHeight") == request.prefs().properties().end())
         return "minPeakHeight is required for Peaks feature";

   return nullptr;
}

}  // namespace geo
