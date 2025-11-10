#ifndef APP_FACTORY_H
#define APP_FACTORY_H

#include "interfaces.h"
#include "yandex_api_client.h"
#include "http_client.h"
#include "app_config.h"
#include <memory>
#include <string>

// Абстрактная фабрика для создания зависимостей
class IAppFactory {
public:
    virtual ~IAppFactory() = default;
    
    virtual std::unique_ptr<IHttpClient> createHttpClient() = 0;
    virtual std::unique_ptr<IMusicApiClient> createApiClient(const std::string& authToken) = 0;
    virtual std::unique_ptr<IAudioPlayer> createAudioPlayer() = 0;
    
    // Валидация возможности создания зависимостей
    virtual bool canCreateHttpClient() const = 0;
    virtual bool canCreateAudioPlayer() const = 0;
};

// Конкретная реализация фабрики
class AppFactory : public IAppFactory {
public:
    // static std::unique_ptr<IHttpClient> createHttpClient();
    // static std::unique_ptr<IMusicApiClient> createApiClient(const std::string& authToken);
    // static std::unique_ptr<IAudioPlayer> createAudioPlayer();
    
    // Фабричные методы с конфигурацией
    static std::unique_ptr<IHttpClient> createHttpClient(const AppConfig& config);
    static std::unique_ptr<IMusicApiClient> createApiClient(const AppConfig& config);
    static std::unique_ptr<IAudioPlayer> createAudioPlayer(const AppConfig& config);
    
    // Реализация интерфейса IAppFactory
    // std::unique_ptr<IHttpClient> createHttpClient() override;
    // std::unique_ptr<IMusicApiClient> createApiClient(const std::string& authToken) override;
    // std::unique_ptr<IAudioPlayer> createAudioPlayer() override;
    
    bool canCreateHttpClient() const override;
    bool canCreateAudioPlayer() const override;

private:
    // Вспомогательные методы для создания конкретных реализаций
    static std::unique_ptr<IHttpClient> createBeastHttpClient(const AppConfig* config = nullptr);
    static std::unique_ptr<IMusicApiClient> createYandexMusicApiClient(
        const std::string& authToken, 
        std::unique_ptr<IHttpClient> httpClient,
        const AppConfig* config = nullptr
    );
    static std::unique_ptr<IAudioPlayer> createStubAudioPlayer();
    
    // Проверка доступности зависимостей
    static bool isBeastAvailable();
    static bool isAudioAvailable();
};

// Фабрика для тестов (может создавать mock-объекты)
#ifdef ENABLE_TESTING
class TestAppFactory : public IAppFactory {
public:
    std::unique_ptr<IHttpClient> createHttpClient() override;
    std::unique_ptr<IMusicApiClient> createApiClient(const std::string& authToken) override;
    std::unique_ptr<IAudioPlayer> createAudioPlayer() override;
    
    bool canCreateHttpClient() const override { return true; }
    bool canCreateAudioPlayer() const override { return true; }
    
    // Методы для настройки mock-объектов
    void setMockHttpResponse(const std::string& response);
    void setMockAudioState(bool canPlay);
    
private:
    std::string mockHttpResponse_;
    bool mockAudioCanPlay_ = true;
};
#endif

#endif
