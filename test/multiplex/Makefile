NAME	= main

SRCS	= \
	server.cpp \
	client.cpp \
	main.cpp

LIBS	= \
	../../libs/multiplex/libmultiplex.a \

# **************************************************************************** #
TOPDIR = $(abspath ../..)
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
	netcat localhost 8080
