#include <stdio.h>
#include <json-c/json.h>
void doarray(json_object *obj, const char *ident);

void doit(json_object *obj, const char *ident) {
	char nextIdent[200] = {0};
	sprintf(nextIdent, "%s      ", ident);
	json_object_object_foreach(obj, key, val) {
		switch (json_object_get_type(val)) {
			case json_type_array:
				printf("\n%s%s type is json_type_array \n\n", ident, key);
				doarray(val, nextIdent);
				break;

			case json_type_object:
				printf("\n%s%s type is json_type_object \n\n", ident, key);
				doit(val, nextIdent);
				break;

			case json_type_null:
				printf("%s%s is json_type_null\n", ident, key);
				printf("%s           value: %s\n", ident, json_object_get_string(val));
				break;

			case json_type_boolean:
				printf("%s%s is json_type_boolean\n", ident, key);
				printf("%s           value: %s\n", ident, json_object_get_boolean(val)? "true": "false");
				break;

			case json_type_double:
				printf("%s%s is json_type_double\n", ident, key);
				printf("%s           value: %lf\n", ident, json_object_get_double(val));
				break;

			case json_type_int:
				printf("%s%s is json_type_int\n", ident, key);
				printf("%s           value: %d\n", ident, json_object_get_int(val));
				break;

			case json_type_string:
				printf("%s%s is json_type_string\n", ident, key);
				printf("%s           value: %s\n", ident, json_object_get_string(val));
				break;

			default:
				printf("%s: %s\n", key, json_object_get_string(val));
				break;
		}
	}
}

void doarray(json_object *obj, const char *ident) {
	int temp_n = json_object_array_length(obj);
	for (int i = 0; i < temp_n; i++) {
		char nextIdent[200] = {0};
		sprintf(nextIdent, "%s      [%d]", ident, i);
		doit(json_object_array_get_idx(obj, i), nextIdent);
	}
}
int main(void) {
	json_object *root  = json_object_from_file("contact.json");
	if (!root)
      return 1;

	doit(root, "");
	
	json_object_put(root);
	return 0;
}
