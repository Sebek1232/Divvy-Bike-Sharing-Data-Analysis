/*util.cpp*/

//
// Sebastian Greczek
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #06: hashing DIVVY data
//
//  Holds untilty function needed to analyze DIVVY data

#include <iostream>
#include <cmath>
#include <regex>
#include <fstream>
#include <sstream>

#include "util.h"
#include "hashmap.h"
#include "hash.h"


using namespace std;


//
// DistBetween2Points
//
// Returns the distance in miles between 2 points (lat1, long1) and 
// (lat2, long2).  Latitudes are positive above the equator and 
// negative below; longitudes are positive heading east of Greenwich 
// and negative heading west.  Example: Chicago is (41.88, -87.63).
//
// NOTE: you may get slightly different results depending on which 
// (lat, long) pair is passed as the first parameter.
// 
double distBetween2Points(double lat1, double long1, double lat2, double long2)
{
  //
  // Reference: http://www8.nau.edu/cvm/latlon_formula.html
  //
  double PI = 3.14159265;
  double earth_rad = 3963.1;  // statue miles:

  double lat1_rad = lat1 * PI / 180.0;
  double long1_rad = long1 * PI / 180.0;
  double lat2_rad = lat2 * PI / 180.0;
  double long2_rad = long2 * PI / 180.0;

  double dist = earth_rad * acos(
    (cos(lat1_rad) * cos(long1_rad) * cos(lat2_rad) * cos(long2_rad))
    +
    (cos(lat1_rad) * sin(long1_rad) * cos(lat2_rad) * sin(long2_rad))
    +
    (sin(lat1_rad) * sin(lat2_rad))
  );

  return dist;
}

//
// isNumeric
//
// Returns true if the given string is numeric (all digits), false
// if not.  If the string is empty, false is returned since there 
// are no digits present.
//
bool isNumeric(string s)
{
	//
	// A string is numeric if it contains 1 or more digits, so let's
	// use a regular expression to check that.
	//
	// we are using POSIX basic notation for regular expressions, see
	//   https://en.cppreference.com/w/cpp/regex/ecmascript
	// where [:d:] means digit, [[:d:]] means match a single digit.
	// The + means match the preceding sub-expression 1 or more times.
	//
	regex pattern("[[:d:]]+", regex::ECMAScript);

	smatch matchResults;  // provides more details on the match

	if (regex_match(s, matchResults, pattern))
		return true;
	else
		return false;
}

//returns true if input is a vaild trip ID, false otherwise
bool isTripId(string input)
{
	regex pattern("Tr[[:d:]]+", regex::ECMAScript);

	smatch matchResults;

	if (regex_match(input, matchResults, pattern))
		return true;
	else
		return false;
}

//returns true if input is a vaild bike ID, false otherwise
bool isBikeId(string input)
{
	regex pattern("B[[:d:]]+", regex::ECMAScript);

	smatch matchResults;

	if (regex_match(input, matchResults, pattern))
		return true;
	else
		return false;
}

//returns true if input is valid nearby command, false otherwise
bool isNearby(string input)
{
	stringstream s(input);
	string word;
	getline(s, word, ' ');
	if (word == "nearby")
		return true;
	return false;
}

//returns true if input is valid similar command, false otherwise
bool isSimilar(string input)
{
	stringstream s(input);
	string word;
	getline(s, word, ' ');
	if (word == "similar")
		return true;
	return false;
}

//parse each word of the input nearby command into varaibles that were passed
//by refrence.  
void parseNearby(string line, string& lat, string& longi, string& dist)
{
	stringstream s(line);
	string word, la, lon, dis;
	getline(s, word, ' ');
	getline(s, la, ' ');
	getline(s, lon, ' ');
	getline(s, dis, ' ');

	lat = la;
	longi = lon;
	dist = dis;
}

//parse each word of the input similar command into varaibles that were passed
//by refrence.  
void parseSimilar(string line, string& trip, double& dist)
{
	stringstream s(line);
	string word, tri, dis;
	getline(s, word, ' ');
	getline(s, tri, ' ');
	getline(s, dis, ' ');

	trip = tri;
	dist = stod(dis);
}

//reads from a formmatted file that stores station data. Two hashmaps are
//passed. the first one stores all station data in a struct and can by accesed by the 
//station id and the second hashmap holds thoso station ids that can be accesed by
//each's station abbreviation. This way we dont have to store StationData twice. 
bool inputStations(string fileName, hashmap<int, StationData>& idHashmap, hashmap<string, int>& abbHashmap)
{
	ifstream sFile(fileName);

	if (!sFile.good())
	{
		cout << "station file not open" << endl;
		return false;
	}

	string line;
	getline(sFile, line);
	while (getline(sFile, line))
	{
		string id, cap;
		StationData sta;
		stringstream s(line);

		getline(s, id, ',');
		getline(s, sta.abbr, ',');
		getline(s, sta.name, ',');
		getline(s, sta.lat, ',');
		getline(s, sta.longi, ',');
		getline(s, cap, ',');
		getline(s, sta.date, ',');

		int ID = stoi(id);
		sta.id = ID;
		sta.cap = stoi(cap);

		idHashmap.insert(ID, sta, hashId);
		abbHashmap.insert(sta.abbr, ID, hashAbb);
	}
	return true;
}

//reads form a formmatted file that stores trip data. Two hash fuction are passed in. One
//holds all trip data in a stuct which can be accesed by the trip id. The second stores number
//of usages of each bike which can be accesed by a bike ID.
bool inputTrips(string fileName, hashmap<string, TripData>& tripHashmap, hashmap<string, int>& bikeHashmap)
{
	ifstream tFile(fileName);

	if (!tFile.good())
	{
		cout << "trip file not open" << endl;
		return false;
	}

	string line;
	getline(tFile, line);
	while (getline(tFile, line))
	{
		string tripId, bikeId, duration, frStation, toStation;
		TripData trip;
		stringstream s(line);

		getline(s, tripId, ',');
		getline(s, trip.startTime, ',');
		getline(s, trip.stopTime, ',');
		getline(s, bikeId, ',');
		getline(s, duration, ',');
		getline(s, frStation, ',');
		getline(s, toStation, ',');
		getline(s, trip.gender, ',');
		getline(s, trip.bd, ',');
		trip.id = tripId;
		trip.bikeId = bikeId;
		trip.duration = stoi(duration);
		trip.frStation = stoi(frStation);
		trip.toStation = stoi(toStation);


		tripHashmap.insert(tripId, trip, hashTrip);

		int usage = 1;
		if (bikeHashmap.search(bikeId, usage, hashBikeId))//check if bike already exists
		{
			usage++; //update usage by one
			bikeHashmap.insert(bikeId, usage, hashBikeId); //overwrite existing bike with new usage info
		}
		else //bike does not exist, so add the bike hash map
			bikeHashmap.insert(bikeId, usage, hashBikeId); 
	}
	return true;
}
