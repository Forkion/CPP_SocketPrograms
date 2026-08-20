GPP = g++ -std=c++20
FLAGS_GPP = -Wall -Werror -Wextra -fPIC
FLAGS_TESTS = -lgtest -lpthread
FLAGS_LCOV_GCOV = -lcov -gcov

LIB = $(wildcard src/lib/source/*.cpp)
TESTS = $(wildcard tests/source/*.cpp)
BUILD = obj
#LIB_OBJ = $(patsubst )

all:

lib: $(LIB)
	$(GPP) $(FLAGS_GPP) $^ -o $@

test: $(LIB) $(TESTS)
	$(GPP) $(FLAGS_GPP) $^ -o $@ $(FLAGS_TESTS)

makedir: $(BUILD)
	mkdir -p $<

clean:
	rm -rf *.a obj/* *.o
