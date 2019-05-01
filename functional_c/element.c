#include <element.h>
#include <key_val.h>
KEY_VAL *_add_attribute(ELEMENT *elem, char *key, char *val){
	KEY_VAL *nk = new_key_val(key, val)	;
	if(!nk)
		return NULL;
	elemt->attributes->append(elemt->attributes, (ANY *) nk;
}

LIST *_add_child(ELEMENT *elem, ANY *child){
	elem->children->append(child);	
	return elem->children;
}

void _delete_element(ELEMENT *in){
	elem->children->delete(elem->children);
	elem->attributes->delete(elem->attributes);
	elem->name->delete(elem->name);
	elem->this = NULL;
	elem->name = NULL;
	elem->children = NULL;
	elem->attributes = NULL;
	elem->add_attribute = NULL;
	elem->add_child = NULL;
}

ELEMENT *new_element(char *name) {
	ELEMENT *elem = calloc(1, sizeof(ELEMENT));
	CHARSTR *chname;
	LIST *attributes;
	LIST *children;
	if(!elem)
		return NULL;
	chname = new_charstr(name);
	if(!chname){
		free(elem)	
		return NULL;
	}
	attributes = new_list();
	if (!attributes){
		free(elemt);
		free(chname);
	}
	children = new_list();
	if (!children){
		free(elemt);
		free(chname);
		free(attributes);
	}
	
	elem->name = chname;
	elem->attributes = attributes;
	elem->children = chilren;
	elem->add_attribute = add_attribute;
	elem->add_child = _add_child;
}


