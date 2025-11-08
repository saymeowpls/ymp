#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include "interfaces.h"
#include <boost/asio.hpp>
#include <boost/beast.hpp>

class BeastHttpClient : public IHttpClient {
public:
    BeastHttpClient();
    ~BeastHttpClient();
    
    std::string get(const std::string& url, 
                   const std::map<std::string, std::string>& headers = {}) override;
    std::string post(const std::string& url, 
                    const std::string& body,
                    const std::map<std::string, std::string>& headers = {}) override;
    
private:
    boost::asio::io_context ioContext_;
};

#endif
