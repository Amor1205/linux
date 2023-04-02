.PHONY:all
all: calClient calServer
calClient:calClient.cc
	g++ -o $@ $^ -std=c++11 -ljsoncpp
calServer:calServer.cc
	g++ -o $@ $^ -std=c++11 -lpthread -ljsoncpp
.PHONY:clean
clean:
	rm -fr calClient calServer