#include "deme.h"
#include "chromosome.h"
#include "climb_chromosome.h"
#include "tsp.h"
#include <random>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <utility>

ClimbChromosome::ClimbChromosome(const cities_t* cities_ptr)
  : Chromosome(cities_ptr)
{

}


// This overwrites the mutate function in chromosome.h to use 
//"local hill climbing" instead of random mutations
void ClimbChromosome::mutate() {
	permutation_t save_my_order = this->my_order;
	double originalFitness = this->calculate_fitness();
	double firstNewFitness = 0;
	double secondNewFitness = 0;
	permutation_t first_reorder = this->my_order;
	permutation_t second_reorder = this->my_order;

 	std::random_device random;
	std::mt19937 generator(random());
	std::uniform_int_distribution<unsigned> unif(0,int(my_order.size()-1));
 	unsigned p_random_point = unif(generator);

 	if(p_random_point != 0){
 		std::iter_swap(my_order.begin() + p_random_point, my_order.begin() + p_random_point-1);
 		first_reorder = this->my_order;
 		firstNewFitness = this->calculate_fitness();
 	}

 	else {
 		std::iter_swap(my_order.begin() + p_random_point, my_order.begin() + my_order.size()-1);
 		first_reorder = this->my_order;
 		firstNewFitness = this->calculate_fitness();
 	}

 	this->my_order = save_my_order;

 	std::iter_swap(my_order.begin() + p_random_point, my_order.begin() + ((p_random_point+1)%(my_order.size())));
 	second_reorder = this->my_order;
 	secondNewFitness = this->calculate_fitness();

 	permutation_t fittestPermutation = save_my_order;

 	if (firstNewFitness > originalFitness && firstNewFitness > secondNewFitness) {
 		fittestPermutation = first_reorder;
 	}
 	if (secondNewFitness > originalFitness && secondNewFitness > firstNewFitness) {
 		fittestPermutation = second_reorder;
 	}

 	this->my_order = fittestPermutation;
 	assert(is_valid());


}
