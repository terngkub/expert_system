CC = g++
CPP_VERSION = -std=c++17
CFLAGS = -Wall -Wextra -Werror $(CPP_VERSION)

NAME = expert_system

HEADER =	expert_system.hpp \
			fact.hpp \
			grammar.hpp \
			options.hpp \
			parser.hpp \
			rule.hpp \
			utils.hpp

FILE =	expert_system.cpp \
		fact.cpp \
		main.cpp \
		options.cpp \
		parser.cpp \
		rule.cpp \
		utils.cpp

BOOST_INC = -I ~/.brew/include
BOOST_LIB = -L ~/.brew/lib -l boost_program_options

SRC_DIR = src
INC_DIR = src
OBJ_DIR = obj

INC_FLAG = -I $(INC_DIR) $(BOOST_INC)
SRC = $(addprefix $(SRC_DIR)/,$(FILE))
INC = $(addprefix $(INC_DIR)/,$(HEADER))
OBJ = $(addprefix $(OBJ_DIR)/,$(FILE:%.cpp=%.o))

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ) $(INC)
	$(CC) $(CFLAGS) $(INC_FLAG) -o $(NAME) $(OBJ) $(BOOST_LIB)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INC_FLAG) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
