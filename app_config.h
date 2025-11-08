#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <string>

struct AppConfig {
    std::string authToken;
    std::string apiBaseUrl = "https://api.music.yandex.net";
    int httpTimeoutSeconds = 30;
    int maxRetries = 3;
    std::string logLevel = "info";
    bool enableCache = true;
    
    // Валидация конфигурации
    bool isValid() const {
        return !authToken.empty() && 
               !apiBaseUrl.empty() && 
               httpTimeoutSeconds > 0 &&
               maxRetries >= 0;
    }
};

#endif
