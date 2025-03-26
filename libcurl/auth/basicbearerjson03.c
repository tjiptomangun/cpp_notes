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
#include <pthread.h>

struct MemoryStruct {
	char *memory;
	size_t size;        
};
struct WriteThis {
  const char *readptr;
  size_t sizeleft;
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

static size_t ReadMemoryCallback(char *dest, size_t size, size_t nmemb, void *userp)
{
  struct WriteThis *wt = (struct WriteThis *)userp;
  size_t buffer_size = size*nmemb;
 
  if(wt->sizeleft) {
    /* copy as much as possible from the source to the destination */
    size_t copy_this_much = wt->sizeleft;
    if(copy_this_much > buffer_size) {
      copy_this_much = buffer_size;
	}
    memcpy(dest, wt->readptr, copy_this_much);
 
    wt->readptr += copy_this_much;
    wt->sizeleft -= copy_this_much;
    return copy_this_much; /* we copied this many bytes */
  }
 
  return 0; /* no more data left to deliver */
}

struct hostToken{
	// pthread_mutex_t statusLock;
	// pthread_cond_t  bearerReady;
	// pthread_cond_t  bearerNotReady;
	char url[500];
	char token[1000];
	char username[100];
	char password[100];
	pthread_rwlock_t rw_lock;
};


struct hostToken host_tokens[1] = {
	{
		// .statusLock = PTHREAD_MUTEX_INITIALIZER,
		// .bearerNotReady = PTHREAD_COND_INITIALIZER,
		// .bearerReady = PTHREAD_COND_INITIALIZER,
		.url = "http://desktop_rumah.com:3001/callGw/v1/getToken",
		.token = "",
		.username = "usertest",
		.password = "12341234",
		.rw_lock = PTHREAD_RWLOCK_INITIALIZER,
	},
};
#define NUM_HOST_TOKEN sizeof(host_tokens)/sizeof(struct hostToken)
void setToken(const char *urlToCheck, char *value) {
	for(int i = 0; i < NUM_HOST_TOKEN; i++) {
		if (!strcmp(host_tokens[i].url, urlToCheck)) {
			pthread_rwlock_wrlock(&(host_tokens[i].rw_lock));
			strncpy(host_tokens[i].token, value, sizeof(host_tokens[0].token));
			pthread_rwlock_unlock(&(host_tokens[i].rw_lock));
		}
	}
}

int retrieveToken(const char *urlToCheck, char *value, int max_size) {
	for(int i = 0; i < NUM_HOST_TOKEN; i++) {
		fprintf(stdout, "%s %s\n", host_tokens[i].url, urlToCheck);
		if (!strcmp(host_tokens[i].url, urlToCheck)) {
			fprintf(stdout, "got match\n");
			pthread_rwlock_rdlock(&(host_tokens[i].rw_lock));
			strncpy(value, host_tokens[i].token, max_size);
			pthread_rwlock_unlock(&(host_tokens[i].rw_lock));
			return strlen(value);
		}
	}
	return 0;
}

/**
 * 
 */
int getTokenFromRemote(CURL *curl, const char *url, char *username, char *password, int numTries) {
	CURLcode res;
	struct MemoryStruct chunk;
	chunk.memory = malloc(1);
	chunk.size = 0; 
	fprintf(stdout, "getTokenFromRemote\n");
	curl_easy_reset(curl);
	//curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 102400L); 
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long) CURLAUTH_BASIC);
	curl_easy_setopt(curl, CURLOPT_USERNAME, username);
	curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "content-type: application/json");
	headers = curl_slist_append(headers, "accept: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		curl_slist_free_all(headers);
		if(numTries) {
			free(chunk.memory);
			return getTokenFromRemote(curl, url, username, password, numTries - 1);
		} else {
			free(chunk.memory);
			return -1;
		}
	}
	else {
		curl_slist_free_all(headers);
		curl_off_t size;
		const long resCode;
		curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &size); 
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode); 
		printf("length: %lu\n", size);
		printf("status: %ld\n", resCode);
		printf("%s\n", chunk.memory);
		if(resCode == 200) {
			json_object *root = json_tokener_parse(chunk.memory);
			json_object *resultData ;
			json_object_object_get_ex(root, "resultData", &resultData);
			json_object *temp = json_object_array_get_idx(resultData, 0);
			json_object *token;
			json_object_object_get_ex(temp, "token", &token);
			const char *tokenStr = json_object_get_string(token);
			fprintf(stdout, "%s %s\n", url, tokenStr);
			setToken(url, (char *)tokenStr);
			json_object_put(root);
			free(chunk.memory);
			return 0;
		} else {
			free(chunk.memory);
			return -1;
		}

	}
}
int getTokenFromUrl(CURL *curl, const char *getTokenUrl) {
	 char * username  = NULL;
	 char * password  = NULL;
	for(int i = 0; i < NUM_HOST_TOKEN; i++) {
		if (!strcmp(host_tokens[i].url, getTokenUrl)) {
			username = host_tokens[i].username;
			password = host_tokens[i].password;
		}
	}
	if (username && password) {
		return getTokenFromRemote(curl, getTokenUrl, username, password, 3);
	} else {
		return -1;
	}
}
		// https://stackoverflow.com/a/8251377
		// https://curl.se/libcurl/c/post-callback.html
int createAppointmentInCallGW(CURL *curl, char *orderId, char *foaUser, char *fbbId, char *foaPhoneNumber, char ** customerPhones, int customerPhoneLength, char *appointmentDate, char* appointmentTime) {
	char tokenBuff [1000] = {0};
	const char *getTokenUrl = "http://desktop_rumah.com:3001/callGw/v1/getToken";
	const char *actionUrl = "http://desktop_rumah.com:3001/callGw/v1/appointment" ;
	int ret = -1;
	CURLcode res;
	int tokenNotExists = retrieveToken(getTokenUrl, tokenBuff, sizeof(tokenBuff)) == 0;
	if (tokenNotExists && getTokenFromUrl(curl, getTokenUrl)) {
		return -1;
	} else {
		if (tokenNotExists == 1) {
			 retrieveToken(getTokenUrl, tokenBuff, sizeof(tokenBuff));
		}
		curl_easy_reset(curl);
		struct MemoryStruct chunk = {
			.memory = malloc(1),
			.size = 0
		};

		json_object *root = json_object_new_object();
		json_object_object_add(root, "orderId", json_object_new_string(orderId));
		json_object_object_add(root, "foaUser", json_object_new_string(foaUser));
		json_object_object_add(root, "fbbId", json_object_new_string(fbbId));
		json_object_object_add(root, "foaPhoneNumber", json_object_new_string(foaPhoneNumber));
		json_object *phone_numbers = json_object_new_array();
		json_object_object_add(root, "customerPhoneNumbers", phone_numbers);
		if (customerPhoneLength) {
			for (int i = 0; i < customerPhoneLength; i++) {
				json_object_array_add(phone_numbers, json_object_new_string(customerPhones[i]));
			}
		}
		json_object_object_add(root, "appointmentDate", json_object_new_string(appointmentDate));
		json_object_object_add(root, "appointmentTime", json_object_new_string(appointmentTime));
		const char *requestBody = json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY)	;

		curl_easy_setopt(curl, CURLOPT_URL, actionUrl);
		curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, tokenBuff);
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
		char authStr [1024] = {0};
		snprintf(authStr, sizeof(authStr), "authorization: Bearer %s", tokenBuff);
		fprintf(stdout, "%s\n", authStr);
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "content-type: application/json");
		headers = curl_slist_append(headers, "accept: application/json");
		headers = curl_slist_append(headers, authStr);
		headers = curl_slist_append(headers, "Transfer-Encoding: chunked");
		struct WriteThis wt = {
			.readptr = requestBody,
			.sizeleft = strlen(requestBody)
		};
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

  		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
		
		//https://curl.se/libcurl/c/CURLOPT_POST.html
		//method not yet defined, you can use CURLOPT_POST, 
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
    	curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadMemoryCallback);		
    	curl_easy_setopt(curl, CURLOPT_READDATA, (void *) &wt);
		res = curl_easy_perform(curl);
		json_object_put(root);
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			curl_slist_free_all(headers);
			free(chunk.memory);
			return -1;
		} else {
			curl_slist_free_all(headers);
			json_object *resultRoot = json_tokener_parse(chunk.memory);
			json_object *resultCode;
			json_object_object_get_ex(resultRoot, "resultCode", &resultCode);
			json_object *developerMessage;
			json_object_object_get_ex(resultRoot, "developerMessage", &developerMessage);
			const char * developerMessageStr = json_object_get_string(developerMessage);
			json_object_put(resultRoot);
			free(chunk.memory);
			return 0;
		}
	}
}
int main(void) {
	
	CURL *curl;
	// chunk.memory = malloc(1);
	// chunk.size = 0; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		char * k[2] = {
			"0987654321",
			"098765432"
		};		

		createAppointmentInCallGW(curl, "foa-order-0018", "foa-user-0003",
			"fbb-0003", "0101234567", k, 2, "2020-11-11", "12:30:00");
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		return 0;
	} else {
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		return 1;
	}	
}
