#ifndef YANDEX_MUSIC_APP_H
#define YANDEX_MUSIC_APP_H

#include "music_service.h"
#include "ui_components.h"
#include "app_config.h"
#include <memory>
#include <string>
// #include <stdexcept>

class YandexMusicApp {
private:
    std::shared_ptr<MusicService> musicService_;
    std::unique_ptr<MusicPlayerUI> ui_;
    AppConfig config_;
    std::string authToken_;

public:
    explicit YandexMusicApp(const AppConfig& config);
    explicit YandexMusicApp(const std::string& authToken);
    
    // Запрещаем копирование и присваивание
    YandexMusicApp(const YandexMusicApp&) = delete;
    YandexMusicApp& operator=(const YandexMusicApp&) = delete;
    
    // Основной метод запуска приложения
    void run();
    
    // Методы для управления жизненным циклом
    void initialize();
    void shutdown();

private:
    // Инициализация зависимостей
    void initializeDependencies();
    
    // Валидация входных данных
    void validateAuthToken() const;

    void initializeLogging();
    void validateConfig() const;
};

#endif
