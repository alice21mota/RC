TARGETS = server client

SERVER_SRC = serverSide/server.cpp serverSide/filesystem.cpp serverSide/functionsUDP.cpp serverSide/manageUsersFolder.cpp serverSide/manageAuctionsFolder.cpp serverSide/utils.cpp serverSide/functionsTCP.cpp common/validations.cpp common/auxFuncs.cpp
CLIENT_SRC = clientSide/client.cpp clientSide/status/processStatus.cpp clientSide/commands/commandsUDP.cpp clientSide/commands/commandsTCP.cpp clientSide/commServer/sendProtocols.cpp common/validations.cpp common/auxFuncs.cpp
all: $(TARGETS)

server: $(SERVER_SRC)
	g++ -std=c++17 -Wno-deprecated-declarations $(SERVER_SRC) -o server

client: $(CLIENT_SRC)
	g++ -std=c++17 $(CLIENT_SRC) -o client

clean:
	rm -f $(TARGETS)