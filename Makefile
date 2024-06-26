LIBS = -lSDL2 -lSDL2_image -lGL
INCLUDES = -Iinclude/imgui

SRC_DIR := src
BUILD_DIR := build

# blackboard files
SRC_FILES := $(wildcard src/*.cpp)

# imgui build
IMGUI_PATH := include/imgui
IMGUI_FILES := $(wildcard $(IMGUI_PATH)/*.cpp)
IMGUI_OBJ_FILES := $(patsubst $(IMGUI_PATH)/%.cpp, $(BUILD_DIR)/imgui/%.o, $(IMGUI_FILES))

CPPFLAGS += -DDEBUG_BUILD

default: $(OBJ_FILES) imgui
	clang++ $(CPPFLAGS) -o blackboard $(SRC_FILES) $(IMGUI_OBJ_FILES) $(INCLUDES) $(LIBS) -Wall

imgui: $(IMGUI_OBJ_FILES)

$(BUILD_DIR)/imgui/%.o: $(IMGUI_PATH)/%.cpp
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $< $(INCLUDES) $(LIBS) -Wall

run: default
	./blackboard