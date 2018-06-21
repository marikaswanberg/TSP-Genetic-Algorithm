/*
 * Declarations for Chromosome class to be used in a genetic algorithm (GA) to
 * approximate the travelling-salesperson problem (TSP).
 * A chromosome represents a permutation on the orders of the cities to visit.
 * Includes common GA operators: mutate and recombine.
 */

#ifndef _CHROMOSOME_H_
#define _CHROMOSOME_H_

#include "tsp.h"

class Chromosome {
 public:
  // Generate a completely random permutation from a list of cities
  Chromosome(const cities_t* cities);

  // Clean up as necessary
  virtual ~Chromosome();

  // Perform a single mutation on this chromosome
  virtual void mutate();

  // Return a pair of offsprings by recombining with another chromosome
  // Note: this method allocates memory for the new offsprings
  virtual std::pair<Chromosome*, Chromosome*>
  recombine(const Chromosome* other) const;

  virtual Chromosome* clone() const{ return new Chromosome(*this);}

  // Compute total distance to traverse cities in ordering:
  double calculate_total_distance() const {
    return total_path_distance(*my_cities_ptr, my_order);
  }

  // Return a positive fitness value, with higher numbers representing
  // fitter solutions (shorter total-city traversal path).
  virtual double calculate_fitness() const;

  // Save city ordering to file.  Cities are represented with their coordinates
  // and are saved in the same order as the chromosome's permutation.
  virtual void save(std::string filename) const;

 protected:
  // For an ordered set of parents, return a child using the ordered crossover.
  // The child will have the same values as p1 in the range [b,e),
  // and all the other values in the same order as in p2.
  virtual Chromosome*
  create_child_of(const Chromosome* p1, const Chromosome* p2, unsigned b, unsigned e) const;

  // A chromsome is valid if it has no repeated values in its permutation,
  // as well as no indices above the range (length) of the chromosome.
  bool is_valid() const;

  // Find whether a certain value appears in a given range of the chromosome.
  // Returns true if value is within the specified the range specified
  // [begin, end) and false otherwise.
  bool is_in_range(unsigned value, unsigned begin, unsigned end) const;


  const cities_t* my_cities_ptr; // Keep ptr to cities, no need for full copy 
  permutation_t my_order;  // The actual permutation of this chromosome
 private:


};

#endif
