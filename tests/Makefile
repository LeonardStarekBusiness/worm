NAME = build/tests

CXX = c++
CXXFLAGS = -Wall -Wextra -Wpedantic -Werror -g -std=c++23 -I../include -Iinclude

TEST_SRC = $(shell find src -type f -name "*.cpp")
IMPL_SRC = $(filter-out ../src/worm.cpp,$(shell find ../src -type f -name "*.cpp"))

TEST_OBJ = $(TEST_SRC:src/%.cpp=build/test_%.o)
IMPL_OBJ = $(IMPL_SRC:../src/%.cpp=build/impl_%.o)
OBJ_FILES = $(TEST_OBJ) $(IMPL_OBJ)

all: $(NAME)

build:
	mkdir -p build

build/test_%.o: src/%.cpp | build
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/impl_%.o: ../src/%.cpp | build
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ_FILES)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $@

clean: 
	rm -f $(OBJ_FILES)

fclean: clean
	rm -f $(NAME)
	rm -f *.tmp
	rm -f vgcore*

test: $(NAME)
	./$(NAME)

re: fclean 
	$(MAKE) test

.PHONY: all clean fclean test re
