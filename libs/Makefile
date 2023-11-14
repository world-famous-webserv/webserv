.DELETE_ON_ERROR:
.DEFAULT_GOAL = all

LIBS = \
	listener/liblistener.a
	sample/libsample.a \
	config/libconfig.a \
	multiplex/libmultiplex.a \

$(LIBS):
	$(MAKE) -C $(dir $@)

all clean fclean re bonus:
	@for dir in $(dir $(LIBS)); do \
		$(MAKE) -C $$dir $@; \
	done

.PHONY: all clean fclean re bonus
