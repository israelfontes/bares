
INC_DIR=./include
SRC_DIR=./src
OBJ_DIR=./build
BIN_DIR=./bin
DOC_DIR=./doc

RM = rm -rf

CC=g++

CPPFLAGS = -Wall -pedantic -ansi -std=c++11

OBJS = $(OBJ_DIR)/parser.o $(OBJ_DIR)/bares.o $(OBJ_DIR)/driver_bares.o

bares: clean dir $(OBJS)
	$(CC) $(CPPFLAGS) -o $(BIN_DIR)/$@ $(OBJS) 

$(OBJ_DIR)/parser.o: $(SRC_DIR)/parser.cpp $(INC_DIR)/parser.hpp $(INC_DIR)/token.hpp
	$(CC) -c $(CPPFLAGS) -lm -I$(INC_DIR)/ -o $@ $<

$(OBJ_DIR)/bares.o: $(SRC_DIR)/bares.cpp $(INC_DIR)/token.hpp $(INC_DIR)/stack.hpp
	$(CC) -c $(CPPFLAGS) -lm -I$(INC_DIR)/ -o $@ $<

$(OBJ_DIR)/driver_bares.o: $(SRC_DIR)/driver_bares.cpp $(INC_DIR)/parser.hpp $(INC_DIR)/bares.hpp $(INC_DIR)/token.hpp
	$(CC) -c $(CPPFLAGS) -lm -I$(INC_DIR)/ -o $@ $<

doxy:
	$(RM) $(DOC_DIR)/*
	doxygen Doxyfile

dir:
	mkdir -p bin build doc

clean: dir
	$(RM) $(BIN_DIR)/*
	$(RM) $(OBJ_DIR)/*
	$(RM) $(DOC_DIR)/*

