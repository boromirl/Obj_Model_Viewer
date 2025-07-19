EXEC = 3DViewer
BASE_FLAGS = -Wall -Werror -Wextra -std=c++20
CXX = g++

all: $(EXEC)

install: $(EXEC)
uninstall: 
	rm -rf build

$(EXEC):
	mkdir -p build && \
	cd build && \
	cmake .. && \
	make


dvi:
	doxygen

GTEST_LIB = -lgtest -lgtest_main
TEST_EXEC = 3DViewer_test.out
TEST_DIR = tests
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cc)
ENGINE_SRCS = utils/obj_parser.cc S21Matrix/s21_matrix.cc engine/model_engine.cc
test:
	$(CXX) -o $(TEST_EXEC) $(TEST_SRCS) $(ENGINE_SRCS) $(GTEST_LIB) $(BASE_FLAGS)
	./$(TEST_EXEC)

REPORT_DIR = ./report
OPEN_BROWSER_CMD = $(shell which firefox || which xdg-open || which open || which x-www-browser)
GCOV_EXEC = gcov_report.out
gcov_report:
	$(CXX) --coverage -o $(GCOV_EXEC) $(TEST_SRCS) $(ENGINE_SRCS) $(GTEST_LIB)
	./$(GCOV_EXEC)
	lcov -t "3DViewer_test" --directory ./ --output-file report.info.raw --capture -q --no-external --ignore-errors mismatch
# удаление нерелевантных файлов
	lcov --remove report.info.raw 'S21Matrix/*' 'tests/*' -o report.info
	genhtml -o $(REPORT_DIR) report.info
	rm report.info.raw report.info *.gcno *.gcda $(GCOV_EXEC)
	$(OPEN_BROWSER_CMD) ./report/index.html


# clang-format
CLANG-FORMAT = clang-format -style='{CommentPragmas: Insert, BasedOnStyle: Google}'
clang_test:
	find . -name "*.h" -or -name "*.cc" -or -name "*.cpp" | xargs $(CLANG-FORMAT) -n
format:
	find . -name "*.h" -or -name "*.cc" -or -name "*.cpp" | xargs $(CLANG-FORMAT) -i


clean: uninstall
	rm -rf docs/html report/ $(TEST_EXEC) *.gcda *.gcno report.info $(GCOV_EXEC)

rebuild: clean all

# TODO: dist, better dvi