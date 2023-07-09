CPP=g++
CPPFLAGS=-std=c++14 -Wall -Wextra -pedantic -Weffc++
LIBS=
EXE=run_vr_project

OBJ_DIR=obj
SRC_DIR=src

_DEPS=collide.hpp color.hpp defs.hpp error.hpp graphics.hpp instance.hpp light.hpp master.hpp mouse.hpp options.hpp parse.hpp program.hpp screen.hpp sound.hpp vector3d.hpp viewer.hpp world.hpp
DEPS=$(patsubst %,$(SRC_DIR)/%,$(_DEPS))

_OBJ=collide.o color.o error.o graphics.o instance.o light.o main.o mouse.o options.o parse.o program.o screen.o sound.o vector3d.o world.o
OBJ=$(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

OS := $(shell uname)

ifeq ($(OS),Darwin)
# Mac OS
	# LIBS+=-framework SDL2
	# CPPFLAGS += -I./$(SRC_DIR)
	CPPFLAGS += `pkg-config --cflags sdl2`
	LIBS += `pkg-config --libs sdl2`
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

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

$(EXE): $(OBJ)
	$(CPP) $(CPPFLAGS) $(LIBS) $^ -o $@

setup_build:
	@mkdir -p $(OBJ_DIR)

.PHONY: clean

clean:
	@echo "Cleaning"
	@rm -f $(OBJ_DIR)/*.o *~ $(SRC_DIR)/*~
