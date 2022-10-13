#include "crawler.h"

#include <sstream>
#include "common/protobuf_utils.h"

namespace flower {
namespace crawler {

int Crawler::request_get_message(const std::string& url, 
        google::protobuf::Message* message) {
    if (message == nullptr) [[unlikely]] {
        return -1;
    }

    std::string header;
    std::string body;
    if (_ripper.get(url, &header, &body) != 0) {
        LOG(WARNING) << "request_author_stat failed, url:" << url;
        return -1;
    }

    // todo:check header

    if (::garden::protobuf_utils::json2protobuf(body, message) != 0) {
        return -1;
    }

    LOG(TRACE) << "request url:" << url 
            << " header:" << header << " body:" << body;

    return 0;
}

int Crawler::save(const std::string& index, 
        const std::string& key, const google::protobuf::Message& message) {
    std::string save_json;
    if (::garden::protobuf_utils::protobuf2json(message, &save_json) != 0) {
        return -1;
    }

    return save(index, key, save_json);
}

int Crawler::save(const std::string& index, 
        const std::string& key, const std::string& json) {
    std::stringstream ss;
    ss << "http://localhost:9200/" << index << "/_doc/" << key;
    std::string doc = ss.str();
    std::string header, body;
    if (_ripper.post(doc, json, &header, &body) != 0) {
        return -1;
    }

    // todo:check header

    LOG(TRACE) << "save, key:" << key << " post.json:" << json
            << " header:" << header << " body:" << body;

    return 0;
}

} // crawler
} // flower