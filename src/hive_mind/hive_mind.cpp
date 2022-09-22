#include "hive_mind.h"
#include "common/app_context.h"
#include "common/log.h"
#include "common/protobuf_utils.h"

namespace flower {
namespace crawler {

int HiveMind::init(const std::string& file) {
    HiveMindConfig config;
    if (::garden::protobuf_utils::file2protobuf(file, &config) != 0) {
        LOG(FATAL) << "file2protobuf failed, file:" << file;
        return -1;
    }

    return init(config);
}

int HiveMind::init(const HiveMindConfig& config) {
    _name = config.name();
    for (int i = 0; i < config.crawlers_size(); ++i) {
        auto& crawler_config = config.crawlers(i);
        auto crawler = garden::AppContext::instance().get<Crawler>(crawler_config.type());
        if (crawler == nullptr) {
            LOG(FATAL) << "Can not find crawler:" << crawler_config.ShortDebugString();
            return -1;
        }
        crawler->set_name(crawler_config.name());
        if (crawler->init(crawler_config) != 0) {
            LOG(FATAL) << "Failed to init crawler:" << crawler_config.ShortDebugString();
            return -1;
        }
        _crawlers.emplace(crawler_config.name(), std::move(crawler));
    }

    return 0;
}

int HiveMind::run() {
    for (auto& [name, crawler] : _crawlers) {
        if (crawler->run() != 0) {
            LOG(WARNING) << "[RUN FAILED] Crawler name:" << name;
            continue;
        }

        LOG(NOTICE) << "[RUN SUCCESS] Crawler name:" << name;
    }

    return 0;
}


} // namespace crawler
} // namespace flower 