NAME = build/worm

CXX = c++
CXXFLAGS = -Wall -Wextra -Wpedantic -Werror -g -std=c++23 -Iinclude

SRC_FILES = $(shell find src -type f -name "*.cpp")
OBJ_FILES = $(SRC_FILES:src/%.cpp=build/%.o)

all: $(NAME)

build:
	mkdir -p build

build/%.o: src/%.cpp | build
	mkdir -p build $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $(NAME)

clean:
	rm -f $(OBJ_FILES)

fclean: clean
	rm -f $(NAME)
	rm -f *.tmp
	rm -f vgcore*

re: fclean all

.PHONY: all clean fclean re
