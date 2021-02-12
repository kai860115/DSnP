/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
   // TODO: design your own class!!
   friend class BSTree<T>;
   friend class BSTree<T>::iterator;

   BSTreeNode(const T &d, BSTreeNode<T> *p = 0, BSTreeNode<T> *l = 0, BSTreeNode<T> *r = 0) : _data(d), _parent(p), _left(l), _right(r) {}
   
   T _data;
   BSTreeNode<T> *_parent;
   BSTreeNode<T> *_left;
   BSTreeNode<T> *_right;
};


template <class T>
class BSTree
{
   // TODO: design your own class!!
public:
   BSTree(): _root(0) {}
   ~BSTree() { clear(); }
   class iterator 
   { 
      friend class BSTree;

   public:
      iterator(BSTreeNode<T>* n=0, BSTreeNode<T>* r=0): _node(n), _root(r) {}
      iterator(const iterator& i) : _node(i._node), _root(i._root) {}
      ~iterator() {}

      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () {
         if (_node->_right) {
            _node = _node->_right;
            while (_node->_left) 
               _node = _node->_left;
         }
         else {
            BSTreeNode<T>* p = _node->_parent;
            while (p && p->_right == _node) {
               _node = p;
               p = p->_parent;
            }
            _node = p;
         }
         return *(this);
      }
      iterator operator ++ (int) {
         iterator temp = *this;
         if (_node->_right) {
            _node = _node->_right;
            while (_node->_left) 
               _node = _node->_left;
         }
         else {
            BSTreeNode<T>* p = _node->_parent;
            while (p && p->_right == _node) {
               _node = p;
               p = p->_parent;
            }
            _node = p;
         }
         return temp;
      }
      iterator& operator -- () {
         if (!_node) {
            BSTreeNode<T>* N = _root;
            while (N->_right)
               N = N->_right;
            _node = N;
         }
         else if (_node->_left) {
            _node = _node->_left;
            while (_node->_right) 
               _node = _node->_right;
         }
         else {
            BSTreeNode<T>* p = _node->_parent;
            while (p && p->_left == _node) {
               _node = p;
               p = p->_parent;
            }
            _node = p;
         }
         return *(this);
      }
      iterator operator -- (int) {
         iterator temp = *this;
         if (!_node) {
            BSTreeNode<T>* N = _root;
            while (N->_right)
               N = N->_right;
            _node = N;
         }
         else if (_node->_left) {
            _node = _node->_left;
            while (_node->_right) 
               _node = _node->_right;
         }
         else {
            BSTreeNode<T>* p = _node->_parent;
            while (p && p->_left == _node) {
               _node = p;
               p = p->_parent;
            }
            _node = p;
         }
         return temp;
      }

      iterator& operator = (const iterator& i) {
         _node = i._node;
         return *(this);
      }

      bool operator != (const iterator& i) const { return _node != i._node; }
      bool operator == (const iterator& i) const { return _node == i._node; }

   private:
      BSTreeNode<T>* _node;
      BSTreeNode<T>* _root;
   };

   iterator begin() const { return _root ? iterator(min(_root), _root) : iterator(0, _root); }
   iterator end() const { return iterator(0, _root); }
   bool empty() const { return !_root; }
   size_t size() const {
      size_t s = 0;
      for (iterator it = begin(); it != end(); it++)
         s++;
      return s;
   }

   void print() const {
      print(_root, 0);
   }

   void insert(const T& x) {
      _root = insert(x, _root, 0);
   }
   void pop_front() {
      if (!empty()) {
         erase(begin());
      }
   }
   void pop_back() {
      if (!empty()) {
         erase(--end());
      }
   }

   bool erase(iterator pos) {
      if (empty())
         return false;
         
      BSTreeNode<T> *N = pos._node;
      BSTreeNode<T> *P = N->_parent;
      if (!N->_left && !N->_right) {
         if (!P)
            _root = 0;
         else if (P->_right == N)
            P->_right = 0;
         else if (P->_left == N) 
            P->_left = 0;
         delete N;
         return true;
      }
      else if (!N->_right) {
         N->_left->_parent = P;
         if (!P)
            _root = N->_left;
         else if (P->_right == N)
            P->_right = N->_left;
         else if (P->_left == N) 
            P->_left = N->_left;
         delete N;
         return true;
      }
      else {
         BSTreeNode<T> *S = succ(N);
         if (S->_parent == N) {
            S->_parent = P;
            S->_left = N->_left;
            if (N->_left)
               N->_left->_parent = S;
            if (!P)
               _root = S;
            else if (P->_right == N)
               P->_right = S;
            else if (P->_left == N)
               P->_left = S;
            delete N;
         }
         else {
            N->_data = S->_data;
            S->_parent->_left = S->_right;
            if (S->_right)
               S->_right->_parent = S->_parent;
            delete S;
         }
         return true;
      }
   }
   bool erase(const T& x) {
      for (iterator it = begin(); it != end(); it++) {
         if (*it == x) {
            erase(it);
            return true;
         }
      }
      return false; 
   }

   iterator find(const T& x) {
      for (iterator it = begin(); it != end(); it++) {
         if (*it == x)
            return it;
      }
      return end();
   }

   void clear() {
      if (_root)
         clear(_root);
      _root = 0;
   }

   void sort() {}

private:
   BSTreeNode<T>*  _root;
   
   BSTreeNode<T>* insert(const T& x, BSTreeNode<T>* N, BSTreeNode<T>* P) {
      if (N == 0) 
         return new BSTreeNode<T>(x, P);
      if (x <= N->_data) 
         N->_left = insert(x, N->_left, N);
      else 
         N->_right = insert(x, N->_right, N);
      return N;
   }

   void print(const BSTreeNode<T> *N, const size_t t) const {
      for (size_t i = 0; i < t; i++)
         cout << " ";
      if (N == 0)
         cout << "[0]\n";
      else {
         cout << N->_data << "\n";
         print(N->_left, t + 2);
         print(N->_right, t + 2);
      }
      
   }

   BSTreeNode<T>* min(BSTreeNode<T>* N) const {
      return N->_left ? min(N->_left) : N;
   }
   BSTreeNode<T>* succ(BSTreeNode<T>* N) const {
      return N->_right ? min(N->_right) : 0;
   }

   void clear(BSTreeNode<T>* N) {
      if (N->_left)
         clear(N->_left);
      if (N->_right)
         clear(N->_right);
      delete N;
   }

};

#endif // BST_H
