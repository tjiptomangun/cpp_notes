#ifndef _OPTION_H_
#define _OPTION_H_

#include <stdbool.h>
#include <any.h>

/**
 * o_option is base struct for Option implementation
 * it is functional so it should also have any
 * member (this) and method. It is just interface, or
 * thread or abstract class, so does not have to
 * have concrete implementation.
 */
typedef struct o_option {
	struct o_o_option *this;
	void (*delete) (struct o_option*);
	bool (* is_some) ();
}OPTION;

/**
 * some is struct for o_option that has data.
 * It is concrete implementation.
 */
typedef struct some{
	struct o_option *this;
	void (*delete) (struct some*);
	bool (* is_some) ();
	ANY* (* get) (struct some *); 
	ANY* wrapped_data;
}SOME;

/**
 * none is struct for o_option that has data
 * It is concrete implementation.
 */
typedef struct none{
	struct o_option *this;
	void (*delete) (struct none*);
	bool (* is_some) ();
}NONE;

/**
 * Option is union of Some and None. 
 * The philosophy of o_option is to prevent using 
 * 'the billion dollar bug' null pointer.
 * Option is common in functional programming concept.
 */
union Option {
	OPTION Optional;
	SOME Some;
	NONE None;
};

/**
 * NAME			: __some_is_some
 * DESCRIPTION	: the implementation of is_some for Some objects
 */
bool __some_is_some();

/**
 * NAME			: __none_is_some
 * DESCRIPTION	: the implementation of is_some for None objects
 */
bool __none_is_some();

/**
 * none_instance, the single None object instance
 */
NONE *none_instance;

/**
 * NAME			: none_object
 * DESCRIPTION	: create none object
 */
OPTION* none_object();

/**
 * NAME			: free_o_option
 * DESCRIPTION	: freeing an o_option. Only free SOME, NONE type will not 
				  be freed since the system only have one NONE.
 * INPUT
 *		opt		: o_option to free
 */
void free_o_option(OPTION *opt); 

/**
 * NAME			: some_object
 * DESCRIPTION	: create Some wrapper of an object.
 * INPUT
 *		obj		: an ANY object, will be duplicated for wrapping.
 */

SOME *some_object(ANY *obj); 
#endif
