#pragma once

#include "crawler/crawler.h"
#include "proto/configs.pb.h"
#include <memory>

namespace flower {
namespace crawler {

// 爬虫抓取总调度器

class HiveMind {
public:
    int init(const std::string& file);
    int init(const HiveMindConfig& config);
    int run();

private:
    std::string _name;
    std::map<std::string, std::unique_ptr<Crawler>> _crawlers;
};

} // namespace crawler
} // namespace flower 