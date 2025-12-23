NAME = abstract_vm

#########
RM = rm -rf
CC = g++
CFLAGS = -Werror -Wextra -Wall -std=c++20 -fsanitize=address -g -O3 -DDEBUG
# CFLAGS = -Werror -Wextra -Wall -g -fsanitize=address -O3 -I$(OPENSSL_BUILD_DIR)/include -I$(THIRD_PARTY_PATH)/cJSON -Iinc -DUSE_SSL #-DDEBUG
# CFLAGS = -Werror -Wextra -Wall -g -fsanitize=thread -O3 -I$(OPENSSL_BUILD_DIR)/include -I$(THIRD_PARTY_PATH)/cJSON -Iinc -DUSE_SSL #-DDEBUG
LDFLAGS = -lm
RELEASE_CFLAGS = -Werror -Wextra -Wall -O3 -std=c++20
#########

#########
FILES = main Operand

SRC = $(addsuffix .cpp, $(FILES))

vpath %.cpp srcs srcs/operand
#########

OBJ_DIR = objs

#########
#########

#########
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))
DEP = $(addsuffix .d, $(basename $(OBJ)))
#########

#########
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	${CC} -MMD $(CFLAGS) -c $< -o $@


all: .gitignore	
	$(MAKE) $(NAME)

debug: CFLAGS = -Werror -Wextra -Wall -g -fsanitize=address -O3 -Iinc -DDEBUG #-lpthread -DDEBUG
debug: all
	@echo "DEBUG BUILD DONE  "

dre: CFLAGS = -Werror -Wextra -Wall -g -fsanitize=address -O3 -Iinc -DDEBUG #-lpthread -DDEBUG
dre: re
	@echo "DEBUG REBUILD DONE  "

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -ldl -lpthread -o $(NAME) $(LDFLAGS)
	@echo "EVERYTHING DONE  "
#	@./.add_path.sh

release: CFLAGS = $(RELEASE_CFLAGS)
release: re
	@echo "RELEASE BUILD DONE  "

clean:
	$(RM) $(OBJ) $(DEP)
	$(RM) -r $(OBJ_DIR)
	@echo "OBJECTS REMOVED   "

fclean: clean
	$(RM) $(NAME)
	@echo "EVERYTHING REMOVED   "

re: fclean
	$(MAKE) all CFLAGS="$(CFLAGS)"

.gitignore:
	@if [ ! -f .gitignore ]; then \
		echo ".gitignore not found, creating it..."; \
		echo ".gitignore" >> .gitignore; \
		echo "$(NAME)" >> .gitignore; \
		echo "$(OBJ_DIR)/" >> .gitignore; \
		echo "*.a" >> .gitignore; \
		echo "*.pem" >> .gitignore; \
		echo ".vscode/" >> ../.gitignore; \
		echo ".gitignore" >> ../.gitignore; \
		echo "log.txt" >> ../.gitignore; \
		echo ".gitignore created and updated with entries."; \
	else \
		echo ".gitignore already exists."; \
	fi

.PHONY: all clean fclean re release .gitignore debug dre


-include $(DEP)
