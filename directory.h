#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>
#include <ctime>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

// --------------- USER CLASS --------------
class USER
{
public:
    string u_name;
    vector<string> groups;

    USER(string name)
    {
        u_name = name;
        groups.push_back("users");
    }

    int Find_group(string gname)
    {
        int pos = -1; //use -1 to indicate no index was found for a pos
        for(int i = 0; i < groups.size(); i++)
        {
            if(groups[i] == gname)
            { pos = i;}
        }
        return pos;
    }
    void addU2G(string gname)
    {
        groups.push_back(gname);
    }
    void rmU2G(string gname)
    {
        int offset = Find_group(gname);
        if(offset == -1) //If a user isn't in a group
        {
            cout << "ERROR: USER NOT IN GROUP\n";
        }
        else
        {
            groups.erase(groups.begin() + offset); // delete user from group
        }
    }
};


// --------------- FILE CLASS --------------
class S_File
{
public:

 string f_name; //Name of file
 string user; //Stores the user-name of file creator
 string group; //Stores the user-group of file creator
 string date; //Stores date of file creation
 int f_size; //The Size of the file
 char f_permissions[9]; //Stores an array of 0's and 1's that indicate permission sets

  // Purpose: Parameterized Constructor for the File Class
  // Postconditions: A file is created with the name "name", time is set to current time/date,
  // size is randomized and user is set to "user by default, Permissions are rw-r--r-- by default
  S_File(string name, string uname)
  {
    srand (time(NULL));
    time_t now = time(0);
    date = ctime(& now);

    f_name = name;
    user = uname;
    group = "users";
    f_size = rand() % 2048;
    f_permissions[0] = '1';
    f_permissions[1] = '1';
    f_permissions[2] = '0';
    f_permissions[3] = '1';
    f_permissions[4] = '1';
    f_permissions[5] = '0';
    f_permissions[6] = '1';
    f_permissions[7] = '0';
    f_permissions[8] = '0';
  }
};

// --------------- DIRECTORY CLASS --------------
class Directory
{
    vector<S_File> file_storage; //Vector for storing Files contained in a directory
    vector<Directory*> subdirectory; //Vector for storing Directories contained in a directory
    Directory* parent_directory; // Pointer to the directory that contains the current directory
    string name; //Name of directory
    string d_date; //Date directory was created
    char d_permissions[9]; //Stores an array of 0's and 1's that indicate permission sets
    string userd; //Stores the user-name of directory creator
    string groupd; //Stores the user-group of directory creator

public:

    // Purpose: Parameterized Constructor for the Directory Class
    // Preconditions: d_name is the name of the directory, parent is a pointer to parent directory
    // Postconditions: A directory is created by the name "d_name" and parent directory set to "parent"
    Directory(string d_name, Directory* parent, string uname);

    // Purpose: Default Destructor for the Directory Class
    // Postconditions: Destruct a Directory
    ~Directory();

    // Purpose: Finds the index location of a file in a Directory's file_storage
    // Preconditions: f_name is the name of the file to be found
    // Postconditions: Return the index of a file with the name "f_name"; if not found return -1
    int Find_file(string f_name);

    // Purpose: Finds the index location of a directory in a Directory's subdirectories
    // Preconditions: d_name is the name of the directories to be found
    // Postconditions: Return the index of a directory with the name "d_name"; if not found return -1
    int Find_directory(string d_name);

    // Purpose: Add file to file_storage
    // Preconditions: f_name is the name of the file that will be generated
    // Postconditions: generate a file named "f_name" and store in file_storage
    void add_file(string f_name, string uname);

    // Purpose: removes file from file_storage
    // Preconditions: f_name is the name of the file that will be deleted
    // Postconditions: A file named f_name is removed from file_storage
    void remove_file(string f_name);

    // Purpose: Add directory to subdirectories
    // Preconditions: d_name is the name of the directory that will be generated, parent is a pointer to parent directory
    // Postconditions: generate a directory named "d_name" and store in subdirectories
    void add_directory(string d_name, Directory* parent, string uname);

    // Purpose: removes directory from subdirectories
    // Preconditions: d_name is the name of the directory that will be deleted
    // Postconditions: A directory named d_name is removed from subdirectories
    void remove_directory(string d_name);

    // Purpose: An accessor function to obtain the parent of a directory
    // Postconditions: returns a pointer to the parent directory
    Directory* get_parent();

    // Purpose: An accessor function to obtain the name of a directory
    // Postconditions: returns the name of the directory
    string get_name();

    // Purpose: A function used to check if a directory is empty (contains no files or subdirectories)
    // Postconditions: returns true if directory is empty, false otherwise
    bool is_empty();

    // Purpose: Get a pointer to a directory in subdirectories
    // Preconditions: d_name is the name of the directory to be found and refrenced
    // Postconditions: Returns pointer to subdirectory named "d_name"/ returns NULL if subdirectory doesn't exist
    Directory* get_subdirectory(string d_name);

    // Purpose: Prints the names of files and subdirectories in a directory
    // Postconditions: Names of files and subdirectories contain in a directory are printed out
    void print_contents(string oname, vector<string> groups);

    // Purpose: Prints the names/date/size/permissions of files and subdirectories in a directory
    // Postconditions: Names/date/size/permissions of files and subdirectories contain in a directory are printed out
    void long_list_print(string oname, vector<string> groups);

    // Purpose: Update the date string of a file/directory
    // Preconditions: f_name is the name of a file or directory
    // Postconditions: The date member of a file/directory named "f_name" is updated to current time/date
    void refresh_date(string f_name);

    // Purpose: Changes permission settings on a file/directory
    // Preconditions: permit is a 3 digit octal number to indicate permission changes, f_name is the name of file/directory to be changed
    // Postconditions: Changes permissions of file/directory named "f_name" to settings dictated by "permit"
    void change_permissions(string permit, string f_name);

    // Purpose: Changes owner settings on a file
    // Preconditions: oname is the name of a user, f_name is the name of the file
    // Postconditions: Changes user setting of file named "f_name" to settings dictated by oname
    void change_owner(string oname, string f_name);

    // Purpose: Changes group setting on a file
    // Preconditions: gname is the name of a group, f_name is the name of the file
    // Postconditions: Changes group setting of file named "f_name" to settings dictated by oname
    void change_group(string gname, string f_name);

    // Purpose: return a string that has RWX permissions between a file and a user
    // Preconditions: f_name is the file name, oname is the name of the user, groups is a vector of groups that the user belongs to
    // Postconditions: a string with information pertaining to permissions that the user has is returned
    string get_permissions(string f_name, string oname, vector<string> groups);

    // Purpose: change groupd of a directory
    // Preconditions: gname is the name that groupd is set to
    // Postconditions: groupd is now equal to gname
    void setGRP(string gname);

    // Purpose: change userd of a directory
    // Preconditions: oname is the name that userd is set to
    // Postconditions: userd is now equal to oname
    void setOWN(string oname);

};

#endif
