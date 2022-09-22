x :
	/usr/local/bin/protoc --cpp_out=. ./src/proto/*.proto
	g++ -std=c++20 ./src/main.cpp ./src/*/*.cpp ./src/*/*.cc -lcurl -lprotobuf -I./src -I.
	rm ./src/proto/*.pb.h ./src/proto/*.pb.cc
