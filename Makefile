CXX_FLAGS = -std=c++20
OPTFLAGS = 
DEBUG_FLAGS = -g -Wall
C2FLAGS = -l Catch2Main -l Catch2
SANITIZERS = -fsanitize=address,undefined,leak
OBJ = obj
DAT = data

exec: $(DAT)/%.txt main.x all 1 2 3 4 exectime
	@echo "\nEjecución terminada. Los archivos .log de los arreglos se encuentran en el directorio figures/fitlogs"

$(DAT)/%.txt: main.x
	./$< input.txt

all: main.x input.txt

main.x: $(OBJ)/main.o $(OBJ)/include.o
	g++ $(CXX_FLAGS) $(OPTFLAGS) $(DEBUG_FLAGS) $(SANITIZERS) $^ -o $@

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

exectime: exectime.gp exectime.txt
	gnuplot exectime.gp

$(OBJ)/test.o: test.cpp include/include.cpp
	g++ -c test.cpp -o $(OBJ)/test.o
	g++ -c include/include.cpp -o $(OBJ)/include.o

test.x: $(OBJ)/test.o $(OBJ)/include.o
	g++ $^ -o $@ $(C2FLAGS)

test: test.x
	./$<

test_gprof:
	g++ -c -pg main.cpp -o $(OBJ)/main.o
	g++ -c -pg include/include.cpp -o $(OBJ)/include.o
	g++ $(DEBUG_FLAGS) -pg $(OPTFLAGS) $(OBJ)/main.o $(OBJ)/include.o -o $@
	./$@ input-profiling.txt
	@rm -f $(OBJ)/main.o $(OBJ)/include.o # conflicts with valgrind files, the -pg flag increases considerably the execution time

gprof: test_gprof gmon.out
	gprof $^ > gprof-report.txt

valgrind.x: $(OBJ)/main.o $(OBJ)/include.o
	g++ $(CXX_FLAGS) $(OPTFLAGS) $(DEBUG_FLAGS) $^ -o $@

cachegrind-report.txt: valgrind.x input-profiling.txt
	valgrind --tool=cachegrind --cachegrind-out-file=$@ --quiet ./$^

cachegrind: cachegrind-report.txt
	cg_annotate --auto=yes $@-report.txt

memcheck: valgrind.x input-profiling.txt
	valgrind --tool=memcheck --leak-check=yes ./$^

# it has to be compiled twice to get the references right. 
# texlive-lang-spanish and texlive-fonts-extra must be installed to compile
report: 1 2 3 4 exectime
	pdflatex --interaction=batchmode report.tex
	bibtex report
	pdflatex --interaction=batchmode report.tex

clean: 
	rm -f *.aux *.bbl *.blg *.log *.out *.pdf *.dtx *.x test_gprof gprof-report.txt cachegrind-report.txt obj/* data/* figures/*.pdf figures/fitlogs/*
