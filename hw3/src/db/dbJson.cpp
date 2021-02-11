/****************************************************************************
  FileName     [ dbJson.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Json member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <cmath>
#include <string>
#include <algorithm>
#include "dbJson.h"
#include "util.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream&
operator << (ostream& os, const DBJsonElem& j)
{
   os << "\"" << j._key << "\" : " << j._value;
   return os;
}

istream& operator >> (istream& is, DBJson& j)
{
   // TODO: to read in data from Json file and store them in a DB 
   // - You can assume the input file is with correct JSON file format
   // - NO NEED to handle error file format
   assert(j._obj.empty());
   string line;
   while (getline(is, line))
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
         j._obj.push_back(DBJsonElem(s[0], stoi(s[1])));
      }
   }
   j._read = true;

   return is;
}

ostream& operator << (ostream& os, const DBJson& j)
{
   // TODO
   os << "{\n";
   for (size_t i = 0; i < j.size(); i++)
   {
      if (i < j.size() - 1) 
         os << "  " << j[i] << ",\n";
      else 
         os << "  " << j[i] << "\n";
   }
   os << "}\n";
   return os;
}

/**********************************************/
/*   Member Functions for class DBJsonElem    */
/**********************************************/
/*****************************************/
/*   Member Functions for class DBJson   */
/*****************************************/
void
DBJson::reset()
{
   // TODO
   _obj.clear();
   _read = false;
}

// return false if key is repeated
bool
DBJson::add(const DBJsonElem& elm)
{
   // TODO
   for (auto& e: _obj) {
      if (e.key() == elm.key()) 
         return false;
   }
   _obj.push_back(elm);
   return true;
}

// return NAN if DBJson is empty
float
DBJson::ave() const
{
   // TODO
   if (_obj.empty())
      return NAN;
   float s = 0.0;
   for (const auto& e : _obj) {
      s += e.value();
   }
   return s / _obj.size();
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::max(size_t& idx) const
{
   // TODO
   int maxN = INT_MIN;
   for (size_t i = 0; i < _obj.size(); i++) {
      if (_obj[i].value() > maxN) {
         maxN = _obj[i].value();
         idx = i;
      }
   }
   return  maxN;
}

// If DBJson is empty, set idx to size() and return INT_MAX
int
DBJson::min(size_t& idx) const
{
   // TODO
   int minN = INT_MAX;
   for (size_t i = 0; i < _obj.size(); i++) {
      if (_obj[i].value() < minN) {
         minN = _obj[i].value();
         idx = i;
      }
   }
   return  minN;
}

void
DBJson::sort(const DBSortKey& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

void
DBJson::sort(const DBSortValue& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

// return 0 if empty
int
DBJson::sum() const
{
   // TODO
   int s = 0;
   for (const auto& e : _obj) {
      s += e.value();
   }
   return s;
}
