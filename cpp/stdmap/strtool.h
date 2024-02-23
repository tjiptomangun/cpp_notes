#ifndef _STRTOOL_H_
#define _STRTOOL_H_
void ltrim(std::string &s) ;
void rtrim(std::string &s);
void trim(std::string &s);
std::string ltrim_copy(std::string s);
std::string rtrim_copy(std::string s);
std::string trim_copy(std::string s);
#endif