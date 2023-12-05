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

testcurl:
	curl -v --http1.1 -X GET "http://localhost:8080"

chunk:
	curl --http1.1 -X POST -H "Transfer-Encoding:chunked" --data-binary @Makefile localhost:8080/dump

conf:
	@cat data/juwon_model.conf | sed 's=PWD=$(PWD)=g' > data/juwon.conf

.PHONY: all clean fclean re bonus conf
