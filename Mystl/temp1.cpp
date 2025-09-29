#define ll long long
#define pb push_back
#include<bits/stdc++.h>
#include<vector>
#include<algorithm>
using namespace std;
vector<int> a;/*
void mergetogether(ll left, ll right, ll mid) {
	vector<int>b1(mid - left + 1);
	vector<int>b2(right - mid);
	for (int i = 0; i < mid - left + 1; i++) {
		b1[i] = a[left + i];
	}
	for (int i = 0; i <  right-mid; i++) {
		b2[i] = a[mid + i+1];
	}
	int i=0, j=0, k=0;
	while (i < mid - left + 1 && j < right - mid) {
		if (b1[i] > b2[j]) {
			a[left + k] = b2[j];
			k++;
			j++;
		}
		else {
			a[left + k] = b1[i];
			k++;
			i++;
		}
		
	}
	if (i < mid - left + 1) {
			for (i; i < mid - left + 1; i++,k++) {
				a[left + k] = b1[i];

			}
		}
	else if (j < right - mid) {
		for (j; j < right - mid; j++, k++) {
			a[left + k] = b2[j];
		}
	}
}
*/
void mergetogether(ll left, ll right, ll mid) {
	int k=0;
	int i=left;
	int j=right;
	vector<int>b(mid - left + 1);
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
	for(int i=left,k=0;i<=right;i++)
		a[i]=b[k++];
}

void mergesort(ll left,ll right) {
	ll mid = left + (right - left) / 2;
	if (right - left > 0) {
		mergesort(left, mid);
		mergesort(mid + 1, right);
	}
	mergetogether(left, right, mid);
}

void solve() {
	int n,k;
	cin >> n>>k;

	for (int i = 0; i < n; i++) {
		int s;
		cin >> s;
		a.pb(s);
	}
	mergesort(0, n - 1);
	cout << a[k];
}
int main()
{
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	int t = 1;
	while (t--) {
		solve();
	}
	return 0;
}
