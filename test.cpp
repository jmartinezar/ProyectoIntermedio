#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2/catch_test_macros.hpp"
#include "include/include.h"

const double err = 1.0e-3;

int Nmol = 0;
int size = 0;
int Nstep = 0;
int seed = 0;
int ratio = 5;

TEST_CASE( "ARROW OF TIME" ) {
    std::ifstream input;
    input.open("input.txt");
    input >> Nmol >> size >> Nstep >> seed;
    input.close();

    std::vector<int> particles{0};
    std::vector<int> particleswithhole{0};
    particles.resize(Nmol);
    particleswithhole.resize(Nmol);

    cuatro_cuadros_centrados(Nmol, size, particles, seed);
    
    std::vector<int> particlest0 = particles;

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis_1{0, Nmol - 1};
    std::uniform_int_distribution<> dis_2{0, 4};

    for (int ii = 1; ii <= 1000; ++ii){
        step(gen, dis_1, dis_2, size, particles);
        int partnum = particleswithhole.size();
        step_with_hole(gen, dis_1, dis_2, size, particleswithhole, ratio, partnum);
    }

    SECTION ( "SECOND LAW OF THERMODYNAMICS" ) {
        REQUIRE(entropia(Nmol, particlest0) - entropia(Nmol, particles) < 0);
    }

    SECTION ( "WALKERS DISPERSE - ERGODICITY" ) {
        REQUIRE(radius(Nmol, particles, size) - radius(Nmol, particlest0, size) > 0);
        // The greater the space, the grater the time to reach equilibrium. Walkers have to fill al walk by a greater space
        REQUIRE(find_t_eq(Nmol, size, particlest0, seed, Nstep) - find_t_eq(Nmol, 2*size, particlest0, seed, Nstep) < 0);
        REQUIRE(particles.size() - particleswithhole.size() > 0); 
    }

}

TEST_CASE( "BOLTZMANN ENTROPY" ) {
    std::ifstream input;
    input.open("input.txt");
    input >> Nmol >> size >> Nstep >> seed;
    input.close();

    std::vector<int> particles{0};
    std::vector<int> particleswithhole{0};
    particles.resize(Nmol);

    cuatro_cuadros_centrados(Nmol, size, particles, seed);

    // check that entropy at t=0 is log(4) since there are 4 squares giving 4 possible states
    // this case is considering k_B = 1 since we have no units and all whe have to check is
    // the behavior of the system
    REQUIRE(entropia(Nmol, particles) - std::log(4) < err);
}
