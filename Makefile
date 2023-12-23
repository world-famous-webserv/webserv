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
	mkdir -p resources/www/uploaded_files
	sed 's=PWD=$(shell pwd)=g;s=PYTHON3=$(shell which python3)=g;s=SHELL=$(shell which bash)=g;' data/webserv.conf.template > data/webserv.conf

test_fileupload:
	curl -X POST -F 'file=@testimage.png' http://localhost:8080/cgi-bin/file_upload.py

test_imageupload:
	curl -X POST --data-binary @testimage.png http://localhost:8080/www/ab.png

.PHONY: all clean fclean re bonus
