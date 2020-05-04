#ifndef BNODE_H
#define BNODE_H

#include <iostream>
#include <math.h>
#include <vector>
#include "Macros.h"
using namespace std;

template<typename T>
class BNode {
  public: 
  typedef T value_t ;
  typedef vector<value_t> container_t;
  typedef vector<BNode*> pcontainer_t;
  typedef BNode<T> Node;

  unsigned int order;
  container_t data;
  pcontainer_t children;
  size_t sizeOfContainer;

  BNode( unsigned int order)
  :order(order){
    //children = new BNode*[B_NODE_ORDER];
    children.resize(order+1,nullptr);
    data.resize(order, -1);
    sizeOfContainer = 0;
  }

  void insertinNode(int pos, value_t value)
  {
        int index = sizeOfContainer;
        while (index > pos) {
            data[index] = data[index - 1];
            children[index + 1] = children[index];
            index--;
        }
        data[index] = value;
        if(children[index] != nullptr)
        {
            children[index + 1] = children[index];
        }

        sizeOfContainer++;
  }
  
  bool isLeaf()
  {
    for(auto iter = children.begin(); iter != children.end(); iter++)
    {
        if(*iter != nullptr)return false;
    }
    return true;
  }

void rootSplit()
    {
        Node* newroot = new Node(order);
        Node* nodesplit = this;
        Node* left = new Node(order);
        Node* right = new Node(order);

        int middleindex;

        if(order%2 == 0)middleindex = (order/2)-1;
        else{middleindex = ceil(order/2);}
        int iter = 0;
        int i;
        for (i = 0; iter < middleindex; i++) {
            left->children[i] = nodesplit->children[iter];
            left->data[i] = nodesplit->data[iter];
            left->sizeOfContainer++;
            iter++;
        }

        left->children[i] = nodesplit->children[iter];
        newroot->data[0] = nodesplit->data[iter];
        iter++;

        for (i = 0; iter < order; i++) {
            right->children[i] = nodesplit->children[iter];
            right->data[i] = nodesplit->data[iter];
            right->sizeOfContainer++;

            iter++;
        }
        right->children[i] = nodesplit->children[iter];


        newroot->children[0] = left;
        newroot->children[1] = right;
        newroot->sizeOfContainer = 1;

        children = newroot->children;
        data = newroot->data;
        sizeOfContainer = newroot->sizeOfContainer;
    }

void splitNode(int pos) {
        Node* nodesplit = children[pos];
        Node *left = new Node(order);
        Node *right = new Node(order);

        int middleindex;
        if(order%2 == 0)middleindex = (order/2)-1;
        else{middleindex = ceil(order/2);}

        int iter = 0;
        int i;

        left->sizeOfContainer = 0;
        for (i = 0; iter < middleindex; i++) {
            left->children[i] = nodesplit->children[iter];
            left->data[i] = nodesplit->data[iter];
            left->sizeOfContainer++;

            iter++;
        }

        insertinNode(pos, nodesplit->data[iter]);
        iter--;
        left->children[i] = nodesplit->children[middleindex];


        iter+=2;

        for (i = 0; iter < order; i++) {
            right->children[i] = nodesplit->children[iter];
            right->data[i] = nodesplit->data[iter];
            right->sizeOfContainer++;

            iter++;
        }
        right->children[i] = nodesplit->children[iter];
        children[pos + 1] = right;
        children[pos] = left;
    }

  bool insertcheck(value_t val)
  {
    int pos = 0;
      while (pos < sizeOfContainer && data[pos] < val) {
          pos++;
      }
      if (children[pos] != nullptr) {
          bool overflow = children[pos]->insertcheck(val);
          if (overflow) {
              splitNode(pos);
          }
      } else {
          insertinNode(pos, val);
      }
      return checkOverflow();
  }

    bool checkOverflow()
    {
        return sizeOfContainer >= order;
    }

  
  void insert(value_t val, unsigned int order)
  {
    bool overflow = insertcheck(val);
      if (overflow) {
          rootSplit();
      }
  }
  ~BNode(void){}
};


#endif