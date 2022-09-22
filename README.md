# Crawler

## Get Start
### install curl
用于请求网页和写es做存储
```
wget https://curl.haxx.se/download/curl-7.85.0.tar.gz
./configure --with-openssl
make
make install
```

### install elasticsearch
存储已抓取的数据
```
download: https://www.elastic.co/cn/downloads/elasticsearch
config: 1. 调整jvm内存占用 config/jvm.options -Xms256 -Xmx256m (取决于机器内存
        2. 关闭xpack扩展 conf/elasticsearch.yml xpack.security.enabled: false 和 xpack.security.http.ssl:enabled: false
start: ./bin/elasticsearch -d
check: curl -X GET "http://localhost:9200/"
```

### install AppContext
https://github.com/griyn/AppContext，一个反射工厂，用于从配置中生成不同抓取器

### start
./a.out，完成一次抓取任务 + 存储到es
