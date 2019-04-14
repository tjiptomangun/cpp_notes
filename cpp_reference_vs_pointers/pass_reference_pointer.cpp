#include <iostream>
using namespace std;

struct demo {
	int a;
};

int main() {
	int x = 5;
	int y = 6; 
	demo d;

	int *p;
	p = &x;
	p = &y;		//pointer can be reassigned
	int &r = x;
	// &r = y	//compile error, reference cannot be reassigned
	r = y;

	p = NULL;
	//&r = NULL;	//compile error, cannot be NULL

	p ++;		//points to nex memory location
	r ++;		//x become 7

	cout << &p << " " << &x << endl;	// Different address
	cout << &r << " " << &x << endl;	// Same address

	demo *q = &d;
	demo &qq = d;

	q->a = 8;
	//q.a = 8; /*compile error*/
	qq.a = 8; /*compile error*/

	cout << p <<endl;	//Print the address
	cout << r <<endl;	//Print the value

	return 0; 
	
}
