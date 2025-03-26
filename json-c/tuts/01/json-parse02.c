#include <stdio.h>
#include <json-c/json.h>

int main(void) {
	json_object *root  = json_object_from_file("contact.json");
	
	json_object *first_name, *last_name, *mid_name;
	if (!root) {
		return 1;
	}
	json_object_object_get_ex(root, "firstName", &first_name);
	json_object_object_get_ex(root, "lastName", &last_name);
	json_object_object_get_ex(root, "midName", &mid_name);
	printf("%s\n", json_object_to_json_string_ext(first_name, JSON_C_TO_STRING_PRETTY));
	printf("%s\n", json_object_to_json_string_ext(mid_name, JSON_C_TO_STRING_PRETTY));

	printf("%s, %s, %s\n", json_object_get_string(last_name), json_object_get_string(first_name), json_object_get_string(mid_name));
	json_object_put(root);
	return 0;
}
