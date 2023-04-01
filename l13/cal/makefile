.PHONY:all
all: calClient calServer
calClient:calClient.cc
	g++ -o $@ $^ -std=c++11
calServer:calServer.cc
	g++ -o $@ $^ -std=c++11 -lpthread
.PHONY:clean
clean:
	rm -fr calClient calServer