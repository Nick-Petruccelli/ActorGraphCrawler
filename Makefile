CC=g++
FLAGS= -lcurl
SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.cpp)
OBJS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
BINDIR=bin
BIN=$(BINDIR)/main
OUT=out


all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $@ $(FLAGS)

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) -c $< -o $@ $(FLAGS)

run: $(BIN)
	./$(BIN)

clean:
	$(RM) $(BINDIR)/* $(OBJ)/*

clear_out:
	$(RM) $(OUT)/*

debug: $(OBJS)
	$(CC) $(OBJS) -g -o $(BINDIR)/$@ $(CFLAGS)
