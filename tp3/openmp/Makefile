CC       = gcc
BIN      = bin/
SRC      = src/
INCLUDES = include/
EXEC     = k_means
THREADS  = 12 # número de processadores
CP_CLUSTERS = 32
 
CFLAGS = -O2 -fopenmp -g -fno-omit-frame-pointer
 
.DEFAULT_GOAL = k_means 
 
k_means: $(BIN)k_means.o 
	$(CC) $(CFLAGS) $(BIN)k_means.o -o $(BIN)$(EXEC) -lm
 
$(BIN)k_means.o: $(SRC)k_means.c $(INCLUDES)k_means.h 
	$(CC) $(CFLAGS) -c $(SRC)k_means.c -o $(BIN)k_means.o
 
clean: 
	rm -r bin/* 

runseq:
	./$(BIN)$(EXEC) 10004352 $(CP_CLUSTERS)

runpar: 
	./$(BIN)$(EXEC) 10004352 $(CP_CLUSTERS) $(THREADS)