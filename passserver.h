// Created by Tyler Starling


#ifndef COP4530_PROGRAM_5_PASSSERVER_H
#define COP4530_PROGRAM_5_PASSSERVER_H

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "hashtable.h"
//#include <crypt.h>
//#include <unistd.h>

using namespace std;
using namespace cop4530;

class PassServer{
public:

    PassServer(size_t size=101);    //  constructor that creates a hash table of the specified size
    ~PassServer();  //  destructor
    bool load(const char *filename);    //  loads a password file into the HashTable object
    bool addUser(pair<string, string> & kv);    //  adds a new user and password
    bool addUser(pair<string, string> && kv);   //  move version of addUser
    bool removeUser(const string & k);  //  deletes an existing user with username k
    bool changePassword(const pair<string, string> &p, const string & newpassword); //  changes an existing user's password
    bool find(const string & user) const; //  checks if a user exists in the hash table
    void dump() const;    //  shows the structure and contents of the HashTable object to the screen
    size_t size() const;  // returns the size of the HashTable
    bool write_to_file(const char *filename) const;   //  saves the username and password combination into a file

private:
    HashTable<string, string> hashBoy;  //  hashtable implemented as adaptee class
    string encrypt(const string & str); //  encrypts the parameter str and returns the encrypted string
};

#endif //COP4530_PROGRAM_5_PASSSERVER_H