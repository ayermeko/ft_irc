NAME	= ircserv

SRCS =   main.cpp\
	    $(addprefix cmd/, INVITE.cpp JOIN.cpp KICK.cpp MODE.cpp NICK.cpp PRIVMSG.cpp QUIT.cpp TOPIC.cpp) \
        $(addprefix srcs/, Channel.cpp Client.cpp Server.cpp)

		  

OBJ_DIR	= obj

OBJS	= $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

CFLAGS	= -g -Wall -Wextra -Werror -std=c++98

CC		= g++

INCLUDE	= -Iincludes/

all: $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re