#include "deme.h"
#include "chromosome.h"
#include "climb_chromosome.h"
#include "tsp.h"
#include <random>
#include <iostream>
#include <cassert>
using namespace std;

  // Generate a Deme of the specified size with all-random chromosomes.
  // Also receives a mutation rate in the range [0-1].

  Deme::Deme(const cities_t* cities_ptr, unsigned pop_size, double mut_rate){
    my_mut_rate = mut_rate;
    my_pop_size = pop_size;

    // Note: we are allocating new memory, so we will have to deallocate that in the destructor
    for (unsigned i = 0 ; i < pop_size; i++) {
      // Chromosome automatically creates a random permutation, so we don't have to do anything
      // on that front.
      Chromosome *current = new ClimbChromosome(cities_ptr); 
      my_population.push_back(current);
    }
  }


  // Clean up as necessary
  Deme::~Deme(){
    // We are deallocating the memory we allocated in the constructor.
      for(unsigned i = 0; i < my_population.size(); i++) {
        delete my_population[i];
      }
  }

  // Evolve a single generation of new chromosomes, as follows:
  // We select pop_size/2 pairs of chromosomes (using the select() method below).
  // Each chromosome in the pair can be randomly selected for mutation, with
  // probability mut_rate, in which case it calls the chromosome mutate() method.
  // Then, the pair is recombined once (using the recombine() method) to generate
  // a new pair of chromosomes, which are stored in the Deme.
  // After we've generated pop_size new chromosomes, we delete all the old ones.
  void Deme::compute_next_generation() {
    Chromosome *parent1;
    Chromosome *parent2;
    vector<Chromosome*> tng_population; //our next generation population

    for (int i = 0; i<int(my_population.size()/2); i++) {
        parent1 = select_parent();
        parent2 = select_parent();

        std::random_device random;
        std::mt19937 generator3(random());
        std::uniform_real_distribution<float> unif(0,1);

        double probability = unif(generator3);
    
        if(probability < my_mut_rate) {
            parent1->mutate();
        }
        probability = unif(generator3);
        if (probability < my_mut_rate) {
            parent2->mutate();
        }

        // Recombining the parents to create the next generation
        std::pair<Chromosome*, Chromosome*> recombinedChildren;
        recombinedChildren = parent2->recombine(parent1);
        tng_population.push_back(recombinedChildren.first); //the new generation
        tng_population.push_back(recombinedChildren.second);
    }

    for (unsigned i = 0; i < my_population.size(); i++) {
      delete my_population[i];
    }

    my_population = tng_population;
  }

  // Return a copy of the chromosome with the highest fitness.
  Chromosome Deme::get_best() const {
    double bestFitness = my_population[0]->calculate_fitness(); //Starting best fitness at the first one because my fitness function can take infinitely high values

    Chromosome *bestChromosome = my_population[0];
    // We are simply looping through all of the chromosomes to find the fittest one.
    for (unsigned i = 0; i<my_pop_size; i++) {
      if (my_population[i]->calculate_fitness() >= bestFitness) {
        bestFitness = my_population[i]->calculate_fitness();
        bestChromosome = my_population[i];
      }
    }
    return *bestChromosome;
  }


  // Randomly select a chromosome in the population based on fitness and
  // return a pointer to that chromosome using Fitness Proportionate selection (see link below).
  // https://www.tutorialspoint.com/genetic_algorithms/genetic_algorithms_parent_selection.htm
  Chromosome* Deme::select_parent(){
    double sumOfFitnesses = 0;
    //calculate total fitness in the population
    for (unsigned i = 0; i < my_pop_size; i++){
      sumOfFitnesses += my_population[i]->calculate_fitness();
    }

    // Not totally sure how all of this random seed stuff works, but it works.
    std::random_device random;
    std::mt19937 generator3(random());
    std::uniform_int_distribution<unsigned> unif(0,my_pop_size-1);

    double partialSum = 0;
    int currentChrom = 0;
    double current;

    while(partialSum < sumOfFitnesses){
      current = unif(generator3); //generates a random number on each loop
      partialSum += my_population[current]->calculate_fitness();
      currentChrom +=1;
    }
   Chromosome *chosenChromosome = my_population[current];
    return chosenChromosome;
  }
