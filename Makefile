NAME = worm

CFLAGGEN = -Wall -Wextra -Werror -g

CC = c++

QUELLE_DATEIEN = Program.cpp worm.cpp

OBJEKT_DATEIEN = $(QUELLE_DATEIEN:%.cpp=%.o)

all: $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGGEN) -c $< -o $@

$(NAME): $(OBJEKT_DATEIEN)
	$(CC) $(CFLAGGEN) $(OBJEKT_DATEIEN) -o $(NAME)

clean: 
	rm -f $(OBJEKT_DATEIEN)

fclean: clean 
	rm -f $(NAME)
	rm -f *.tmp
	rm -f vgcore*

re: fclean all

.PHONY: all clean fclean re
