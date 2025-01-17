#include <utility>
#include "VaultClient.h"

VaultClient::VaultClient(VaultConfig& config,
                         AuthenticationStrategy& authStrategy)
  : host_(config.getHost())
  , port_(config.getPort())
  , namespace_(config.getNamespace())
  , tls_(config.getTls())
  , authStrategy_(authStrategy)
  , httpClient_(HttpClient(config))
  , debug_(config.getDebug())
{
  auto result = authStrategy_.authenticate(*this);
  if (result) {
    token_ = result.value().token;
  }
}

VaultClient::VaultClient(VaultConfig& config,
                         AuthenticationStrategy& authStrategy,
                         HttpErrorCallback httpErrorCallback)
  : host_(config.getHost())
  , port_(config.getPort())
  , namespace_(config.getNamespace())
  , tls_(config.getTls())
  , authStrategy_(authStrategy)
  , httpClient_(HttpClient(config, std::move(httpErrorCallback)))
  , debug_(config.getDebug())
{
  auto result = authStrategy_.authenticate(*this);
  if (result) {
    token_ = result.value().token;
  }
}

VaultClient::VaultClient(const VaultClient& other, const Token& token)
    : host_(other.getHost())
    , port_(other.getPort())
    , namespace_(other.getNamespace())
    , tls_(other.getTls())
    , authStrategy_(other.getAuthenticationStrategy())
    , httpClient_(other.getHttpClient())
    , debug_(other.getDebug())
    , token_(token)
    {}

Url VaultClient::getUrl(const std::string& base, const Path& path) const {
  return Url{(tls_ ? "https://" : "http://") + host_ + ":" + port_ + base + path};
}
