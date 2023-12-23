TOPDIR = $(abspath ../..)

NAME	= main

LIBS	= \
	$(TOPDIR)/libs/multiplex/libmultiplex.a \

SRCS	= \
	server.cpp \
	client.cpp \
	main.cpp

# **************************************************************************** #
include $(TOPDIR)/build/Rules.mk

CPPFLAGS	+= 
CXXFLAGS	+= 
LDFLAGS		+= 
LDLIBS		+= 

$(NAME): $(OBJS) 
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

# **************************************************************************** #
.PHONY: test

test: $(NAME)
	nc -zv localhost 8080

test2: $(NAME)
	nc -v localhost 8080
