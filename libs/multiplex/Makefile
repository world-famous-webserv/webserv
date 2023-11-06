NAME	= libmultiplex.a

SRCS	= \
	io_event.cpp \
	multiplex.cpp

# **************************************************************************** #
TOPDIR = $(abspath ../..)
include $(TOPDIR)/build/Rules.mk

CPPFLAGS	+= 
CXXFLAGS	+= 
LDFLAGS		+= 
LDLIBS		+=

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
