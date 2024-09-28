.PHONY: all install uninstall clean dvi dist open tests style fixstyle leaks gcov_report remove_qsettings

CXX = g++
PROJECT = 3DViewer
PROJECT_DIR := $(shell pwd)
SOURCE_DIR := ${PROJECT_DIR}/sources
EXECUTABLE = 3d_viewer_test

CFLAGS = -Wall -Werror -Wextra -std=c++17 -pedantic
LDFLAGS = $(shell pkg-config --cflags --libs gtest) -I$(SOURCE_DIR)
RM = rm -rf
REPORTDIR = report
LCOVEXEC = $(EXECUTABLE).info
GCOV := -fprofile-arcs -ftest-coverage
COMMON_FILES := $(shell find . -maxdepth 1 -name '*.cc' -or -name '*.h') \
                $(shell find ${SOURCE_DIR} -name '*.cc' -or -name '*.h') \
                $(shell find ./tests -name '*.cc' -or -name '*.h')

OS := $(shell uname -s)
ifeq ($(OS), Linux)
	APP = $(PROJECT)
	RUN = ./build/$(PROJECT)
	SETTINGS = ~/.config/SAVE_3DVIEWER
	LIBS=-lgtest -g -lpthread
	LEAKS= valgrind --leak-check=full
	OPEN = xdg-open
endif
ifeq ($(OS), Darwin)
	APP = 3DViewer.app/Contents/MacOS/3DViewer
	RUN = open build/$(PROJECT).app
	SETTINGS = ~/Library/Preferences/com.save-3dviewer.3DViewer.plist
	LIBS=-lgtest
	LEAKS=CK_FORK=no leaks --atExit --
	OPEN = open
endif

all: build

build: $(COMMON_FILES)
	cmake -B build
	make -C build

install: build
	mkdir -p app
	cp build/${APP} app

uninstall:
	$(MAKE) clean
	$(RM) build
	$(RM) app

clean:
	$(RM) valgrind.txt 3DViewer.tar.gz
	$(RM) $(EXECUTABLE)
	$(RM) report
	$(RM) *.gcno *.gcda  *.info
	$(RM) gcov_report
	$(RM) 3dviewer*
	@$(RM) doc/output.texi

dvi:
	@(cd doc && sed 's|@BASE_PATH@|$(PROJECT_DIR)|g' 3d_doc.texi > output.texi)
	makeinfo --html --force doc/output.texi
	@$(OPEN) 3dviewer_html/index.html

dist: install
	tar -czf $(PROJECT).tar.gz --directory=sources/ .

run:
	$(RUN)

tests: clean
	$(CXX) $(CFLAGS) ${PROJECT_DIR}/tests/*.cc ${SOURCE_DIR}/model/*.cc -o $(EXECUTABLE) $(LDFLAGS) $(LIBS)
	./$(EXECUTABLE)

gcov_report:
	$(CXX) $(CFLAGS) $(GCOV) ${PROJECT_DIR}/tests/*.cc ${SOURCE_DIR}/model/*.cc $(LIBS) -o gcov_report ${LDFLAGS}
	./gcov_report
	@mkdir -p $(REPORTDIR)
	gcovr --object-directory . --output $(LCOVEXEC)
	gcovr --output $(REPORTDIR)/lcov-report.html --html --html-details
	$(OPEN) $(REPORTDIR)/lcov-report.html

style: fixstyle
	@clang-format -style=Google -n $(COMMON_FILES)

fixstyle:
	@clang-format -i -style=Google $(COMMON_FILES)

leaks: tests
	$(LEAKS) ./$(EXECUTABLE)

remove_qsettings:
	$(RM) $(SETTINGS)
