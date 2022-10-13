#include "crawler/bilibili_crawler.h"
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
    // 请求作者粉丝状态
    static std::string url_author_stat = "https://api.bilibili.com/x/relation/stat?vmid=";
    std::string url = url_author_stat + uid;
    auto* from_author_stat = new ::flower::crawler::bilibili::FromAuthorStat;
    if (request_get_message(url, from_author_stat) != 0) {
        LOG(WARNING) << "request_get_message failed, url:" << url;
        return -1;
    }

    // 请求作者信息
    static std::string url_author_space = "https://api.bilibili.com/x/space/acc/info?mid=";
    url = url_author_space + uid;
    auto* from_author_space = new ::flower::crawler::bilibili::FromAuthorSpace;
    if (request_get_message(url, from_author_space) != 0) {
        LOG(WARNING) << "request_get_message failed, url:" << url;
        return -1;
    }

    // 生成写库数据
    ::flower::crawler::bilibili::Save save_data;
    save_data.set_uid(uid);
    save_data.set_date(::garden::common::get_time_of_day_d());
    save_data.set_allocated_from_author_stat(from_author_stat);
    save_data.set_name(from_author_space->data().name());
    save_data.set_allocated_from_author_space(from_author_space);

    // 写库
    if (save("crawler_bilibili", uid + "@" + save_data.date(), save_data) != 0) {
        return -1;
    }

    return 0;
}

} // namespace crawler
} // namespace flower 