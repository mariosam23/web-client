CC=g++
CFLAGS=-I.

client: client.cpp requests.cpp helpers.cpp buffer.cpp validator.cpp
	$(CC) -o client client.cpp requests.cpp helpers.cpp buffer.cpp validator.cpp -Wall

run: client
	./client

clean:
	rm -f *.o client
