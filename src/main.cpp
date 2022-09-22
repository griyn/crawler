#include <iostream>
#include <string>
#include <google/protobuf/util/json_util.h>
#include <sstream>
#include <time.h>
#include <iomanip>
#include <google/protobuf/util/field_mask_util.h>
#include "ripper/ripper.h"
#include "crawler/bilibili_crawler.h"
#include "proto/bilibili.pb.h"
#include "hive_mind/hive_mind.h"
#include "common/app_context.h"

int main() {
    ::flower::crawler::HiveMind hive_mind;
    if (hive_mind.init("conf/config.json") != 0) {
        return -1;
    }

    hive_mind.run();

    return 0;
}
