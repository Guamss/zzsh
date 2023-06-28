CC = gcc
CFLAGS = -Wall -Wextra -Werror

NAME = zzsh
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib
SOURCES = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
LIBRARIES =  $(shell find $(LIB_DIR) -maxdepth 1 -mindepth 1 -type d)
DEPS = $(OBJECTS:.o=.d)

all: $(NAME)

$(LIBRARIES):
	@echo Create $@
	@$(MAKE) -C $@

$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(NAME)

-include $(DEPS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@echo Compiling $< -> $@
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)
