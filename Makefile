CXX_FLAGS = -std=c++20 -O3
DEBUG_FLAGS = -g -Wall -fsanitize=address,undefined,leak
OBJ = obj
DAT = data

main: main.x input.txt input4.txt
	./$<

main.x: $(OBJ)/main.o $(OBJ)/include.o
	g++ $(CXX_FLAGS) $(DEBUG_FLAGS) $^ -o $@

$(OBJ)/main.o: main.cpp include/include.cpp
	g++ -c main.cpp -o $(OBJ)/main.o
	g++ -c include/include.cpp -o $(OBJ)/include.o

# avoids the implicit tule for.o files , every time make finds a .cpp file, it 
# will "compile" it to a .o file by doing nothing, as this rule indicates
%.o: %.cpp

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

test_val_cache.x: main.cpp include.cpp
	g++ -O3 -g $^ -o $@

cacheg.out: test_val_cache.x
	valgrind --tool=cachegrind ./$<

cachegrind: cachegrind.out.*
	cg_annotate --auto=yes $<

memcheck.x: main.cpp include.cpp
	g++ -g -O3 $^ -o $@

memcheck: memcheck.x
	valgrind --tool=memcheck --leak-check=yes ./$<

clean: 
	rm *.x obj/* data/* figures/*.pdf figures/fitlogs/*
