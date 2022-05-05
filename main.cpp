#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;



struct Rational
{
    int num;
    int den;
    Rational(int num_ = 0, int den_ = 1) : num(num_), den(den_)
        {
            Make();
        }
    void Make ();
    friend std::ostream& operator << (std::ostream& , Rational const&);
    friend std::istream& operator>> (std::istream& , Rational&);
    friend bool operator< (Rational const&, Rational const&);
        friend bool operator== (Rational const&, Rational const&);
        friend bool operator> (Rational const&, Rational const&);
};

void Rational::Make()
{
    if(!den)
        throw std::invalid_argument("denominator should not be zero");
    if(((num >> (sizeof(num) * 8 - 1)) ^ (den >> (sizeof(den) * 8 - 1))) & 1)
        num = -std::abs(num);
    else
        num = std::abs(num);
    den = std::abs(den);
}


std::ostream& operator <<(std::ostream& ost, Rational const& r)
{
    return ost << r.num << '/' << r.den;
}
 
std::istream& operator>>(std::istream& ist, Rational& r) // e.g. (5, -2)
{
    std::string s;
    std::getline(ist, s, ')');
    std::string::size_type ind;
    while((ind = s.find_first_of("(,)")) != std::string::npos)
        s.replace(ind, 1, 1, ' ');
    std::istringstream iss(s);
    iss >> r.num >> r.den;
    r.Make();
    return ist;
}

bool operator< (Rational const& lhs, Rational const& rhs)
{
    return double(lhs.num) / lhs.den < double(rhs.num) / rhs.den;
}
 
bool operator== (Rational const& lhs, Rational const& rhs)
{
    return !(lhs < rhs || rhs < lhs);
}
 
bool operator> (Rational const& lhs, Rational const& rhs)
{
    return rhs < lhs;
}


struct node
{
    int count, level;
    Rational key;
    node *right;
    node *left;
    node *parent;
    node *root;
}*root;
 

class AATree
{
    public:
        int lookup(Rational key);
        void skew(node *);
        bool split(node *);
        void rebal(node *);
        node *insert(node *,node *);
        void print(node *);
        int countnode(node *);
        AATree()
    {
            root = NULL;
        }
};



int AATree::lookup(Rational key)
{
    node *temp = new node;
    temp->key = key;
    temp->level = 1;
    temp->count = 0;
    temp->left = NULL;
    temp->right = NULL;
    temp->parent = NULL;
    temp = insert(root, temp);
    return temp->count;
}
 

void AATree::skew(node *temp)
{
    node *ptr = temp->left;
    if (temp->parent->left == temp)
        temp->parent->left = ptr;
    else
        temp->parent->right = ptr;
    ptr->parent = temp->parent;
    temp->parent = ptr;
    temp->left = ptr->right;
    if (temp->left != NULL)
        temp->left->parent = temp;
    ptr->right = temp;
    temp->level = (temp->left ? temp->left->level + 1 : 1);
}
 

bool AATree::split(node *temp)
{
    node* ptr = temp->right;
    if (ptr && ptr->right && (ptr->right->level == temp->level))
    {
        if (temp->parent->left == temp)
            temp->parent->left = ptr;
        else
            temp->parent->right = ptr;
        ptr->parent = temp->parent;
        temp->parent = ptr;
        temp->right = ptr->left;
        if (temp->right != NULL)
            temp->right->parent = temp;
        ptr->left = temp;
        ptr->level = temp->level + 1;
        return true;
    }
    return false;
}
 

void AATree::rebal(node* temp)
{
    temp->left = NULL;
    temp->right = NULL;
    temp->level = 1;
    for (temp = temp->parent; temp != root; temp = temp->parent)
    {
        if (temp->level != (temp->left ? temp->left->level + 1 : 1 ))
        {
            skew(temp);
            if (temp->right == NULL)
                temp = temp->parent;
            else if (temp->level != temp->right->level)
                temp = temp->parent;
        }
        if (temp->parent != root)
        {
            if (split(temp->parent) == false)
                break;
        }
    }
}
 

node* AATree::insert(node* temp, node* ins)
{
    if (root == NULL)
    {
        ins->count = 1;
        ins->parent = NULL;
        ins->left = NULL;
        ins->right = NULL;
        root = ins;
        return root;
    }
    if (ins->key < temp->key)
    {
        if (temp->left)
            return insert(temp->left, ins);
        temp->left = ins;
        ins->parent = temp;
        ins->count = 1;
        rebal(ins);
        return ins;
    }
    if (ins->key > temp->key)
    {
        if (temp->right)
            return insert(temp->right, ins);
        temp->right = ins;
        ins->parent = temp;
        ins->count = 1;
        rebal(ins);
        return ins;
    }
    temp->count++;
    delete ins;
    return temp;
}
 

void AATree::print(node* temp)
{
    if (!temp)
        return;
    print(temp->left);
    cout <<"Value: "<<temp->key << "  Count:" << temp->count;
    cout<<"  Level: "<<temp->level<<endl;
    print(temp->right);
}
 

int AATree::countnode(node* temp)
{
    if (!temp)
        return 0;
    int count = 1;
    count = count + countnode(temp->left);
    count = count + countnode(temp->right);
    return count;
}

 


int main()
{

    return 0;
}
