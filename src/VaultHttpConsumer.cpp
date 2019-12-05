#include "json.hpp"
#include "VaultClient.h"

boost::optional<std::string>
VaultHttpConsumer::post(const VaultClient& client,
                        const Url& url,
                        Parameters parameters) {
  if (!client.is_authenticated()) {
    return boost::none;
  }

  nlohmann::json j = nlohmann::json::object();
  std::for_each(
    parameters.begin(),
    parameters.end(),
    [&](std::pair<std::string, std::string> pair) {
      j[pair.first] = pair.second;
    }
  );

  auto response =
    client.getHttpClient().post(
      url,
      client.getToken(),
      client.getNamespace(),
      j.dump()
    );

  return HttpClient::is_success(response)
    ? boost::optional<std::string>(response.value().body.value())
    : boost::none;
}
