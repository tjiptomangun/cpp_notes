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
#include <unistd.h>

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
	char url[500];
	char token[1000];
	char username[100];
	char password[100];
	pthread_rwlock_t rw_lock;
};


struct hostToken host_tokens[1] = {
	{
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
		if (!strcmp(host_tokens[i].url, urlToCheck)) {
			pthread_rwlock_rdlock(&(host_tokens[i].rw_lock));
			strncpy(value, host_tokens[i].token, max_size);
			pthread_rwlock_unlock(&(host_tokens[i].rw_lock));
			return strlen(value);
		}
	}
	return 0;
}
/**
 * success return 1
 * fail 0
 *  
 */
int callGWIsSuccess(CURL *curl, char *responseBody) {
	const long resCode;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode); 
	if(resCode == 200) {
		json_object *root = json_tokener_parse(responseBody);
		json_object *resultCode ;
		json_object_object_get_ex(root, "resultCode", &resultCode);
		const char *resultCodeStr = json_object_get_string(resultCode);
		int toRet = !strcmp(resultCodeStr, "20000");
		json_object_put(root);
		return toRet;
	} else {
		return 0;
	}	
}

/**
 * success return 1
 * fail 0
 *  
 */
int callGWIsSuccessJSON(CURL *curl, json_object *root, const char *info) {
	const long resCode;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode); 
	if(resCode == 200) {
		json_object *resultCode ;
		json_object_object_get_ex(root, "resultCode", &resultCode);
		const char *resultCodeStr = json_object_get_string(resultCode);
		int toRet = !strcmp(resultCodeStr, "20000");
		return toRet;
	} else {
		json_object *developerMessage ;
		json_object_object_get_ex(root, "developerMessage", &developerMessage);
		const char *developerMessageStr = json_object_get_string(developerMessage);		
		printf("not success = %lu %s %s\n", resCode, developerMessageStr, info);
		return 0;
	}	
}

/**
 * success return 1
 * fail 0
 *  
 */
int callGWIsInvalidTokenJSON(CURL *curl, json_object *root) {
	const long resCode;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode); 
	if(resCode >= 401 && resCode <= 404) {
		json_object *developerMessage ;
		json_object_object_get_ex(root, "developerMessage", &developerMessage);
		const char *developerMessageStr = json_object_get_string(developerMessage);		
		int toRet = (!strcmp(developerMessageStr, "invalid_token")) || 
			(!strcmp(developerMessageStr, "session_expired")) ||
			(!strcmp(developerMessageStr, "token_expired"));
		return toRet;
	} else {
		return 0;
	}	
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
		free(chunk.memory);
		if(numTries) {
			return getTokenFromRemote(curl, url, username, password, numTries - 1);
		} else {
			return -1;
		}
	}
	else {
		curl_slist_free_all(headers);
		curl_off_t size;
		const long resCode;
		curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &size); 
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode); 
		json_object *root = json_tokener_parse(chunk.memory);
		if (callGWIsSuccessJSON(curl, root, "getTokenFromRemote")) {
			json_object *resultData ;
			json_object_object_get_ex(root, "resultData", &resultData);
			json_object *temp = json_object_array_get_idx(resultData, 0);
			json_object *token;
			json_object_object_get_ex(temp, "token", &token);
			const char *tokenStr = json_object_get_string(token);
			printf("get token success\n");
			setToken(url, (char *)tokenStr);
			json_object_put(root);
			free(chunk.memory);
			return 0;
		}
		else {
			const long resCode;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode); 
			json_object *developerMessage ;
			json_object_object_get_ex(root, "developerMessage", &developerMessage);
			const char *developerMessageStr = json_object_get_string(developerMessage);		
			printf("get token failed = %lu %s\n", resCode, developerMessageStr);
			json_object_put(root);
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

struct curl_slist * createGenericAppointmentHeaders(CURL *curl, char *tokenBuff, int chunked) {
	struct curl_slist *headers = NULL;
	char authStr [1024] = {0};
	snprintf(authStr, sizeof(authStr), "authorization: Bearer %s", tokenBuff);
	headers = curl_slist_append(headers, "content-type: application/json");
	headers = curl_slist_append(headers, "accept: application/json");
	headers = curl_slist_append(headers, authStr);
	if (chunked) {
		headers = curl_slist_append(headers, "Transfer-Encoding: chunked");
	}

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	return headers;
}

char *createAppointmentJSONBody(char *orderId, char *foaUser, char *fbbId, char *foaPhoneNumber, char ** customerPhones, int customerPhoneLength, char *appointmentDate, char* appointmentTime, char *result, size_t maxResultLength) {
		json_object *root = json_object_new_object();
		if (orderId && strlen(orderId)) {
			json_object_object_add(root, "orderId", json_object_new_string(orderId));
		}
		if (foaUser && strlen(foaUser)) {
			json_object_object_add(root, "foaUser", json_object_new_string(foaUser));
		}
		if (fbbId && strlen(fbbId)) {
			json_object_object_add(root, "fbbId", json_object_new_string(fbbId));
		}
		if (foaPhoneNumber && strlen(foaPhoneNumber)) {
			json_object_object_add(root, "foaPhoneNumber", json_object_new_string(foaPhoneNumber));
		}
		if (customerPhoneLength) {
			json_object *phone_numbers = json_object_new_array();
			json_object_object_add(root, "customerPhoneNumbers", phone_numbers);
			for (int i = 0; i < customerPhoneLength; i++) {
				json_object_array_add(phone_numbers, json_object_new_string(customerPhones[i]));
			}
		}
		if (appointmentDate && strlen(appointmentDate)) {
			json_object_object_add(root, "appointmentDate", json_object_new_string(appointmentDate));
		}
		if (appointmentTime && strlen(appointmentTime)) {
			json_object_object_add(root, "appointmentTime", json_object_new_string(appointmentTime));
		}
		const char *requestBody = json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY)	;
		strncpy(result, requestBody, maxResultLength);
		json_object_put(root);
		return result;
}

// https://stackoverflow.com/a/8251377
// https://curl.se/libcurl/c/post-callback.html
int createAppointmentInCallGW(CURL *curl, char *orderId, char *foaUser, char *fbbId, char *foaPhoneNumber, char ** customerPhones, int customerPhoneLength, char *appointmentDate, char* appointmentTime) {
	char tokenBuff [1000] = {0};
	const char *getTokenUrl = "http://desktop_rumah.com:3001/callGw/v1/getToken";
	const char *actionUrl = "http://desktop_rumah.com:3001/callGw/v1/appointment" ;
	CURLcode res;
	char bodyBuff[2048] = {0};
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
		createAppointmentJSONBody(orderId, foaUser, fbbId, foaPhoneNumber, customerPhones, 
		customerPhoneLength, appointmentDate, appointmentTime, bodyBuff, sizeof(bodyBuff));
		const char *requestBody = bodyBuff;

		curl_easy_setopt(curl, CURLOPT_URL, actionUrl);
		struct curl_slist *headers = createGenericAppointmentHeaders(curl, tokenBuff, 1);
		struct WriteThis wt = {
			.readptr = requestBody,
			.sizeleft = strlen(requestBody)
		};
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
		
		//https://curl.se/libcurl/c/CURLOPT_POST.html
		//method not yet defined, you can use CURLOPT_POST, 
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
    	curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadMemoryCallback);
    	curl_easy_setopt(curl, CURLOPT_READDATA, (void *) &wt);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			curl_slist_free_all(headers);
			free(chunk.memory);
			return -1;
		}  else {
			curl_slist_free_all(headers);
			json_object *resultRoot = json_tokener_parse(chunk.memory);
			if (callGWIsSuccessJSON(curl, resultRoot, "createAppointmentInCallGW")) {
				json_object_put(resultRoot);
				free(chunk.memory);
				return 0;
			} else if(callGWIsInvalidTokenJSON(curl, resultRoot)) {
				json_object_put(resultRoot);
				free(chunk.memory);
				if (getTokenFromUrl(curl, getTokenUrl) == 0) {
					return createAppointmentInCallGW(curl, orderId, foaUser, fbbId, foaPhoneNumber, customerPhones, customerPhoneLength, appointmentDate, appointmentTime);
				} else {
					return -1;
				}
			} else {
				json_object_put(resultRoot);
				free(chunk.memory);
				return -1;
			}
		}
	}
}

int updateAppointmentInCallGW(CURL *curl, char *orderId, char *foaUser, char *fbbId, char *foaPhoneNumber, char ** customerPhones, int customerPhoneLength, char *appointmentDate, char* appointmentTime) {
	char tokenBuff [1000] = {0};
	const char *getTokenUrl = "http://desktop_rumah.com:3001/callGw/v1/getToken";
	const char *actionUrl = "http://desktop_rumah.com:3001/callGw/v1/appointment";
	char actionUrl2[500] = {0};
	char bodyBuff[2048] = {0};
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
		createAppointmentJSONBody(NULL, foaUser, fbbId, foaPhoneNumber, customerPhones, 
		customerPhoneLength, appointmentDate, appointmentTime, bodyBuff, sizeof(bodyBuff));
		const char *requestBody = bodyBuff;
		struct curl_slist *headers = createGenericAppointmentHeaders(curl, tokenBuff, 1);
		snprintf(actionUrl2, sizeof(actionUrl2), "%s/%s", actionUrl, orderId);
		curl_easy_setopt(curl, CURLOPT_URL, actionUrl2);
		struct WriteThis wt = {
			.readptr = requestBody,
			.sizeleft = strlen(requestBody)
		};
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
  		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
  		curl_easy_setopt(curl, CURLOPT_FTP_SKIP_PASV_IP, 1L);
  		curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadMemoryCallback);
    	curl_easy_setopt(curl, CURLOPT_READDATA, (void *) &wt);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			curl_slist_free_all(headers);
			free(chunk.memory);
			return -1;
		} else {
			curl_slist_free_all(headers);
			json_object *resultRoot = json_tokener_parse(chunk.memory);
			if (callGWIsSuccessJSON(curl, resultRoot, "updateAppointmentInCallGW")) {
				json_object_put(resultRoot);
				free(chunk.memory);
				return 0;
			} else if(callGWIsInvalidTokenJSON(curl, resultRoot)) {
				json_object_put(resultRoot);
				free(chunk.memory);
				if (getTokenFromUrl(curl, getTokenUrl) == 0) {
					return updateAppointmentInCallGW(curl, orderId, foaUser, fbbId, foaPhoneNumber, customerPhones, customerPhoneLength, appointmentDate, appointmentTime);
				} else {
					return -1;
				}
			} else {
				json_object_put(resultRoot);
				free(chunk.memory);
				return -1;
			}
		}
	}
}

int deleteAppointmentInCallGW2(CURL *curl, char *orderId) {
	char tokenBuff [1000] = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6InVzZXJ0ZXN0Iiwia2luZCI6ImFjY2Vzc190b2tlbiIsInVzZXJUeXBlIjoid2ViIiwiaWF0IjoxNzA4OTE3NDM5LCJleHAiOjE3MDkwMDM4Mzl9.OUmy_3bWhf7vznGS_DBuWOWoocYAbeOW-gasJ_K4vgI";
	const char *actionUrl = "http://desktop_rumah.com:3001/callGw/v1/appointment";
	char actionUrl2[500] = {0};
	CURLcode res;
	// int tokenNotExists = retrieveToken(getTokenUrl, tokenBuff, sizeof(tokenBuff)) == 0;
	// if (tokenNotExists && getTokenFromUrl(curl, getTokenUrl)) {
	// 	return -1;
	// } else {
	// 	if (tokenNotExists == 1) {
	// 		 retrieveToken(getTokenUrl, tokenBuff, sizeof(tokenBuff));
	// 	}
		curl_easy_reset(curl);
		struct MemoryStruct chunk = {
			.memory = malloc(1),
			.size = 0
		};
		// createAppointmentJSONBody(NULL, foaUser, fbbId, foaPhoneNumber, customerPhones, 
		// customerPhoneLength, appointmentDate, appointmentTime, bodyBuff, sizeof(bodyBuff));
		struct curl_slist *headers = createGenericAppointmentHeaders(curl, tokenBuff, 0);
		snprintf(actionUrl2, sizeof(actionUrl2), "%s/%s", actionUrl, orderId);
		curl_easy_setopt(curl, CURLOPT_URL, actionUrl2);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
		// curl_easy_setopt(curl, CURLOPT_POST, 1L);
  		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
  		curl_easy_setopt(curl, CURLOPT_FTP_SKIP_PASV_IP, 1L);
  		curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
		// curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
		// curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)0);


		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			curl_slist_free_all(headers);
			free(chunk.memory);
			return -1;
		} else {
			curl_slist_free_all(headers);
			json_object *resultRoot = json_tokener_parse(chunk.memory);
			json_object_put(resultRoot);
			free(chunk.memory);
			return 0;
		}
	// }
}

int deleteAppointmentInCallGW(CURL *curl, char *orderId)  {
	char tokenBuff [1000] = {0};
	const char *getTokenUrl = "http://desktop_rumah.com:3001/callGw/v1/getToken";
	const char *actionUrl = "http://desktop_rumah.com:3001/callGw/v1/appointment";
	char actionUrl2[500] = {0};
	char bodyBuff[2] = {0};
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
		const char *requestBody = bodyBuff;
		struct curl_slist *headers = createGenericAppointmentHeaders(curl, tokenBuff, 0);
		snprintf(actionUrl2, sizeof(actionUrl2), "%s/%s", actionUrl, orderId);
		curl_easy_setopt(curl, CURLOPT_URL, actionUrl2);
		struct WriteThis wt = {
			.readptr = requestBody,
			.sizeleft = strlen(requestBody)
		};
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
  		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
  		curl_easy_setopt(curl, CURLOPT_FTP_SKIP_PASV_IP, 1L);
  		curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadMemoryCallback);
    	curl_easy_setopt(curl, CURLOPT_READDATA, (void *) &wt);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			curl_slist_free_all(headers);
			free(chunk.memory);
			return -1;
		} else {
			curl_slist_free_all(headers);
			json_object *resultRoot = json_tokener_parse(chunk.memory);
			if (callGWIsSuccessJSON(curl, resultRoot, "deleteAppointmentInCallGW")) {
				json_object_put(resultRoot);
				free(chunk.memory);
				return 0;
			} else if(callGWIsInvalidTokenJSON(curl, resultRoot)) {
				json_object_put(resultRoot);
				free(chunk.memory);
				if (getTokenFromUrl(curl, getTokenUrl) == 0) {
					return deleteAppointmentInCallGW(curl, orderId);
				} else {
					return -1;
				}
			} else {
				json_object_put(resultRoot);
				free(chunk.memory);
				return -1;
			}
		}		
	}
	
}

//https://everything.curl.dev/helpers/url/append-query
//https://everything.curl.dev/helpers/url/get-part
//https://everything.curl.dev/helpers/url/set-part
int getAppointmentInCallGW(CURL *curl, char *orderId, char * foaUser, char *fbbId, char *foaPhoneNumber, char *customerPhones, char *appointmentDate, char *appointmentTime)  {
	char tokenBuff [1000] = {0};
	CURLU *urlp = curl_url();	
	CURLUcode rc = CURLUE_OK;
	CURLcode res ;
	char  qOrderId[100] = "orderId=";
	char  qFoaUser[100] = "foaUser=";
	char  qFbbId[100] = "fbbId=";
	char  qFoaPhoneNumber[100] = "foaPhoneNumber=";
	char  qCustomerPhones[100] = "customerPhones=";
	char  qAppointmentDate[100] = "appointmentDate=";
	char  aAppointmentTime[100] = "appointmentTime=";
	
	if ((rc = curl_url_set(urlp, CURLUPART_HOST, "desktop_rumah.com", 0)) != CURLUE_OK) {
		curl_url_cleanup(urlp);
		return -1;		
	} else if ((rc = curl_url_set(urlp, CURLUPART_SCHEME, "http", 0))!= CURLUE_OK) {
		curl_url_cleanup(urlp);
		return -1;
	} else if ((rc = curl_url_set(urlp, CURLUPART_PORT, "3001", 0)) != CURLUE_OK) {
		curl_url_cleanup(urlp);
		return -1;
	} else if ((rc = curl_url_set(urlp, CURLUPART_PATH, "/callGw/v1/appointment", 0)) != CURLUE_OK) {
		curl_url_cleanup(urlp);
		return -1;
	} else if (orderId && strlen(orderId) && ((rc = curl_url_set(urlp, CURLUPART_QUERY, strcat(qOrderId, orderId), CURLU_APPENDQUERY | CURLU_URLENCODE)) != CURLUE_OK)) {
		curl_url_cleanup(urlp);
		return -1;
	} else if (foaUser && strlen(foaUser) && ((rc = curl_url_set(urlp, CURLUPART_QUERY, strcat(qFoaUser, foaUser), CURLU_APPENDQUERY | CURLU_URLENCODE)) != CURLUE_OK)) {
		curl_url_cleanup(urlp);
		return -1;
	} else if (fbbId && strlen(fbbId) && ((rc = curl_url_set(urlp, CURLUPART_QUERY, strcat(qFbbId, fbbId), CURLU_APPENDQUERY | CURLU_URLENCODE)) != CURLUE_OK)) {
		curl_url_cleanup(urlp);
		return -1;
	} else if (foaPhoneNumber && strlen(foaPhoneNumber) && ((rc = curl_url_set(urlp, CURLUPART_QUERY, strcat(qFoaPhoneNumber, fbbId), CURLU_APPENDQUERY | CURLU_URLENCODE)) != CURLUE_OK)) {
		curl_url_cleanup(urlp);
		return -1;
	} else if (customerPhones && strlen(customerPhones) && ((rc = curl_url_set(urlp, CURLUPART_QUERY, strcat(qCustomerPhones, customerPhones), CURLU_APPENDQUERY | CURLU_URLENCODE)) != CURLUE_OK)) {
		curl_url_cleanup(urlp);
		return -1;
	} else if (appointmentDate && strlen(appointmentDate) && ((rc = curl_url_set(urlp, CURLUPART_QUERY, strcat(qAppointmentDate, appointmentDate), CURLU_APPENDQUERY | CURLU_URLENCODE)) != CURLUE_OK)) {
		curl_url_cleanup(urlp);
		return -1;
	} else if (appointmentTime && strlen(appointmentTime) && ((rc = curl_url_set(urlp, CURLUPART_QUERY, strcat(aAppointmentTime, appointmentTime), CURLU_APPENDQUERY | CURLU_URLENCODE)) != CURLUE_OK)) {
		curl_url_cleanup(urlp);
		return -1;
	} 	
	else {
		char bodyBuff[2] = {0};
			const char *getTokenUrl = "http://desktop_rumah.com:3001/callGw/v1/getToken";
			
		int tokenNotExists = retrieveToken(getTokenUrl, tokenBuff, sizeof(tokenBuff)) == 0;
		if (tokenNotExists && getTokenFromUrl(curl, getTokenUrl)) {
			curl_url_cleanup(urlp);
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
			const char *requestBody = bodyBuff;
			struct curl_slist *headers = createGenericAppointmentHeaders(curl, tokenBuff,  0);
			struct WriteThis wt = {
				.readptr = requestBody,
				.sizeleft = strlen(requestBody)
			};
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
			curl_easy_setopt(curl, CURLOPT_FTP_SKIP_PASV_IP, 1L);
			curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
  			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadMemoryCallback);
			curl_easy_setopt(curl, CURLOPT_READDATA, (void *) &wt);
			curl_easy_setopt(curl, CURLOPT_CURLU, urlp);
			// curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
			res = curl_easy_perform(curl);
			if (res != CURLE_OK) {
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
				curl_slist_free_all(headers);
				free(chunk.memory);
				curl_url_cleanup(urlp);
				return -1;
			} else {
				curl_slist_free_all(headers);
				json_object *resultRoot = json_tokener_parse(chunk.memory);
				if (callGWIsSuccessJSON(curl, resultRoot, "getAppointmentInCallGW")) {
					json_object_put(resultRoot);
					free(chunk.memory);
					return 0;
				} else if(callGWIsInvalidTokenJSON(curl, resultRoot)) {
					json_object_put(resultRoot);
					free(chunk.memory);
					if (getTokenFromUrl(curl, getTokenUrl) == 0) {
						return getAppointmentInCallGW(curl, orderId, foaUser, fbbId, foaPhoneNumber, customerPhones, appointmentDate, appointmentTime);
					} else {
						return -1;
					}
				} else {
					json_object_put(resultRoot);
					free(chunk.memory);
					return -1;
				}
			}
		}
	}
}
struct  appointmentInfoStruct {
	char orderId [20];
	char foaUser [20];
	char fbbId [20];
	char foaPhoneNumber[20];
	char customerPhoneNumbers[2][20];
	char *customerPhoneNumberPtr[2];
	char appointmentDate[20];
	char appointmentTime[20];
};

void * threadFunction(void *info) {
	pthread_t tid = pthread_self();
	CURL *curl;
	struct appointmentInfoStruct * appt = (struct appointmentInfoStruct *)info;
	curl = curl_easy_init();
	const char *newAppointmentTime = "adsdsadsadsdsa";
	while(1) {
		createAppointmentInCallGW(curl, appt->orderId, appt->foaUser,
				appt->fbbId, appt->foaPhoneNumber, 
				appt->customerPhoneNumberPtr, 2, 
				appt->appointmentDate, 
				appt->appointmentTime);	

		getAppointmentInCallGW(curl, appt->orderId, appt->foaUser,
			NULL, NULL, NULL, NULL, NULL);

		updateAppointmentInCallGW(curl, appt->orderId, appt->foaUser,
				appt->fbbId, appt->foaPhoneNumber, 
				appt->customerPhoneNumberPtr, 2, 
				appt->appointmentDate, 
				newAppointmentTime);	

		deleteAppointmentInCallGW(curl, appt->orderId);		
		sleep(1);
	}

	curl_easy_cleanup(curl);

}
int main(void) {	
	// chunk.memory = malloc(1);
	// chunk.size = 0; 
	pthread_t p1, p2, p3, p4;
	curl_global_init(CURL_GLOBAL_ALL);
	struct appointmentInfoStruct forT1 = {
		.orderId = "foa-order-0018",
		.foaUser  = "foa-user-0018",
		.fbbId  = "fbb-0018",
		.foaPhoneNumber = "0101234567",
		.customerPhoneNumbers = {"0987654321", "0987654321"},
		.appointmentDate = "2020-11-11",
		.appointmentTime = "12:30:00",
	};

	forT1.customerPhoneNumberPtr[0] = forT1.customerPhoneNumbers[0];
	forT1.customerPhoneNumberPtr[1] = forT1.customerPhoneNumbers[1];

	struct appointmentInfoStruct forT2 = {
		.orderId = "foa-order-0019",
		.foaUser  = "foa-user-0019",
		.fbbId  = "fbb-0019",
		.foaPhoneNumber = "0101234568",
		.customerPhoneNumbers = {"0987654322", "0987654322"},
		.appointmentDate = "2020-11-12",
		.appointmentTime = "12:30:00",
	};

	forT2.customerPhoneNumberPtr[0] = forT2.customerPhoneNumbers[0];
	forT2.customerPhoneNumberPtr[1] = forT2.customerPhoneNumbers[1];

	struct appointmentInfoStruct forT3 = {
		.orderId = "foa-order-0020",
		.foaUser  = "foa-user-0020",
		.fbbId  = "fbb-0020",
		.foaPhoneNumber = "0101234569",
		.customerPhoneNumbers = {"0987654323", "0987654323"},
		.appointmentDate = "2020-11-13",
		.appointmentTime = "12:30:00",
	};

	forT3.customerPhoneNumberPtr[0] = forT3.customerPhoneNumbers[0];
	forT3.customerPhoneNumberPtr[1] = forT3.customerPhoneNumbers[1];

	struct appointmentInfoStruct forT4 = {
		.orderId = "foa-order-0021",
		.foaUser  = "foa-user-0021",
		.fbbId  = "fbb-0021",
		.foaPhoneNumber = "0101234570",
		.customerPhoneNumbers = {"0987654324", "0987654324"},
		.appointmentDate = "2020-11-14",
		.appointmentTime = "12:30:00",
	};

	forT4.customerPhoneNumberPtr[0] = forT4.customerPhoneNumbers[0];
	forT4.customerPhoneNumberPtr[1] = forT4.customerPhoneNumbers[1];

	if (pthread_create(&p1, NULL, threadFunction, &forT1)) {
		curl_global_cleanup();
		return 1;
	} 
	else if (pthread_create(&p2, NULL, threadFunction, &forT2)) {
		curl_global_cleanup();
		return 1;
	}
	else if (pthread_create(&p3, NULL, threadFunction, &forT3)) {
		curl_global_cleanup();
		return 1;		
	} 
	else if (pthread_create(&p4, NULL, threadFunction, &forT4)) {
		curl_global_cleanup();
		return 1;
	} 
	else if (pthread_join(p1, NULL)) {
		curl_global_cleanup();
		return 1;
	}
	else if (pthread_join(p2, NULL)) {
		curl_global_cleanup();
		return 1;
	}
	else if (pthread_join(p3, NULL)) {
		curl_global_cleanup();
		return 1;
	} 
	else if (pthread_join(p4, NULL)) {
		curl_global_cleanup();
		return 1;
	} 
	else {
		curl_global_cleanup();
	}
	

}
