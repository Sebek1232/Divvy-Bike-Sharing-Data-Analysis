/*hashmap.h*/

//
// <<Sebastian Greczek>>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #06: hashing
//
// References: original non-collision code written by Prof. Hummel (UIC)
// 
// hashmap implements a hash table. It does not need a perfect hash function.
// The class is designed to handle collisions.  The class provides
// the underlying hash table along with insert() and search() functions;
// the user of this class must provided a Hash(key, N) function that 
// hashes the given key and returns an integer in the range 0..N-1, 
// inclusive, if the key is valid.  If the key is invalid, the Hash() 
// function must return a negative value.
//
// The hash function is provided in the calls to insert and search.
// Pass the name of your hash function as the 3rd parameter.  Example:
//
//   int myHash(string key, int N)
//   { ... }
//
//   int N = 10000;
//   hashmap<string, double>  hmap(N);  
//
//   if (hmap.insert("deep-dish pizza", 19.99, myHash))
//     cout << "inserted" << endl;
//   else
//     cout << "not inserted?!" << endl;
// 

#pragma once

#include <iostream>
#include <functional>
#include <vector>

using namespace std;

template<typename KeyT, typename ValueT>
class hashmap
{
private:
  struct HashEntry
  {
    bool   ContainsData;  // false => empty, true => contains data
    KeyT   Key;           // (key, value)
    ValueT Value;

    HashEntry()
    {
      ContainsData = false;
    }
  };

  HashEntry* HT;  // hash table array of structs
  int        N;   // capacity of hash table (# of locations or array size)
  int        count; // number of elements inserted
public:
  //
  // constructor: N is the size of the underlying hash table
  //
  hashmap(int N)
  {
    this->N = N;
	this->count = 0;
    this->HT = new HashEntry[N];
  }
  //
  //copy constructor
  //
  hashmap(const hashmap& other)
  {
	  this->N = other.N;
	  this->HT = new HashEntry[this->N];
	  for (int i = 0; i < other.N; i++)
	  {
		  if (other.HT[i].ContainsData)
		  {
			  this->HT[i].ContainsData = other.HT[i].ContainsData;
			  this->HT[i].Key = other.HT[i].Key;
			  this->HT[i].Value = other.HT[i].Value;
		  }
	  }
  }
  
  //
  // destructor
  //
  virtual ~hashmap()
  {
    delete[] this->HT;
  }

  //
  // operator=
  //
  hashmap& operator=(const hashmap& other)
  {
	  delete[] this->HT;
	  this->N = other.N;
	  this->HT = new HashEntry[this->N];
	  for (int i = 0; i < other.N; i++)
	  {
		  if (other.HT[i].ContainsData)
		  {
			  this->HT[i].ContainsData = other.HT[i].ContainsData;
			  this->HT[i].Key = other.HT[i].Key;
			  this->HT[i].Value = other.HT[i].Value;
		  }
	  }
	  return *this;
  }
  //
  // getN
  //
  // Returns the capacity of the hash table, i.e. the value N that
  // was passed to the constructor.
  //
  int getN() const
  {
    return this->N;
  }

  //returns the number of elements
  int getCount()
  {
	  return count;
  }

  //
  // insert
  //
  // Inserts the given (key, value) pair into the hash table, overwriting
  // the previous value if already inserted.  If the insert was successful
  // then true is returned, otherwise false is returned (meaning the key
  // was not hashed successfully, e.g. due to improper formatting).
  //
  // NOTE: the caller must provide a function Hash(key, N) that returns
  // an integer in the the range 0..N-1, inclusive, if the key is valid.
  // If the key is invalid, the hash function must return a negative
  // integer. The hash function is provided by simply calling insert
  // with the name of your hash function.  Example:
  //
  //   int myHash(string key, int N)
  //   { ... }
  //
  //   int N = 10000;
  //   hashmap<string, double>  hmap(N);  
  //
  //   if (hmap.insert("deep-dish pizza", 19.99, myHash))
  //     cout << "inserted" << endl;
  //   else
  //     cout << "not inserted?!" << endl;
  //
  bool insert(KeyT key, ValueT value, function<int(KeyT,int)> Hash)
  {
    //
    // Call the provided hash function with the key, and N, the size
    // of our hash table:
    //
    int index = Hash(key, N);

    //
    // insert:
    //
    if (index < 0 || index >= N)  // invalid key or hash function:
    {
      return false;
    }
	
    while (HT[index].ContainsData)// Probe until an empty location is found
	{
		if (HT[index].Key == key)//overwritting of existing key
		{
			HT[index].Value = value;
			return true;
		}
		if (index == N - 1) // wrap to beginning
			index = 0;
		else
			index++;
	}
    HT[index].Key = key;
    HT[index].Value = value;
    HT[index].ContainsData = true;  // array location now contains data

	count++;

    return true;
  }

  //
  // search
  //
  // Searches the hash table for the matching key, and if found then
  // (1) the value is returned via the reference parameter and (2) true
  // is returned.  If the key could not be found, or if the hash 
  // function returned an invalid value (e.g. due to an improperly 
  // formatted key), then false is returned.
  //
  // NOTE: the caller must provide a function Hash(key, N) that returns
  // an integer in the the range 0..N-1, inclusive, if the key is valid.
  // If the key is invalid, the hash function must return a negative
  // integer. The hash function is provided by simply calling search
  // with the name of your hash function.  Example:
  //
  //   int myHash(string key, int N)
  //   { ... }
  //
  //   int N = 10000;
  //   hashmap<string, double>  hmap(N);
  //   ...
  //   double price;
  //   if (hmap.search("deep-dish pizza", price, myHash))
  //     cout << "deep-dish piazza costs " << price << endl;
  //   else
  //     cout << "not found?!" << endl;
  //
  bool search(KeyT key, ValueT& value, function<int(KeyT,int)> Hash) const
  {
    //
    // Call the provided hash function with the key, and N, the size
    // of our hash table:
    //
    int index = Hash(key, N);

    //
    // search:
    //
    if (index < 0 || index >= N)  // invalid key or hash function:
    {
      return false;
    }
	while (HT[index].ContainsData) // probe until HashTable Key equals passed key or 
	{                              // until the HashTable contains no Data.
		if (HT[index].Key == key)
		{
			value = HT[index].Value;
			return true;
		}
		if (index == N - 1) // wrap to beginning
			index = 0;
		else
			index++;
	}
	return false; //Not found 
	}


  //Returns a vector of keys that were inserted in the map
  void getVector(vector<KeyT>& keys)
  {
	  for (int i = 0; i < N; i++)
	  {
		  if (HT[i].ContainsData)
			  keys.push_back(HT[i].Key);
	  }
  }
};


