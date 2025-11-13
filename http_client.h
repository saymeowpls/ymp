#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include "interfaces.h"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl/ssl_stream.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>
#include <string>
#include <map>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;

class BeastHttpClient : public IHttpClient {
public:
    BeastHttpClient();
    // explicit BeastHttpClient(int timeoutSeconds);
    ~BeastHttpClient() override;
    
    // Основные методы интерфейса
    std::string get(const std::string& url, 
                   const std::map<std::string, std::string>& headers = {}) override;
    std::string post(const std::string& url, 
                    const std::string& body,
                    const std::map<std::string, std::string>& headers = {}) override;
    
    // Дополнительные методы
    void setDefaultHeaders(const std::map<std::string, std::string>& headers);
    void printHeaders();
    void setTimeout(int seconds);
    void setMaxRedirects(int maxRedirects);
    
    // Статические утилиты
    static bool isValidUrl(const std::string& url);

private:
    net::io_context ioContext_;
    std::unique_ptr<ssl::context> sslContext_;
    int timeoutSeconds_;
    int maxRedirects_;
    std::map<std::string, std::string> defaultHeaders_;
    
    // Внутренние методы для выполнения запросов
    std::string performHttpRequest(const std::string& url,
                                  http::verb method,
                                  const std::string& body = "",
                                  const std::map<std::string, std::string>& headers = {});
    
    // Разбор URL
    struct ParsedUrl {
        std::string protocol;
        std::string host;
        std::string port;
        std::string target;
    };
    
    ParsedUrl parseUrl(const std::string& url) const;
    
    // Создание запроса
    http::request<http::string_body> createRequest(
        const ParsedUrl& parsedUrl,
        http::verb method,
        const std::string& body = "",
        const std::map<std::string, std::string>& headers = {}) const;
    
    // Выполнение HTTP запроса (без SSL)
    http::response<http::string_body> performHttp(
        const ParsedUrl& parsedUrl,
        const http::request<http::string_body>& request);
    
    // Выполнение HTTPS запроса (с SSL)
    http::response<http::string_body> performHttps(
        const ParsedUrl& parsedUrl,
        const http::request<http::string_body>& request);
    
    // Обработка редиректов
    std::string handleRedirect(const http::response<http::string_body>& response,
                              const std::string& originalUrl,
                              http::verb originalMethod,
                              const std::string& originalBody,
                              const std::map<std::string, std::string>& originalHeaders,
                              int redirectCount);
    
    // Обработка ошибок
    void handleError(const std::string& operation, const boost::system::error_code& ec);
    void checkResponse(const http::response<http::string_body>& response);
};

#endif
