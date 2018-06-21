/*
 * Implementation for Chromosome class
 */

#include <algorithm>
#include <cassert>
#include <random>
#include <fstream>
#include <iostream>

#include "chromosome.h"
using namespace std;

// Generate a completely random permutation from a list of cities
Chromosome::Chromosome(const cities_t* cities_ptr)
  : my_cities_ptr(cities_ptr), my_order(random_permutation(cities_ptr->size()))
{
  assert(is_valid());
}

// Clean up as necessary
Chromosome::~Chromosome(){}


// Perform a single mutation on this chromosome
void
Chromosome::mutate()
{
  std::random_device random;
  std::mt19937 generator3(random());
  std::uniform_int_distribution<unsigned> unif(0,int(my_order.size())-1);

  unsigned firstIndex = unif(generator3);
  unsigned secondIndex = firstIndex;

 // This loop checks that the two indices are not equal and keeps picking new values until they are not equal.
  while (firstIndex == secondIndex) {
    secondIndex = unif(generator3);
  }


  iter_swap(my_order.begin() + firstIndex, my_order.begin() + secondIndex);

  assert(is_valid());
 
}

// Return a pair of offsprings by recombining with another chromosome
// Note: this method allocates memory for the new offsprings


std::pair<Chromosome*, Chromosome*>
Chromosome::recombine(const Chromosome* other) const
{

  assert(is_valid());
  assert(other->is_valid());

  std::random_device random;
  std::mt19937 generator3(random());
  std::uniform_int_distribution<unsigned> unif(0,int(my_order.size()-1));

  unsigned beginRange = unif(generator3);
  unsigned endRange = unif(generator3);

  if (beginRange > endRange) {
    unsigned tmp = endRange;
    endRange = beginRange;
    beginRange = tmp;
  }

  Chromosome* firstChild = create_child_of(this, other, beginRange, endRange); 
  Chromosome* secondChild = create_child_of(other, this, beginRange, endRange); //FIGURE OUT HOW TO DEAL WITH INT VS UNSIGNED
  pair<Chromosome*, Chromosome*> chromosomePointers;
  chromosomePointers = make_pair(firstChild, secondChild);
  return chromosomePointers;
}

// For an ordered set of parents, return a child using the ordered crossover.
// The child will have the same values as p1 in the range [b,e),
// and all the other values in the same order as in p2.
Chromosome*
Chromosome::create_child_of(const Chromosome* p1, const Chromosome* p2,
                           unsigned b, unsigned e) const
{
  Chromosome* child = p1->clone();

  // We iterate over both parents separately, copying from parent1 if the
  // value is within [b,e) and from parent2 otherwise
  unsigned i = 0, j = 0;
  
  for ( ; i < p1->my_order.size() && j < p2->my_order.size(); ++i) {
    if (i >= b and i < e) {
      child->my_order[i] = p1->my_order[i];
    }
    else { // Increment j as long as its value is in the [b,e) range of p1
      while (p1->is_in_range(p2->my_order[j], b, e)) {
        ++j;
      }
      assert(j < p2->my_order.size());
      child->my_order[i] = p2->my_order[j];
      j++;
    }
  }

  assert(child->is_valid());
  return child;
}


// Return a positive fitness value, with higher numbers representing
// fitter solutions (shorter total-city traversal path).
double
Chromosome::calculate_fitness() const
{
  double fitness;
  fitness = (*this).calculate_total_distance();
  // Taking the multiplicative inverse of the distance ensures that larger (worse) distances 
  // recieve worse fitness values. I am a litle worried about floating-point error though CHECK THIS!
  fitness = 1/(fitness+1);
  return fitness;
}

// Save city ordering to file.  Cities are represented with their coordinates
// and are saved in the same order as the chromosome's permutation.
void
Chromosome::save(std::string filename) const
{
 save_permuted_cities(filename, *my_cities_ptr, my_order);
}


// A chromsome is valid if it has no repeated values in its permutation,
// as well as no indices above the range (length) of the chromosome.
// We implement this check with a sort, which is a bit inefficient, but simple

bool
Chromosome::is_valid() const
{
  bool isValid = true;
  // Creating a vector that contains all of the values to check for repeats, values above the threshhold
  // and gaps
  vector<unsigned> sortedPermutation = my_order;
  unsigned smallest;
  unsigned smallestIndex;
  //using selection sort to sort the permutation
  for (unsigned i = 0; i < sortedPermutation.size()-1; i++){
    smallest = sortedPermutation[i];
    smallestIndex = i;
    for (unsigned j = i; j < sortedPermutation.size()-1; j++){
      if (sortedPermutation[j] < smallest) {
        smallest = sortedPermutation[j];
        smallestIndex = j;

      }
    }
    sortedPermutation[smallestIndex] = sortedPermutation[i];
    sortedPermutation[i] = smallest;
  }

  for (unsigned i = 0; i<sortedPermutation.size(); i++) {
    if (sortedPermutation[i] >=sortedPermutation.size()) {
      isValid = false;
      return isValid;
    }
  }


 /* cout << "sorted permutation: ";
  for (unsigned i = 0; i < sortedPermutation.size() ; i++) {
    cout << sortedPermutation[i];
  }
  cout << endl;
  */
    // we are checking for repeats. By the Pigeonhole principle, if the terms are all 
    // in range, and there are no repeats, then there will be no gaps either
  for (unsigned i = 0; i<sortedPermutation.size()-1; i++) {
    if (sortedPermutation[i] == sortedPermutation[i+1]) {
      std::cout << "repeat or gap fail";
      isValid = false;
      return isValid;
      }
  }

  return isValid;
}

// Find whether a certain value appears in a given range of the chromosome.
// Returns true if value is within the specified the range specified
// [begin, end) and false otherwise.
bool
Chromosome::is_in_range(unsigned value, unsigned begin, unsigned end) const
{
  for (unsigned idx = begin; idx < end; ++idx) {
    if (value == my_order[idx]) {
      return true;
    }
  }
  return false;
}

