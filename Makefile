# **************************************************************************** #
#                                MAKEFILE                                     #
# **************************************************************************** #

NAME        := irc_server
CC          := c++
CFLAGS      := -Wall -Wextra -Werror -std=c++98
RM          := rm -rf

SRCDIR      := src
OBJDIR      := obj
INCDIR      := include

SOURCES     := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS     := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# **************************************************************************** #
#                                RULES                                        #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJECTS)
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)
	@echo "\033[32m[✔] Compilation successful!\033[0m"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@$(RM) $(OBJDIR)
	@echo "\033[33m[✔] Object files removed!\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "\033[31m[✔] Executable removed!\033[0m"

re: fclean all

.PHONY: all clean fclean re

