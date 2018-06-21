#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include "tsp.h"
using namespace std;
// Read the coordinates of an unknown number of cities from a tab-separated file
// into a cities_t vector and return it.
cities_t read_cities(string filename){
	ifstream in;
	in.open(filename.c_str());
	if (in.fail()) {
		cerr << "Unable to open file :(" << endl;
	}

	cities_t allCities; //this vector holds all of our city coordinates
	string currentCity;
	while(getline(in, currentCity)) {
		coord_t currentCityCoordinates; //this coordinate pair will hold our current city's coordinates
		//parsing the string as two ints using istringstream
		istringstream stream(currentCity); 
		stream >> currentCityCoordinates.first >> currentCityCoordinates.second;
		if (stream.fail() || !stream.eof()) {
			cerr << "Error!" << endl;
		}
		allCities.push_back(currentCityCoordinates); // here we are adding the coordinate pair of the current city to a vector
	}
	in.close();
	return allCities;
} 

// Given two coordinates (x1, y1) and (x2, y2), this computes the euclidean distance between 
// the coordinates and returns the value.
double euclidean_distance(int x1, int y1, int x2, int y2){
	double distance = sqrt(pow((x2 - x1),2) + pow((y2 - y1),2)); // eudclidean formula for distance
	return distance;
}

// For a given list of cities and a permutation of that list,
// compute how long it would take to traverse all the cities in the
// order of the permutation, and then returning to the first city.
// The distance between any two cities is
// computed as the Euclidean distance on a plane between their coordinates.
double total_path_distance(cities_t cityList, permutation_t ordering) {
	double totalDistance = 0;
	// we are adding up all of the distances between points
	for (unsigned i = 0; i<ordering.size()-1; i++) {
		totalDistance += euclidean_distance(cityList[ordering[i+1]].first, cityList[ordering[i+1]].second, cityList[ordering[i]].first, cityList[ordering[i]].second);
	}
	totalDistance += euclidean_distance(cityList[ordering[0]].first, cityList[ordering[0]].second, cityList[ordering[ordering.size()-1]].first, cityList[ordering[ordering.size()-1]].second);

	return totalDistance;
}

default_random_engine generator2((unsigned int) time(0)); // this sets the random seed outside of the function so 
														// a new permutation is generated each time

// For a given length size, generate a random permutation of all the numbers
// in the range 0 .. size-1 (with no repeats and no gaps).
permutation_t random_permutation(unsigned size){
	vector<unsigned> permutation;
	// creating a vector of the form {0, 1, 2, ..., size-1}
	for (unsigned i = 0; i<size; i++){
		permutation.push_back(i);
	} 
	// the method I've implemented for creating a random permutation is doing a series of swaps:
	// for each index, randomly pick an index with which to swap the index
	for (unsigned i= 0; i<size; i++) {
		uniform_int_distribution<int> distribution(i, size-1);
		unsigned random_index = distribution(generator2);
		iter_swap(permutation.begin() + i, permutation.begin() + random_index);

	}
	return permutation;
}

// Save the coordinates of all cities to a file of the given names, in the order
// defined in a given permutation.
void save_permuted_cities(std::string filename, cities_t cities, permutation_t ordering) {
	ofstream out;
	out.open(filename.c_str());
	if (out.fail()) {
		cerr << "unable to open file" << endl;
	}
	// we are writing out the city coordinates to a file
	for(unsigned i = 0; i<ordering.size(); i++){
		out<< cities[ordering[i]].first << "  "<< cities[ordering[i]].second << endl;
	}
	out.close();
}
