TARGETS = server client

SERVER_SRC = serverSide/server.cpp serverSide/filesystem.cpp serverSide/functionsUDP.cpp serverSide/manageUsersFolder.cpp serverSide/utils.cpp serverSide/functionsTCP.cpp
CLIENT_SRC = client.cpp validationFunc/validations.cpp

all: $(TARGETS)

server: $(SERVER_SRC)
	g++ -std=c++17 $(SERVER_SRC) -o server

client: $(CLIENT_SRC)
	g++ -std=c++17 $(CLIENT_SRC) -o client

clean:
	rm -f $(TARGETS)