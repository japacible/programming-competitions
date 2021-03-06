#include <iostream>
using namespace std;

const int MAXP = 40000;

struct coord
{
	int x, ymin, ymax;
} points[MAXP]; 

struct extrema
{
	int exleft, exright;
	int xl, xr;
} maxy[MAXP], miny[MAXP];

struct hullp
{
	int x, y;
	hullp *next;
};

struct hull
{
	hullp *front, *back, *start;
} hull;

bool lessThan(int a, int b, int c, int d)
{
	if (a < c)
		return true;
	return (a == c && b < d);
}

int input(int n)
{
	int len=0;
	for(int i=0; i<n; i++) {
		int x, y;
		cin >> x >> y;
		for(int k=0; k<4; k++) {
			int xx = x+5*(k/2);			// break up 10x10 blocks into
			int yy = y+5*(k%2);			//   4 5x5 blocks
			int j=len;
			while (j > 0 && lessThan(xx,yy,points[j-1].x, points[j-1].ymin)) {
				points[j] = points[j-1];
				j--;
			}
			points[j].x = xx;
			points[j].ymin = points[j].ymax = yy;
			len++;
		}
	}
	len=1;
	for(int i=1; i<4*n; i++) {
		if (points[len-1].x == points[i].x)
			points[len-1].ymax = points[i].ymax;
		else {
			points[len] = points[i];
			len++;
		}
	}
	return len;
}
		
hullp* newHullp(int x, int y, hullp *next)
{
	hullp* p = new hullp;
	p->x = x;
	p->y = y;
	p->next = next;
	return p;
}

int main()
{
	int n, icase=0;
	hull.front = hull.back = hull.start = 0;
	cin >> n;
	while (n != 0) {
		icase++;
		int len = input(n);
		hull.front = newHullp(points[0].x, points[0].ymin, 0);
										// upper left of hull
		int i=1;
		hullp *pupper = hull.front;
		while (i < len) {
			if (points[i].ymin < pupper->y) {
				pupper->next = newHullp(points[i].x, pupper->y, 0);
				pupper = pupper->next;
				pupper->next = newHullp(pupper->x, points[i].ymin, 0);
				pupper = pupper->next;
			}
			i++;
		}
										// upper right of hull
		hullp *pright = newHullp(points[len-1].x, points[len-1].ymin, 0);
		hullp *plower = pright;
		i = len-2;
		while (i >- 0 && points[i].x >= pupper->x) {
			if (points[i].ymin < pright->y) {
				hullp* newp = newHullp(points[i].x, pright->y, pright);
				pright = newp;
				newp = newHullp(pright->x, points[i].ymin, pright);
				pright = newp;
			}
			i--;
		}
		pupper->next = pright;
										// lower right of hull
		if (points[len-1].ymin < points[len-1].ymax) {
			plower = plower->next = newHullp(points[len-1].x, points[len-1].ymax, 0);
		}
		i = len-2;
		while (i >= 0) {
			if (points[i].ymax > plower->y) {
				plower->next = newHullp(points[i].x, plower->y, 0);
				plower = plower->next;
				plower->next = newHullp(plower->x, points[i].ymax, 0);
				plower = plower->next;
			}
			i--;
		}
										// lower left of hull
		hullp *pleft = newHullp(points[0].x, points[0].ymax, 0);
		i=1;
		while (i < len && points[i].x <= plower->x) {
			if (points[i].ymax > pleft->y) {
				hullp* newp = newHullp(points[i].x, pleft->y, pleft);
				pleft = newp;
				newp = newHullp(pleft->x, points[i].ymax, pleft);
				pleft = newp;
			}
			i++;
		}
		plower->next = pleft;
											// have to reverse list - easiest
											//   just to reverse it instead
											//   of rewriting all the code above
		hullp *p = 0;
		while (hull.front->next != 0) {
			hullp *q = hull.front->next;
			hull.front->next = hull.front->next->next;
			q->next = p;
			p = q;
		}
		cout << "Case " << icase << ":";
		hull.front->next = p;
		for(hullp *p = hull.front; p != 0 ; p = p->next) {
			if (p->x % 10 == 5)
				p->x += 4;
			if (p->y % 10 == 5)
				p->y += 4;
			cout << ' ' << p->x << ' ' << p->y;
		}
		cout << endl;
		cin >> n;
	}
}
