NAME	= ircserv

SRCS	= main.cpp \
		  $(wildcard srcs/*.cpp) \
		  $(wildcard utils/*.cpp)

OBJ_DIR	= obj

# Object files
OBJS	= $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

# Compiler flags
CFLAGS	= -Wall -Wextra -Werror -std=c++98

# Compiler
CC		= g++

# Include directory
INCLUDE	= -Iincludes/

# VPATH for source directories
VPATH	= srcs:utils

# Default target
all: $(NAME)

# Rule to compile .cpp files into .o files
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Rule to link object files into the final executable
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# Rule to create the object directory
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Clean up object files
clean:
	@rm -rf $(OBJ_DIR)

# Clean up object files and the executable
fclean: clean
	@rm -rf $(NAME)

# Rebuild the project
re: fclean all

# Phony targets
.PHONY: all clean fclean re