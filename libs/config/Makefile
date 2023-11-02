NAME	= libconfig.a

SRCS = \
	config.cpp

# **************************************************************************** #
TOPDIR = $(abspath ../..)
include $(TOPDIR)/build/Rules.mk

CPPFLAGS	+= -I$(LIB_DIR)/config
CXXFLAGS	+= 
LDFLAGS		+= -L$(LIB_DIR)/config
LDLIBS		+= -lsample -lconfig

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
