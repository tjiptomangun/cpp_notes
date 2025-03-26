#include <stdio.h>
#include <json-c/json.h>

int main(void) {
	const char *filename = "contact2.json";
	json_object *root = json_object_new_object();
	if (!root) {
		return 1;
	}

	json_object_object_add(root, "firstName", json_object_new_string("Bart"));
	json_object_object_add(root, "lastName", json_object_new_string("Johnson"));
	json_object_object_add(root, "isAlive", json_object_new_boolean(TRUE));
	json_object_object_add(root, "age", json_object_new_int(57));

	json_object *address = NULL;
	json_object_object_add(root, "address", address);

	json_object *phone_numbers = json_object_new_array();
	json_object_object_add(root, "phoneNumbers", phone_numbers);

	json_object *phone_home = json_object_new_object();
	json_object_object_add(phone_home, "type", json_object_new_string("home"));
	json_object_object_add(phone_home, "number", json_object_new_string("304-888-5686"));
	json_object_array_add(phone_numbers, phone_home);

	
	json_object *phone_cell= json_object_new_object();
	json_object_object_add(phone_cell, "type", json_object_new_string("cell"));
	json_object_object_add(phone_cell, "number", json_object_new_string("304-888-3023")); 
	json_object_array_add(phone_numbers, phone_cell);

	// children array
	json_object *children= json_object_new_array();
	json_object_object_add(root, "children", children);

	json_object *justine = json_object_new_object();
	json_object_object_add(justine, "name", json_object_new_string("Justine Johnson"));
	json_object_object_add(justine, "age", json_object_new_int(29));
	json_object_array_add(children, justine);

	json_object_object_add(root, "spouse", json_object_new_string("Bobbi Johnson"));

	printf("The json representation:\n\n%s\n\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));

	if (json_object_to_file(filename, root)) {
		printf("Error: failed to save %s!!\n", filename);
	}else {
		printf("%s saved", filename);
	}

	//cleanup and exit
	json_object_put(root);
	return 0; 
}
