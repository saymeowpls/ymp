#include "music_service.h"
#include "ui_components.h"
#include "app_config.h"
#include "app_factory.h"

class YandexMusicApp {
private:
    std::shared_ptr<MusicService> musicService_;
    std::unique_ptr<MusicPlayerUI> ui_;
    AppConfig config_;

public:
    explicit YandexMusicApp(const AppConfig& config);
    // ... остальные методы

private:
    void initializeDependencies();
    void initializeLogging();
    void validateConfig() const;
};

// yandex_music_app.cpp (обновленная версия)
YandexMusicApp::YandexMusicApp(const AppConfig& config) 
    : config_(config) {
    
    validateConfig();
    initializeLogging();
    initializeDependencies();
}

void YandexMusicApp::initializeLogging() {
    // Настройка spdlog в соответствии с config_.logLevel
    // ...
}

void YandexMusicApp::validateConfig() const {
    if (!config_.isValid()) {
        throw std::invalid_argument("Invalid app configuration");
    }
}

void YandexMusicApp::initializeDependencies() {
    auto httpClient = AppFactory::createHttpClient(config_);
    auto apiClient = AppFactory::createApiClient(config_);
    auto audioPlayer = AppFactory::createAudioPlayer(config_);
    
    musicService_ = std::make_shared<MusicService>(
        std::move(apiClient), 
        std::move(audioPlayer)
    );
    
    ui_ = std::make_unique<MusicPlayerUI>(musicService_);

#ifndef YANDEX_MUSIC_APP_
#define YANDEX_MUSIC_APP_

// #include "music_service.h"
// #include "ui_components.h"
// #include "app_factory.h"
// #include <memory>
// #include <string>
// #include <stdexcept>
//
//
// class YandexMusicApp {
// private:
//     std::shared_ptr<MusicService> musicService_;
//     std::unique_ptr<MusicPlayerUI> ui_;
//     std::string authToken_;
// public:
//     // Конструктор принимает токен авторизации
//     explicit YandexMusicApp(const std::string& authToken);
//
//     // Запрещаем копирование и присваивание
//     YandexMusicApp(const YandexMusicApp&) = delete;
//     YandexMusicApp& operator=(const YandexMusicApp&) = delete;
//
//     // Основной метод запуска приложения
//     void run();
//
//     // Методы для управления жизненным циклом
//     void initialize();
//     void shutdown();
//
// private:
//     // Инициализация зависимостей
//     void initializeDependencies();
//
//     // Валидация входных данных
//     void validateAuthToken() const;
// };
//
// #endif
