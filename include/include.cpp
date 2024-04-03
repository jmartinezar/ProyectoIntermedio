#include "include.h"
#include <iostream>
#include <fstream>

template<typename T, typename U>
void print_results(std::ofstream & output, T x, U y)
{
  output << x << "\t" << y << "\n";
}

void cuatro_cuadros_centrados(int Nmol, int size, std::vector<int> &vector, int seed)
{
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> dis{0, 1};
  int esquina = (size - 1)/2;
  
  for (int n = 0; n < Nmol; n++) {
    int i = 0, j = 0;
    i = esquina + dis(gen);
    j = esquina + dis(gen);
    
    vector[n] = i*size + j;
  }
}

void move_particle(int mol, int paso, int size, std::vector<int> & vector)
{
  switch(paso)
    {
    case 1:
      (vector[mol]/size != 0) ? (vector[mol] -= size) : (vector[mol] += (size - 1) * size);
      break;
    case 2:
      (vector[mol]/size != size - 1) ? (vector[mol] += size) : (vector[mol] = vector[mol]%size);
      break;
    case 3:
      (vector[mol] % size != 0) ? (vector[mol] -= 1) : (vector[mol] += size - 1);
      break;
    case 4:
      (vector[mol] % size != size - 1) ? (vector[mol] += 1) : (vector[mol] = size * (vector[mol] / size));
      break;
    }
}

void step(std::mt19937 & gen, std::uniform_int_distribution<> & dis_1, std::uniform_int_distribution<> & dis_2, int size, std::vector<int> & vector)
{
  int mol = dis_1(gen);
  int paso = dis_2(gen);
  move_particle(mol, paso, size, vector);
}

int find_t_eq(int Nmol, int size, std::vector<int> & vector, int seed, int Nstep)
{
  std::mt19937 gen(seed);
  
  std::uniform_int_distribution<> dis_1{0, Nmol - 1};
  std::uniform_int_distribution<> dis_2{0, 4};
  
  for (int ii = 1; ii <= Nstep; ++ii){
    step(gen, dis_1, dis_2, size, vector);
    int aux = ((entropia(Nmol, vector) < 1.5*std::log(size-1)) ? 0 : 1);
    switch(aux)
      {
        case 0:
          break;

        case 1:
          return ii;
      }
    }
  return -1;
}

void evolution(int Nmol, int size, std::vector<int> & vector, int seed, int Nstep, std::ofstream & output, int point)
{
  std::mt19937 gen(seed);
  
  std::uniform_int_distribution<> dis_1{0, Nmol - 1};
  std::uniform_int_distribution<> dis_2{0, 4};
  
  // case 3 stuff
  // double cup_size = std::pow(size,2); // area of the squared cup
  double cup_size = M_PI * std::pow(size/2.0,2); // area of the circumscribed circle in the squared cup 
  // double cup_size = 200.0;
  double drop_size = 0.0;
  int t_fill = 0;
  bool done = false;

  switch(point){
    case 1:
      print_results(output, 0, entropia(Nmol, vector));
      break;

    case 3:
      drop_size = M_PI*std::pow(radius(Nmol, vector, size),2);
      print_results(output, 0, drop_size);
      break;
    }

  for (int ii = 1; ii <= Nstep; ++ii){
    step(gen, dis_1, dis_2, size, vector);
    
    if(ii%100 == 0){
      switch(point)
        {
          case 1:
            print_results(output, ii, entropia(Nmol, vector));
            break;
          
          case 3:
          // checks when the drop fills the cup
            if (done == false){
              if(drop_size <= cup_size){
                t_fill = ii;
              }
              else
                done = true;
            }

            drop_size = M_PI*std::pow(radius(Nmol, vector, size),2);
            print_results(output, ii, drop_size);

            break;
        }
      }
    }
  
  switch(point){
    case 1:
      break;

    case 3:
      output << 2147483647 << "\t" << t_fill;
      output << "\n#The second column of this last value corresponds to the time when the drop fills the cup";
      break;
  }
}

double entropia(int Nmol, std::vector<int> & vector)
{  
  std::sort(vector.begin(), vector.end());
  
  double sum = 0.0;
  double aux = 1.0;
  
  for (int ii = 0; ii < Nmol; ++ii){
    if(ii != (Nmol - 1)){
      if (vector[ii] != vector[ii + 1])
        {
          aux = aux / Nmol;
          sum -= aux * (std::log(aux));
          aux = 1.0;
        }
      else
        {
          aux += 1.0;
        }
      }
    else
      {    
        aux = aux/Nmol;
        sum -= aux*(std::log(aux));
      }
    }
  return sum;
}

double radius(int Nmol, std::vector<int> & vector, int size)
{
  double r = 0.0;
  for (int ii = 0; ii < Nmol; ++ii){
    int x = (vector[ii]/size) - (size/2) + 1;
    int y = (vector[ii]%size) - (size/2) + 1;
    
    r += std::pow(x, 2) + std::pow(y, 2);
    }
  
  r = r/Nmol;
  
  return std::sqrt(r);
}

void move_with_hole(int mol, int paso, int size, std::vector<int> & vector, int ratio, int & numero)
{
  switch(paso){
    case 1:
      (vector[mol] / size != 0) ? (vector[mol] += -size) : (vector[mol] += (size - 1) * size);
      break;
    case 2:
      (vector[mol] / size != size - 1) ? (vector[mol] += size) : (vector[mol] = vector[mol] % size);
      break;
    case 3:
      (vector[mol] % size != 0) ? (vector[mol] += -1) : (vector[mol] += size - 1);
      break;
    case 4:
      if (vector[mol] % size != size - 1)
	      vector[mol] += 1;
      else
	    {
	      if (((size - 1 - size/ratio) <= vector[mol]/size) && (vector[mol]/size <= size - 1))
	        {
            vector.erase(vector.begin() + mol);
            numero = vector.size();
	        }
	      else
	        vector[mol] = size * (vector[mol] / size);
	    }
      break;
    }
}

void step_with_hole(std::mt19937 & gen, std::uniform_int_distribution<> & dis_1, std::uniform_int_distribution<> & dis_2, int size, std::vector<int> & vector, int ratio, int & numero)
{
  int mol = dis_1(gen);
  int paso = dis_2(gen);
  if(mol <= numero - 1)
    {
      move_with_hole(mol, paso, size, vector, ratio, numero);
    }
}

void with_hole(int size, std::vector<int> &vector, int seed, int Nstep, int ratio, std::ofstream & output)
{
  std::mt19937 gen(seed);
  int numero = vector.size();
  int aux = numero;
  std::uniform_int_distribution<> dis_1{0, numero - 1};
  std::uniform_int_distribution<> dis_2{0, 4};
  for (int ii = 1; ii <= Nstep; ++ii)
    {
      if(numero >= 2)
      {
        step_with_hole(gen, dis_1, dis_2, size, vector, ratio, numero);
      }
      
      else
      {
        break;
      }
      
      if((ii > 5000) && (aux != numero))
      {
        print_results(output, ii, numero);
        aux = numero;
      }
    }
}