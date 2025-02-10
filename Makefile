NAME	= ircserv

SRCS	= main.cpp \
		  $(wildcard srcs/*.cpp) \
		  $(wildcard cmd/*.cpp)

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