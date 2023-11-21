TOPDIR = $(abspath ../..)

NAME	= libhttp.a

LIBS	= \

SRCS	= \
	http.cpp \
	http_request.cpp \
	http_response.cpp \

# **************************************************************************** #
include $(TOPDIR)/build/Rules.mk

CPPFLAGS	+=
CXXFLAGS	+=
LDFLAGS		+=
LDLIBS		+=

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
