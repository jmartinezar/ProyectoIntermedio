#include <iostream>
#include "include/include.h"
#include <fstream>
#include <vector>

int main(int argc, char **argv)
{
  ////////////////////////////////////////////////////////////////////////
  //////////////////////// 1st point /////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////
  
  std::string inputfile = argv[1]; // reads the filename from the command line

  int Nmol = 0;
  int size = 0;
  int Nstep = 0;
  int seed = 0;

  std::ifstream input;
  input.open(inputfile);
  input >> Nmol >> size >> Nstep >> seed;
  input.close();
  
  std::vector<int> particles{0};
  particles.resize(Nmol);

  cuatro_cuadros_centrados(Nmol, size, particles, seed);
  
  std::ofstream output1;
  output1.open("data/1.txt", std::ios::out);
  
  evolution(Nmol, size, particles, seed, Nstep, output1, 1);
  
  output1.close();

  ////////////////////////////////////////////////////////////////////////
  //////////////////////// 2nd point /////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////
    
  particles = {0};
  particles.resize(Nmol);
  
  cuatro_cuadros_centrados(Nmol, size, particles, seed);
  
  int t = find_t_eq(Nmol, size, particles, seed, Nstep);
  
  std::ofstream output2("data/2.txt", std::ios::out);
  
  output2 << size << "\t" << t << "\n";
  
  for(int ii = size+1; ii < 40; ++ii)
    {
      particles = {0};
      particles.resize(Nmol);
      cuatro_cuadros_centrados(Nmol, ii, particles, seed);	
      t = find_t_eq(Nmol, ii, particles, seed, Nstep);
      print_results(output2, ii, t);
      // output2 << ii << "\t" << t << "\n";
    }

  output2.close();

  ////////////////////////////////////////////////////////////////////////
  //////////////////////// 3rd point /////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////

  particles = {0};
  particles.resize(Nmol);

  cuatro_cuadros_centrados(Nmol, size, particles, seed);

  std::ofstream output3;
  output3.open("data/3.txt", std::ios::out);

  evolution(Nmol, size, particles, seed, Nstep, output3, 3);

  output3.close();

  ////////////////////////////////////////////////////////////////////////
  //////////////////////// 4th point /////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////
  
  int ratio = 5;

  particles = {0};
  particles.resize(Nmol);

  cuatro_cuadros_centrados(Nmol, size, particles, seed);

  std::ofstream output4("data/4.txt", std::ios::out);

  with_hole(size, particles, seed, Nstep, ratio, output4);

  output4.close();  
  
  return 0;
}
