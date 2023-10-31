NAME	= webserv

SRCS = \
	main.cpp

# **************************************************************************** #
TOPDIR = $(abspath ..)
include $(TOPDIR)/build/Rules.mk

CPPFLAGS	+= -I$(LIB_DIR)/sample -I$(LIB_DIR)/config
CXXFLAGS	+= 
LDFLAGS		+= -L$(LIB_DIR)/sample -L$(LIB_DIR)/config
LDLIBS		+= -lsample -lconfig

$(NAME): $(OBJS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@
