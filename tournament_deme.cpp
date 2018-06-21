#include "deme.h"
#include "chromosome.h"
#include "climb_chromosome.h"
#include "tsp.h"
#include "tournament_deme.h"
#include <random>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cassert>

TournamentDeme::TournamentDeme(const cities_t* cities_ptr, unsigned pop_size, double mut_rate) : Deme(cities_ptr, pop_size, mut_rate){
	my_mut_rate = mut_rate;
	my_pop_size = pop_size;
	// Note: we are allocating new memory, so we will have to deallocate that in the destructor
	/*
	for (unsigned i = 0 ; i < pop_size; i++) {
      // Chromosome automatically creates a random permutation, so we don't have to do anything
      // on that front.
      ClimbChromosome *current = new ClimbChromosome(cities_ptr); 
      my_population.push_back(current);
    }
	*/
}

  // Clean up as necessary
TournamentDeme::~TournamentDeme(){
    // We are deallocating the memory we allocated in the constructor.
    for(unsigned i = 0; i < my_population.size(); i++) {
      //delete my_population[i];
    }
}

Chromosome* TournamentDeme::select_parent(){
	unsigned powerOfTwo = 1;
	// We are taking the largest power of two possible
	while(2*powerOfTwo < my_population.size()) {
		powerOfTwo = powerOfTwo*2;
	}

	// Generating a random permutation, and then we'll just use the [0...powerOfTwo] indicies to determine
	// which parents to select from my_population
	if(my_population.size() == 0){
	  assert(false);
	}
	permutation_t randomParentSelector = random_permutation(my_population.size());
	std::vector<Chromosome*> randomParents ;//= {}; // this will hold our randomly-selected parents
	std::vector<Chromosome*> fitterRandomParents ;//= {}; // this will hold the p/2 fitter parents 

	//	std::cout << powerOfTwo << std::endl;
	//	std::cout << my_population.size();
	// We are populating our randomParents vector with parents determined by our random permutation
	for (unsigned i = 0; i < powerOfTwo; i+=1) {
	  int val = randomParentSelector[i];
	  Chromosome*c = my_population[val];
	  randomParents.push_back(c);
	  //	  randomParents.push_back(my_population[randomParentSelector[i]]);
	}

	while(powerOfTwo > 1) {
		for (unsigned i = 0; i < powerOfTwo; i+=2) {
			if (randomParents[i]->calculate_fitness() >= randomParents[i+1]->calculate_fitness()) {
				fitterRandomParents.push_back(randomParents[i]);
			}
			else {
				fitterRandomParents.push_back(randomParents[i+1]);
			}
		}
		randomParents = fitterRandomParents;
		fitterRandomParents = {};
		powerOfTwo = powerOfTwo/2;
	}
	return randomParents[0]; 
}
