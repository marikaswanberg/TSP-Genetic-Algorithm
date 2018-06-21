#ifndef _CLIMB_CHROMOSOME_H_
#define _CLIMB_CHROMOSOME_H_

#include <iostream>
#include <random>
#include <vector>
#include "tsp.h"
#include "chromosome.h"
#include "deme.h"


class ClimbChromosome: public Chromosome {
	public:
		ClimbChromosome(const cities_t* cities);

		// This overwrites the mutate function in chromosome.h to use 
		//"local hill climbing" instead of random mutations
		virtual void mutate();

		virtual Chromosome* clone() const {return new ClimbChromosome(*this);}

	private:
		//		const cities_t* my_cities_ptr; // Keep ptr to cities, no need for full copy 
		//  		permutation_t my_order;  // The actual permutation of this Chromosome
};

#endif 
