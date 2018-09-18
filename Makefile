all: ruptimeServer ruptimeClient

ruptimeServer: server.c
	gcc -Wall -Werror -g -o ruptimeServer server.c

ruptimeClient: client.c
	gcc -Wall -Werror -g -o ruptimeClient client.c

clean:
	rm -f ruptimeServer ruptimeClient *~
