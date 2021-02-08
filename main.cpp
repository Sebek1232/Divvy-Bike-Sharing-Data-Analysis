//
// Sebastian Greczek
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #06: hashing DIVVY data
// 
// Uses Hash maps to store DIVVY data. The program is able to look up
// Station data either thru the Station Id or Station abrreviation. It also
// Stores data about trips and can be acceses using the Trip ID. It also can
// show nearby stations within a specified distance from a latitude and longitude.
// It also shows smililar trips taken that started near a specified distance from start 
// staion and ended near a specified distance from end station. 



#include <iostream>
#include <algorithm>
#include <set>
#include <iomanip>

#include "util.h"
#include "hashmap.h"
#include "hash.h"

using namespace std;

int main() 
{
	hashmap<int, StationData>stationsById(10000);//Holds all StationData
	hashmap<string,int>stationsByAbbrev(10000);  //Holds key to the previous map 
	hashmap<string, TripData>tripsById(2500000); //Holds all TripData
	hashmap<string, int>bikesById(50000);        //Holds number of usages for each bike

	string input = "";
	string stationsFileName = "stations.csv";
	string tripsFileName = "trips.csv";
	string filenameStations;
	string filenameTrips;


	cout << "** DIVVY analysis program **" << endl << endl;
	cout << "Enter stations file> ";
	getline(cin, filenameStations);

	if (filenameStations != "")
		stationsFileName = filenameStations;

	cout << "Enter trips file> ";
	getline(cin, filenameTrips);

	if (filenameTrips != "")
		tripsFileName = filenameTrips;

	cout << endl;
	cout << "Reading " << stationsFileName << endl;
	cout << "Reading " << tripsFileName << endl << endl;

	inputStations(stationsFileName, stationsById, stationsByAbbrev);
	inputTrips(tripsFileName, tripsById, bikesById);

	cout << "# of stations: " << stationsById.getCount() << endl;
	cout << "# of trips: " << tripsById.getCount() << endl;
	cout << "# of bikes: " << bikesById.getCount() << endl << endl;

	cout << "Please enter a command, help, or #> ";
	getline(cin,input);

	while (input != "#") 
	{
		if (input == "help") //Help command
		{
			cout << endl;
			cout << "Available commands: " << endl;
			cout << " Enter a station id (e.g. 341)" << endl;
			cout << " Enter a station abbreviation (e.g. Adler)" << endl;
			cout << " Enter a trip id (e.g. Tr10426561)" << endl;
			cout << " Enter a bike id (e.g. B5218)" << endl;
			cout << " Nearby stations (e.g. nearby 41.86 -87.62 0.5)" << endl;
			cout << " Similar trips (e.g. similar Tr10424639)" << endl << endl;
		}

		else if (isNumeric(input))// Command #1
		{
			StationData staData;
			if (stationsById.search(stoi(input), staData, hashId))
			{
				cout << "Station:" << endl;
				cout << " ID: " << staData.id << endl;
				cout << " Abbrev: " << staData.abbr << endl;
				cout << " Fullname: " << staData.name << endl;
				cout << " Location: (" << stod(staData.lat) << ", " << stod(staData.longi) << ")" << endl;
				cout << " Capacity: " << staData.cap << endl;
				cout << " Online date: " << staData.date << endl;
			}
			else
				cout << "Station not found" << endl;
		}

		else if (isTripId(input))// Command #3
		{
			TripData triData;
			if (tripsById.search(input, triData, hashTrip))
			{
				int hours = triData.duration / 3600;
				int mins = triData.duration / 60;
				int seconds = triData.duration % 60;

				cout << "Trip:" << endl;
				cout << " ID: " << triData.id << endl;
				cout << " Starttime: " << triData.startTime << endl;
				cout << " Bikeid: " << triData.bikeId << endl;

				if (hours > 0 && mins > 0)
					cout << " Duration: " << hours << " hours, " << mins << " minutes, " << seconds << " seconds" << endl;
				else if(mins > 0 && hours == 0)
					cout << " Duration: " << mins << " minutes, " << seconds << " seconds" << endl;
				else
					cout << " Duration: " << seconds << " seconds" << endl;

				StationData staData;

				stationsById.search(triData.frStation, staData, hashId); //uses Station Id to find the name
				string fromStation = staData.abbr;                       //of the station for proper output

				stationsById.search(triData.toStation, staData, hashId);
				string toStation = staData.abbr;

				cout << " From station: " << fromStation << " (" << triData.frStation << ")" << endl;
				cout << " To station: " << toStation << " (" << triData.toStation << ")" << endl;
				cout << " Rider identifies as: " << triData.gender << endl;
				cout << " Birthyear: " << triData.bd << endl;
			}
			else
				cout << "Trip not found" << endl;
		}

		else if (isBikeId(input)) // Command #4
		{
			int usage = 0;
			if (bikesById.search(input, usage, hashBikeId))
			{
				cout << "Bike: " << endl;
				cout << " ID: " << input << endl;
				cout << " Usage: " << usage << endl;
			}
			else
				cout << "Bike not found" << endl;
		}

		else if (isNearby(input)) // Command #5
		{
			double lat, longi, lat2, longi2, dist;
			string latitude, longitude, distan;

			vector<int> keys; //holds all stations IDs.
			vector<pair<int, double>> nearby;// holds station id paired with its distance to cord.

			stationsById.getVector(keys);

			parseNearby(input, latitude, longitude, distan);
			lat = stod(latitude);
			longi = stod(longitude);
			dist = stod(distan);

			for (int i = 0; i < keys.size(); i++)
			{
				StationData staData;
				stationsById.search(keys[i], staData, hashId);

				lat2 = stod(staData.lat);
				longi2 = stod(staData.longi);

				double distance = distBetween2Points(lat, longi, lat2, longi2);

				if (distance < dist) //fill valid pairs of ID and distances
				{
					pair<int, double> stationDistances;
					stationDistances = make_pair(staData.id, distance);
					nearby.push_back(stationDistances);
				}
			}

			sort(nearby.begin(), nearby.end(), ordering()); // sorts by each stations distance to cord.

			cout <<  "Stations within " << distan << " miles of (" << latitude << ", " << longitude << ")" << endl;

			if (nearby.empty())
				cout << " none found" << endl;
			else
			{
				for (int i = 0; i < nearby.size(); i++) 
				{
					cout << " station " << nearby[i].first << ": " << nearby[i].second << " miles" << endl;
				}
			}
		}

		else if (isSimilar(input)) // Command #6
		{
			string trip;
			double dist;
			parseSimilar(input, trip, dist);

			TripData triData;

			if (tripsById.search(trip, triData, hashTrip)) //Trip Exists
			{
				int start = triData.frStation;
				int end = triData.toStation;

				StationData staDataS;
				StationData staDataD;

				set<int> S; //holds all stations near start Station
				set<int> D; //holds all stations near end Station

				stationsById.search(start, staDataS, hashId);
				stationsById.search(end, staDataD, hashId);

				double latS = stod(staDataS.lat);
				double longiS = stod(staDataS.longi);
				double latD = stod(staDataD.lat);
				double longiD = stod(staDataD.longi);

				vector<int> keys; //holds all Station IDs
				stationsById.getVector(keys);

				for (int i = 0; i < keys.size(); i++) //traverse all Stations
				{
					StationData staData;
					stationsById.search(keys[i], staData, hashId);

					double lat2 = stod(staData.lat);
					double longi2 = stod(staData.longi);

					double distanceS = distBetween2Points(latS, longiS, lat2, longi2);
					double distanceD = distBetween2Points(latD, longiD, lat2, longi2);

					if (distanceS < dist)      //Fills the sets 
						S.insert(staData.id);  //with valid Stations
					if (distanceD < dist)
						D.insert(staData.id);
				}

				vector<string> tripId; //holds all trip IDs
				tripsById.getVector(tripId);

				int tripcount = 0;;
				for (int i = 0; i < tripId.size(); i++) //Traverse all trips
				{
					TripData triData;
					tripsById.search(tripId[i], triData, hashTrip);

					if (S.count(triData.frStation) > 0 && D.count(triData.toStation) > 0) //if stations are in sets S and D
						tripcount++;                                                      //then the counter is updated
				}

				cout << setprecision(1) << fixed << "Trips that follow a similar path (+/-" << dist << " miles) as " << trip << endl;

				cout << " nearby starting points: ";
				for (int e : S)
					cout << e << " ";
				cout << endl;

				cout << " nearby ending points: ";
				for (int e : D)
					cout << e << " ";
				cout << endl;

				cout << " trip count: " << tripcount << endl;
			}
			else
			{
				cout << setprecision(1) << fixed << "Trips that follow a similar path (+/-" << dist << " miles) as " << trip << endl;
				cout << " no such trip" << endl;
			}

		}

		else // Command #2
		{
			StationData staData;
			int id = 0;
			if (stationsByAbbrev.search(input, id, hashAbb))
			{
				stationsById.search(id, staData, hashId);
				cout << "Station:" << endl;
				cout << " ID: " << staData.id << endl;
				cout << " Abbrev: " << staData.abbr << endl;
				cout << " Fullname: " << staData.name << endl;
				cout << " Location: (" << stod(staData.lat) << ", " << stod(staData.longi) << ")" << endl;
				cout << " Capacity: " << staData.cap << endl;
				cout << " Online date: " << staData.date << endl;
			}
			else 
				cout << "Station not found" << endl;
		}
		cout << endl;
		cout << "Please enter a command, help, or #> ";
		getline(cin, input);
	}
}



