#include "GeoUtils.h"

#include <cmath>

// From https://stackoverflow.com/a/74798098
//

namespace
{

// Minimum valid latitude value (-90 degrees)
const double sc_minLatitude = -90;
// Maximum valid latitude value (90 degrees)
const double sc_maxLatitude = 90;
// Minimum valid longitude value (-180 degrees)
const double sc_minLongitude = -180;
// Maximum valid longitude value (180 degrees)
const double sc_maxLongitude = 180;

// Converts degrees to radians
// @param deg Angle in degrees
// @return Angle in radians
double degreesToRadian(double deg)
{
   return deg * M_PI / 180.0;
}

// Converts radians to degrees
// @param rad Angle in radians
// @return Angle in degrees
double radianToDegrees(double rad)
{
   return rad * 180.0 / M_PI;
}

// Calculates the Earth's radius at a given latitude using the WGS84 model
// @param lat_radians Latitude in radians
// @return Earth's radius at given latitude in meters
double wgs84EarthRadius(double lat_radians)
{
   static const double WGS84_a = 6378137.0;  // Major semiaxis [m]
   static const double WGS84_b = 6356752.3;  // Minor semiaxis [m]
   double An = WGS84_a * WGS84_a * cos(lat_radians);
   double Bn = WGS84_b * WGS84_b * sin(lat_radians);
   double Ad = WGS84_a * cos(lat_radians);
   double Bd = WGS84_b * sin(lat_radians);

   return std::sqrt((An * An + Bn * Bn) / (Ad * Ad + Bd * Bd));
}

}  // namespace

namespace geo
{

bool IsValidLatitude(double lat)
{
   return lat >= sc_minLatitude && lat <= sc_maxLatitude;
}

bool IsValidLongitude(double lon)
{
   return lon >= sc_minLongitude && lon <= sc_maxLongitude;
}

BoundingBox CreateBoundingBox(double latitude, double longitude, uint32_t rangeMeters)
{
   double lat = degreesToRadian(latitude);
   double lon = degreesToRadian(longitude);

   uint32_t halfSide = rangeMeters;

   double radius = wgs84EarthRadius(lat);
   double pradius = radius * cos(lat);

   double latMin = lat - halfSide / radius;
   double latMax = lat + halfSide / radius;
   double lonMin = lon - halfSide / pradius;
   double lonMax = lon + halfSide / pradius;

   return {std::max(std::min(radianToDegrees(latMin), sc_maxLatitude), sc_minLatitude),
      std::max(std::min(radianToDegrees(lonMin), sc_maxLongitude), sc_minLongitude),
      std::max(std::min(radianToDegrees(latMax), sc_maxLatitude), sc_minLatitude),
      std::max(std::min(radianToDegrees(lonMax), sc_maxLongitude), sc_minLongitude)};
}

std::vector<BoundingBox> CreateBoundingBoxes(
   double latitude, double longitude, std::uint32_t rangeMeters, std::uint32_t maxBoxWidth, std::uint32_t maxBoxHeight)
{
   const auto fullBox = CreateBoundingBox(latitude, longitude, rangeMeters);
   std::vector<std::array<double, 2>> xSplits;
   for (double x = fullBox[0]; x < fullBox[2]; x += maxBoxWidth)
   {
      const auto left = x;
      const auto right = std::min(x + maxBoxWidth, fullBox[2]);
      xSplits.push_back({left, right});
   }

   std::vector<std::array<double, 2>> ySplits;
   for (double y = fullBox[1]; y < fullBox[3]; y += maxBoxHeight)
   {
      const auto left = y;
      const auto right = std::min(y + maxBoxHeight, fullBox[3]);
      ySplits.push_back({left, right});
   }

   std::vector<BoundingBox> v;
   for (const auto& x : xSplits)
      for (const auto& y : ySplits)
         v.push_back({x[0], y[0], x[1], y[1]});
   return v;
}

std::pair<double, double> GetBoundingBoxDimensionsKm(const BoundingBox& bbox)
{
   // Convert degrees to radians
   double latMinRad = degreesToRadian(bbox[0]);
   double latMaxRad = degreesToRadian(bbox[2]);
   double lonMinRad = degreesToRadian(bbox[1]);
   double lonMaxRad = degreesToRadian(bbox[3]);

   // Compute midpoint latitude (radians) for Earth radius calculation
   double latMidRad = (latMinRad + latMaxRad) / 2.0;

   // Earth radius at mid-latitude (meters)
   double radius = wgs84EarthRadius(latMidRad);

   // Height calculation (distance between latitudes)
   double heightMeters = radius * (latMaxRad - latMinRad);

   // Normalize longitude difference to handle wrapping over ±180°
   double lonDiff = lonMaxRad - lonMinRad;
   if (lonDiff < 0)
      lonDiff += 2 * M_PI;

   // Clamp longitude difference to max PI (180°) to avoid overestimation
   if (lonDiff > M_PI)
      lonDiff = 2 * M_PI - lonDiff;

   // Radius at given latitude projected to longitude circle (meters)
   double pradius = radius * cos(latMidRad);

   // Width calculation (distance between longitudes)
   double widthMeters = pradius * lonDiff;

   // Convert meters to kilometers
   double heightKm = heightMeters / 1000.0;
   double widthKm = widthMeters / 1000.0;

   return std::make_pair(widthKm, heightKm);
}

}  // namespace geo
