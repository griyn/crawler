#pragma once

#include "proto/configs.pb.h"
#include "ripper/ripper.h"
#include "common/log.h"

namespace flower {
namespace crawler {

// 抓取接口，由nest统一调用

class Crawler {
public:
    virtual ~Crawler() {}

    virtual int init(const CrawlerConfig& config) = 0;
    virtual int run() = 0;

    void set_name(const std::string& name) {
        _name = name;
    }

    const std::string& name() {
        return _name;
    }

    // 基类crawler提供的一些基础函数
    // Get请求，把返回body并解析成message
    int request_get_message(const std::string& url, 
            google::protobuf::Message* message);

    // 默认写存储方法，使用es
    int save(const std::string& index, 
            const std::string& key, const google::protobuf::Message& message);

    int save(const std::string& index, 
            const std::string& key, const std::string& json);

private:
    std::string _name;
    ::flower::crawler::Ripper _ripper;
};

} // namespace crawler
} // namespace flower 