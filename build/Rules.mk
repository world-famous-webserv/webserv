.DELETE_ON_ERROR:
.DEFAULT_GOAL = all

SRC_DIR		:= $(TOPDIR)/src
LIB_DIR		:= $(TOPDIR)/libs
BUILD_DIR	:= .objs

RM			= rm -rf
ARFLAGS		= rsc

CXX 		= c++
CXXFLAGS	= -Wall -Wextra -Werror -Wpedantic -std=c++98 -MMD -MP
CPPFLAGS	= -I.

ifeq ($(shell uname), Linux)
TARGET_ARCH	+= -D__LINUX__
else
TARGET_ARCH	=
endif

ifdef DEBUG
CPPFLAGS	+= -D__DEBUG__
CXXFLAGS	+= -g -fsanitize=address,undefined
LDFLAGS		+= -fsanitize=address,undefined
endif

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

# **************************************************************************** #
# library rule
# **************************************************************************** #

$(LIBS):
	$(MAKE) -C $(@D)

CPPFLAGS	+= $(patsubst %,-I%,$(dir $(LIBS)))
LDFLAGS		+= $(patsubst %,-L%,$(dir $(LIBS)))
LDLIBS		+= $(patsubst lib%.a,-l%,$(notdir $(LIBS)))

# **************************************************************************** #
# default rule
# **************************************************************************** #

OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))
DEPS = $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.d))
-include $(DEPS)

all: $(LIBS)
	$(MAKE) $(NAME)

bonus:
	$(MAKE) BONUS=1 $(NAME)

clean:
	$(RM) $(BUILD_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re bonus
