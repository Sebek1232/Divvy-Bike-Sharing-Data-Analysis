/*hash.cpp*/

//
// Sebastian Greczek
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Hash functions for DIVVY hash maps
// 

#include <iostream>
#include <string>
#include <cctype>  /*isdigit*/
#include <regex>   /*regular expressions*/
#include "hash.h"

using namespace std;

//hashes station ID, passes N the is size of hashmap
int hashId(int id, int N)
{
	return id % N;
}

//hashes station abberivation, passes N the is size of hashmap
int hashAbb(string abb, int N)
{
	int index = 0;
	for (int i = 0; i < abb.length(); i++)
	{
		index += abb[i];
	}
	return index % N;
}

//hashes Trip ID, passes N the is size of hashmap
int hashTrip(string trip, int N)
{
	trip.erase(0, 2);
	int index = stoi(trip);
	return index % N;
}

//hashes Bike ID, passes N the is size of hashmap
int hashBikeId(string bId, int N)
{
	bId.erase(0,1);
	int index = stoi(bId);
	return index % N;
}


