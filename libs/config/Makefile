TOPDIR = $(abspath ../..)

NAME	= libconfig.a

LIBS	= \

SRCS	= \
	config.cpp \
	utils.cpp \
	block.cpp \
	simple.cpp \

# **************************************************************************** #
include $(TOPDIR)/build/Rules.mk

CPPFLAGS	+=
CXXFLAGS	+=
LDFLAGS		+=
LDLIBS		+=

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
