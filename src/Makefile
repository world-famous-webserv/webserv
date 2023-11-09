TOPDIR = $(abspath ..)

NAME	= webserv

LIBS	= \
	$(TOPDIR)/libs/sample/libsample.a \

SRCS	= \
	main.cpp

# **************************************************************************** #
include $(TOPDIR)/build/Rules.mk

CPPFLAGS	+=
CXXFLAGS	+=
LDFLAGS		+=
LDLIBS		+=

$(NAME): $(OBJS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@
