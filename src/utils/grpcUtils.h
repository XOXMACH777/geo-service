#pragma once

#include <string>

namespace grpc
{
class CallbackServerContext;
}  // namespace grpc

namespace geo
{

// Extracts client ID from gRPC request metadata
std::string ExtractClientId(grpc::CallbackServerContext& context);

}  // namespace geo
