/****************************************************************************
  FileName     [ p2Main.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define main() function ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2016-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include "p2Json.h"

using namespace std;

int main()
{
   Json json;

   // Read in the csv file. Do NOT change this part of code.
   string jsonFile;
   cout << "Please enter the file name: ";
   cin >> jsonFile;
   if (json.read(jsonFile))
      cout << "File \"" << jsonFile << "\" was read in successfully." << endl;
   else
   {
      cerr << "Failed to read in file \"" << jsonFile << "\"!" << endl;
      exit(-1); // jsonFile does not exist.
   }

   // TODO read and execute commands
   // cout << "Enter command: ";
   while (true)
   {
      string com, arg;
      cout << "Enter command: ";
      cin >> com;
      getline(cin, arg);
      auto arg_start = arg.find_first_not_of(" ");
      auto arg_end = arg.find_last_not_of(" ");
      if (arg_start != string::npos)
         arg = arg.substr(arg_start, arg_end - arg_start + 1);
      else 
         arg = "";

      if (com == "SUM")
      {
         if (!arg.empty())
            cerr << "Error: Extra argument \"" << arg << "\"!!\n";
         else
            json.sum();
      }
      else if (com == "AVE")
      {
         if (!arg.empty())
            cerr << "Error: Extra argument \"" << arg << "\"!!\n";
         else
            json.ave();
      }
      else if (com == "MAX")
      {
         if (!arg.empty())
            cerr << "Error: Extra argument \"" << arg << "\"!!\n";
         else
            json.max();
      }
      else if (com == "MIN")
      {
         if (!arg.empty())
            cerr << "Error: Extra argument \"" << arg << "\"!!\n";
         else
            json.min();
      }
      else if (com == "ADD")
      {
         vector<string> args;
         size_t start = 0;
         size_t end = 0;

         if (arg.empty())
            cerr << "Error: Missing argument!!\n";
         else
         {
            while (1)
            {
               start = arg.find_first_not_of(" ", end);
               if (start == string::npos)
                  break;
               end = arg.find_first_of(" ", start);
               args.push_back(arg.substr(start, end - start));
            }
            if (args.size() < 2)
            {
               cerr << "Error: Missing argument after \"" << args[0] << "\"!!\n";
            }
            else if (args.size() > 2)
            {
               cerr << "Error: Extra argument \"" << args[2] << "\"!!\n";
            }
            else
            {
               bool args1IsInt = true;
               for (size_t i = 0; i < args[1].size(); i++)
               {
                  if (i == 0)
                  {
                     if (args[1][i] != '-' && !isdigit(args[1][i]))
                        args1IsInt = false;
                  }
                  else
                  {
                     if (!isdigit(args[1][i]))
                        args1IsInt = false;
                  }
               }
               if (args1IsInt)
                  json.add(args[0], stoi(args[1]));
               else
                  cerr << "Error: Illegal argument \"" << args[1] << "\"!!\n";
            }
         }
      }
      else if (com == "PRINT")
      {
         if (!arg.empty())
            cerr << "Error: Extra argument \"" << arg << "\"!!\n";
         else
            json.print();
      }
      else if (com == "EXIT")
      {
         break;
      }
      else
      {
         cerr << "Error: unknown command: \"" << com << "\"\n";
      }
   }
}
