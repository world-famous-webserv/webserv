NAME	= liblistener.a

SRCS = \
		ASocket.cpp			\
		ClientSocket.cpp	\
		ServerSocket.cpp

# **************************************************************************** #
TOPDIR = $(abspath ../..)
include $(TOPDIR)/build/Rules.mk

CPPFLAGS	+= 
CXXFLAGS	+= 
LDFLAGS		+= 
LDLIBS		+=

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
