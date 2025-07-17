EXEC = 3DViewer

all: $(EXEC)

install: $(EXEC)
uninstall: 
	rm -rf build

$(EXEC):
	mkdir -p build && \
	cd build && \
	cmake .. && \
	make

clean: uninstall
	rm -rf docs/html

rebuild: clean all

dvi:
	doxygen

# clang-format
CLANG-FORMAT = clang-format -style='{CommentPragmas: Insert, BasedOnStyle: Google}'
clang_test:
	find . -name "*.h" -or -name "*.cc" -or -name "*.cpp" | xargs $(CLANG-FORMAT) -n
format:
	find . -name "*.h" -or -name "*.cc" -or -name "*.cpp" | xargs $(CLANG-FORMAT) -i

# TODO: тесты, покрытие, dist