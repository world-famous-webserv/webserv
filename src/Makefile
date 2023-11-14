TOPDIR = $(abspath ..)

NAME	= webserv

LIBS	= \
	$(TOPDIR)/libs/sample/libsample.a \

SRCS	= \
	main.cpp

# **************************************************************************** #
include $(TOPDIR)/build/Rules.mk

CPPFLAGS	+= -I$(LIB_DIR)/listener
CXXFLAGS	+= 
LDFLAGS		+= -L$(LIB_DIR)/listener
LDLIBS		+= -llistener

$(NAME): $(OBJS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@
