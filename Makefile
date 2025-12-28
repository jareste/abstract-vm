NAME = abstract_vm
TEST_NAME = test_abstract_vm

#########
RM = rm -rf
CC = g++
CFLAGS = -Werror -Wextra -Wall -std=c++20 -O3 #-fsanitize=address -g  #-DDEBUG
# CFLAGS = -Werror -Wextra -Wall -g -fsanitize=address -O3 -I$(OPENSSL_BUILD_DIR)/include -I$(THIRD_PARTY_PATH)/cJSON -Iinc -DUSE_SSL #-DDEBUG
# CFLAGS = -Werror -Wextra -Wall -g -fsanitize=thread -O3 -I$(OPENSSL_BUILD_DIR)/include -I$(THIRD_PARTY_PATH)/cJSON -Iinc -DUSE_SSL #-DDEBUG
LDFLAGS = -lm
RELEASE_CFLAGS = -Werror -Wextra -Wall -O3 -std=c++20
#########

#########
COMMON_FILES = Operand OperandFactory InputReader
FILES = main ${COMMON_FILES}
FILES_TEST = test_operand ${COMMON_FILES}

SRC = $(addsuffix .cpp, $(FILES))
SRC_TEST = $(addsuffix .cpp, $(FILES_TEST))

vpath %.cpp srcs srcs/operand srcs/exception srcs/tests srcs/parser
#########

OBJ_DIR = objs
OBJ_DIR_TEST = objs/tests

#########
#########

#########
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))
TEST_OBJ = $(addprefix $(OBJ_DIR_TEST)/, $(SRC_TEST:.cpp=.o))
DEP = $(addsuffix .d, $(basename $(OBJ)))
DEP_TEST = $(addsuffix .d, $(basename $(TEST_OBJ)))
#########

#########
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	${CC} -MMD $(CFLAGS) -c $< -o $@

$(OBJ_DIR_TEST)/%.o: %.cpp
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

$(TEST_NAME): CFLAGS += -DTEST_OPERAND_MAIN
$(TEST_NAME): $(TEST_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

test: $(TEST_NAME)

release: CFLAGS = $(RELEASE_CFLAGS)
release: re
	@echo "RELEASE BUILD DONE  "

clean:
	$(RM) $(OBJ) $(DEP) $(TEST_OBJ) $(DEP_TEST)
	$(RM) -r $(OBJ_DIR) $(OBJ_DIR_TEST)
	@echo "OBJECTS REMOVED   "

fclean: clean
	$(RM) $(NAME) $(TEST_NAME)
	@echo "EVERYTHING REMOVED   "

re: fclean
	$(MAKE) all CFLAGS="$(CFLAGS)"

.gitignore:
	@if [ ! -f .gitignore ]; then \
		echo ".gitignore not found, creating it..."; \
		echo ".gitignore" >> .gitignore; \
		echo "$(NAME)" >> .gitignore; \
		echo "$(TEST_NAME)" >> .gitignore; \
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
-include $(DEP_TEST)
