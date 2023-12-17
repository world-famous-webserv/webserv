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

testconf:
	sed 's=PWD=$(shell pwd)=g;' data/test_template.conf > data/test.conf

defaultconf:
	sed 's=PWD=$(shell pwd)=g;s=PYTHON3=$(shell which python3)=g;s=PHP=$(shell which php)=g;' data/default_template.conf > data/default.conf

test_fileupload:
	curl -X POST -F 'file=@Makefile' http://localhost:8080/cgi-bin/file_upload.py


.PHONY: all clean fclean re bonus
