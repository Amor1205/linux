all : client
client:client1.cc
	g++ -g -o $@ $^ -std=c++11
server:server.cc
	g++ -g -o $@ $^ -std=c++11  -pthread
.PHONY:clean
clean:
	rm -f client server

# main:main.cc
# 	g++ -o main.cc ../source/server.hpp -std=c++11
# .PHONY:clean
# clean:
# 	rm -f main