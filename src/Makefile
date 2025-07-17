.PHONY: all clean rebuild clang check test leak_test gcov_report dvi

# Компилятор и флаги
CXX = g++
CXXSTD = -std=c++20
CXXFLAGS = $(CXXSTD) -Wall -Werror -Wextra -g -pedantic

# Qt6: и заголовки, и библиотеки
QT_CFLAGS = $(shell pkg-config --cflags Qt6Widgets Qt6OpenGLWidgets)
QT_LIBS = $(shell pkg-config --libs Qt6Widgets Qt6OpenGLWidgets)

# Для покрытия gcov
GCOVFLAGS = --coverage
GCOV = gcovr

# Нахождение clang-format
CLANG_FORMAT = ../materials/linters/.clang-format

# Инструменты
MOC = $(shell which moc 2>/dev/null)

# Директории для сборки
BUILD_DIR = build
MOC_DIR   = $(BUILD_DIR)/moc
OBJ_DIR   = $(BUILD_DIR)/obj

# Пути
BINDIR = dist
DOCDIR = doc

# Цель — исполняемый файл
EXEC = 3DViewer

# Список исходников и заголовков, участвующих в генерации moc
SRCS = main.cc \
ui/main_window.cc \
ui/render_widget.cc \
ui/settings_dialog.cc \
engine/model_engine.cc \
utils/obj_parser.cc \
controller/controller.cc \
ui/shader.cc \
S21Matrix/s21_matrix.cc

# Заголовки с Q_OBJECT
HDRS_MOC = ui/main_window.h ui/render_widget.h ui/settings_dialog.h

# Объекты из .cc
OBJS = $(SRCS:.cc=.o)

# Генерация списка MOC-файлов (они будут лежать в build/moc)
MOC_SRCS = $(patsubst ui/%.h,$(MOC_DIR)/moc_%.cc,$(HDRS_MOC))

# Объектные файлы из исходников: build/obj/%.o
OBJS = $(patsubst %.cc,$(OBJ_DIR)/%.o,$(SRCS))

# Объектные файлы из MOC-файлов: build/obj/moc_%.o
MOC_OBJS = $(patsubst $(MOC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(MOC_SRCS))

#  Указываем, что MOC-файлы не должны удаляться до завершения сборки
.SECONDARY: $(MOC_SRCS)

# -----------------------
all: $(EXEC)

# Основная цель: линковка
$(EXEC): $(OBJS) $(MOC_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(QT_LIBS)

# Правила для создания директорий
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(MOC_DIR): | $(BUILD_DIR)
	mkdir -p $(MOC_DIR)

$(OBJ_DIR): | $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)

# Генерация MOC-файлов
$(MOC_DIR)/moc_%.cc: ui/%.h | $(MOC_DIR)
	$(MOC) $< -o $@

# Компиляция MOC-файла в объектный файл
$(OBJ_DIR)/moc_%.o: $(MOC_DIR)/moc_%.cc | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(QT_CFLAGS) -c $< -o $@

# Правило для .o
$(OBJ_DIR)/%.o: %.cc | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(QT_CFLAGS) -c $< -o $@

# Полная пересборка
rebuild: clean all

# Установочные цели
install: $(EXEC)
	@mkdir -p $(BINDIR)
	install -m 755 $(EXEC) $(BINDIR)/

uninstall:
	@rm -f $(BINDIR)/$(EXEC)

# Очистка артефактов
clean:
	@rm -rf $(BUILD_DIR) $(EXEC)

# Тесты

# Отчёт по покрытию

# Генерация документации с помощью doxygen
dvi:
	doxygen

# Формируем дистрибутив


# Форматирование кода с помощью clang-format
clang:	
	clang-format --style=file:$(CLANG_FORMAT) -i $(SRCS) 
	clang-format --style=file:$(CLANG_FORMAT) -n $(SRCS) 


# Проверка cppcheck
check:
	cppcheck --library=qt -q --enable=warning,portability,style --inconclusive $(SRCS)
