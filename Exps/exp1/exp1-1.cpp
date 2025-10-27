#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<random>
#include<ctime>
#define ll long long

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
  // 重载等于
  bool operator == (const Compelex& c) const
  {
      return re == c.re && im == c.im;
  }
  // 重载不等于
  bool operator != (const Compelex& c) const
  {
      return !(*this == c);
  }
  // 重载小于
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
  // 重载小于等于
  bool operator <= (const Compelex& c) const
  {
      double m1=modle;
      double m2=c.modle;
      if(fabs(m1-m2)<1e-10)
      {
          if(fabs(re - c.re)<1e-10)
          {
              return im <= c.im;
          }
          else
          {
              return re <= c.re;
          }
      }
      return modle <= c.modle;
  }
  // 重载大于
  bool operator > (const Compelex& c) const
  {
      double m1=modle;
      double m2=c.modle;
      if(fabs(m1-m2)<1e-10)
      {
          if(fabs(re - c.re)<1e-10)
          {
              return im > c.im;
          }
          else
          {
              return re > c.re;
          }
      }
      return modle > c.modle;
  }
// 重载大于等于
  bool operator >= (const Compelex& c) const
  {
      double m1=modle;
      double m2=c.modle;
      if(fabs(m1-m2)<1e-10)
      {
          if(fabs(re - c.re)<1e-10)
          {
              return im >= c.im;
          }
          else
          {
              return re >= c.re;
          }
      }
      return modle >= c.modle;
  }
  // 重载输出
  friend std::ostream& operator << (std::ostream& os,const Compelex& c)
  {
      os<<"("<<c.re<<","<<c.im<<")";
      return os;
  }
  // 打印 a+bi 形式
  void print()
  {
      cout << re;
        if (im >= 0)
            cout << "+" << im << "i"<<"\n";
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

//区间查找函数
vector<Compelex> searchComplex(vector<Compelex> a,double m1,double m2)
{
    vector<Compelex> res;
    bool f=(a[0] < a[2] ^ m1 < m2);
    auto low=lower_bound(a.begin(),a.end(),Compelex(m1,0));
    auto up=upper_bound(a.begin(),a.end(),Compelex(m2,0));
    for(auto it=low;it!=up;it++)
    {
        res.push_back(*it);
    }
    return res;
}

//归并排序
void mergetogether(vector<Compelex>& a , ll left , ll right , ll mid) {
  int k=0;
  int i=left;
  int j=mid+1;
  vector<Compelex>b(right - left + 1);
  while(i<=mid&&j<=right){
	  	if(a[i]<=a[j])
	  		b[k++]=a[i++]; 
	  	else
	  		b[k++]=a[j++];
  	}
	  while(i<=mid)
  		b[k++]=a[i++];
  	while(j<=right)
  		b[k++]=a[j++];
  	for(int i=0;i<k;i++)
  		a[left+i]=b[i];
  }
  // 归并排序  递归
void mergesort(vector<Compelex>& a,ll left,ll right) {
  	if(left>=right)return;

  	ll mid = left + (right - left) / 2;
	  if (right - left > 0) {
  		mergesort(a,left, mid);
  		mergesort(a,mid + 1, right);
  	}
  	mergetogether(a,left, right, mid);
  }

void bubleSort(vector<Compelex>& a)
  {
      int n=a.size();
      for(int i=0;i<n-1;i++)
      {
          for(int j=0;j<n-i-1;j++)
          {
              if(a[j]>a[j+1])
              {
                  swap(a[j],a[j+1]);
              }
          }
      }
  }

int Partition(vector<Compelex>& a, int left, int right) {
    Compelex pivot = a[right];
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (a[j] <= pivot) {
            i++;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[right]);
    return i + 1;
}

void qsort(vector<Compelex>& a, int left, int right) {
    if(left < right) {
        int mid = Partition(a, left, right);
        qsort(a, left, mid - 1);
        qsort(a, mid + 1, right);
    }
}

int main()
{
    vector<Compelex> v;
    v=randomComplexVector(10);
    cout<<"生成的随机复数向量为:"<<endl;
    for(int i=0;i<v.size();i++)
    {
      v[i].print();
      cout<<" ";
    }
    cout<<endl;
    permutecomplex(v);
    cout<<"置乱后的向量:"<<endl;
    for(int i=0;i<v.size();i++)
    {
       v[i].print();
        cout<<" ";
    }
    cout<<endl;

    Compelex target=v[3];
    int loc=find(v.begin(),v.end(),target)-v.begin();
    cout<<"查找 "<<target<<" 的位置: "<<loc<<endl;

    Compelex a=Compelex(7.5,-3.2);
    v.insert(v.begin()+2,a);
    cout<<"在位置2插入(7.5,-3.2)后的列表:"<<endl;
    for(int i=0;i<v.size();i++)
    {
      v[i].print();
    }
    cout<<endl;

    v.push_back(Compelex(152,15));
    cout<<"添加元素(152,15)后的向量:"<<endl;
    for(int i=0;i<v.size();i++)
    {
      v[i].print();
    }
    cout<<endl;

    v.erase(v.begin()+2);
    cout<<"删除位置2元素(7.5,-3.2)后的向量:"<<endl;
    for(int i=0;i<v.size();i++)
    {
        v[i].print();
    }
    cout<<endl;

    auto it =unique(v.begin(),v.end());
    v.erase(it,v.end());
    cout<<"唯一化后的向量:"<<endl;
    for(int i=0;i<v.size();i++)
    {
        v[i].print();
    }
    cout<<endl;


    vector<Compelex> v2=randomComplexVector(10000);

    cout<<"排序效率比较"<<endl;
    vector<Compelex> sortv1=v2;
    vector<Compelex> sortv2=v2;
    vector<Compelex> sortv3=v2;
    vector<Compelex> sortv4=v2;
    clock_t start1=clock();
    sort(sortv1.begin(),sortv1.end());
    clock_t end1=clock();
    cout<<"标准库sort排序时间:"<<double(end1-start1)/CLOCKS_PER_SEC<<"秒"<<endl;
    clock_t start2=clock();
    mergesort(sortv2,0,sortv2.size()-1);
    clock_t end2=clock();
    cout<<"自定义归并排序时间:"<<double(end2-start2)/CLOCKS_PER_SEC<<"秒"<<endl;
    clock_t start3=clock();
    bubleSort(sortv3);
    clock_t end3=clock();
    cout<<"自定义冒泡排序时间:"<<double(end3-start3)/CLOCKS_PER_SEC<<"秒"<<endl;
    clock_t startq=clock();
    qsort(sortv4,0,sortv4.size()-1);
    clock_t endq=clock();
    cout<<"自定义快速排序时间:"<<double(endq-startq)/CLOCKS_PER_SEC<<"秒"<<endl;
    cout<<"\n";


    reverse(sortv1.begin(),sortv1.end());
    reverse(sortv2.begin(),sortv2.end());
    reverse(sortv3.begin(),sortv3.end());
    clock_t start4=clock();
    sort(sortv1.begin(),sortv1.end());
    clock_t end4=clock();
    cout<<"标准库sort逆序排序时间:"<<double(end4-start4)/CLOCKS_PER_SEC<<"秒"<<endl;
    clock_t start5=clock();
    mergesort(sortv2,0,sortv2.size()-1);
    clock_t end5=clock();
    cout<<"自定义归并排序逆序排序时间:"<<double(end5-start5)/CLOCKS_PER_SEC<<"秒"<<endl;
    clock_t start6=clock();
    bubleSort(sortv3);
    clock_t end6=clock();
    cout<<"自定义冒泡排序逆序排序时间:"<<double(end6-start6)/CLOCKS_PER_SEC<<"秒"<<endl;    
    clock_t startr=clock();
    qsort(sortv4,0,sortv4.size()-1);
    clock_t endr=clock();
    cout<<"自定义快速排序逆序排序时间:"<<double(endr-startr)/CLOCKS_PER_SEC<<"秒"<<endl;
    cout<<"\n";


    permutecomplex(sortv1);
    permutecomplex(sortv2);
    permutecomplex(sortv3);
    clock_t start7=clock();
    sort(sortv1.begin(),sortv1.end());
    clock_t end7=clock(); 
    cout<<"标准库sort随机排序时间:"<<double(end7-start7)/CLOCKS_PER_SEC<<"秒"<<endl;
    clock_t start8=clock();
    mergesort(sortv2,0,sortv2.size()-1);
    clock_t end8=clock();
    cout<<"自定义归并排序随机排序时间:"<<double(end8-start8)/CLOCKS_PER_SEC<<"秒"<<endl;
    clock_t start9=clock();
    bubleSort(sortv3);
    clock_t end9=clock();
    cout<<"自定义冒泡排序随机排序时间:"<<double(end9-start9)/CLOCKS_PER_SEC<<"秒"<<endl;
    clock_t starts=clock();
    qsort(sortv4,0,sortv4.size()-1);
    clock_t ends=clock();
    cout<<"自定义快速排序随机排序时间:"<<double(ends-starts)/CLOCKS_PER_SEC<<"秒"<<endl;
    cout<<"\n";

    vector<Compelex> res=searchComplex(v,4.0,1000.0);
    cout<<"区间查找结果:"<<endl;  
    for(int i=0;i<res.size();i++)
    {
        res[i].print();
    }
    cout<<endl;
}