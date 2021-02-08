/*util.h*/

//
// Sebastian Greczek
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #06: hashing DIVVY data
// 
// Holds untilty function needed to analyze DIVVY data

#include "hashmap.h"

using namespace std;

struct StationData
{
	int id, cap;
	string lat, longi, abbr, name, date;
};

struct TripData
{
	int duration, frStation, toStation;
	string id, startTime, stopTime, bikeId, gender, bd;
};

// ordering class used for sorting pair<int,double> of station keys and distance from station
class ordering
{
public:
	bool operator()(const pair<int, double>& p1, const pair<int, double>& p2) const
	{
		return p1.second < p2.second;
	}
};


double distBetween2Points(double lat1, double long1, double lat2, double long2);
bool isNumeric(string s);
bool isTripId(string input);
bool isBikeId(string input);
bool isNearby(string input);
bool isSimilar(string input);
void parseNearby(string line, string& lat, string& longi, string& dist);
void parseSimilar(string line, string& trip, double& dist);

bool inputStations(string fileName, hashmap<int, StationData>& idHashmap, hashmap<string, int>& abbHashmap);
bool inputTrips(string fileName, hashmap<string, TripData>& tripHashmap, hashmap<string, int>& bikeHashmap);