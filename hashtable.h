// Created by Tyler Starling

#ifndef COP4530_PROGRAM_5_HASHTABLE_H
#define COP4530_PROGRAM_5_HASHTABLE_H

#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <stdio.h>
using namespace std;
namespace cop4530 {

    //  used by helpful functions
    static const unsigned int max_prime = 1301081;
    //  used if the initial capacity of the underlying vector of the hash table is zero
    static const unsigned int default_capacity = 11;

    template<typename K, typename V>

    class HashTable{
        public:
            HashTable(size_t size = 101);    //  constructor that creates a hash table where the size of the vector is set to prime_below(size)
            ~HashTable();    //  destructor
            bool contains(const K & k) const;  //  checks if key k is in the hash table
            bool match(const pair<K, V> &kv) const;   //  checks if key-value pair is in the hash table
            bool insert(const pair<K, V> &kv);  //  adds the key-value pair kv into the hash table
            bool insert(pair<K, V> &&kv);   //  move version of insert
            bool remove(const K & k);    //  deletes the key k and the corresponding value if it is in the hash table
            void clear();    //  deletes all elements in the hash table
            bool load(const char *filename); //  loads the content of the file with name filename into the hash table
            void dump() const; //  displays all entries in the hash table
            size_t size() const;   //  returns the size of the hash table (number of entries stored)
            bool write_to_file(const char *filename) const;    //  writes all elements in the hash table into a file with name filename
            void setSize(size_t s); //  function to set the size of the hash table
        private:
            int entries;    //  number of entries in the hash table
            vector<list<pair<K,V>>> chaining;    //  vector of lists (internal data structure of hash table)
            void makeEmpty();   //  deletes all elements in the hash table; clear() will call this function
            void rehash();  //  called when the number of elements in the hash table is greater than the size of the vector
            size_t myhash(const K &k) const;  //  returns the index of the vector entry where k should be stored

            // helpful to determine the proper prime numbers used in setting up the vector size
            unsigned long prime_below(unsigned long);
            void setPrimes(vector<unsigned long>&);
    };
#include "hashtable.hpp"
}
#endif
