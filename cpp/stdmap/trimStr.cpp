// https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
// https://stackoverflow.com/a/217605

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <iostream>
// trim from start (in place)
void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

// trim from start (copying)
std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
std::string trim_copy(std::string s) {
    trim(s);
    return s;
}
#ifdef TEST
int main(int argc, char **argv) {
    std::string inStr("    abc   def    ghi   ");
    std::string cpStr1(inStr);
    std::string cpStr2(inStr);
    std::string resWC = trim_copy(cpStr2);
    trim(cpStr1);

    std::cout <<  "cpStr1 <" << cpStr1 << "> cpStr2 <" << cpStr2 << "> resWC <" << resWC << ">" <<std::endl;

}
#endif