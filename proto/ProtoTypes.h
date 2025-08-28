#pragma once

#include "geo.pb.h"

#include <vector>

namespace geo
{

using GeoProtoTaggedFeature = geoproto::Place::TaggedFeature;
using GeoProtoTaggedFeatures = std::vector<GeoProtoTaggedFeature>;
using GeoProtoPlace = geoproto::Place;
using GeoProtoPlaces = std::vector<GeoProtoPlace>;

}  // namespace geo
