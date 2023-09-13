NAME	= webserv

SRCS = \
	main.cpp

# **************************************************************************** #
TOPDIR = $(abspath ..)
include $(TOPDIR)/build/Rules.mk

CPPFLAGS	+= -I$(LIB_DIR)/sample
CXXFLAGS	+= 
LDFLAGS		+= -L$(LIB_DIR)/sample
LDLIBS		+= -lsample

$(NAME): $(OBJS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@
