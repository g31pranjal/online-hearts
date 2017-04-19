TARGET1 = client
TARGET2 = server

all:
	gcc -w client.c -o client
	gcc -w server.c -o server

clean:
	rm -rf $(TARGET1)
	rm -rf $(TARGET2)