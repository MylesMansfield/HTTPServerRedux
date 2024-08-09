CC 	   = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

TARGET  = httpserver

SOURCES = $(SRC)/httpserver.c $(SRC)/bind.c $(SRC)/httprequest.c $(SRC)/httphandle.c
HEADERS = $(SRC)/bind.h $(SRC)/httprequest.h $(SRC)/httphandle.h
OBJECTS = $(OBJ)/httpserver.o $(OBJ)/bind.o $(OBJ)/httprequest.o $(OBJ)/httphandle.o

SRC = src
OBJ = obj


httpserver: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

obj/httpserver.o: $(SRC)/httpserver.c $(HEADERS)
	$(CC) $(CFLAGS) -c $(SRC)/httpserver.c -o $(OBJ)/httpserver.o

obj/bind.o: $(SRC)/bind.c $(SRC)/bind.h
	$(CC) $(CFLAGS) -c $(SRC)/bind.c -o $(OBJ)/bind.o

obj/httprequest.o: $(SRC)/httprequest.c $(SRC)/httprequest.h
	$(CC) $(CFLAGS) -c $(SRC)/httprequest.c -o $(OBJ)/httprequest.o

obj/httphandle.o: $(SRC)/httphandle.c $(SRC)/httphandle.h
	$(CC) $(CFLAGS) -c $(SRC)/httphandle.c -o $(OBJ)/httphandle.o


clean:
	rm -f $(TARGET) $(OBJECTS)