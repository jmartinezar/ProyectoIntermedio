CXX_FLAGS = -std=c++20 -O3
DEBUG_FLAGS = -g -Wall
SANITIZERS = -fsanitize=address,undefined,leak
OBJ = obj
DAT = data

exec: main.x all
	./$< input.txt

all: main.x input.txt input4.txt

main.x: $(OBJ)/main.o $(OBJ)/include.o
	g++ $(CXX_FLAGS) $(DEBUG_FLAGS) $(SANITIZERS) $^ -o $@

$(OBJ)/main.o: main.cpp include/include.cpp
	g++ -c main.cpp -o $(OBJ)/main.o
	g++ -c include/include.cpp -o $(OBJ)/include.o

%.o: %.cpp
# avoids the implicit tule for.o files , every time make finds a .cpp file, it 
# will "compile" it to a .o file by doing nothing, as this rule indicates

1: 1.gp $(DAT)/1.txt
	gnuplot 1.gp

2: 2.gp $(DAT)/2.txt
	gnuplot 2.gp

3: 3.gp $(DAT)/3.txt
	gnuplot 3.gp

4: 4.gp $(DAT)/4.txt
	gnuplot 4.gp

test_gprof.x: main.cpp include.cpp
	g++ -O3 -Wall -pg -g $^ -o $@
	./$@

gprof: test_gprof.x gmon.out
	gprof $^ > gprof.txt

valgrind.x: $(OBJ)/main.o $(OBJ)/include.o
	g++ $(CXX_FLAGS) $(DEBUG_FLAGS) $^ -o $@

cachegrind-report.txt: valgrind.x input-profiling.txt
	valgrind --tool=cachegrind --cachegrind-out-file=$@ --quiet ./$^

cachegrind: cachegrind-report.txt
	cg_annotate --auto=yes $@-report.txt

memcheck: valgrind.x input-profiling.txt
	valgrind --tool=memcheck --leak-check=yes ./$^

clean: 
	rm *.x cachegrind-report.txt obj/* data/* figures/*.pdf figures/fitlogs/*
