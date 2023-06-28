CC = gcc
CFLAGS = -Wall -Wextra -Werror

NAME = zzsh
SRC_DIR = src
OBJ_DIR = obj
SOURCES = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
DEPS = $(OBJECTS:.o=.d)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(NAME) $(OBJECTS)

-include $(DEPS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)
