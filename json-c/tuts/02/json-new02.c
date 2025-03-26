#include <stdio.h>
#include <json-c/json.h>

int main(void) {
	const char *filename = "contact3.json";
	json_object *root = json_object_new_object();
	if (!root) {
		return 1;
	}
	
	json_object *name= json_object_new_object();
	json_object_object_add(name, "type", json_object_new_string("unknown"));
	json_object *common= json_object_new_object();
	json_object_object_add(common, "properties", NULL);
	json_object *properties = json_object_new_object();
	json_object_object_add(properties, "name", name);
	json_object *items = json_object_new_object();
	json_object_object_add(items, "properties", properties);
	json_object_object_add(root, "items", items);
	json_object_object_add(root, "common", common);

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
