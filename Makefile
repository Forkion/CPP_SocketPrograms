GPP = g++ -std=c++20
FLAGS_GPP = -Wall -Werror -Wextra -fPIC
FLAGS_GTEST = -lgtest -lpthread
FLAGS_LCOV_GCOV = -lcov -gcov
VALGRIND_FLAGS = --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes -q
FLAGS_GCOV = -fprofile-arcs -ftest-coverage --coverage

SRC_LIB = bin/lib/src/
SRC_TESTS = tests/src/
LIB = $(wildcard $(SRC_LIB)*.cpp)
TESTS = $(wildcard $(SRC_TESTS)*.cpp)
BUILD = build/
OBJ = $(BUILD)obj/
GCOV = $(BUILD)gcov_report/
LIB_OBJ = $(patsubst $(SRC_LIB)%.cpp, $(OBJ)%.o, $(LIB))
LIB_NAME = lib.so
LIB_SO = build/$(LIB_NAME)

SRC_APP_GEN = bin/app_gen/src/*.cpp
SRC_APP1 = bin/app1/src/*.cpp
SRC_APP2 = bin/app2/src/*.cpp

all: clean lib app1 app2

lib: $(LIB_SO)

$(OBJ)%.o: $(SRC_LIB)%.cpp
	mkdir -p $(OBJ)
	$(GPP) $(FLAGS_GPP) -c $< -o $@

$(LIB_SO): $(LIB_OBJ)
	$(GPP) -shared $^ -o $@

app1: $(SRC_APP1) $(SRC_APP_GEN) $(LIB_SO)
	rm -rf $(BUILD)app1
	$(GPP) $(FLAGS_GPP) $^ -L$(BUILD) -l:$(LIB_NAME) -o $(BUILD)$@
# 	./$(BUILD)app1

app2: $(SRC_APP2) $(SRC_APP_GEN) $(LIB_SO)
	rm -rf $(BUILD)app2
	$(GPP) $(FLAGS_GPP) $^ -L$(BUILD) -l:$(LIB_NAME) -o $(BUILD)$@
# 	./$(BUILD)app2 &

run_app1:
	./$(BUILD)app1

run_app2:
	./$(BUILD)app2

test: clean test1


test1: $(TESTS) $(LIB_SO)
	$(GPP) $(FLAGS_GPP) $^ -L$(BUILD) -l:$(LIB_NAME) -o $(BUILD)$@ $(FLAGS_GTEST)
	./$(BUILD)$@

valgrind_test: clean valgrind_test1

valgrind_test1: $(TESTS) $(LIB_SO)
	$(GPP) $(FLAGS_GPP) $^ -L$(BUILD) -l:$(LIB_NAME) -o $(BUILD)$@ $(FLAGS_GTEST)
	valgrind $(VALGRIND_FLAGS) ./$(BUILD)$@

gcov_report: $(TESTS) $(LIB)
	mkdir -p $(GCOV)
	$(GPP) $(FLAGS_GPP) -g -fno-inline $^ -o $(GCOV)test $(FLAGS_GTEST) $(FLAGS_GCOV) -lpthread
	./$(GCOV)test 2>/dev/null || true
	@gcov $(GCOV)*.gcda 2>/dev/null || true
	@lcov -t "gcov_report" -o $(GCOV)test.info --no-external -c -d . --ignore-errors mismatch 2>/dev/null || true
	@genhtml -o $(GCOV)coverage $(GCOV)test.info --ignore-errors empty 2>/dev/null || true
	@echo "Coverage report: $(GCOV)coverage/index.html"
	@rm -rf *.gcda *.gcno *.gcov $(GCOV)*.gcda $(GCOV)*.gcno $(GCOV)*.gcov
	@rm test_roundtrip.txt

dvi:
	doxygen Doxyfile
	@echo "Documentation generated"

dist:
	@mkdir -p dist
	@tar -czvf dist/CPP_SocketPrograms.tar.gz \
		Dockerfile Makefile README.md bin/* tests/* \

clang:
	@clang-format --style=Google -i \
		bin/lib/src/*.cpp bin/lib/hdr/*.hpp \
		bin/app_gen/src/*.cpp bin/app_gen/hdr/*.hpp \
		bin/app1/src/*.cpp bin/app1/hdr/*.hpp \
		bin/app2/src/*.cpp bin/app2/hdr/*.hpp \
		tests/src/*.cpp tests/hdr/*.hpp

check_clang:
	@clang-format --style=Google -n \
		bin/lib/src/*.cpp bin/lib/hdr/*.hpp \
		bin/app_gen/src/*.cpp bin/app_gen/hdr/*.hpp \
		bin/app1/src/*.cpp bin/app1/hdr/*.hpp \
		bin/app2/src/*.cpp bin/app2/hdr/*.hpp \
		tests/src/*.cpp tests/hdr/*.hpp

stop: stop_app1 stop_app2
 	#sudo fuser -k 32929/tcp

stop_app1:
	@pkill app1 || true

stop_app2:
	@pkill app2 || true

clean:
	@rm -rf *.a $(BUILD)* lib test doxygen dist
