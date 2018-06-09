CPP=g++
CPPFLAGS=-std=c++11 -Wall -Wextra
LIBS=
EXE=run_vr_project

OBJ_DIR=obj
SRC_DIR=src

_DEPS=collide.hpp defs.hpp error.hpp graphics.hpp mouse.hpp palette.hpp parse.hpp pcomnds.hpp pfuncs.hpp program.hpp screen.hpp setup.hpp sound.hpp vector3d.hpp
DEPS=$(patsubst %,$(SRC_DIR)/%,$(_DEPS))

_OBJ=collide.o error.o graphics.o main.o mouse.o palette.o parse.o pcomnds.o pfuncs.o program.o screen.o setup.o sound.o vector3d.o
OBJ=$(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

OS := $(shell uname)

ifeq ($(OS),Darwin)
# Mac OS
	LIBS+=-framework SDL2
else
# Assume Linux for now
	LIBS+=-lSDL2
endif

default: debug

debug: CPPFLAGS += -g
debug: build

release: CPPFLAGS += -O2
release: build

build: setup_build $(EXE)
	@echo "Build finished"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CPP) $(CPPFLAGS) -c -o $@ $<

$(EXE): $(OBJ)
	$(CPP) $(CPPFLAGS) $(LIBS) $^ -o $@

setup_build:
	@mkdir -p $(OBJ_DIR)

.PHONY: clean

clean:
	@echo "Cleaning"
	@rm -f $(OBJ_DIR)/*.o *~ $(SRC_DIR)/*~
