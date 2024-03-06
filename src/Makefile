CC = gcc 
CFALGS = -Wall -Werror -Wextra -std=c11  
STATLIB = s21_3dviewer.a
OS := $(shell uname)

ifeq ($(OS),Linux)
	FLAGS = -g -lcheck -lm -lpthread -lrt -lsubunit -fprofile-arcs -ftest-coverage
	OPEN = xdg-open
else
	FLAGS = -lcheck -lm -lpthread -fprofile-arcs -ftest-coverage
	OPEN = open
endif

all: clean install

install:
	-mkdir build
	cd ./build/ && qmake ../ && make

uninstall: clean

dvi:
	open read_dvi/openme.md
	
dist: 
	mkdir dist/
	mkdir dist/src
	mkdir dist/src/QtgifImage
	mkdir dist/src/QtgifImage/giflib
	cp Makefile *.c *.h *.cpp *.ui *.pro *.user dist/src/
	cp QtgifImage/*.h dist/src/QtgifImage
	cp QtgifImage/*.cpp dist/src/QtgifImage
	cp QtgifImage/*.pro dist/src/QtgifImage
	cp QtgifImage/giflib/* dist/src/QtgifImage/giflib
	tar cvzf $(HOME)/Desktop/dist.tgz dist/
	rm -rf dist*

$(STATLIB): 
	$(CC) $(CFALGS) -c -g s21_*.c
	ar -rcs $(STATLIB) s21_*.o

tests: clean $(STATLIB)
	$(CC) $(CFLAGS)  tests/s21_tests.c $(STATLIB) $(FLAGS) -o test
	./test

gcov_report: 
	$(CC) $(CFLAGS) tests/*.c s21*.c $(FLAGS) -o test
	./test
	lcov -t "./test" -o report.info --no-external -c -d .
	genhtml --ignore-errors unmapped -o report report.info
	open ./report/index.html
	rm -rf *.gcno *gcda *.gcov

clang: 
	clang-format -n *.c tests/*.c *.h *.cpp
	clang-format -i *.c tests/*.c *.h *.cpp

clean: 
	rm -rf *.o $(STATLIB) test *.gcno *gcda *.gcov gcov report report.info *.dSYM ./build* ./dist* a.out
