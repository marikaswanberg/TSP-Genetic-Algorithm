#ifndef _TOURNAMENT_DEME_H_
#define _TOURNAMENT_DEME_H_

#include <iostream>
#include <random>
#include <vector>
#include "tsp.h"
#include "chromosome.h"
#include "deme.h"
#include "climb_chromosome.h"

class TournamentDeme: public Deme {
	public:
		TournamentDeme(const cities_t* cities_ptr, unsigned pop_size, double mut_rate);

		// Clean up as neccesry
		virtual ~TournamentDeme();
	
		virtual Chromosome* select_parent();

	
	private:
		//		std::vector<ClimbChromosome*> my_population;
		//		cities_t* my_cities_ptr;
		//		unsigned my_pop_size;
		//		double my_mut_rate;
		
};

#endif 
