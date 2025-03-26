#include <stdio.h>
#include <json-c/json.h>
int main(void) {
char*toParse = "\
HTTP/1.1 400 Bad Request\r\n\
X-Powered-By: Express\r\n\
Content-Type: application/json; charset=utf-8\r\n\
Content-Length: 72\r\n\
ETag: W/\"48-icYmtuevqB5IlXzhsqtc+eZjHsI\"\r\n\
Date: Thu, 07 Mar 2024 08:15:31 GMT\r\n\
Connection: close\r\n\
\r\n\
{\"resultCode\":\"40000\",\"developerMessage\":\"missing_or_invalid_parameter\"}\r\n";

char*toParse2 = "\
HTTP/1.1 400 Bad Request\r\n\
X-Powered-By: Express\r\n\
Content-Type: application/json; charset=utf-8\r\n\
Content-Length: 72\r\n\
ETag: W/\"48-icYmtuevqB5IlXzhsqtc+eZjHsI\"\r\n\
Date: Thu, 07 Mar 2024 08:15:31 GMT\r\n\
Connection: close\r\n\
\r\n\
\"resultCode\":\"40000\",\"developerMessage\":\"missing_or_invalid_parameter\"}\r\n";
char *body = strstr(toParse, "\r\n\r\n");
json_object *root = json_tokener_parse(body);
	if(root == NULL) {
		fprintf(stderr, "Not a valid json");
	} else {
		printf("%s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
		json_object *resultCode;
		json_object_object_get_ex(root, "resultCode", &resultCode);
		char *resultCodeStr = json_object_get_string(resultCode);
		int resultInt = json_object_get_int(resultCode);
		fprintf(stderr, "\nvalid json %s %d\n", resultCodeStr, resultInt);
		json_object_put(root);

	}


body = strstr(toParse2, "\r\n\r\n");
root = json_tokener_parse(body);
	if(root == NULL) {
		fprintf(stderr, "Not a valid json");
	} else {
		printf("%s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
		
		json_object *resultCode;
		json_object_object_get_ex(root, "resultCode", &resultCode);
		if (json_object_get_type(resultCode) == json_type_null) {
			fprintf(stderr, "Not a valid json  no resultCode");
			json_object_put(root);
			
		} else { 
			char *resultCodeStr = json_object_get_string(resultCode);
			fprintf(stderr, "\nvalid json %s\n", resultCodeStr);
			json_object_put(root);
		}

	}
}

