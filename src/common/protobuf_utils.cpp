#include "protobuf_utils.h"

#include "common/log.h"
#include <fstream>
#include <streambuf>

namespace garden {
namespace protobuf_utils {

int protobuf2json(const google::protobuf::Message& message, std::string* json) {
    if (json == nullptr) [[unlikely]] {
        return -1;
    }

    auto status = ::google::protobuf::util::MessageToJsonString(message, json);
    if (!status.ok()) {
        LOG(WARNING) << "protobuf2json failed:" << status.message();
        return -1;
    }

    return 0;
}

int json2protobuf(const std::string& json, google::protobuf::Message* message) {
    if (message == nullptr) [[unlikely]] {
        return -1;
    }

    ::google::protobuf::util::JsonParseOptions options;
    options.ignore_unknown_fields = true; // 不检查所有字段都有对应
    auto status = ::google::protobuf::util::JsonStringToMessage(json, message, options);
    if (!status.ok()) {
        LOG(WARNING) << "json2protobuf failed:" << status.message();
        return -1;
    }

    return 0;
}

int merge_message_to(const google::protobuf::Message& src, google::protobuf::Message* dest) {
    if (dest == nullptr) [[unlikely]] {
        return -1;
    }

    google::protobuf::util::FieldMaskUtil::MergeOptions foptions;
    foptions.set_replace_repeated_fields(true);
    google::protobuf::FieldMask mask;
    google::protobuf::util::FieldMaskUtil::GetFieldMaskForAllFields(src.GetDescriptor(), &mask);

    google::protobuf::util::FieldMaskUtil::MergeMessageTo(src, mask, foptions, dest);

    return 0;
}

int file2protobuf(const std::string& file, google::protobuf::Message* message) {
    if (message == nullptr) [[unlikely]] {
        return -1;
    }

    std::ifstream ifs(file);
    if (!ifs.good()) {
        LOG(FATAL) << "read file failed, file:" << file;
        return -1;
    }

    std::string json {(std::istreambuf_iterator<char>(ifs)), 
            std::istreambuf_iterator<char>()};
    
    return json2protobuf(json, message);
}

} // protobuf_utils
} // garden