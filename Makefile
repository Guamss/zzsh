CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

NAME = zzsh
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib
SOURCES = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
LIBRARIES = lib/bozolib/bozolib.a -lreadline
DEPS = $(OBJECTS:.o=.d)

all: $(NAME) $(LIBRARIES)

$(NAME): $(OBJECTS)
	make -C lib/bozolib
	$(CC) $(OBJECTS) -o $(NAME) $(LIBRARIES)

-include $(DEPS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)
