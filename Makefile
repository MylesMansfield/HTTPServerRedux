CC 	   = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

TARGET  = httpserver

SOURCES = $(SRC)/httpserver.c $(SRC)/bind.c $(SRC)/httprequest.c $(SRC)/httphandle.c $(SRC)/logger.c $(SRC)/threadpool.c $(SRC)/interrupthandler.c
HEADERS = $(SRC)/bind.h $(SRC)/httprequest.h $(SRC)/httphandle.h $(SRC)/logger.h $(SRC)/threadpool.h $(SRC)/interrupthandler.h
OBJECTS = $(OBJ)/httpserver.o $(OBJ)/bind.o $(OBJ)/httprequest.o $(OBJ)/httphandle.o $(OBJ)/logger.o $(OBJ)/threadpool.o $(OBJ)/interrupthandler.o

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

obj/logger.o: $(SRC)/logger.c $(SRC)/logger.h
	$(CC) $(CFLAGS) -c $(SRC)/logger.c -o $(OBJ)/logger.o

obj/threadpool.o: $(SRC)/threadpool.c $(SRC)/threadpool.h
	$(CC) $(CFLAGS) -c $(SRC)/threadpool.c -o $(OBJ)/threadpool.o

obj/interrupthandler.o: $(SRC)/interrupthandler.c $(SRC)/interrupthandler.h
	$(CC) $(CFLAGS) -c $(SRC)/interrupthandler.c -o $(OBJ)/interrupthandler.o


clean:
	rm -f $(TARGET) $(OBJECTS)