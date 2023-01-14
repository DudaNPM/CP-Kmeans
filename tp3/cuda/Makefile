CXX 		= nvcc
BIN 		= bin/
SRC 		= src/
INCLUDES 	= include/
EXEC 		= stencil

CXXFLAGS	= -O2 -g -std=c++11 -arch=sm_70 -Wno-deprecated-gpu-targets 

.DEFAULT_GOAL = stencil

stencil: $(BIN)stencil.o
	$(CXX) $(CXXFLAGS) $(BIN)stencil.o -o $(BIN)$(EXEC)

$(BIN)stencil.o: $(SRC)stencil.cu $(INCLUDES)k_means_cuda.h 
	$(CXX) -c $(CXXFLAGS) $(SRC)stencil.cu -o $(BIN)stencil.o

clean:
	rm -r bin/*

run:
	./$(BIN)$(EXEC)