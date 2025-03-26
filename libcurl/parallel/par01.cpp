// https://stackoverflow.com/questions/24459626/can-libcurl-be-used-to-make-multiple-concurrent-requests
#include <cmath>
#include <stdio.h>
#include <curl/curl.h>
#include <time.h>

void curl_request();
size_t write_data(void *, size_t, size_t, void *);

static struct timeval tm1;
static int num_requests = 10;

static inline void start() {
	gettimeofday(&tm1, NULL);
}

static inline void stop() {
	struct timeval tm2;
	gettimeofday(&tm2, NULL);
	unsigned long long t = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
	printf("%d requests in %llu ms\n",num_requests , t);	
}

void curl_request() {
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			fprintf(stderr, "curl_request() failed: %s\n", curl_easy_strerror(res)); 
		} 
		curl_easy_cleanup(curl);

	}

}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
	return size * nmemb;
}

int main() {
	start ();
    #pragma omp parallel for
    for(int n=0; n<num_requests; ++n){
        curl_request();
    }
    stop();

    return 0;
}
