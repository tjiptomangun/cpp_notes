//https://gist.github.com/p120ph37/71fb340f7a0d38fa5443

/**
 * HTTP POST with authentication using "basic" method
 * success case 
 * 
 */

/**
 * semua transaksi akan langsung kirim token
 * jika token error maka jalankan get token
 * jika berhasil ulangi transaksi dengan token ini
 * jika gagal ya sudah transaksi gagal
 * untuk test pertama ini tak perlu pakai multithread dulu
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>

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

struct hostToken{
	// pthread_mutex_t statusLock;
	// pthread_cond_t  bearerReady;
	// pthread_cond_t  bearerNotReady;
	char url[500];
	char token[1000];
	char username[100];
	char password[100];
};


struct hostToken k[1] = {
	{
		// .statusLock = PTHREAD_MUTEX_INITIALIZER,
		// .bearerNotReady = PTHREAD_COND_INITIALIZER,
		// .bearerReady = PTHREAD_COND_INITIALIZER,
		.url = "http://desktop_rumah.com:3001/callGw/v1/getToken",
		.token = "",
		.username = "usertest",
		.password = "12341234",
	},
};
void getToken(CURL *curl, char *url, char *username, char *password, struct MemoryStruct *chunk, int numTries) {
	CURLcode res;
	fprintf(stdout, "getToken\n");
	//curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 102400L); 
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long) CURLAUTH_BASIC);
	curl_easy_setopt(curl, CURLOPT_USERNAME, username);
	curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) chunk);
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "content-type: application/json");
	headers = curl_slist_append(headers, "accept: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		curl_slist_free_all(headers);
		if(numTries) {
			chunk->size = 0;
			free(chunk->memory);
			chunk->memory = malloc(1);
			getToken(curl, url, username, password, chunk, numTries - 1);
		}
		
	}
	else {
		curl_off_t size;
		long resCode;
		curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &size); 
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode); 
		printf("length: %lu\n", size);
		printf("status: %ld\n", resCode);
		printf("%s\n", chunk->memory);
		if(resCode == 200) {
			json_object *root = json_tokener_parse(chunk->memory);
			json_object *resultData ;
			json_object_object_get_ex(root, "resultData", &resultData);
			json_object *temp = json_object_array_get_idx(resultData, 0);
			json_object *token;
			json_object_object_get_ex(temp, "token", &token);
			char *tokenStr = json_object_get_string(token);
			fprintf(stdout, "%s\n", tokenStr);
			json_object_put(root);
		}
		curl_slist_free_all(headers);
	}

}

int main(void) {
	
	CURL *curl;
	CURLcode res;
	struct MemoryStruct chunk;
	chunk.memory = malloc(1);
	chunk.size = 0; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		getToken(curl, k[0].url, k[0].username, k[0].password, &chunk, 3);
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		free(chunk.memory);
		return 0;
	} else {
		curl_global_cleanup();
		free(chunk.memory);
		return 1;
	}
	

	
}
