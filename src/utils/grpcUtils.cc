#include "grpcUtils.h"

#include <grpcpp/server_context.h>

namespace geo
{

std::string ExtractClientId(grpc::CallbackServerContext& context)
{
   auto& md = context.client_metadata();            // Get reference to client metadata map
   auto it = md.find("client-id");                  // Look for "client-id" key in metadata
   return it == md.end() ? "" : it->second.data();  // Return empty string if not found, otherwise client ID value
}

}  // namespace geo
