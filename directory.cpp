#include "directory.h"

Directory :: Directory(string d_name, Directory* parent, string uname)
{
  name = d_name;
  parent_directory = parent;
  d_permissions[0] = '1';
  d_permissions[1] = '1';
  d_permissions[2] = '1';
  d_permissions[3] = '1';
  d_permissions[4] = '1';
  d_permissions[5] = '1';
  d_permissions[6] = '1';
  d_permissions[7] = '0';
  d_permissions[8] = '1';

  time_t now = time(0);
  d_date = ctime(& now);
  userd = uname;
  groupd = "users";
}

Directory :: ~Directory()
{
    for(int i = 0; i < subdirectory.size();i++)
    {
      delete subdirectory[i]; //remove subdirectory pointers from a directory to prevent memory leak
      subdirectory.erase(subdirectory.begin() + i);
    }

    for(int j = 0; j < file_storage.size();j++) //remove files pointers from a directory to prevent memory leak
    {
      file_storage.erase(file_storage.begin() + j);
    }
}

int Directory :: Find_file(string f_name)
{
   int pos = -1; //use -1 to indicate no index was found for a file
   for(int i = 0; i < file_storage.size(); i++)
   {
    if(file_storage[i].f_name == f_name)
    { pos = i;}
   }
   return pos;
}

int Directory :: Find_directory(string d_name)
{
  int pos = -1; //use -1 to indicate no index was found for a directory
  for(int i = 0; i < subdirectory.size(); i++)
  {
    if(subdirectory[i]->name == d_name)
    { pos = i;}
  }
  return pos;
}

void Directory :: add_file(string f_name, string uname)
{
  if(Find_file(f_name) == -1) //If a files doesn't exist, generate one and store in file_storage
  {
    S_File* temp = new S_File(f_name, uname);
    file_storage.push_back(*temp);
    delete temp;
  }
  else
  {
    cout << "ERROR: FILE '" << f_name << "' ALREADY EXISTS\n";
  }
}

void Directory :: remove_file(string f_name)
{
 int offset = Find_file(f_name);
  if(offset == -1) //If a file exist in a directory throw error
  {
    cout << "ERROR: FILE DOES NOT EXIST IN THIS DIRECTORY\n";
  }
  else
  {
    file_storage.erase(file_storage.begin() + offset); // delete file in a directory at index offset in file_storage
  }
}

void Directory :: add_directory(string d_name, Directory* parent, string uname)
{
  if(Find_directory(d_name) == -1) //If a directory doesn't exist, generate one and store in subdirectories
  {
    Directory* temp = new Directory(d_name, parent, uname);
    subdirectory.push_back(temp);
  }
  else
  {
    cout << "ERROR: DIRECTORY '" << d_name << "' ALREADY EXISTS\n";
  }
}

void Directory :: remove_directory(string d_name)
{
  int offset = Find_directory(d_name);
  if(offset == -1) // If a file doesn't exist in a directory throw error
  {
    cout << "ERROR: SUBDIRECTORY DOES NOT EXIST IN THIS DIRECTORY\n";
  }
  else
  {
    if(subdirectory[offset]->is_empty() == true)
    {
      delete subdirectory[offset]; //delete pointer in subdirectory to prevent memory leak
      subdirectory.erase(subdirectory.begin() + offset); // erase pointers from subdirectory
    }
    else
    {
        cout << "ERROR: DIRECTORY IS NOT EMPTY" << endl; //throw error if directory contains files/subdirectories
    }
  }
}

Directory* Directory :: get_parent()
{
  return parent_directory; //returns pointer to parent directory
}

string Directory :: get_name()
{
  return name; //returns name of directory
}

bool Directory :: is_empty()
{
  bool directory_empty = false;
  if(subdirectory.empty() && file_storage.empty())
    { directory_empty = true; }
  return directory_empty; //returns true if directory is empty/ false otherwise
}

Directory* Directory :: get_subdirectory(string d_name)
{
  int pos = Find_directory(d_name);
  Directory* directory_location = NULL;
  if(pos == -1)
  {
    return directory_location; //return NULL if subdirectory doesn't exit
  }
  else
  {
    directory_location = subdirectory[pos];
    return directory_location; //return pointer to subdirectory
  }
}

void Directory :: print_contents(string oname, vector<string> groups)
{
  vector<string> listing;
  string permission = "";
  for(int i = 0; i < subdirectory.size(); i++)
  {
    permission = get_permissions(subdirectory[i]->name, oname, groups);
    if(permission[0] == '1')
    {
       listing.push_back(subdirectory[i]->name); //put the names of all files in file_storage into listing vector
    }
  }
  for(int i = 0; i < file_storage.size(); i++)
  {
    permission = get_permissions(file_storage[i].f_name, oname, groups);
    if(permission[0] == '1')
    {
        listing.push_back(file_storage[i].f_name); //put the names of all directories in subdirectories into listing vector
    }
  }

  sort(listing.begin(), listing.end()); //sort listing into alphabetical order
  for(int i = 0; i < listing.size(); i++)
  {
    cout << listing[i] << "\t"; //print listing
  }
}

void Directory :: long_list_print(string oname, vector<string> groups)
{
  char refrence[10] = "rwxrwxrwx"; //FULL permission char array
  string permission = "";
  cout << "total " << file_storage.size() << endl;
  //This loop is to print files
  for(int k = 0; k < file_storage.size(); k++)
  {
    permission = get_permissions(file_storage[k].f_name, oname, groups);
    if(permission[0] == '1')
    {
    cout << "-";
    for(int j = 0; j < 9; j++)
    {
      if(file_storage[k].f_permissions[j] == '1')
      {
          cout << refrence[j];
      }
      else
      {
          cout << "-";
      }
    }
    cout << " 1 " << file_storage[k].user << " " << file_storage[k].group << "\t" << file_storage[k].f_size << "\t" << file_storage[k].date << " " << file_storage[k].f_name << endl;
    }
  }
  //This loop is to print directories
    for(int k = 0; k < subdirectory.size(); k++)
  {
    permission = get_permissions(subdirectory[k]->name, oname, groups);
    if(permission[0] == '1')
    {
    cout << "d";
    for(int j = 0; j < 9; j++)
    {
      if(subdirectory[k]->d_permissions[j] == '1')
      {
          cout << refrence[j];
      }
      else
      {
          cout << "-";
      }
    }
    cout << " 2 " << subdirectory[k]->userd << " " << subdirectory[k]->groupd << "\t 0 \t" << subdirectory[k]->d_date << " " << subdirectory[k]->name << endl;
    }
  }

}

void  Directory :: refresh_date(string f_name)
{
    int pos = Find_directory(f_name); //find index of directory/file to updated
    if(pos != -1)
    {
      time_t now = time(0);
      subdirectory[pos]->d_date = ctime(& now);
    }
    else
    {
      pos = Find_file(f_name);
      time_t now = time(0);
      file_storage[pos].date = ctime(& now);
    }
}

void Directory :: change_permissions(string permit, string f_name)
{
  int location, conveter;
  string digit;
  if(Find_file(f_name) != -1)
  {
    location = Find_file(f_name);
    for(int i = 0; i < 3; i++)
    {
      digit = permit[i]; //parse digits of permit
      conveter = stoi(digit);

      switch (conveter)
      {
        case 0:
            file_storage[location].f_permissions[3*i] = '0';
            file_storage[location].f_permissions[3*i + 1] = '0';
            file_storage[location].f_permissions[3*i + 2] = '0';
            break;
        case 1:
            file_storage[location].f_permissions[3*i] = '0';
            file_storage[location].f_permissions[3*i + 1] = '0';
            file_storage[location].f_permissions[3*i + 2] = '1';
            break;
        case 2:
            file_storage[location].f_permissions[3*i] = '0';
            file_storage[location].f_permissions[3*i + 1] = '1';
            file_storage[location].f_permissions[3*i + 2] = '0';
            break;
        case 3:
            file_storage[location].f_permissions[3*i] = '0';
            file_storage[location].f_permissions[3*i + 1] = '1';
            file_storage[location].f_permissions[3*i + 2] = '1';
            break;
        case 4:
            file_storage[location].f_permissions[3*i] = '1';
            file_storage[location].f_permissions[3*i + 1] = '0';
            file_storage[location].f_permissions[3*i + 2] = '0';
            break;
        case 5:
            file_storage[location].f_permissions[3*i] = '1';
            file_storage[location].f_permissions[3*i + 1] = '0';
            file_storage[location].f_permissions[3*i + 2] = '1';
            break;
        case 6:
            file_storage[location].f_permissions[3*i] = '1';
            file_storage[location].f_permissions[3*i + 1] = '1';
            file_storage[location].f_permissions[3*i + 2] = '0';
            break;
        case 7:
            file_storage[location].f_permissions[3*i] = '1';
            file_storage[location].f_permissions[3*i + 1] = '1';
            file_storage[location].f_permissions[3*i + 2] = '1';
            break;
      }
    }
  }

  else if(Find_directory(f_name) != -1)
  {
    location = Find_directory(f_name);

    for(int i = 0; i < 3; i++)
    {
      digit = permit[i];
      conveter = stoi(digit); //parse digits of permit

      switch (conveter)
      {
        case 0:
            subdirectory[location]->d_permissions[3*i] = '0';
            subdirectory[location]->d_permissions[3*i + 1] = '0';
            subdirectory[location]->d_permissions[3*i + 2] = '0';
            break;
        case 1:
            subdirectory[location]->d_permissions[3*i] = '0';
            subdirectory[location]->d_permissions[3*i + 1] = '0';
            subdirectory[location]->d_permissions[3*i + 2] = '1';
            break;
        case 2:
            subdirectory[location]->d_permissions[3*i] = '0';
            subdirectory[location]->d_permissions[3*i + 1] = '1';
            subdirectory[location]->d_permissions[3*i + 2] = '0';
            break;
        case 3:
            subdirectory[location]->d_permissions[3*i] = '0';
            subdirectory[location]->d_permissions[3*i + 1] = '1';
            subdirectory[location]->d_permissions[3*i + 2] = '1';
            break;
        case 4:
            subdirectory[location]->d_permissions[3*i] = '1';
            subdirectory[location]->d_permissions[3*i + 1] = '0';
            subdirectory[location]->d_permissions[3*i + 2] = '0';
            break;
        case 5:
            subdirectory[location]->d_permissions[3*i] = '1';
            subdirectory[location]->d_permissions[3*i + 1] = '0';
            subdirectory[location]->d_permissions[3*i + 2] = '1';
            break;
        case 6:
            subdirectory[location]->d_permissions[3*i] = '1';
            subdirectory[location]->d_permissions[3*i + 1] = '1';
            subdirectory[location]->d_permissions[3*i + 2] = '0';
            break;
        case 7:
            subdirectory[location]->d_permissions[3*i] = '1';
            subdirectory[location]->d_permissions[3*i + 1] = '1';
            subdirectory[location]->d_permissions[3*i + 2] = '1';
            break;
      }
    }

  }

  else
  {
      cout << "ERROR: FILE/DIRECTORY NOT FOUND" << endl;
  }
}

void  Directory :: change_owner(string oname, string f_name)
{
    int pos = Find_file(f_name); //find index of file to updated
    if(pos != -1)
       {
          file_storage[pos].user = oname;
       }
    else
        {
          pos = Find_directory(f_name);
          subdirectory[pos]->setOWN(oname);
        }
}

void  Directory :: change_group(string gname, string f_name)
{
    int pos = Find_file(f_name); //find index of file to updated
    if(pos != -1)
    {
        file_storage[pos].group = gname;
    }
    else
    {
        pos = Find_directory(f_name);
        subdirectory[pos]->setGRP(gname);
    }
}

string Directory :: get_permissions(string f_name, string oname, vector<string> groups)
{
    string n_permissions; //3 char string that ranges from 000-111 to represent RWX permissions

    int pos = Find_file(f_name);
    if(pos != -1)
    {
        if(file_storage[pos].user == oname) //check if user is owner
        {
            n_permissions += file_storage[pos].f_permissions[0];
            n_permissions += file_storage[pos].f_permissions[1];
            n_permissions += file_storage[pos].f_permissions[2];
        }
        else if(find(groups.begin(),groups.end(), file_storage[pos].group) != groups.end()) //check if user is in group
        {
            n_permissions += file_storage[pos].f_permissions[3];
            n_permissions += file_storage[pos].f_permissions[4];
            n_permissions += file_storage[pos].f_permissions[5];
        }
        else //use default group is all else fails
        {
            n_permissions += file_storage[pos].f_permissions[6];
            n_permissions += file_storage[pos].f_permissions[7];
            n_permissions += file_storage[pos].f_permissions[8];
        }
    }
    else
    {
        pos = Find_directory(f_name);
        if(pos != -1)
        {
           if(subdirectory[pos]->userd == oname) //check if user is owner
            {
            n_permissions += subdirectory[pos]->d_permissions[0];
            n_permissions += subdirectory[pos]->d_permissions[1];
            n_permissions += subdirectory[pos]->d_permissions[2];
            }
            else if(find(groups.begin(),groups.end(), subdirectory[pos]->groupd) != groups.end()) //check if user is in group
            {
            n_permissions += subdirectory[pos]->d_permissions[3];
            n_permissions += subdirectory[pos]->d_permissions[4];
            n_permissions += subdirectory[pos]->d_permissions[5];
            }
            else //use default group is all else fails
            {
            n_permissions += subdirectory[pos]->d_permissions[6];
            n_permissions += subdirectory[pos]->d_permissions[7];
            n_permissions += subdirectory[pos]->d_permissions[8];
            }
        }
        else if(f_name == name)
        {
           if(userd == oname) //check if user is owner
            {
            n_permissions += d_permissions[0];
            n_permissions += d_permissions[1];
            n_permissions += d_permissions[2];
            }
            else if(find(groups.begin(),groups.end(), subdirectory[pos]->groupd) != groups.end()) //check if user is in group
            {
            n_permissions += d_permissions[3];
            n_permissions += d_permissions[4];
            n_permissions += d_permissions[5];
            }
            else //use default group is all else fails
            {
            n_permissions += d_permissions[6];
            n_permissions += d_permissions[7];
            n_permissions += d_permissions[8];
            }
        }
        else
        {
            n_permissions += "000"; //Fail safe in the event that file/directory doesn't exist
        }
    }
    return n_permissions;
}

void Directory :: setGRP(string gname)
{
    groupd = gname;
}

void Directory :: setOWN(string oname)
{
    userd = oname;
}
