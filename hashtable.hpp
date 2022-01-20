// Created by Tyler Starling

#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <stdio.h>
#include "hashtable.h"

using namespace std;
using namespace cop4530;

/******** Public Member Functions ********/
//  constructor that creates a hash table where the size of the vector
//  is set to prime_below(size) -size defaults to 101-
template<typename K, typename V>
HashTable<K,V>::HashTable(size_t size) {
    chaining.resize(prime_below(size));
    entries = 0;
}

template<typename K, typename V>
HashTable<K, V>::~HashTable() {   //  destructor; deletes all elements in the hash table
    clear();
}

template<typename K, typename V>
bool HashTable<K, V>::contains(const K &k) const{  //  checks if key k is in the hash table
    for(auto i = chaining[myhash(k)].begin(); i != chaining[myhash(k)].end(); ++i)   //  iterate from begin to end
        if(i->first == k) //  if we find k, then return true
            return true;

    return false;
}

template<typename K, typename V>
bool HashTable<K, V>::match(const pair<K,V> &kv) const{   //  checks if key-value pair is in the hash table
    auto &checker = chaining[myhash(kv.first)];
    if(find(checker.begin(),checker.end(), kv) != checker.end()){
        return true;
    }else{
        return false;
    }
}

//  adds the key-value pair kv into the hash table don't add if kv is already in the hash table
//  if they key is in the hash table but with a different value, the value should be updated to
//  the new one with kv
//  return true if kv is inserted or the value is updated; return false otherwise (kv in the has table)
template<typename K, typename V>
bool HashTable<K, V>::insert(const pair<K, V> &kv){
    if(match(kv)){
        return false;   //  kv is already in the table
    }else if(contains(kv.first)) {  //  if the key is in the hash table, but with a different value
        auto &checker = chaining[myhash(kv.first)];
        for(auto i = checker.begin(); i != checker.end(); ++i){ //  check
            if(i->first == kv.first){   //  similar code to contains here
                auto newChain = checker.erase(i);   //  calls upon the vector's built-in erase and stores the value for future use
                checker.insert(newChain, kv);   //  inserts the new value
            }
        }
        return true;
    }else{  //  brand new key-value pair conditional
        //  add the new pair to the hash table
        chaining[myhash(kv.first)].push_back(kv);
        //  we need to check for rehash
        if(++entries > chaining.capacity()){
            rehash();
        }
        return true;
    }
}

template<typename K, typename V>
bool HashTable<K, V>::insert(pair<K, V> && kv){  //  move version of insert
    pair<K,V> newPair = move(kv);
    return insert(newPair);
}

//  deletes the key k and the corresponding value if it's in the hash table
//  return true if k is deleted, return false otherwise (if k is not in hash table)
template<typename K, typename V>
bool HashTable<K, V>::remove(const K & k){
    auto & checker = chaining[myhash(k)];
    for(auto i = checker.begin(); i != checker.end(); ++i){
        if(i->first == k){
            checker.erase(i);
            --entries;
            return true;
        }
    }
    return false;
}

template<typename K, typename V>
void HashTable<K, V>::clear(){    //  deletes all elements in the hash table
    makeEmpty();
}

//  loads the contents of the file into the hash table
//  in the file, each line contains a single pair of key
//  and value, separated by a white space
template<typename K, typename V>
bool HashTable<K, V>::load(const char *filename){
    fstream inFile;
    pair<K, V> newPair;

    inFile.open(filename, fstream::in);
    if(!inFile){    //  bad file name given
        cout << "You have entered an invalid file name\n";
        return false;
    }
    char x; //  used for white space extraction
    while(inFile){  // loop to load all of the pairs
        getline(inFile, newPair.first, ' ');    //  get the key and use the white space as a delimiter to stop
        while(isspace(inFile.peek())){  //  if the next char is a space
            inFile.get(x);  //  get that space off the stream
            getline(inFile, newPair.second, '\n');  //  get the value and use the new line as a delimiter to stop
            insert(newPair);    //  inserts the loaded-in pair into the hash table
        }
    }
    inFile.close(); //  stop memory leaks
    return true;
}

//  displays all entries in the hash table
//  if an entry contains multiple key-value pairs,
//  separate them by a colon character
template<typename K, typename V>
void HashTable<K, V>::dump() const{
    for(int i = 0; i < chaining.size(); i++){   //  runs the size of the hash table
        cout << "v[" << i << "]:";
        for(auto j = chaining[i].begin(); j != chaining[i].end(); ++j){
            if(j != chaining[i].begin()) {  //  if an entry contains multiple key-value pairs
                cout << ":";
            }
            cout << j->first << " " << j->second;   //  displays the key, a space, and then the value
        }
        cout << "\n";
    }
}

template<typename K, typename V>
size_t HashTable<K, V>::size() const{  //  returns the size of the hash table
    return entries;
}

//  writes all of the elements in the hash table into a file with name filename
//  similar to the file format in the load function, each line contains
//  a pair of key-value pair, separated by a white space
template<typename K, typename V>
bool HashTable<K, V>::write_to_file(const char *filename) const{
    ofstream os;

    os.open(filename);
    if(os){
        for(int i = 0; i < chaining.size(); i++){   // runs the size of the hash table
            for(auto j = chaining[i].begin(); j != chaining[i].end(); ++j){
                os << j->first << " " << j->second << "\n"; //  writes each pair to the file
            }
        }
    }else{
        return false;
    }
    os.close(); //  stop the memory leaks
    return true;
}

/******* Private Helper Functions *******/
template<typename K, typename V>
void HashTable<K, V>::makeEmpty(){    //  deletes all elements in the hash table; clear() calls this function
    for(auto & chains: chaining){
        chaining.clear();
    }
    entries = 0;
}

template<typename K, typename V>
void HashTable<K, V>::rehash(){   //  called when the number of elements in the hash table is greater than the size of the vector
    auto temp = chaining;   //  store the old Hash Table to a temp variable
    chaining.resize(prime_below(chaining.size() * 2));  //  resize the hash table to twice the old amount

    for(auto & chains: chaining){   //  let's delete the old stuff
        chains.clear();
    }
    entries = 0;    //  finish em off


    for(auto & chains: temp){   //  out with the old, in with the new
        for(auto & pairs: chains){   //  inner loop to grab all the pairs at the index
            insert(move(pairs)); //  move semantics X.X
        }
    }
}

template<typename K, typename V>
size_t HashTable<K, V>::myhash(const K &k) const{   //  returns the index of the vector entry where k should be stored
    static hash<K> hf;
    return hf(k) % chaining.size();
}
template<typename K, typename V>
void HashTable<K, V>::setSize(size_t s){    //  similar to our constructor, but public
    chaining.resize(prime_below(s));
    entries = 0;
}

//  credit to Bob Myers
// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below (unsigned long n)
{
    if (n > max_prime)
    {
        cerr << "** input too large for prime_below()\n";
        return 0;
    }
    if (n == max_prime)
    {
        return max_prime;
    }
    if (n <= 1)
    {
        cerr << "** input too small \n";
        return 0;
    }

    // now: 2 <= n < max_prime
    vector <unsigned long> v (n+1);
    setPrimes(v);
    while (n > 2)
    {
        if (v[n] == 1)
            return n;
        --n;
    }

    return 2;
}

//  credit to Bob Myers
//Sets all prime number indexes to 1. Called by method prime_below(n)
template <typename K, typename V>
void HashTable<K, V>::setPrimes(vector<unsigned long>& vprimes)
{
    int i = 0;
    int j = 0;

    vprimes[0] = 0;
    vprimes[1] = 0;
    int n = vprimes.capacity();

    for (i = 2; i < n; ++i)
        vprimes[i] = 1;

    for( i = 2; i*i < n; ++i)
    {
        if (vprimes[i] == 1)
            for(j = i + i ; j < n; j += i)
                vprimes[j] = 0;
    }
}