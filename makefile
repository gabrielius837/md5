CC = g++
CFLAGS = -Wall -Werror -O2

SRC = main.cpp md5.cpp
OBJ = $(SRC:.cpp=.o)
OUT = md5

.cpp.o:
	$(CC) $(CFLAGS) -c $<

$(OUT): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

clean:
	rm *.o md5
