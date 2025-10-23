#define _CRT_SECURE_NO_WARNINGS 1
#include<bits/stdc++.h>
typedef long long ll;
typedef unsigned long long ull;
#define testin freopen("testin1.txt", "r", stdin)
#define testout freopen("testout1.txt", "w", stdout)
#define NO cout<<"No"<<endl
#define YES cout<<"Yes"<<endl
#define MAYBE cout<<"MAYBE"<<endl
#define endl '\n'
#define ALL(x) x.begin(),x.end()
#define INS(x)inserter(x,x.begin())
using namespace std;
vector<int> a(100005);
int creatn()
{
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> lendist(1,100000);
  int n=lendist(gen);
  uniform_int_distribution<int> valdist(0,10000);
  for(int i=1;i<=n;i++)
  {
    a[i]=valdist(gen);
  }
  return n;
}
void solve(int x)
{
  vector<int> l(x+2);
  vector<int> r(x+2);

  int maxn;
  int n=x;
  l[1]=-1;
  for(int i=2;i<=n;i++)
  {
    int t=i-1;
    while(t>=0&&a[t]>=a[i])
      t=l[t];
    l[i]=t;
  }
  r[n]=n;
  for(int i=n-1;i>=1;i--)
  {
    int t=i+1;
    while(t<n&&a[t]>=a[i])
      t=r[t];
    r[i]=t;
  }
  maxn=-1;
  for(int i=1;i<=n;i++)
  {
    int s=a[i]*(r[i]-l[i]-1);
    maxn=max(s,maxn);

  }
  cout<<maxn<<"\n";
  /*for(int i=1;i<=n;i++)
    cout<<a[i]<<" ";
  cout<<"\n";
  for(int i=1;i<=n;i++)
    cout<<l[i]<<" ";
  cout<<"\n";
  for(int i=1;i<=n;i++) 
    cout<<r[i]<<" ";
  cout<<"\n";
  for(int i=1;i<=n;i++)
    cout<<a[i]*(r[i]-l[i]-1)<<" ";
  cout<<"\n";*/
}
int main()
{

  int n;
  n=creatn();
  solve(n);
}