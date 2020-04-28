#include <iostream>
#include <vector>
#include <math.h>


using namespace std;

template <typename T>
class SS_Traits{
public:
    typedef T  value_t;
    typedef std::vector<value_t> container_t;
    typedef typename container_t::iterator iterator_t;

    class simple_search{
    public:
        int operator() (container_t a, value_t v){
            int i = 0;
            for(auto iter = a.begin(); iter != a.end(); iter++)
            {
                if(v == (*iter))
                {
                    return i;
                }
                i++;
            }
            return -1;
        }
    };

    typedef simple_search functor_t;
};

template <typename T>
class BS_Traits{
public:
    typedef T value_t;
    typedef std::vector<value_t> container_t;
    typedef typename container_t::iterator iterator_t;

    class binary_search{
    public:
        int binarySearch(container_t arr, int left, int right, int x)
        {
            if (right >= left) {
                int mid = left + (right - left) / 2;

                if (arr[mid] == x)
                    return mid;

                if (arr[mid] > x)
                    return binarySearch(arr, left, mid - 1, x);


                return binarySearch(arr, mid + 1, right, x);
            }

            return -1;
        }
        int operator() (container_t a, value_t v){
            int counter = 0;
            for(auto iter = a.begin(); iter != a.end(); iter++)
            {
                if(*iter != -1)
                {
                    counter++;
                }
            }
            return binarySearch(a, 0, counter, v);
        }
    };


    typedef binary_search functor_t;
};

template <typename T, int S>
class BNode {
public:
    typedef typename T::value_t value_t;
    typedef typename T::container_t container_t;
    typedef typename T::iterator_t iterator_t;
    typedef std::vector< BNode<T,S>* > pcontainer_t;

    container_t keys;
    pcontainer_t ptrs;
    std::size_t  order;
    int counter = 0;


    void insertinNode(int pos, value_t value) {
        int index = counter;
        while (index > pos) {
            keys[index] = keys[index - 1];
            ptrs[index + 1] = ptrs[index];
            index--;
        }
        keys[index] = value;
        if(ptrs[index] != NULL)
        {
            ptrs[index + 1] = ptrs[index];
        }


        counter++;
    }

    bool isLeaf()
    {
        for(auto iter = ptrs.begin(); iter != ptrs.end(); iter++)
        {
            if(*iter != NULL)return false;
        }
        return true;
    }

    BNode(void):order(S){
        keys=container_t(order,-1);
        ptrs=pcontainer_t(order+1,NULL);

    }

    ~BNode(void){}
};

template<typename N>
class PrePrint
{
  public:
  class pre_order
  {
    void printKeys(N *node)
    {
        for(auto iter = node->keys.begin(); iter != node->keys.end(); iter++)
        {
            if((*iter) != -1)
            cout<<(*iter)<<" ";
        }
        cout<<endl;
    }
    public:
    void print(N *ptr)
    {
      if (ptr) {
          printKeys(ptr);
          for(auto iter = ptr->ptrs.begin(); iter != ptr->ptrs.end(); iter++)
          {
              print(*iter);
          }

      }
    }
    void operator()(N *node){
      print(node);
    }
  };
    typedef pre_order print_t;
};


template<typename N>
class PostPrint
{
  public:
  class post_order
  {
    void printKeys(N *node)
    {
        for(auto iter = node->keys.begin(); iter != node->keys.end(); iter++)
        {
            if((*iter) != -1)
            cout<<(*iter)<<" ";
        }
        cout<<endl;
    }
    public:
    void print(N *ptr)
    {
      if (ptr) {
          for(auto iter = ptr->ptrs.begin(); iter != ptr->ptrs.end(); iter++)
          {
              print(*iter);
          }
          printKeys(ptr);
      }
    }
    void operator()(N *node){
      print(node);
    }
  };
    typedef post_order print_t;
};


template <typename P, typename T, int S>
class BTree {
public:
    typedef typename T::value_t value_t;
    typedef typename T::container_t container_t;
    typedef typename T::functor_t functor_t;
    typedef typename P::print_t print_t;
    typedef BNode<T,S> Node;

    Node* root;
    print_t print;
    functor_t search;

    BTree()
    {
        root = new Node;
    }

    ~BTree(void){}

    Node* searchNode(Node* ptr, value_t val)
    {
        int i = 0;
        while(i < ptr->counter && val > ptr->keys[i])
        {
            i++;
        }
        if(val == ptr->keys[i])
        {
            return ptr;
        }
        if(ptr->isLeaf())
        {
            return NULL;
        }

        return searchNode(ptr->ptrs[i], val);
    }

    bool checkOverflow(Node* ptr)
    {
        return ptr->counter >= ptr->order;
    }

    void insert(value_t val) {
        bool overflow = insert(root, val);
        if (overflow) {
            rootSplit(root, val);
        }
    }

    bool insert(Node* node,  value_t val)
    {
        int pos = 0;
        while (pos < node->counter && node->keys[pos] < val) {
            pos++;
        }
        if (node->ptrs[pos] != NULL) {
            bool overflow = insert(node->ptrs[pos], val);
            if (overflow) {
                splitNode(node, pos);
            }
        } else {
            node->insertinNode(pos, val);
        }
        return checkOverflow(node);
    }

    void splitNode(Node *node, int pos) {
        Node *nodesplit = node->ptrs[pos];
        Node *left = new Node();
        Node *right = new Node();

        int middleindex;
        if(S%2 == 0)middleindex = (S/2)-1;
        else{middleindex = ceil(S/2);}

        int iter = 0;
        int i;

        left->counter = 0;
        for (i = 0; iter < middleindex; i++) {
            left->ptrs[i] = nodesplit->ptrs[iter];
            left->keys[i] = nodesplit->keys[iter];
            left->counter++;

            iter++;
        }

        node->insertinNode(pos, nodesplit->keys[iter]);
        iter--;
        left->ptrs[i] = nodesplit->ptrs[middleindex];


        iter+=2;

        for (i = 0; iter < S; i++) {
            right->ptrs[i] = nodesplit->ptrs[iter];
            right->keys[i] = nodesplit->keys[iter];
            right->counter++;

            iter++;
        }
        right->ptrs[i] = nodesplit->ptrs[iter];
        node->ptrs[pos + 1] = right;
        node->ptrs[pos] = left;
    }

    void rootSplit(Node* &nodo,  value_t val)
    {
        Node* nodesplit = nodo;
        Node* newroot = new Node;
        Node* left = new Node;
        Node* right = new Node;
        int middleindex;

        if(S%2 == 0)middleindex = (S/2)-1;
        else{middleindex = ceil(S/2);}
        int iter = 0;
        int i;
        for (i = 0; iter < middleindex; i++) {
            left->ptrs[i] = nodesplit->ptrs[iter];
            left->keys[i] = nodesplit->keys[iter];
            left->counter++;
            iter++;
        }

        left->ptrs[i] = nodesplit->ptrs[iter];
        newroot->keys[0] = nodesplit->keys[iter];
        iter++;

        for (i = 0; iter < S; i++) {
            right->ptrs[i] = nodesplit->ptrs[iter];
            right->keys[i] = nodesplit->keys[iter];
            right->counter++;

            iter++;
        }
        right->ptrs[i] = nodesplit->ptrs[iter];


        newroot->ptrs[0] = left;
        newroot->ptrs[1] = right;
        newroot->counter = 1;
        nodo = newroot;
    }

    bool find(value_t val) {

        auto nodetocheck = searchNode(root, val);
        if(nodetocheck==NULL || search(nodetocheck->keys, val) <= -1)
        {
            return false;
        }
        else{return true;}

    }

    friend std::ostream& operator<<(std::ostream& out, BTree<P,T,S> tree){
        tree.print(tree.root);
        return out;
    }


};

int main() {
    typedef BS_Traits<int> btrait_t;
    typedef PrePrint<BNode<btrait_t, 4>> preprint_t;
    typedef PostPrint<BNode<btrait_t, 4>> postprint_t;
    BTree<postprint_t, btrait_t,4> tree;
    for(int i = 1; i<=20; i++)
    {
        tree.insert(i);
    }

    cout<<tree;

    /*typedef SS_Traits<char> strait_t;
    BTree<strait_t,3> stree;
    stree.insert('a');
    stree.insert('b');
    stree.insert('c');
    stree.insert('d');
    cout<<stree.find('b');
    cout<<endl;*/

    //stree.print1();
}
