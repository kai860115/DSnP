/****************************************************************************
  FileName     [ p2Json.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Json and JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "p2Json.h"

using namespace std;

// Implement member functions of class Row and Table here
bool Json::read(const string &jsonFile)
{
   fstream fin(jsonFile, fstream::in);
   if (!fin.is_open())
      return false;
   string line;
   while (getline(fin, line))
   {
      if (line == "{" || line == "}")
         continue;
      string s[2];
      auto key_start = line.find_first_of("\"");
      if (key_start < line.length())
      {
         int key_end = line.find_first_of("\"", key_start + 1);
         s[0] = line.substr(key_start + 1, key_end - key_start - 1);
         for (auto e : line.substr(key_end, line.length() - key_end))
         {
            if (e == ' ' || e == ',' || e == '"' || e == ':')
               continue;
            else
               s[1] += e;
         }
         _obj.push_back(JsonElem(s[0], stoi(s[1])));
      }
   }
   return true; // TODO
}

void Json::print()
{
   cout << "{\n";
   for (size_t i = 0; i < _obj.size(); i++)
   {
      if (i < _obj.size() - 1) 
         cout << "  " << _obj[i] << ",\n";
      else 
         cout << "  " << _obj[i] << "\n";
   }
   cout << "}\n";
}

void Json::sum()
{
   if (_obj.empty())
      cerr << "Error: No element found!!\n";
   else
   {
      int s = 0;
      for (auto &e : _obj)
      {
         s += e.get_value();
      }
      cout << "The summation of the values is: " << s << ".\n";
   }
}

void Json::ave()
{
   if (_obj.empty())
      cerr << "Error: No element found!!\n";
   else
   {
      double s = 0;
      for (auto &e : _obj)
      {
         s += e.get_value();
      }
      cout << "The average of the values is: " << fixed << setprecision(1) << s / _obj.size() << ".\n";
   }
}

void Json::max()
{
   if (_obj.empty())
      cerr << "Error: No element found!!\n";
   else
   {
      JsonElem max = _obj[0];
      for (auto &e : _obj)
      {
         if (e.get_value() > max.get_value())
         {
            max = e;
         }
      }
      cout << "The maximum element is: { " << max << " }.\n";
   }
}

void Json::min()
{
   if (_obj.empty())
      cerr << "Error: No element found!!\n";
   else
   {
      JsonElem min = _obj[0];
      for (auto &e : _obj)
      {
         if (e.get_value() > min.get_value())
         {
            min = e;
         }
      }
      cout << "The minimum element is: { " << min << " }.\n";
   }
}

void Json::add(const string &k, int v)
{
   for (auto &e : _obj)
   {
      if (k == e.get_key())
      {
         cerr << "Error: Key \"" << k << "\" is repeated!!\n";
         return;
      }
   }
   _obj.push_back(JsonElem(k, v));
}

ostream &
operator<<(ostream &os, const JsonElem &j)
{
   return (os << "\"" << j._key << "\" : " << j._value);
}
