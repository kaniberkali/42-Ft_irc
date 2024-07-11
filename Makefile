
NAME = ircserv

SRC = main.cpp \
	  Checkerer/Checker.cpp \
	  Server/Server.cpp \

OBJ = $(SRC:.cpp=.o)

RM = rm -f

CC = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: 
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re