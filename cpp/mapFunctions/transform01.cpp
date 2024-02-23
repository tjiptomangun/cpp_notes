//https://stackoverflow.com/a/2029579
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

int multiply(int);

int main() {
	vector <int> source;
	for(int i = 0; i <=5; i++) {
		source.push_back(i);
	}

	vector<int> result;
	result.resize(source.size());
	transform(source.begin(), source.end(), result.begin(), multiply);

	for(vector <int>::iterator it = result.begin(); it != result.end(); ++it) {
		cout << *it << endl;
	}
}

int multiply(int value) {
	return value * 10;
}
