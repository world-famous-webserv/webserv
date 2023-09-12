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

ifdef DEBUG
CPPFLAGS	+= -D__DEBUG__
CXXFLAGS	+= -g -fsanitize=address,undefined
LDFLAGS		+= -fsanitize=address,undefined
endif

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

# **************************************************************************** #
# default rule
# **************************************************************************** #

OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))
DEPS = $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.d))
-include $(DEPS)

all:
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
