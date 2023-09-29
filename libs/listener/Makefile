NAME	= Listener.a

SRCS = \
	Listener.cpp	\
	TcpStream.cpp	\
	server.cpp

# **************************************************************************** #
TOPDIR = $(abspath ../..)
include $(TOPDIR)/build/Rules.mk

CPPFLAGS	+= 
CXXFLAGS	+= 
LDFLAGS		+= 
LDLIBS		+=

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
