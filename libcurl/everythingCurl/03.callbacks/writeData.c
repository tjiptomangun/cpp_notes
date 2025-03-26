#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

struct response {
	char *memory;
	size_t size;
};


static size_t mem_cb(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	struct response *mem = (struct response *) userp;

	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if (!ptr) {
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

int main() {
    CURLcode ret;
    CURL *curl_handle;

    curl_handle = curl_easy_init();

	struct response chunk = {
		.memory = malloc(0),
		.size = 0
	};
	struct response header = {
		.memory = malloc(0),
		.size = 0
	};

	curl_easy_setopt(curl_handle, CURLOPT_URL, "http://example.com"); 

	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, mem_cb);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) &chunk);

	curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, mem_cb);
	curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, (void *) &header);

	curl_easy_perform(curl_handle);
	FILE *fp = fopen("responseBodyData.txt", "wb");
	fwrite(chunk.memory, chunk.size, 1, fp);
	fclose(fp);

	fp = fopen("responseHeaderData.txt", "wb");
	fwrite(header.memory, header.size, 1, fp);
	fclose(fp);

	free(chunk.memory);
	free(header.memory);
}
