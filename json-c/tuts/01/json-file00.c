#include <stdio.h>
#include <json-c/json.h>

int main(void) {
	json_object *root  = json_object_from_file("contact.json");
	if (!root) {
		return 1;
	}
	printf("The json file:\n\n%s\n", json_object_to_json_string(root));
	json_object_put(root);
	return 0;
}
