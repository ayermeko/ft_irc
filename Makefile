NAME	= ircserv
SRCS	= main.cpp \
		  $(wildcard srcs/*.cpp) \
		  $(wildcard utils/*.cpp)
OBJ_DIR	= obj
INCLUDE	= -Iincludes/
OBJS	= $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
CFLAGS	= -Wall -Wextra -Werror -std=c++98
CC		= g++
#do not use wildcards

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