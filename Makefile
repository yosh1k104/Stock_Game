all:game_server client-base 

game_server:game_server.c
	gcc -o game_server game_server.c

client-base:client-base.c
	gcc -o client-base client-base.c
