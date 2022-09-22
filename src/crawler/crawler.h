#pragma once

#include "proto/configs.pb.h"

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

private:
    std::string _name;
};

} // namespace crawler
} // namespace flower 