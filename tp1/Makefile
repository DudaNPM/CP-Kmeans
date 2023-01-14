CC       = gcc
BIN      = bin/
SRC      = src/
INCLUDES = include/
EXEC     = k_means
 
CFLAGS = -O2 -ftree-vectorize -msse4
 
.DEFAULT_GOAL = k_means 
 
k_means: $(BIN)k_means.o 
	$(CC) $(CFLAGS) $(BIN)k_means.o -o $(BIN)$(EXEC) -lm
 
$(BIN)k_means.o: $(SRC)k_means.c $(INCLUDES)k_means.h 
	$(CC) $(CFLAGS) -c $(SRC)k_means.c -o $(BIN)k_means.o
 
clean: 
	rm -r bin/* 

run: 
	./$(BIN)$(EXEC) 