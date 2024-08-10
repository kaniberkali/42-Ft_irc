
NAME = ircserv

SRC = main.cpp \
	  Utils/Utils.cpp \
	  Checker/Checker.cpp \
	  Server/Server.cpp \
	  Logger/Logger.cpp \
	  Parser/Parser.cpp \
	  Client/Client.cpp \
	  Command/Command.cpp \
	  Message/Message.cpp \
	  Channel/Channel.cpp \
	  Mode/Mode.cpp

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