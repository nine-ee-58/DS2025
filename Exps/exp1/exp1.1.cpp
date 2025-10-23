#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;
class Compelex
{
private:
  /* data */
  double re;
  double im;
  double ma;
  
public:
  Compelex():re(0),im(0){};
  Compelex(double r,double i):re(r),im(i),ma(sqrt(r*r+i*i));
  ~Compelex();
};

Compelex::Compelex(/* args */)
{
}

Compelex::~Compelex()
{
}
