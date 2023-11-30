TOPDIR = $(abspath ../..)

NAME	= libconfig.a

LIBS	= \

SRCS	= \
	config.cpp \
	block.cpp \
	simple.cpp \
	conf.cpp \

# **************************************************************************** #
include $(TOPDIR)/build/Rules.mk

CPPFLAGS	+=
CXXFLAGS	+=
LDFLAGS		+=
LDLIBS		+=

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
