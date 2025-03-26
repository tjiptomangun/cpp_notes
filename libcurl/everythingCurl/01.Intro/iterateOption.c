/**require libcurl 7.73*/
#include <curl/curl.h>

int main(int argc, char *argv[]) {
	const struct curl_easyoption *opt;
	opt = curl_easy_option_by_next(NULL);
	while(opt) {
		printf("Name: %s\n", opt->name);
		opt = curl_easy_option_by_next(opt);
	}
}
