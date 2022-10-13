#pragma once

#include "crawler/crawler.h"

namespace flower {
namespace crawler {

class BilibiliCrawler : public Crawler {
public:
    int init(const CrawlerConfig& config) override;
    int run() override;

private:
    int run(const std::string& uid);

private:
    std::set<std::string> _uids;
};

} // namespace crawler
} // namespace flower 