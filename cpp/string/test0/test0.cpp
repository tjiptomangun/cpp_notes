#include <iostream>
#include <string>
 
int main()
{
    const std::string e("Exemplar");
    for (unsigned i = e.length() - 1; i != 0; i /= 2)
        std::cout << e[i];
    std::cout << '\n';
 
    const char* c = &e[0];
    std::cout << c << '\n'; // print as a C string
 
    // Change the last character of s into a 'y'
    std::string s("Exemplar ");
    s[s.size() - 1] = 'y'; // equivalent to s.back() = 'y';
    std::cout << s << '\n';

	const std::string f("");
	std::cout << "f[0] is "  << std::hex << f[0];
    std::cout <<  '\n';
	bool isZeroF = f[0] == 0;
	std::cout << "isZeroF "  << isZeroF << '\n';
	bool isZeroE = e[0] == 0;
	std::cout << "isZeroE "  << isZeroE << '\n';

	
	
}
