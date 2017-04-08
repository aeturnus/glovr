CC = g++
LD = ld
CFLAGS =-Wall -Wextra -Wno-unused-parameter -Wno-ignored-qualifiers -g
SOURCES = ../src/ds/*.c
INCLUDE = ../src

TEST_DIR = .
GTEST_EXEC = ds-test.elf
GTEST_SRC = $(TEST_DIR)/*.cpp

$(GTEST_EXEC):
	$(CC) $(CFLAGS) -o $(GTEST_EXEC) $(SOURCES) -I $(INCLUDE) $(GTEST_SRC) -lgtest

clean:
	@rm $(GTEST_EXEC)

test: $(GTEST_EXEC)
	@./$(GTEST_EXEC)
