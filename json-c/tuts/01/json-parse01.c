#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

int main(void) {
	json_object *root  = json_object_from_file("contact.json");
	char *fname, *lname, *mname;
	if (!root) {
		return 1;
	}
	json_object *first_name = json_object_object_get(root, "firstName");
	json_object *last_name = json_object_object_get(root, "lastName");
	json_object *mid_name = json_object_object_get(root, "midName");
	fname = strdup(json_object_get_string(first_name));
	lname = strdup(json_object_get_string(last_name));
	if (mid_name) {
		mname = strdup(json_object_get_string(mid_name));
	}
	else {
		mname = NULL;
	}
	

	printf("%s, %s %s\n", fname, lname, mname);
	json_object_put(root);
	return 0;
}
