#include "crawler/bilibili_crawler.h"
#include "common/log.h"
#include "common/common.h"
#include "common/protobuf_utils.h"
#include "proto/bilibili.pb.h"
#include "common/app_context.h"

namespace flower {
namespace crawler {

APPCONTEXT_REGISTER(BilibiliCrawler, Crawler);

int BilibiliCrawler::init(const CrawlerConfig& config) {
    if (!config.has_bilibili_config()) {
        LOG(FATAL) << "do not has special config:" << config.ShortDebugString();
        return -1;
    }
    auto& biliconfig = config.bilibili_config();

    if (biliconfig.uids_size() <= 0) {
        LOG(FATAL) << "need uids" << config.ShortDebugString();
        return -1;
    }

    for (int i = 0; i < biliconfig.uids_size(); ++i) {
        _uids.emplace(biliconfig.uids(i));
    }

    return 0;
}

int BilibiliCrawler::run() {
    for (auto& uid : _uids) {
        run(uid);
    }

    return 0;
}

int BilibiliCrawler::run(const std::string& uid) {
    ::flower::crawler::Ripper ripper;

    // 请求
    std::string out;
    if (request_author_stat(uid, ripper, &out) != 0) {
        return -1;
    }

    // 解析
    auto* from_author_stat = new ::flower::crawler::bilibili::FromAuthorStat;
    if (::garden::protobuf_utils::json2protobuf(out, from_author_stat) != 0) {
        return -1;
    }
/*
    // 生成写库数据
    ::flower::crawler::bilibili::Save save_data;
    save_data.set_uid(uid);
    save_data.set_date(::garden::common::get_time_of_day_d());
    // save.set_name("嘉然今天吃什么"); todo记录用户名
    save_data.set_allocated_from_author_stat(from_author_stat);

    std::string save_json;
    if (::garden::protobuf_utils::protobuf2json(save_data, &save_json) != 0) {
        return -1;
    }

    // 写库
    if (save(uid + "@" + save_data.date(), save_json, ripper) != 0) {
        return -1;
    }
*/
    return 0;
}

int BilibiliCrawler::request_author_stat(const std::string& uid, 
        ::flower::crawler::Ripper& ripper, std::string* body) {
    if (body == nullptr) [[unlikely]] {
        return -1;
    }

    static std::string url_author_stat = "https://api.bilibili.com/x/relation/stat?vmid=";
    std::string url = url_author_stat + uid;
    std::string header;
    if (ripper.get(url, &header, body) != 0) {
        LOG(WARNING) << "request_author_stat failed, uid:" << uid;
        return -1;
    }

    // todo:check header

    LOG(TRACE) << "request_author_stat, uid:" << uid 
            << " header:" << header << " body:" << *body;

    return 0;
}

int BilibiliCrawler::save(const std::string& key, const std::string& json,
        ::flower::crawler::Ripper& ripper) {
    static std::string doc_prefix = "http://localhost:9200/crawler_bilibili/_doc/";
    std::string doc = doc_prefix + key;
    std::string header, body;
    if (ripper.post(doc, json, &header, &body) != 0) {
        return -1;
    }

    // todo:check header

    LOG(TRACE) << "save, key:" << key << " post.json:" << json
            << " header:" << header << " body:" << body;

    return 0;
}

} // namespace crawler
} // namespace flower 