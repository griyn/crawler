#include "ripper.h"

#include <cstdlib>
#include "common/log.h"

namespace flower {
namespace crawler {

class GlobalCurlInit {
public:
    GlobalCurlInit() {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }
    ~GlobalCurlInit() {
        curl_global_cleanup();
    }
    GlobalCurlInit(const GlobalCurlInit&) = delete;
    GlobalCurlInit(GlobalCurlInit&&) = delete;
};

static GlobalCurlInit global_curl_init;

Ripper::Ripper() {
    _curl = curl_easy_init();
    if (_curl == nullptr) {
        LOG(FATAL) << "_curl == nullptr";
        std::abort(); // Raise SIGABRT
    }
}

Ripper::~Ripper() {
    curl_easy_cleanup(_curl);
}

size_t write_header(void* buffer, size_t size, size_t nmemb, void* userp) {
    std::string* out = static_cast<std::string*>(userp);
    //将void* 指针转换成char* 指针， 并且读取对应长度
    out->append(static_cast<char*>(buffer), size * nmemb);
    return size * nmemb;
}

size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp) {
    std::string* out = static_cast<std::string*>(userp);
    //将void* 指针转换成char* 指针， 并且读取对应长度
    out->assign(static_cast<char*>(buffer), size * nmemb);
    return size * nmemb;
}

int Ripper::get(const std::string& url, std::string* header, std::string* body) {
    if (header == nullptr || body == nullptr) [[unlikely]] {
        LOG(FATAL) << "output nullptr";
        return -1;
    }

    curl_easy_reset(_curl);

    struct curl_slist* request_headers = NULL;
    request_headers = curl_slist_append(request_headers, 
            "User-Agent:Mozilla/5.0 (iPhone; CPU iPhone OS 13_2_3 like Mac OS X) AppleWebKit/605.1.15"
                " (KHTML, like Gecko) Version/13.0.3 Mobile/15E148 Safari/604.1");

    curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, request_headers);

    curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(_curl, CURLOPT_HTTPGET, 1L);

    curl_easy_setopt(_curl, CURLOPT_HEADERFUNCTION, &write_header);
    curl_easy_setopt(_curl, CURLOPT_HEADERDATA, header);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, &write_data);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, body);

    CURLcode res = curl_easy_perform(_curl);
    if (res != CURLE_OK) {
        curl_slist_free_all(request_headers);
        LOG(WARNING) << "curl_easy_perform() failed:" << curl_easy_strerror(res);
        return -1;
    }

    curl_slist_free_all(request_headers);

    return 0;
}

int Ripper::post(const std::string& url, const std::string& json, 
        std::string* header, std::string* body) {
    if (header == nullptr || body == nullptr) [[unlikely]] {
        LOG(FATAL) << "output nullptr";
        return -1;
    }

    curl_easy_reset(_curl);

    struct curl_slist* request_headers = NULL;
    request_headers = curl_slist_append(request_headers, 
            "Content-Type:application/json;charset=UTF-8");
    curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, request_headers);

    curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(_curl, CURLOPT_POST, 1L);
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, json.c_str());
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, json.size());

    curl_easy_setopt(_curl, CURLOPT_HEADERFUNCTION, &write_header);
    curl_easy_setopt(_curl, CURLOPT_HEADERDATA, header);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, &write_data);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, body);

    CURLcode res = curl_easy_perform(_curl);
    if (res != CURLE_OK) {
        curl_slist_free_all(request_headers);
        LOG(WARNING) << "curl_easy_perform() failed:" << curl_easy_strerror(res);
        return -1;
    }

    curl_slist_free_all(request_headers);

    return 0;
}

} // namepsace crawler
} // namespace flower