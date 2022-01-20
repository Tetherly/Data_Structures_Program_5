// Created by Tyler Starling

#include "passserver.h"
#include "hashtable.h"

using namespace cop4530;

/***** Public Member Functions *****/

PassServer::PassServer(size_t size){    //  constructor that creates a hash table of the specified size
    hashBoy.setSize(size);
}

PassServer::~PassServer(){  //  destructor
    hashBoy.clear();
}

bool PassServer::load(const char *filename){    //  loads a password file into the hash table
    return hashBoy.load(filename);
}

//  adds a new username and password; password is passed in
//  as plaintext and should be encrypted before insertion
bool PassServer::addUser(pair<string, string> & kv){
    string encryption = encrypt(kv.second); //  creates a variable to hold the encrypted password
    pair<string, string> encryptedPair(kv.first, encryption); //  creates a new pair (username, encrypted password)
    return hashBoy.insert(encryptedPair);   //  inserts the username and encrypted password into the hash table
}

bool PassServer::addUser(pair<string, string> && kv){   //  move version of addUser
    pair<string, string> newPair(move(kv));    //  creates a new pair and stores the old pair's data into it
    return addUser(newPair);    //  calls the other version of addUser
}

bool PassServer::removeUser(const string & k){  //  deletes an existing user with username k
    hashBoy.remove(k);
}

//  changes an existing user's password; both passwords are passed in as plaintexts
//  encrypt the passwords before interacting with the hash table
//  return false if the user is not in the hash table, if p.second doesn't match
//  the current password or new password = old password, return false
bool PassServer::changePassword(const pair<string, string> &p, const string & newpassword){
    if(p.second != newpassword && hashBoy.contains(p.first)){    //  if the username exists and the new password doesn't equal the old password
        //  encrypt the passwords first
        string oldPW = encrypt(p.second);
        string newPW = encrypt(newpassword);

        //  create a new pair to contain the old encrypted password
        pair<string, string> oldPairEncrypted(p.first, oldPW);

        if(hashBoy.match(oldPairEncrypted)){
            //  if the old username and encrypted password are in the hash table, make a new pair with the new encrypted password
            pair<string, string> newPairEncrypted(p.first, newPW);
            //  insert the new pair into the hash table and return true
            hashBoy.insert(newPairEncrypted);
            return true;
        }else{  //  username and/or old password was not part of the hash table; return false
            return false;
        }
    }else{
        return false;
    }
}

bool PassServer::find(const string & user) const{   //  checks if a user exists in the hash table
    return hashBoy.contains(user);
}

void PassServer::dump() const { //  shows the structure and contents of the hash table to the screen
    hashBoy.dump();
}

size_t PassServer::size() const{    //  returns the size of the hash table
    return hashBoy.size();
}

bool PassServer::write_to_file(const char *filename) const {    //  saves the username & password combo into a file
    return hashBoy.write_to_file(filename);
}

/***** Private Helper Function *****/

string PassServer::encrypt(const string &str) {

    char salt[] = "$1$########";
    string key = str;

    char * password = new char [100];
    char * encrypted = new char[22];
    strcpy (password, crypt(key.c_str(), salt));

    for(int i = 12; i < 34; i++){
        encrypted[i - 12] = password[i];
    }
    return encrypted;
}