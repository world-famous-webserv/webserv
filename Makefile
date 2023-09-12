.DELETE_ON_ERROR:
.DEFAULT_GOAL = all

NAME	= webserv

src/$(NAME):
	$(MAKE) -C src

$(NAME): src/$(NAME)
	ln -s $^ $@

all bonus:
	$(MAKE) -C libs $@
	$(MAKE) -C src $@
	$(MAKE) $(NAME)

clean:
	$(MAKE) -C libs $@
	$(MAKE) -C src $@

fclean:
	$(MAKE) -C libs $@
	$(MAKE) -C src $@
	$(RM) $(NAME)

re: fclean
	$(MAKE)

.PHONY: all clean fclean re bonus
