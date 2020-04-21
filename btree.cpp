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
            return 0;
        }
    };

    class post_order_print{
    public:
        void operator() (void){
            std::cout << "post order" << std::endl;
        }
    };

    typedef simple_search functor_t;
    typedef post_order_print print_t;
};

template <typename T>
class BS_Traits{
public:
    typedef T value_t;
    typedef std::vector<value_t> container_t;
    typedef typename container_t::iterator iterator_t;

    class binary_search{
    public:
        int operator() (container_t a, value_t v){
            return 0;
        }
    };

    class pre_order_print{
    public:
        void operator() (void){
            std::cout << "pre order" << std::endl;
        }
    };

    typedef binary_search functor_t;
    typedef pre_order_print print_t;
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

    BNode(void):order(S){
        keys=container_t(order,-1);
        ptrs=pcontainer_t(order+1,NULL);

    }

    ~BNode(void){}
};

template <typename T, int S>
class BTree {
public:
    typedef typename T::value_t value_t;
    typedef typename T::container_t container_t;
    typedef typename T::functor_t functor_t;
    typedef typename T::print_t print_t;
    typedef BNode<T,S> Node;

    Node* root;
    print_t print;
    functor_t search;

    BTree()
    {
        root = new Node;
    }

    ~BTree(void){}


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
        left->ptrs[i] = nodesplit->ptrs[iter];


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

    bool find(const value_t = 0) const{
        // TODO :: SEARCH
        // search(x); inside each page
        return false;
    }

    friend std::ostream& operator<<(std::ostream& out, BTree<T,S> tree){
        tree.print();// (out)
        // IN PRE POST LEVEL ORDER
        return out;
    }

    void print1() {
        print2(root, 0);
        std::cout << "________________________\n";
    }

    void print2(Node *ptr, int level) {
        if (ptr) {
            int i;
            for (i = ptr->counter - 1; i > 0; i--) {
                print2(ptr->ptrs[i + 1], level + 1);

                for (int k = 0; k < level; k++) {
                    std::cout << "    ";
                }
                std::cout << ptr->keys[i] << "\n";
            }
            print2(ptr->ptrs[i + 1], level + 1);
        }
    }

};

int main() {
    typedef BS_Traits<int> btrait_t;
    BTree<btrait_t,4> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
     tree.insert(4);
     //tree.insert(2);
     //tree.insert(3);


    tree.print1();

    /*typedef SS_Traits<float> strait_t;
    BTree<strait_t,10> stree;
    std::cout<<stree<< std::endl;*/
}
