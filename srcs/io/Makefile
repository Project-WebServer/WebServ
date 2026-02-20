NAME		:=	webserv
RM			:=	rm -rf
CPP			:=	c++
CPPFLAGS	:=	-Wall -Wextra  -std=c++11 #-Werror

SRCDIR		:=	srcs
SRCS		:=	$(shell find $(SRCDIR) -iname "*.cpp")

INCL		:=	-I./inc

OBJDIR		:=	.build
OBJ			:=	$(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

all:		$(NAME)

$(NAME):	$(OBJ)
			@$(CPP) $(OBJ) -o $(NAME) $(INCL)
# 			@printf "$(CREATED)" $@ $(CUR_DIR)

$(OBJDIR)/%.o:	$(SRCDIR)/%.cpp
			@mkdir -p $(@D)
			@$(CPP) $(CPPFLAGS) $(INCL) -c $< -o $@
# 			@printf "$(UPDATED)" $@ $(CUR_DIR)

clean:
			$(RM) $(OBJDIR)
# 			@printf "$(REMOVED)" $(OBJDIR) $(CUR_DIR)

fclean: clean
			$(RM) $(NAME)
# 			@printf "$(REMOVED)" $(NAME) $(CUR_DIR)

re:			fclean all

info-%:
	$(info $($*))

.PHONY: all clean fclean re