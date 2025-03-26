/**
 * https://everything.curl.dev/libcurl/libcurl
 */
#include <curl/curl.h>

int main(int argc, char *argv[]) {
	CURLcode ret;
	CURL *hnd;

	hnd = curl_easy_init();
	curl_easy_setopt(hnd, CURLOPT_URL, "http://example.com");
	curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.45.0");
	curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, "50L");
	curl_easy_setopt(hnd, CURLOPT_SSH_KNOWNHOSTS, "/home/henky/.ssh/known_hosts");
	curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

	ret = curl_easy_perform(hnd);
	hnd = NULL;
	return (int) ret;
	
}
