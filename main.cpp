#include <sstream>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include "directory.h"

int Find_user(string uname, vector<USER> database)
{
  int pos = -1; //use -1 to indicate no index was found for a user
  for(int i = 0; i < database.size(); i++)
  {
    if(database[i].u_name == uname)
    { pos = i;}
  }
  return pos;
}

// Purpose: prints out location of current directory
// Preconditions: current is pointer of directory to backtracked from
// Postconditions: location of current directory is printed
void print_directory(Directory* current, string uname)
{
  string listing = "$ ";
  Directory* navigator = current;
  while(navigator->get_parent() != NULL)
    {
      listing = "/" + navigator->get_name() + listing;
      navigator = navigator->get_parent();
    }
  listing = navigator->get_name() + listing;
  listing = uname + ":" + listing;
  cout << "\n" << listing;
}

// Purpose: prints out location of current directory and root location (for pwd)
// Preconditions: current is pointer of directory to backtracked from
// Postconditions: location of current directory is printed
void print_work_directory(Directory* current)
{
  string listing = "";
  Directory* navigator = current;
  while(navigator->get_parent() != NULL)
    {
      listing = "/" + navigator->get_name() + listing;
      navigator = navigator->get_parent();
    }
  listing = "/usr/local/home/user" + listing;
  cout << "\n" << listing;
}

int main()
{
 Directory* current_directory = NULL; //pointer to current directory
 Directory* root = NULL; //pointer to the Root directory
 bool quit_state = false; //Tell program when to stop
 string command, command_segment, chmod_permission, permission_check; //These are used for storing commands and parsing
 stringstream parser; // used to slowly feed command word by word
 int string_to_int; //used to convert from string to int (for permissions)
 bool fulldigit;
 int position = 0;
 char permissionbit;
 vector<USER> userbase; // vector that holds USER information
 vector<string> groups; // vector that holds groups created
 string current_user; //keeps track of current user;

 // Make default user and group
 userbase.push_back( USER("root"));
 current_user = "root";
 groups.push_back("users");

 //If root directory doesn't exist, generate one. "~" is the name of the root
 if(root == NULL)
 {
   root = new Directory("~", NULL, current_user);
   current_directory = root;
 }

 // main loop
 while (quit_state == false)
 {
   parser.str(""); // clean stringstream for clean parsing
   parser.clear();

   print_directory(current_directory, current_user); //print current location
   getline(cin, command); //take command from user
   parser.str(command);
   parser >> command_segment;


  if(command_segment == "ls")
  {
      if(parser.rdbuf()->in_avail() == 0)
      {
        position = Find_user(current_user, userbase);
        command = current_directory->get_permissions(current_directory->get_name(), current_user, userbase[position].groups);
        if(command[0] == '1')
        {
            current_directory->print_contents(current_user, userbase[position].groups);
        }
        else
        {
           cout << "ls ERROR: PERMISSION DEINED" << endl;
        }
      }
      else
      {
        parser >> command_segment;
        if(command_segment == "-l")
        {
          position = Find_user(current_user, userbase);
          command = current_directory->get_permissions(current_directory->get_name(), current_user, userbase[position].groups);
          if(command[0] == '1')
            {
                current_directory->long_list_print(current_user, userbase[position].groups);
            }
            else
            {
            cout << "ls ERROR: PERMISSION DEINED" << endl;
            }
        }
        else
        {
           cout << "FLAG OR COMMAND '" << command_segment << "' not recognized\n";
        }
      }
  }

  else if (command_segment == "pwd")
  {
    print_work_directory(current_directory);
  }

  else if (command_segment == "cd")
  {
    if(parser.rdbuf()->in_avail() == 0)
    {
        current_directory = root;
    }
    else
    {
      parser >> command_segment;
      if(parser.rdbuf()->in_avail() == 0)
      {
        if(command_segment == "..")
        {
            if(current_directory->get_parent() != NULL)
            {
              current_directory = current_directory->get_parent();
            }
        }
        else if(current_directory->get_subdirectory(command_segment) == NULL)
        {
         cout << "ERROR: DIRECTORY '" << command_segment << "' IS NOT FOUND";
        }
       else
        {
            position = Find_user(current_user, userbase);
            command = current_directory->get_permissions(command_segment, current_user, userbase[position].groups);
            if(command[2] == '1')
                {
                    current_directory = current_directory->get_subdirectory(command_segment);
                }
                else
                {
                    cout << "cd ERROR: PERMISSION DEINED" << endl;
                }
        }
      }
      else
      {
        cout << "CD COMMAND: '" << command << "' has too many arguments\n";
      }
    }
  }

  else if (command_segment == "mkdir")
  {
    while(parser.rdbuf()->in_avail() != 0)
    {
       parser >> command_segment;
        position = Find_user(current_user, userbase);
        command = current_directory->get_permissions(current_directory->get_name(), current_user, userbase[position].groups);
        if(command[1] == '1')
        {
            current_directory->add_directory(command_segment, current_directory, current_user);
        }
        else
        {
           cout << "mkdir ERROR: PERMISSION DEINED" << endl;
        }
    }
  }

  else if (command_segment == "rmdir")
  {
    while(parser.rdbuf()->in_avail() != 0)
    {
       parser >> command_segment;
       position = Find_user(current_user, userbase);
        command = current_directory->get_permissions(current_directory->get_name(), current_user, userbase[position].groups);
       if(command[1] == '1')
       {
            current_directory->remove_directory(command_segment);
       }
       else
       {
          cout << "rmdir ERROR: PERMISSION DEINED" << endl;
       }
    }
  }

  else if (command_segment == "touch")
  {
    while(parser.rdbuf()->in_avail() != 0)
    {
       parser >> command_segment;
       if(current_directory->Find_file(command_segment) == -1 && current_directory->Find_directory(command_segment) == -1)
       {
            position = Find_user(current_user, userbase);
            command = current_directory->get_permissions(current_directory->get_name(), current_user, userbase[position].groups);
            if(command[1] == '1')
            {
                current_directory->add_file(command_segment, current_user);
            }
            else
            {
            cout << "touch ERROR: PERMISSION DEINED" << endl;
            }
       }
       else
       {
           position = Find_user(current_user, userbase);
            command = current_directory->get_permissions(command_segment, current_user, userbase[position].groups);
            if(command[1] == '1')
            {
                current_directory->refresh_date(command_segment);
            }
            else
            {
            cout << "touch ERROR: PERMISSION DEINED" << endl;
            }
       }
    }
  }

  else if (command_segment == "rm")
  {
    while(parser.rdbuf()->in_avail() != 0)
    {
       parser >> command_segment;
       position = Find_user(current_user, userbase);
       command = current_directory->get_permissions(current_directory->get_name(), current_user, userbase[position].groups);
       if(command[1] == '1')
       {
           current_directory->remove_file(command_segment);
       }
       else
       {
          cout << "rm ERROR: PERMISSION DEINED" << endl;
       }
    }
  }

  else if (command_segment == "chmod")
  {
      fulldigit = true;
      parser >> chmod_permission;

      for(int k = 0; k < chmod_permission.length(); k++) //check if string is composed of all digits
      {
          if(! (chmod_permission[k] >= '0' && chmod_permission[k] <= '9' || chmod_permission[k] == ' '))
          {
            fulldigit  = false;
          }
      }

      string_to_int = 0;
      if(fulldigit == true) //a hack to prevent stoi from crashing when used on a string without digits
      {
          string_to_int = stoi(chmod_permission);
      }

      if(string_to_int < 0 || string_to_int > 777 || chmod_permission.length() != 3 || fulldigit == false)
      {
         cout << "chmod ERROR: PERMISSIONS NOT VALID" << endl;
      }
      else
      {
        if(parser.rdbuf()->in_avail() == 0)
        {
           cout << "chmod ERROR: NEED FILE/DIRECTORY INPUT" << endl;
        }
        else
        {
          parser >> command_segment;
          if(parser.rdbuf()->in_avail() != 0)
          {
           cout << "chmod ERROR: TOO MANY FILE/DIRECTORY INPUTS" << endl;
          }
          else
          {
            position = Find_user(current_user, userbase);
            command = current_directory->get_permissions(command_segment, current_user, userbase[position].groups);
            if(command[1] == '1')
            {
                current_directory->change_permissions(chmod_permission, command_segment);
            }
            else
            {
            cout << "chmod ERROR: PERMISSION DEINED" << endl;
            }
          }
        }
      }

  }

  else if(command_segment == "exit" || command_segment == "quit")
  {
    quit_state = true;
  }

  else if(command_segment == "userdel")
  {
    parser >> command_segment;
    if(command_segment == "-G")
    {
        parser >> command_segment;
        command = command_segment;

        parser >> command_segment;
        position = Find_user(command_segment, userbase);
        if(position != -1)
        {
            userbase[position].rmU2G(command);
        }
        else
        {
            cout << "userdel ERROR: USER NOT FOUND" << endl;
        }
    }
    else
    {
        parser >> command_segment;
        position = Find_user(command_segment, userbase);
        if(position != -1)
        {
            if(userbase.size() == 1)
            {
                cout << "userdel ERROR: CANNOT DELETE LAST REMAINING USER" << endl;
            }
            else
            {
                if(current_user == command_segment)
                {
                   cout << "userdel ERROR: CAN NOT DELETE CURRENT USER" << endl;;
                }
                else
                {
                    userbase.erase(userbase.begin() + position);
                }
            }
        }
        else
        {
            cout << "userdel ERROR: USER NOT FOUND" << endl;
        }
    }
  }

  else if(command_segment == "groupadd")
  {
    if(parser.rdbuf()->in_avail() == 0)
        {
           cout << "groupadd ERROR: NEED NAME INPUT" << endl;
        }
    else
    {
        parser >> command_segment;
        groups.push_back(command_segment);
    }
  }

  else if(command_segment == "usermod")
  {
    parser >> command_segment;
    if(command_segment == "-a")
    {
        parser >> command_segment;
        if(command_segment == "-G")
        {
            parser >> command_segment;
            if(find(groups.begin(),groups.end(), command_segment) != groups.end())
            {
               position = Find_user(current_user, userbase);
               if(find(userbase[position].groups.begin(),userbase[position].groups.end(), command_segment) == userbase[position].groups.end())
               {
                    userbase[position].groups.push_back(command_segment);
               }
            }
            else
            {
               cout << "usermod ERROR: GROUP NOT FOUND" << endl;
            }
        }

    }
    else
    {
        cout << "usermod ERROR: INVALID FLAG" << endl;
    }
  }

  else if(command_segment == "useradd")
  {
    parser >> command_segment;
    if(command_segment == "-G")
    {
        parser >> command_segment;
        command = command_segment;

        parser >> command_segment;
        position = Find_user(command_segment, userbase);

        parser.str(""); // clean stringstream for clean parsing
        parser.clear();

        parser.str(command);
        if(position == -1)
        {
            userbase.push_back( USER(command_segment));
            position = Find_user(command_segment, userbase);
            while(parser.good())
            {
                getline( parser, chmod_permission, ',');
                if(chmod_permission != "users")
                {
                    if(find(groups.begin(),groups.end(), chmod_permission) != groups.end())
                    {
                        userbase[position].groups.push_back(chmod_permission);
                    }
                    else
                    {
                        cout << "useradd ERROR: "  <<  chmod_permission << " DOES NOT EXIST" << endl;
                    }
                }
            }
        }
        else
        {
            cout << "useradd ERROR: USER ALREADY EXIST" << endl;
        }
    }
    else
    {
       parser >> command_segment;
       position = Find_user(command_segment, userbase);
       if(position = -1)
       {
           userbase.push_back( USER(command_segment));
       }
       else
       {
            cout << "useradd ERROR: USER ALREADY EXIST" << endl;
       }
    }
  }

  else if(command_segment == "switchto")
  {
    if(parser.rdbuf()->in_avail() == 0)
    {
        cout << "switchto ERROR: NEED USERNAME INPUT" << endl;
    }
    else
    {
        parser >> command_segment;
        position = Find_user(command_segment, userbase);

        if(position == -1)
        {
           cout << "switchto ERROR: USER NOT FOUND" << endl;
        }
        else
        {
            current_user = command_segment;
        }
    }
  }

  else if(command_segment == "chown")
  {
      parser >> command_segment;
      position = Find_user(command_segment, userbase);
      if(position != -1)
      {
          command = command_segment;
          parser >> command_segment;
          if(current_directory->Find_file(command_segment) != -1 || current_directory->Find_directory(command_segment) != -1)
          {
            position = Find_user(current_user, userbase);
            permission_check = current_directory->get_permissions(command_segment, current_user, userbase[position].groups);
            if(permission_check[1] == '1')
            {
                current_directory->change_owner(command, command_segment);
            }
            else
            {
            cout << "chown ERROR: PERMISSION DEINED" << endl;
            }
          }
          else
          {
             cout << "chown ERROR: FILE NOT FOUND" << endl;
          }
      }
      else
      {
        cout << "chown ERROR: USER NOT FOUND" << endl;
      }
  }

  else if(command_segment == "groups")
  {
    if(parser.rdbuf()->in_avail() == 0)
    {
        position = Find_user(current_user, userbase);
        for(int i = 0; i < userbase[position].groups.size(); i++)
        {
            cout << userbase[position].groups[i] << " ";
        }
    }
    else
    {
        cout << "groups ERROR: GROUPS DOES NOT TAKE PARAMETERS" << endl;
    }
  }

  else if(command_segment == "chgrp")
  {
      parser >> command_segment;
      if(find(groups.begin(),groups.end(), command_segment) != groups.end())
      {
          command = command_segment;
          parser >> command_segment;
          if(current_directory->Find_file(command_segment) != -1 || current_directory->Find_directory(command_segment) != -1)
          {
            position = Find_user(current_user, userbase);
            permission_check = current_directory->get_permissions(command_segment, current_user, userbase[position].groups);
            if(permission_check[1] == '1')
            {
                current_directory->change_group(command, command_segment);
            }
            else
            {
            cout << "chgrp ERROR: PERMISSION DEINED" << endl;
            }
          }
          else
          {
             cout << "chgrp ERROR: FILE NOT FOUND" << endl;
          }
      }
      else
      {
        cout << "chgrp ERROR: GROUP NOT FOUND" << endl;
      }
  }

  else if(command_segment[0] == '.' && command_segment[1] == '/')
  {
    command_segment.erase(0,2);
    if(current_directory->Find_file(command_segment) != -1)
    {
        position = Find_user(current_user, userbase);
        command = current_directory->get_permissions(command_segment, current_user, userbase[position].groups);

        if(command[2] == '1')
        {
            cout << command_segment << " IS EXECUTED" << endl;
        }
        else
        {
           cout << "./ ERROR: PERMISSION DEINED" << endl;
        }
    }
    else
    {
        cout << "./ ERROR: FILE NOT FOUND" << endl;
    }
  }

  else
  {
    cout << "ERROR: Command '" << command << "' not found";
  }

 }
    delete root;
    return 0;
}

