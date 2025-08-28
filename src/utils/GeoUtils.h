#pragma once

#include <array>
#include <cstdint>
#include <vector>

namespace geo
{

// Checks if the latitude is valid (within the range of -90 to 90 degrees)
// @param lat Latitude value to check in degrees
// @return true if latitude is valid, false otherwise
bool IsValidLatitude(double lat);

// Checks if the longitude is valid (within the range of -180 to 180 degrees)
// @param lon Longitude value to check in degrees
// @return true if longitude is valid, false otherwise
bool IsValidLongitude(double lon);

// Type alias for a bounding box represented as [minLat, minLon, maxLat, maxLon]
using BoundingBox = std::array<double, 4>;

// Creates a bounding box around a given point with a specified range in meters
// @param latitude Center point latitude in degrees
// @param longitude Center point longitude in degrees
// @param rangeMeters Distance from center point to box edges in meters
// @return Bounding box as [minLat, minLon, maxLat, maxLon]
BoundingBox CreateBoundingBox(double latitude, double longitude, std::uint32_t rangeMeters);

// Creates a set of bounding boxes by splitting the main bounding box into smaller parts
// @param latitude Center point latitude in degrees
// @param longitude Center point longitude in degrees
// @param rangeMeters Distance from center point to edges of main box in meters
// @param maxBoxWidth Maximum width of each sub-box in degrees longitude
// @param maxBoxHeight Maximum height of each sub-box in degrees latitude
// @return Vector of bounding boxes covering the original area
std::vector<BoundingBox> CreateBoundingBoxes(
   double latitude, double longitude, std::uint32_t rangeMeters, std::uint32_t maxBoxWidth, std::uint32_t maxBoxHeight);

// Calculates the width and height of a bounding box in kilometers
// @param bbox Bounding box with min/max latitudes and longitudes in degrees
// @return Pair<double, double> containing width (longitude distance) and height (latitude distance) in kilometers
std::pair<double, double> GetBoundingBoxDimensionsKm(const BoundingBox& bbox);

}  // namespace geo
