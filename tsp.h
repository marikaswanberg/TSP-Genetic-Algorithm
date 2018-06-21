/*
 * API file for Travelling-Salesperson solving utilities.
 */

#ifndef __TSP_H__
#define __TSP_H__

#include <string>
#include <vector>

//////////////// Types

// A pair of integral coordinates for each city
using coord_t = std::pair<int, int>;

// A container holding coordinates for all the cities the TS has to visit:
using cities_t = std::vector<coord_t>;

// An ordering of the cities in cities_t. Each value represents a unique index
// into the cities_t vector.
using permutation_t = std::vector<unsigned int>;


//////////////// Functions

// Read the coordinates of an unknown number of cities from a tab-separated file
// into a cities_t vector and return it.
cities_t read_cities(std::string filename);

// For a given list of cities and a permutation of that list,
// compute how long it would take to traverse all the cities in the
// order of the permutation, and then returning to the first city.
// The distance between any two cities is
// computed as the Euclidean distance on a plane between their coordinates.
double total_path_distance(cities_t cities, permutation_t ordering);

// For a given length size, generate a random permutation of all the numbers
// in the range 0 .. size-1 (with no repeats and no gaps).
permutation_t random_permutation(unsigned size);

// Save the coordinates of all cities to a file of the given names, in the order
// defined in a given permutation.
void save_permuted_cities(std::string filename, cities_t cities, permutation_t ordering);

#endif
