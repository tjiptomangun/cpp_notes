#ifndef _OPTION_H_
#define _OPTION_H_

#include <stdbool.h>

/**
 * option is base struct for Option implementation
 * it is functional so it should also have any
 * member (this) and method. It is just interface, or
 * thread or abstract class, so does not have to
 * have concrete implementation.
 */
typedef struct option {
	struct o_option *this;
	void (*delete) (struct option*);
	bool (* is_some) ();
}OPTION;

/**
 * some is struct for option that has data.
 * It is concrete implementation.
 */
typedef struct some{
	struct option *this;
	void (*delete) (struct some*);
	bool (* is_some) ();
	void* (* get) (struct some *); 
	void *wrapped_data;
}SOME;

/**
 * none is struct for option that has data
 * It is concrete implementation.
 */
typedef struct none{
	struct option *this;
	void (*delete) (struct none*);
	bool (* is_some) ();
}NONE;

/**
 * Option is union of Some and None. 
 * The philosophy of option is to prevent using 
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
 * NAME			: get_str
 * DESCRIPTION	: the implementation of get for Some<char *>
 */
char *get_str(SOME *indata);


/**
 * NAME			: some_string
 * DESCRIPTION	: create Option string wrapper. 
 * INPUT
 *		instr	: string to wrap. This string will be
 *				  duplicated so the original can be 
 *				  freed without affecting created object.
 */
OPTION* some_string(char *instr);

/**
 * NAME			: none_object
 * DESCRIPTION	: create none object
 */
OPTION* none_object();

/**
 * NAME			: free_option
 * DESCRIPTION	: freeing an option. Only free SOME, NONE type will not 
				  be freed since the system only have one NONE.
 * INPUT
 *		opt		: option to free
 */
void free_option(OPTION *opt); 
#endif
