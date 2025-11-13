#include "http_client.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>

// Константы
constexpr int DEFAULT_TIMEOUT = 30;
constexpr int DEFAULT_MAX_REDIRECTS = 5;
constexpr const char* DEFAULT_HTTP_PORT = "80";
constexpr const char* DEFAULT_HTTPS_PORT = "443";

BeastHttpClient::BeastHttpClient() 
    : timeoutSeconds_(DEFAULT_TIMEOUT)
    , maxRedirects_(DEFAULT_MAX_REDIRECTS) {


    std::cout << "beast ctor" << std::endl;
      // std::map<std::string, std::string> headers = {
          // {"Authorization", "OAuth "}, // TODO: fix
          // {"Accept", "application/json"},
          // {"Content-Type", "application/json"},
          // {"User-Agent", "Yandex-Music-API"}
      // };
    // std::cout << " headers: " << headers["User-Agent"] << std::endl;
    // setDefaultHeaders(headers);
    
    try {
        sslContext_ = std::make_unique<ssl::context>(ssl::context::tls_client);
        sslContext_->set_verify_mode(ssl::verify_peer);
        sslContext_->set_default_verify_paths();
    } catch (const std::exception& e) {
        std::cerr << "Warning: Failed to initialize SSL context: " << e.what() << std::endl;
        sslContext_.reset();
    }
}

// BeastHttpClient::BeastHttpClient(int timeoutSeconds)
//     : timeoutSeconds_(timeoutSeconds)
//     , maxRedirects_(DEFAULT_MAX_REDIRECTS) {
//
//     try {
//         sslContext_ = std::make_unique<ssl::context>(ssl::context::tls_client);
//         sslContext_->set_verify_mode(ssl::verify_peer);
//         sslContext_->set_default_verify_paths();
//     } catch (const std::exception& e) {
//         std::cerr << "Warning: Failed to initialize SSL context: " << e.what() << std::endl;
//         sslContext_.reset();
//     }
// }

BeastHttpClient::~BeastHttpClient() {
    // Деструктор должен быть в .cpp файле из-за unique_ptr с неполным типом
}

std::string BeastHttpClient::get(const std::string& url, 
                                const std::map<std::string, std::string>& headers) {
    std::cout << "BeastHttpClient::get()" << std::endl;
    return performHttpRequest(url, http::verb::get, "", headers);
}

std::string BeastHttpClient::post(const std::string& url, 
                                 const std::string& body,
                                 const std::map<std::string, std::string>& headers) {
    // std::cout << "BeastHttpClient::post() body: " << body << " headers: " << headers["User-Agent"] << std::endl;
    printHeaders();
    return performHttpRequest(url, http::verb::post, body, headers);
}

void BeastHttpClient::setDefaultHeaders(const std::map<std::string, std::string>& headers) {
    defaultHeaders_ = headers;
}

void BeastHttpClient::printHeaders() {
  for (const auto& pair : defaultHeaders_)
  {
      std::cout << pair.first << pair.second << std::endl;
  }
}

void BeastHttpClient::setTimeout(int seconds) {
    timeoutSeconds_ = seconds;
}

void BeastHttpClient::setMaxRedirects(int maxRedirects) {
    maxRedirects_ = maxRedirects;
}

bool BeastHttpClient::isValidUrl(const std::string& url) {
    return url.find("http://") == 0 || url.find("https://") == 0;
}

// Основной метод выполнения HTTP запроса
std::string BeastHttpClient::performHttpRequest(const std::string& url,
                                               http::verb method,
                                               const std::string& body,
                                               const std::map<std::string, std::string>& headers) {
    if (!isValidUrl(url)) {
        throw std::invalid_argument("Invalid URL: " + url);
    }
    std::cout << "performHttpRequest " << url << std::endl;
    
    // std::cout << "BeastHttpClient::post() body: " << body << " headers: " << static_cast<std::map<std::string, std::string>>(headers)["User-Agent"] << "ALl" << std::endl;
    try {
        auto parsedUrl = parseUrl(url);
        auto request = createRequest(parsedUrl, method, body, headers);
        
        http::response<http::string_body> response;
        
        if (parsedUrl.protocol == "https") {
            if (!sslContext_) {
                throw std::runtime_error("HTTPS not supported: SSL context not available");
            }
            response = performHttps(parsedUrl, request);
        } else {
            response = performHttp(parsedUrl, request);
        }
        
        // Проверяем статус ответа
        checkResponse(response);
        
        // Обрабатываем редиректы
        if (response.result() == http::status::moved_permanently ||
            response.result() == http::status::found ||
            response.result() == http::status::see_other ||
            response.result() == http::status::temporary_redirect ||
            response.result() == http::status::permanent_redirect) {
            
            auto location = response.base()["Location"];
            if (!location.empty()) {
                return handleRedirect(response, url, method, body, headers, 0);
            }
        }
        
        return response.body();
        
    } catch (const std::exception& e) {
        throw std::runtime_error("HTTP request failed: " + std::string(e.what()));
    }
}

// Разбор URL
BeastHttpClient::ParsedUrl BeastHttpClient::parseUrl(const std::string& url) const {
    ParsedUrl result;
    
    // Определяем протокол
    if (url.find("https://") == 0) {
        result.protocol = "https";
        result.port = DEFAULT_HTTPS_PORT;
    } else {
        result.protocol = "http";
        result.port = DEFAULT_HTTP_PORT;
    }
    
    // Извлекаем host и target
    size_t protocolEnd = url.find("://") + 3;
    size_t hostEnd = url.find('/', protocolEnd);
    
    if (hostEnd == std::string::npos) {
        result.host = url.substr(protocolEnd);
        result.target = "/";
    } else {
        result.host = url.substr(protocolEnd, hostEnd - protocolEnd);
        result.target = url.substr(hostEnd);
    }
    
    // Проверяем наличие порта в host
    size_t portPos = result.host.find(':');
    if (portPos != std::string::npos) {
        result.port = result.host.substr(portPos + 1);
        result.host = result.host.substr(0, portPos);
    }
    
    return result;
}

// Создание HTTP запроса
http::request<http::string_body> BeastHttpClient::createRequest(
    const ParsedUrl& parsedUrl,
    http::verb method,
    const std::string& body,
    const std::map<std::string, std::string>& headers) const {
    
    http::request<http::string_body> request{method, parsedUrl.target, 11}; // HTTP 1.1
    
    // Устанавливаем стандартные заголовки
    request.set(http::field::host, parsedUrl.host);
    request.set(http::field::user_agent, "YandexMusicClient/1.0");
    
    if (method == http::verb::post) {
        request.set(http::field::content_type, "application/json");
        request.body() = body;
        request.prepare_payload();
    }
    
    // Добавляем пользовательские заголовки
    for (const auto& [key, value] : headers) {
        request.set(key, value);
    }
    
    // Добавляем заголовки по умолчанию
    for (const auto& [key, value] : defaultHeaders_) {
        if (!request[key].empty()) continue; // Не перезаписываем существующие
        request.set(key, value);
    }
    
    return request;
}

// Выполнение HTTP запроса
http::response<http::string_body> BeastHttpClient::performHttp(
    const ParsedUrl& parsedUrl,
    const http::request<http::string_body>& request) {
    
    beast::tcp_stream stream(ioContext_);
    
    // Разрешаем доменное имя
    net::ip::tcp::resolver resolver(ioContext_);
    auto const results = resolver.resolve(parsedUrl.host, parsedUrl.port);
    
    // Устанавливаем таймаут
    stream.expires_after(std::chrono::seconds(timeoutSeconds_));
    
    // Устанавливаем соединение
    stream.connect(results);
    
    // Отправляем запрос
    http::write(stream, request);
    
    // Получаем ответ
    beast::flat_buffer buffer;
    http::response<http::string_body> response;
    http::read(stream, buffer, response);
    
    // Закрываем соединение
    beast::error_code ec;
    stream.socket().shutdown(net::ip::tcp::socket::shutdown_both, ec);
    
    return response;
}

// Выполнение HTTPS запроса
http::response<http::string_body> BeastHttpClient::performHttps(
    const ParsedUrl& parsedUrl,
    const http::request<http::string_body>& request) {
    
    if (!sslContext_) {
        throw std::runtime_error("SSL context not available");
    }
    
    beast::ssl_stream<beast::tcp_stream> stream(ioContext_, *sslContext_);
    
    // Устанавливаем SNI Hostname
    if (!SSL_set_tlsext_host_name(stream.native_handle(), parsedUrl.host.c_str())) {
        beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
        throw beast::system_error{ec};
    }
    
    // Разрешаем доменное имя
    net::ip::tcp::resolver resolver(ioContext_);
    auto const results = resolver.resolve(parsedUrl.host, parsedUrl.port);
    
    // Устанавливаем таймаут
    beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(timeoutSeconds_));
    
    // Устанавливаем соединение
    beast::get_lowest_layer(stream).connect(results);
    
    // Выполняем SSL handshake
    stream.handshake(ssl::stream_base::client);
    
    // Отправляем запрос
    http::write(stream, request);
    
    // Получаем ответ
    beast::flat_buffer buffer;
    http::response<http::string_body> response;
    http::read(stream, buffer, response);
    
    // Закрываем соединение
    beast::error_code ec;
    stream.shutdown(ec);
    
    return response;
}

// Обработка редиректов
std::string BeastHttpClient::handleRedirect(const http::response<http::string_body>& response,
                                           const std::string& originalUrl,
                                           http::verb originalMethod,
                                           const std::string& originalBody,
                                           const std::map<std::string, std::string>& originalHeaders,
                                           int redirectCount) {
    
    if (redirectCount >= maxRedirects_) {
        throw std::runtime_error("Too many redirects");
    }
    
    auto location = response.base()["Location"]; // TODO: fix
    std::string location2 = std::string(location.data(), location.length());
    
    // Для некоторых типов редиректов меняем метод на GET
    http::verb newMethod = originalMethod;
    std::string newBody = originalBody;
    
    if (response.result() == http::status::see_other) {
        newMethod = http::verb::get;
        newBody = "";
    }
    
    return performHttpRequest(location2, newMethod, newBody, originalHeaders);
}

// Обработка ошибок
void BeastHttpClient::handleError(const std::string& operation, const boost::system::error_code& ec) {
    std::stringstream ss;
    ss << operation << " failed: " << ec.message() << " (" << ec.value() << ")";
    throw std::runtime_error(ss.str());
}

void BeastHttpClient::checkResponse(const http::response<http::string_body>& response) {
    auto status = response.result();
    
    if (status == http::status::ok || 
        status == http::status::created ||
        status == http::status::accepted) {
        return; // Успешные статусы
    }
    
    // Для редиректов не бросаем исключение - они обрабатываются отдельно
    if (status == http::status::moved_permanently ||
        status == http::status::found ||
        status == http::status::see_other ||
        status == http::status::temporary_redirect ||
        status == http::status::permanent_redirect) {
        return;
    }
    
    std::stringstream ss;
    ss << "HTTP error: " << status << " - " << response.reason();
    throw std::runtime_error(ss.str());
}
