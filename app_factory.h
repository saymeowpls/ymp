#ifndef APP_FACTORY_H
#define APP_FACTORY_H

#include "interfaces.h"
#include "app_config.h"
#include <memory>
#include <string>

class AppFactory {
public:
    static std::unique_ptr<IMusicApiClient> createApiClient(AppConfig& config);
    static std::unique_ptr<IAudioPlayer> createAudioPlayer(AppConfig& config);
    static std::unique_ptr<IHttpClient> createHttpClient(AppConfig& config);
};

#endif
