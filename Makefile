test: test.c
	gcc test.c -o test

udp_server: udp_server.c
	gcc udp_server.c -o udp_server

tcp_client: tcp_client.tcp_client
	gcc tcp_client.c -o tcp_server