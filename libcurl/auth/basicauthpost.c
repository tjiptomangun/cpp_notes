//https://gist.github.com/p120ph37/71fb340f7a0d38fa5443

/**
 * HTTP POST with authentication using "basic" method
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct MemoryStruct {
	char *memory;
	size_t size;        
};

static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *context){
	size_t bytec = size * nmemb;
	struct MemoryStruct * mem = (struct MemoryStruct *) context;
	mem->memory = realloc(mem->memory, mem->size + bytec + 1);
	if (mem->memory == NULL) {
	    printf("not enough memory (realloc returned NULL)\n");
	    return 0;	
	}
	memcpy(&(mem->memory[mem->size]), ptr, bytec);
	mem->size += bytec;
	mem->memory[mem->size] = 0;
	return nmemb;
}

int main(void) {
	CURL *curl;
	CURLcode res;
	struct MemoryStruct chunk;
	chunk.memory = malloc(2);
	chunk.size = 1;
	chunk.memory[chunk.size] = 0;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://ameriwether.com/cgi-bin/info.pl");
		char *val1 = curl_easy_escape(curl, "easy & ugly", 0);
		char *val2 = curl_easy_escape(curl, "Hello from cURL!!!", 0);
		char *fields = malloc( 4 + strlen(val1) + 1 + 4 + strlen(val2) + 1);
		sprintf(fields, "foo=%s&bar=%s", val1, val2);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields);
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long) CURLAUTH_BASIC);
		curl_easy_setopt(curl, CURLOPT_USERNAME, "user");
		curl_easy_setopt(curl, CURLOPT_PASSWORD, "password");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);

		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Accept: text/plain");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "not ok\n");
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		else {
			printf("ok\n");
			printf("%s\n", chunk.memory);
		}
		free(fields);
		curl_free(val1);
		curl_free(val2);
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
		free(chunk.memory);
		curl_global_cleanup();
	}
	return 0;
}
