// https://en.cppreference.com/w/cpp/algorithm/remove
// g++ stdRemove01.cpp -o stdRemove01 

/**
 *
 * Removes all elements satisfying specific criteria from the range [first, last) 
 * and returns a past-the-end iterator for the new end of the range. 
**/
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <string_view>

int main() {
	std::string str1{"Text with  some    spaces"};
	auto noSpaceEnd = std::remove(str1.begin(), str1.end(), ' ');
	//noSpaceEnd is the iterator past the end after all removed 
	//string replaced and subsequent string move to left

	//std::cout << std::string_view(str1.begin(), noSpaceEnd)
	std::cout << str1 ;
	std::cout << " size: " << str1.size() << std::endl;

	//remove unused space
	str1.erase(noSpaceEnd, str1.end());

	std::cout << str1 << " size: " << str1.size() << '\n';

	std::string str2 = "Text\n with\tsome \t  whitepaces\n\n";
	str2.erase(std::remove_if(str2.begin(), str2.end(), 
		[](unsigned char x) { return std::isspace(x);}),
		str2.end());

	std::cout << str2 << '\n';
}
