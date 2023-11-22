.DELETE_ON_ERROR:
.DEFAULT_GOAL = all

LIBS = \
	config/libconfig.a \
	multiplex/libmultiplex.a \
	http/libhttp.a \

$(LIBS):
	$(MAKE) -C $(dir $@)

all clean fclean re bonus:
	@for dir in $(dir $(LIBS)); do \
		$(MAKE) -C $$dir $@; \
	done

.PHONY: all clean fclean re bonus
