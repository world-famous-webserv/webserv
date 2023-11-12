TOPDIR = $(abspath ../..)

NAME	= libconfig.a

LIBS	= \

SRCS	= \
	config.cpp \
	configFileReader.cpp \
	configLocation.cpp \
	configServer.cpp \
	configStringUtils.cpp \

# **************************************************************************** #
include $(TOPDIR)/build/Rules.mk

CPPFLAGS	+=
CXXFLAGS	+=
LDFLAGS		+=
LDLIBS		+=

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
