#include "app_factory.h"
#include "http_client.h"
#include "interfaces.h"
#include <stdexcept>
#include <iostream>

// Заглушка для аудиоплеера (временная реализация)
class StubAudioPlayer : public IAudioPlayer {
public:
    void play(const Track& track) override {
        std::cout << "[AUDIO] Playing: " << track.title << " by ";
        for (const auto& artist : track.artists) {
            std::cout << artist << " ";
        }
        std::cout << std::endl;
    }
    
    void pause() override {
        std::cout << "[AUDIO] Paused" << std::endl;
    }
    
    void resume() override {
        std::cout << "[AUDIO] Resumed" << std::endl;
    }
    
    void stop() override {
        std::cout << "[AUDIO] Stopped" << std::endl;
    }
    
    void setVolume(int volume) override {
        std::cout << "[AUDIO] Volume set to: " << volume << std::endl;
    }
};

// Реализация статических методов AppFactory

// std::unique_ptr<IHttpClient> AppFactory::createHttpClient() {
    // return createBeastHttpClient();
// }

// std::unique_ptr<IMusicApiClient> AppFactory::createApiClient(const std::string& authToken) {
    // auto httpClient = createHttpClient();
    // return createYandexMusicApiClient(authToken, std::move(httpClient));
// }

// std::unique_ptr<IAudioPlayer> AppFactory::createAudioPlayer() {
    // return createStubAudioPlayer();
// }

std::unique_ptr<IHttpClient> AppFactory::createHttpClient(const AppConfig& config) {
    return createBeastHttpClient(&config);
}

// std::unique_ptr<IMusicApiClient> AppFactory::createApiClient(const AppConfig& config) {
    // auto httpClient = createHttpClient(config);
    // return createYandexMusicApiClient(config.authToken, std::move(httpClient), &config);
// }

std::unique_ptr<IAudioPlayer> AppFactory::createAudioPlayer(const AppConfig& config) {
    // Пока используем заглушку, но можно добавить логику выбора на основе конфигурации
    return createStubAudioPlayer();
}

// Реализация методов интерфейса IAppFactory

// std::unique_ptr<IHttpClient> AppFactory::createHttpClient() {
    // return createBeastHttpClient();
// }

// std::unique_ptr<IMusicApiClient> AppFactory::createApiClient(const std::string& authToken) {
    // auto httpClient = createHttpClient();
    // return createYandexMusicApiClient(authToken, std::move(httpClient));
// }

// std::unique_ptr<IAudioPlayer> AppFactory::createAudioPlayer() {
    // return createStubAudioPlayer();
// }

bool AppFactory::canCreateHttpClient() const {
    return isBeastAvailable();
}

bool AppFactory::canCreateAudioPlayer() const {
    return isAudioAvailable();
}

// Приватные вспомогательные методы

std::unique_ptr<IHttpClient> AppFactory::createBeastHttpClient(const AppConfig* config) {
    try {
        auto client = std::make_unique<BeastHttpClient>();
        // Здесь можно применить конфигурацию, если она предоставлена
        if (config) {
            std::map<std::string, std::string> headers = {
                {"Authorization", "OAuth "}, // TODO: fix
                {"Accept", "application/json"},
                {"Content-Type", "application/json"}
                // {"User-Agent", "Yandex-Music-API"}
            };
            client->setDefaultHeaders(headers);
            client->printHeaders();
            // Настройка таймаутов, повторных попыток и т.д.
            // Например: client->setTimeout(config->httpTimeoutSeconds);
        }
        
        return client;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to create HTTP client: " + std::string(e.what()));
    }
}

// std::unique_ptr<IMusicApiClient> AppFactory::createYandexMusicApiClient(
//     const std::string& authToken, 
//     std::unique_ptr<IHttpClient> httpClient,
//     const AppConfig* config
// ) {
//     if (authToken.empty()) {
//         throw std::invalid_argument("Auth token cannot be empty");
//     }
//
//     if (!httpClient) {
//         throw std::invalid_argument("HTTP client cannot be null");
//     }
//
//     try {
//         auto apiClient = std::make_unique<YandexMusicApiClient>(
//             authToken, 
//             std::move(httpClient)
//         );
//
//         // Применяем конфигурацию, если предоставлена
//         if (config) {
//             // Например: apiClient->setBaseUrl(config->apiBaseUrl);
//         }
//
//         return apiClient;
//     } catch (const std::exception& e) {
//         throw std::runtime_error("Failed to create API client: " + std::string(e.what()));
//     }
// }

std::unique_ptr<IAudioPlayer> AppFactory::createStubAudioPlayer() {
    return std::make_unique<StubAudioPlayer>();
}

bool AppFactory::isBeastAvailable() {
    // Простая проверка доступности Boost.Beast
    // В реальном проекте можно сделать более сложную проверку
    try {
        // Пробуем создать простой объект Boost.Asio
        boost::asio::io_context ctx;
        return true;
    } catch (...) {
        return false;
    }
}

bool AppFactory::isAudioAvailable() {
    // Проверка доступности аудио подсистемы
    // Пока всегда возвращаем true, так как используем заглушку
    return true;
}

// Реализация TestAppFactory (только если включено тестирование)
#ifdef ENABLE_TESTING

#include <gmock/gmock.h>

class MockHttpClient : public IHttpClient {
public:
    MOCK_METHOD(std::string, get, (const std::string& url, const std::map<std::string, std::string>& headers), (override));
    MOCK_METHOD(std::string, post, (const std::string& url, const std::string& body, const std::map<std::string, std::string>& headers), (override));
};

class MockAudioPlayer : public IAudioPlayer {
public:
    MOCK_METHOD(void, play, (const Track& track), (override));
    MOCK_METHOD(void, pause, (), (override));
    MOCK_METHOD(void, resume, (), (override));
    MOCK_METHOD(void, stop, (), (override));
    MOCK_METHOD(void, setVolume, (int volume), (override));
};

std::unique_ptr<IHttpClient> TestAppFactory::createHttpClient() {
    auto mockClient = std::make_unique<MockHttpClient>();
    
    // Настраиваем поведение по умолчанию
    if (!mockHttpResponse_.empty()) {
        ON_CALL(*mockClient, get(testing::_, testing::_))
            .WillByDefault(testing::Return(mockHttpResponse_));
    }
    
    return mockClient;
}

std::unique_ptr<IMusicApiClient> TestAppFactory::createApiClient(const std::string& authToken) {
    // Для тестов можно вернуть mock или реальный клиент с mock HTTP клиентом
    auto httpClient = createHttpClient();
    return std::make_unique<YandexMusicApiClient>(authToken, std::move(httpClient));
}

std::unique_ptr<IAudioPlayer> TestAppFactory::createAudioPlayer() {
    auto mockPlayer = std::make_unique<MockAudioPlayer>();
    
    if (mockAudioCanPlay_) {
        ON_CALL(*mockPlayer, play(testing::_))
            .WillByDefault(testing::Return());
    }
    
    return mockPlayer;
}

void TestAppFactory::setMockHttpResponse(const std::string& response) {
    mockHttpResponse_ = response;
}

void TestAppFactory::setMockAudioState(bool canPlay) {
    mockAudioCanPlay_ = canPlay;
}

#endif
