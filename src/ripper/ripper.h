#pragma once

#include <curl/curl.h>
#include <string>

namespace flower {
namespace crawler {

// http请求工具，curl 的封装

class Ripper {
public:
    Ripper();
    virtual ~Ripper();

    int get(const std::string& url, std::string* header, std::string* body);
    int post(const std::string& url, const std::string& json, 
            std::string* header, std::string* body);

private:
    CURL* _curl;
};

} // crawler
} // flower