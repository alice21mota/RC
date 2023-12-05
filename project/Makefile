TARGETS = server client

all: $(TARGETS)

server: server.cpp
	g++ -std=c++11 server.cpp -o server

client: client.cpp
	g++ -std=c++11 client.cpp -o client

clean:
	rm -f $(TARGETS)