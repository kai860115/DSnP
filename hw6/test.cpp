#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <bits/stdc++.h>
#include <ctime>

using namespace std;

class Base {
public:
   virtual string s() {
      return "b\n";
   }
   virtual void print() {
      cout << s();
   }
};

class C : public Base {
   string s() {
      return "c\n";
   }
};

class A : public Base {
public:
   void print() {
      cout << "==================================================\n";
      cout << "= " << setw(47) << left << "a"
           << "=\n";
      cout << "==================================================\n";
      ;
   }
};

int main() {
   size_t n = 100000000;
   clock_t Start,End;
   Start = clock();
   for (size_t i = 0; i < 5; i++) {
      vector<size_t> v;
      for (size_t i = 0; i < n; i++) {
         v.push_back(0);
      }
   }
   End = clock();
   cout << (End-Start)/CLOCKS_PER_SEC << endl;

   Start = clock();
   for (size_t i = 0; i < 5; i++) {
      vector<size_t> v(n, 0);
   }
   End = clock();
   cout << (End-Start)/CLOCKS_PER_SEC << endl;

   Start = clock();
   for (size_t i = 0; i < 5; i++) {
      vector<size_t> v;
      v.resize(n);
      fill(v.begin(), v.end(), 0);
   }
   End = clock();
   cout << (End-Start)/CLOCKS_PER_SEC << endl;

   Start = clock();
   for (size_t i = 0; i < 5; i++) {
      vector<size_t> v;
      v.reserve(n);
      for (size_t i = 0; i < n; i++) {
         v.push_back(0);
      }
   }
   End = clock();
   cout << (End-Start)/CLOCKS_PER_SEC << endl;
   
   Start = clock();
   for (size_t i = 0; i < 5; i++) {
      vector<size_t> v;
      v.resize(n);
      for (size_t i = 0; i < n; i++) {
         v[i] = 1;
      }
   }
   End = clock();
   cout << (End-Start)/CLOCKS_PER_SEC << endl;


   return 0;
}
