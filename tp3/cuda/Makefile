CXX 		= nvcc
BIN 		= bin/
SRC 		= src/
INCLUDES 	= include/
EXEC 		= k_means_cuda

CXXFLAGS	= -O2 -g -std=c++11 -arch=sm_35 -Wno-deprecated-gpu-targets 

.DEFAULT_GOAL = k_means_cuda

k_means_cuda: $(BIN)k_means_cuda.o
	$(CXX) $(CXXFLAGS) $(BIN)k_means_cuda.o -o $(BIN)$(EXEC)

$(BIN)k_means_cuda.o: $(SRC)k_means_cuda.cu $(INCLUDES)k_means_cuda.h 
	$(CXX) -c $(CXXFLAGS) $(SRC)k_means_cuda.cu -o $(BIN)k_means_cuda.o

clean:
	rm -r bin/*

run:
	./$(BIN)$(EXEC)