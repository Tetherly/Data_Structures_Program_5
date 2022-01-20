// Created by Tyler Starling
#include <iostream>
#include<stdio.h>
#include "passserver.h"
#include "hashtable.h"

void Menu();

int main() {

    int input;
    char userInput;

    cout << "Please enter a preferred hash table capacity: ";   //  prompt the user for hash table capacity
    cin >> input;

    if(input <= 0){ //  input validation
        cout << "\nInvalid capacity. Setting hash table capacity to default capacity of 11\n";
        input = 11;
    }

    PassServer ps(input);   //  creates the PassServer based on user input
    char bufferClear;

    do{ //  loops until the user enter x
       Menu();  //  loops the menu function
       cin >> userInput;

       if(userInput == 'l' || userInput == 'L'){    //  load from file

            string file;


            cout << "\nEnter a file name to load from: ";

            if(isspace(cin.peek()) && !cin.eof()){  //  clears the buffer
                cin.get(bufferClear);
            }
            getline(cin, file);
       }else if(userInput == 'a' || userInput == 'A'){  //  add user

           string username, password;

           cout << "\nEnter the username you'd like to add: ";
           if(isspace(cin.peek()) && !cin.eof()){  //  clears the buffer
               cin.get(bufferClear);
           }
           getline(cin, username);  //  gets the username

           cout << "\nEnter the password you'd like to add: ";
           if(isspace(cin.peek()) && !cin.eof()){  //  clears the buffer
               cin.get(bufferClear);
           }
           getline(cin, password);  //  gets the password

           //  creates a unique pair for the new user/password
           pair<string, string> newUser(username, password);

           //   attempts to add the new user; outputs if done or user already existed
           if(ps.addUser(newUser)){
               cout << "\nUser " << username << " has been added.\n";
           }else{
               cout << "\nCould not add the username. User already exists.\n";
           }
       }else if(userInput == 'r' || userInput == 'R'){  //  remove user

           string username;

           cout << "\nEnter the username you'd like to remove: ";
           if(isspace(cin.peek()) && !cin.eof()){  //  clears the buffer
               cin.get(bufferClear);
           }
           getline(cin, username);  //  gets the username to be removed

           //   attempts to remove the user specified; outputs if done or if the user couldn't be removed (didn't exist)
           if(ps.removeUser(username)){
               cout << "\nUser " << username << " has been deleted.\n";
           }else{
               cout << "\nCould not remove the user you've specified. User was not found.\n";
           }
       }else if(userInput == 'c' || userInput == 'C'){  //  change user password

           string username, oldPassword, newPassword;

           cout << "\nEnter the username: ";
           if(isspace(cin.peek()) && !cin.eof()){  //  clears the buffer
               cin.get(bufferClear);
           }
           getline(cin, username);  //  gets the username

           cout << "\nEnter the current password: ";
           if(isspace(cin.peek()) && !cin.eof()){  //  clears the buffer
               cin.get(bufferClear);
           }
           getline(cin, oldPassword);  //  gets the username

           cout << "\nEnter the new password: ";
           if(isspace(cin.peek()) && !cin.eof()){  //  clears the buffer
               cin.get(bufferClear);
           }
           getline(cin, newPassword);  //  gets the username

           //   create a pair for the username and old password
           pair<string, string> old(username, oldPassword);

           //   attempts to change the password; outputs if the password was changed or failed to change
           if(ps.changePassword(old, newPassword)){
               cout << "\nThe password has been updated for the user " << username << ".\n";
           }else{
               cout << "\nAn error occurred. Could not change " << username << "'s password\n";
           }
       }else if(userInput == 'f' || userInput == 'F'){  //  find user

           string username;

           cout << "\nEnter the username you'd like found: ";
           if(isspace(cin.peek()) && !cin.eof()){  //  clears the buffer
               cin.get(bufferClear);
           }
           getline(cin, username);  //  gets the username for the user to be found

           //   attempts to find the user specified and outputs if the user was found successfully or not
           if(ps.find(username)){
               cout << "\nUser " << username << " was found.\n";
           }else{
               cout << "\nUser " << username << " was not found.\n";
           }
       }else if(userInput == 'd' || userInput == 'D'){  //  dump
           cout << "\n";
           ps.dump();   //  dumping it
       }else if(userInput == 's' || userInput == 'S'){  //  gets the size of the hashtable
           cout << "\nThe size of the hashtable is: " << ps.size() << "\n";   //  shows the size of the current hashtable
       }else if(userInput == 'w' || userInput == 'W'){  //  writes to file
            string file;

            cout << "\nEnter the file name to write to: ";
           if(isspace(cin.peek()) && !cin.eof()){  //  clears the buffer
               cin.get(bufferClear);
           }
           getline(cin, file);
           ps.write_to_file(file.c_str());
       }else if(userInput == 'x' || userInput == 'X'){  //  exit
            //  exit case
       }else{
           cout << "\nPlease enter a valid character from the menu.\n";
       }
    }while(userInput != 'x' && userInput != 'X');

    return 0;
}

void Menu()
{
    cout << "\n\n";
    cout << "l - Load From File" << endl;
    cout << "a - Add User" << endl;
    cout << "r - Remove User" << endl;
    cout << "c - Change User Password" << endl;
    cout << "f - Find User" << endl;
    cout << "d - Dump HashTable" << endl;
    cout << "s - HashTable Size" << endl;
    cout << "w - Write to Password File" << endl;
    cout << "x - Exit program" << endl;
    cout << "\nEnter choice : ";
}
