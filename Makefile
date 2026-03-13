NAME		:=	webserv
RM			:=	rm -rf
CPP			:=	c++

RESET   = \033[0m
GREEN   = \033[32m
YELLOW  = \033[33m
BOLD    = \033[1m

CPPFLAGS := -Wall -Wextra -Werror -MMD -MP -g
CPPFLAGS += -std=c++20
LDFLAGS  := -fsanitize=address

SRCDIR		:=	srcs
SRCS		:=	$(shell find $(SRCDIR) -iname "*.cpp")

INCL		:=	-I./inc

OBJDIR		:=	.build
OBJ			:=	$(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEP			:=	$(OBJ:.o=.d)

all:		$(NAME)

$(NAME):	$(OBJ)
			@$(CPP) $(OBJ) $(LDFLAGS) -o $(NAME)
			@echo "\n$(GREEN)Compiling program $(NAME)...$(RESET)"
			@echo "$(GREEN)Done!$(RESET)\n"

-include $(DEP)

$(OBJDIR)/%.o:	$(SRCDIR)/%.cpp
			@mkdir -p $(@D)
			@$(CPP) $(CPPFLAGS) $(LDFLAGS) $(INCL) -c $< -o $@
			@echo "$(GREEN)Compiling: $< into $@$(RESET)"

clean:
			@$(RM) $(OBJDIR)
# 			@printf "$(REMOVED)" $(OBJDIR) $(CUR_DIR)

fclean: clean
			@$(RM) $(NAME)
# 			@printf "$(REMOVED)" $(NAME) $(CUR_DIR)

re:			fclean all

info-%:
	$(info $($*))

.PHONY: all clean fclean re