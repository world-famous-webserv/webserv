TOPDIR = $(abspath ..)

NAME	= webserv

LIBS	= \
	$(TOPDIR)/libs/multiplex/libmultiplex.a \
	$(TOPDIR)/libs/config/libconfig.a \

SRCS	= \
	server.cpp \
	client.cpp \
	http/http.cpp \
	http/session.cpp \
	http/cookie.cpp \
	http/http_request.cpp \
	http/http_response.cpp \
	http/cgi.cpp \
	http/http_error.cpp \
	http/method/get.cpp \
	http/method/put.cpp \
	http/method/post.cpp \
	http/method/delete.cpp \
	http/method/index.cpp \
	main.cpp

# **************************************************************************** #
include $(TOPDIR)/build/Rules.mk

CPPFLAGS	+=
CXXFLAGS	+=
LDFLAGS		+=
LDLIBS		+=

$(NAME): $(OBJS) $(LIBS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@
