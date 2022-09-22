#pragma once

#include "crawler/crawler.h"
#include "ripper/ripper.h"

namespace flower {
namespace crawler {

class BilibiliCrawler : public Crawler {
public:
    int init(const CrawlerConfig& config) override;
    int run() override;

private:
    int run(const std::string& uid);
    int request_http_get(const std::string& url, 
            ::flower::crawler::Ripper& ripper, std::string* body);
    int save(const std::string& key, const std::string& json,
            ::flower::crawler::Ripper& ripper);

private:
    std::set<std::string> _uids;
};

} // namespace crawler
} // namespace flower 