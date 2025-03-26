#include <stdio.h>
#include <string.h> 
#include <pcrecpp.h>
#include <iostream>
#include "logregx.h"//mandatory to link with c

using std::string;
using std::cout;
using pcrecpp::StringPiece;
using pcrecpp::RE;
using pcrecpp::RE_Options;

struct ReplaceStruct {
	const char *regexp;
	const char *rewrite;
	const char *original;
	const char *single;
	const char *global;
	int globalCount;
};

bool replaceWithOne(const char*regexp, const char *rewrite, const char* original, const char *single, const char *global, int globalCount) {

	struct ReplaceStruct rs = {
		regexp, rewrite, original, single, global
	}; 
	RE re(rs.regexp, RE_Options(PCRE_NEWLINE_CRLF).set_utf8(true));
	cout << re.error();
	string one(rs.original);
	return re.Replace(rs.rewrite, &one);
}

