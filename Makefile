BUILD_DIR ?= build
BUILD_TYPE ?= Debug

.PHONY: default configure debug release build clean

default: debug

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

debug:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(BUILD_DIR)

release:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR)

build:
	cmake --build $(BUILD_DIR)

clean:
	cmake --build $(BUILD_DIR) --target clean
