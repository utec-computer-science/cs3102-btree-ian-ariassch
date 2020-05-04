#include <iostream>
#include <vector>
#include <stack>
#include <time.h>
#include <chrono> 
#include <stdio.h>
#include <unistd.h>
 
#include "Tree.h"




int main() {
  typedef BSTNode<float> bst_node;
  typedef BNode<int> b_node;
  typedef Tree<b_node> b_tree;

  b_tree tree(3);
  tree.insert(30);
  tree.insert(10);
  tree.insert(60);
  tree.insert(80);
  tree.insert(20);
  tree.insert(12);
  tree.insert(15);
  tree.insert(9);
  tree.insert(65);
  tree.insert(18);
  tree.insert(19);
  tree.insert(7);



    //tree.insert(20);


  
  cout << tree << endl;
  return 0;
}
// https://repl.it/login?goto=/join/znxxkwam-carlosarias3
