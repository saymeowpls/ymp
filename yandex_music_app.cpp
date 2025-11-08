#include "yandex_music_app.h"
#include <iostream>
// #include <spdlog/spdlog.h> // Рекомендую добавить логирование

YandexMusicApp::YandexMusicApp(const std::string& authToken)
    : authToken_(authToken) {
    
    // Валидация токена при создании
    validateAuthToken();
    
    // Инициализация зависимостей
    initializeDependencies();
}

void YandexMusicApp::validateAuthToken() const {
    if (authToken_.empty()) {
        throw std::invalid_argument("Auth token cannot be empty");
    }
    
    // Можно добавить более сложную валидацию формата токена
    if (authToken_.length() < 10) { // Примерная проверка
        throw std::invalid_argument("Invalid auth token format");
    }
}

void YandexMusicApp::initializeDependencies() {
    try {
        // spdlog::info("Initializing Yandex Music App dependencies...");
        
        // Создаем зависимости через фабрику
        auto httpClient = AppFactory::createHttpClient();
        auto apiClient = AppFactory::createApiClient(authToken_);
        auto audioPlayer = AppFactory::createAudioPlayer();
        
        // Создаем сервисный слой
        musicService_ = std::make_shared<MusicService>(
            std::move(apiClient), 
            std::move(audioPlayer)
        );
        
        // Создаем UI
        ui_ = std::make_unique<MusicPlayerUI>(musicService_);
        
        // spdlog::info("Dependencies initialized successfully");
        
    } catch (const std::exception& e) {
        // spdlog::error("Failed to initialize dependencies: {}", e.what());
        throw; // Пробрасываем исключение дальше
    }
}

void YandexMusicApp::initialize() {
    try {
        // spdlog::info("Initializing Yandex Music App...");
        
        // Инициализация UI компонентов
        if (ui_) {
            ui_->initializeUI();
        }
        
        // Здесь можно добавить дополнительную инициализацию:
        // - Загрузка кешированных данных
        // - Восстановление состояния плеера
        // - Проверка соединения с API
        
        // spdlog::info("Yandex Music App initialized successfully");
        
    } catch (const std::exception& e) {
        // spdlog::error("Failed to initialize app: {}", e.what());
        throw;
    }
}

void YandexMusicApp::run() {
    try {
        // spdlog::info("Starting Yandex Music App...");
        
        // Убедимся, что приложение инициализировано
        if (!ui_) {
            throw std::runtime_error("UI not initialized");
        }
        
        // Запускаем главный UI цикл
        ui_->run();
        
    } catch (const std::exception& e) {
        // spdlog::critical("Critical error while running app: {}", e.what());
        std::cerr << "Critical error: " << e.what() << std::endl;
        throw;
    }
}

void YandexMusicApp::shutdown() {
    try {
        // spdlog::info("Shutting down Yandex Music App...");
        
        // Здесь можно добавить cleanup:
        // - Сохранение состояния
        // - Остановка аудиоплеера
        // - Закрытие сетевых соединений
        
        if (musicService_) {
            // Вызываем методы cleanup у сервиса, если они есть
        }
        
        // spdlog::info("Yandex Music App shutdown completed");
        
    } catch (const std::exception& e) {
        // spdlog::error("Error during shutdown: {}", e.what());
    }
}
