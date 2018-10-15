#include <bits/stdc++.h>
using namespace std;
const int N = 10000;
typedef int arr[N];
arr a, b, c;
struct node
{
	int x, y, id;
	bool operator<(const node &o) const
	{
		return y < o.y;
	}
};
node calc(int id, int x)
{
	node nd;
	nd.x = x;
	nd.y = a[id]*a[id]*x + b[id] * x + c[id];
	nd.id = id;
	return nd;
}

int main()
{
	int n, m;
	cin >> n >> m;
	priority_queue<node> qu;
	for(int i = 1; i <= n; ++i)
	{
		cin >> a[i] >> b[i] >> c[i];
		qu.push(calc(i, 1));
	}
	for(int i = 1; i <= m; ++i)
	{
		node nd = qu.top();
		qu.pop();

		cout << nd.y << " ";
		qu.push(calc(nd.id, nd.x + 1));
	}
	
}
