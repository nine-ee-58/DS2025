#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include <random>
#include <ctime>

using namespace std;
class Compelex
{
private:
  /* data */
  double re;
  double im;
public:
  double modle;
  Compelex(double r=0.0,double i=0.0):re(r),im(i),modle(std::sqrt(r*r+i*i)){};
  double getRe(){return re;};
  double getIm(){return im;};
  double getModle(){return modle;};
  bool operator == (const Compelex& c) const
  {
      return re == c.re && im == c.im;
  }
  bool operator != (const Compelex& c) const
  {
      return !(*this == c);
  }
  bool operator < (const Compelex& c) const
  {
      double m1=modle;
      double m2=c.modle;
      if(fabs(m1-m2)<1e-10)
      {
          if(fabs(re - c.re)<1e-10)
          {
              return im < c.im;
          }
          else
          {
              return re < c.re;
          }
      }
      return modle < c.modle;
  }
  friend std::ostream& operator << (std::ostream& os,const Compelex& c)
  {
      os<<"("<<c.re<<","<<c.im<<")";
      return os;
  }
  void print()
  {
      cout << re;
        if (im >= 0)
            cout << "+" << im << "i)";
        else
            cout<< im << "i"<<"\n";
  }
  void printModle()
  {
      cout<<modle;
  }
  
};

//置乱函数
void permutecomplex(vector<Compelex>& vec)
  {
    for (int i = vec.size(); i  > 0; i--)
    {
      swap(vec[i-1],vec[rand()%i]);
    }
  }

//生成随机复数向量
vector<Compelex> randomComplexVector(int n)
{
    vector<Compelex> v;
    srand((unsigned)time(NULL));
    for(int i=0;i<n;i++)
    {
        double re=(rand()%2000-1000)/10.0;
        double im=re;
        Compelex c(re,im);
        v.push_back(c);
    }
    return v;
}


int main()
{
  vector<Compelex> v;
  v=randomComplexVector(10);
  for(int i=0;i<v.size();i++)
  {
      v[i].print();
      cout<<" ";
  }
  cout<<endl;
  permutecomplex(v);
  for(int i=0;i<v.size();i++)
  {
      v[i].print();
      cout<<" ";
  }
  cout<<endl;
  find(v.begin(),v.end(),Compelex(10.0,10.0));
  
}