CC=g++
CFLAGS=-I.

client: client.cpp requests.cpp helpers.cpp buffer.cpp validator.cpp commands.cpp
	$(CC) -o client client.cpp requests.cpp helpers.cpp buffer.cpp validator.cpp commands.cpp -Wall

run: client
	./client

zip:
	zip -r Sampetru_Mario_321CA_Tema3PC.zip *.cpp *.hpp Makefile README.md

clean:
	rm -f *.o client
