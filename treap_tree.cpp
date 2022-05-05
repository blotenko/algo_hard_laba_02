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


struct item {
    int prior;
    Rational key;
    item * l, * r;
    item() { }
    item(int key, int prior) : key(key), prior(prior), l(NULL), r(NULL) { }
};
typedef item * pitem;

void split(pitem t, Rational key, pitem & l, pitem & r) {
    if(!t)
        l = r = NULL;
    else if (key < t->key)
        split(t->l, key, l, t->l),  r = t;
    else
        split(t->r, key, t->r, r),  l = t;
}

void insert(pitem & t, pitem it) {
    if (!t)
        t = it;
    else if(it->prior > t->prior)
        split(t, it->key, it->l, it->r),  t = it;
    else
        insert(it->key < t->key ? t->l : t->r, it);
}

void merge(pitem & t, pitem l, pitem r) {
    if (!l || !r)
        t = l ? l : r;
    else if (l->prior > r->prior)
        merge(l->r, l->r, r),  t = l;
    else
        merge(r->l, l, r->l),  t = r;
}

void erase (pitem & t, int key) {
    if(t->key == key)
        merge(t, t->l, t->r);
    else
        erase(key < t->key ? t->l : t->r, key);
}

pitem unite (pitem l, pitem r) {
    if(!l || !r)  return l ? l : r;
    if(l->prior < r->prior)  swap (l, r);
    pitem lt, rt;
    split(r, l->key, lt, rt);
    l->l = unite(l->l, lt);
    l->r = unite(l->r, rt);
    return l;
}
int main() {
    

    return 0;
}
