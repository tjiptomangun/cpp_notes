//https://stackoverflow.com/a/2029632
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>

using namespace std;
struct MulBy: public std::unary_function<int, int> {
	MulBy(int v): v_(v) {} //constructor
	int operator()(int lhs) const {
		return lhs * v_;
	}
private:
	int v_;
		
};

int main() {
	int incoming[5] = {1, 2, 3, 4, 5};
	int result[5] = {0, 0, 0, 0, 0};
	transform(&incoming[0], &incoming[5], &result[0], MulBy(10));
	copy(&result[0], &result[5], ostream_iterator<int>(cout, " "));
	return 0;
}
