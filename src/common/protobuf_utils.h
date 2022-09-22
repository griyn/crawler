#pragma once

#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/field_mask_util.h>

namespace garden {
namespace protobuf_utils {

// json工具使用了json_util，尽量避免使用bytes类型，它会发成base64的转换
int protobuf2json(const google::protobuf::Message& message, std::string* json);

int json2protobuf(const std::string& json, google::protobuf::Message* message);

// 以替换repeated类型的方式merge message
int merge_message_to(const google::protobuf::Message& src, google::protobuf::Message* dest);

int file2protobuf(const std::string& file, google::protobuf::Message* message);

}
}