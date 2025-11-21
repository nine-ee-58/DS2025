#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include <random>
#include <ctime>
#define ll long long
using namespace std;
class Compelex {
  private:
  /* data */
  double re;
  double im;
  public:
  double modle;
  Compelex(double r = 0.0, double i = 0.0) :re(r), im(i), modle(std::sqrt(r* r + i * i)) {};
  double getRe() { return re; };
  double getIm() { return im; };
  double getModle() { return modle; };
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
    double m1 = modle;
    double m2 = c.modle;
    if (fabs(m1 - m2) < 1e-10) {
      if (fabs(re - c.re) < 1e-10) {
        return im < c.im;
      }
      else {
        return re < c.re;
      }
    }
    return modle < c.modle;
  }
  bool operator <= (const Compelex& c) const
  {
    double m1 = modle;
    double m2 = c.modle;
    if (fabs(m1 - m2) < 1e-10) {
      if (fabs(re - c.re) < 1e-10) {
        return im <= c.im;
      }
      else {
        return re <= c.re;
      }
    }
    return modle <= c.modle;
  }
  bool operator > (const Compelex& c) const
  {
    double m1 = modle;
    double m2 = c.modle;
    if (fabs(m1 - m2) < 1e-10) {
      if (fabs(re - c.re) < 1e-10) {
        return im > c.im;
      }
      else {
        return re > c.re;
      }
    }
    return modle > c.modle;
  }
  friend std::ostream& operator << (std::ostream& os, const Compelex& c)
  {
    os << "(" << c.re << "," << c.im << ")";
    return os;
  }
  void print()
  {
    cout << re;
    if (im >= 0)
      cout << "+" << im << "i" << "\n";
    else
      cout << im << "i" << "\n";
  }
  void printModle()
  {
    cout << modle;
  }

};

vector<Compelex> searchComplex(vector<Compelex> a, double m1, double m2)
{
  vector<Compelex> res;
  auto comp = [](const Compelex& c, double mod) {
    return c.modle < mod;
    };
  auto low = lower_bound(a.begin(), a.end(), m1, comp);
  auto up = lower_bound(a.begin(), a.end(), m2, comp);
  res.insert(res.end(), low, up);
  return res;
}

void mergetogether(vector<Compelex>& a, ll left, ll right, ll mid)
{
  int k = 0;
  int i = left;
  int j = mid + 1;
  vector<Compelex>b(right - left + 1);
  while (i <= mid && j <= right) {
    if (a[i] <= a[j])
      b[k++] = a[i++];
    else
      b[k++] = a[j++];
  }
  while (i <= mid)
    b[k++] = a[i++];
  while (j <= right)
    b[k++] = a[j++];
  for (int i = 0;i < k;i++)
    a[left + i] = b[i];
}
// 归并排序  递归
void mergesort(vector<Compelex>& a, ll left, ll right)
{
  if (left >= right)return;

  ll mid = left + (right - left) / 2;
  if (right - left > 0) {
    mergesort(a, left, mid);
    mergesort(a, mid + 1, right);
  }
  mergetogether(a, left, right, mid);
}
void bubleSort(vector<Compelex>& a)
{
  int n = a.size();
  for (int i = 0;i < n - 1;i++) {
    for (int j = 0;j < n - i - 1;j++) {
      if (a[j] > a[j + 1]) {
        swap(a[j], a[j + 1]);
      }
    }
  }
}
int main()
{
  vector<Compelex> v;
  v.push_back(Compelex(6, 0));
  v.push_back(Compelex(0, 4));
  v.push_back(Compelex(4, 4));
  v.push_back(Compelex(0, 6));
  v.push_back(Compelex(0, 4));
  v.push_back(Compelex(4, 0));
  v.push_back(Compelex(2, 2));
  v.push_back(Compelex(3, 3));
  v.push_back(Compelex(0, 5));
  v.push_back(Compelex(10, 0));
  for (int i = 0;i < v.size();i++) {
    v[i].print();
  }
  cout << endl;
  bubleSort(v);
  cout << "排序后的向量:" << endl;
  for (int i = 0;i < v.size();i++) {
    v[i].print();
  }
  auto it = unique(v.begin(), v.end());
  v.erase(it, v.end());
  cout << "唯一化后的向量:" << endl;
  for (int i = 0;i < v.size();i++) {
    v[i].print();
  }
  vector<Compelex> res = searchComplex(v, 4.0, 6.0);
  cout << "区间查找结果:" << endl;
  for (int i = 0; i < res.size(); i++) {
    res[i].print();
  }
}