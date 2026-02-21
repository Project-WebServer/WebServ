

NAME = Conftest

RESET   = \033[0m
GREEN   = \033[32m
YELLOW  = \033[33m
BOLD    = \033[1m

CXXFLAGS = -Wall -Wextra -Werror -MMD -g
CXXFLAGS += -std=c++20
CXXFLAGS += -fsanitize=address

SRCS = $(wildcard *.cpp)
OBJS_DIR = objs/
OBJS_O = $(SRCS:.cpp=.o)
OBJS = $(addprefix $(OBJS_DIR), $(OBJS_O))
DEP = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
		@g++ $(CXXFLAGS) $(OBJS) -o $(NAME)
		@echo "\n$(GREEN)Compiling program $(NAME)...$(RESET)"
		@echo "$(GREEN)Done!$(RESET)\n"

-include $(DEP)

$(OBJS_DIR)%.o: %.cpp
		@mkdir -p $(dir $@)
		@g++ $(CXXFLAGS) -c $< -o $@
		@echo "$(GREEN)Compiling: $< into $@$(RESET)"

clean:
		@rm -fr $(OBJS_DIR)

fclean: clean
		@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
