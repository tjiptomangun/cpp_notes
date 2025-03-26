A C++ function can return a reference in a similar way as it returns a pointer.
https://www.tutorialspoint.com/cplusplus/returning_values_by_reference.htm

When a function returns a reference, it returns an implicit pointer to its return value.
This way, a function can be used on the left side of an assignment statement.

https://stackoverflow.com/questions/7813728/pointer-vs-reference-return-types
The difference between return pointer type and return reference type is that
returning null reference will cause run time error while returning
null pointer will not cause run time error. 


https://stackoverflow.com/questions/2837241/c-returning-pointers-references 
Pointer and reference
* (pointer),	when used as part of a type indicates that the type is pointer:
				int is a type, so int* is a pointer to a type, and int** is a 
				pointer-to-pointer-to-int type
& (reference),	when used as port of a type indicates that the type is 
				reference. int is a type, so int& is a reference-to-int
				(there is no such thing as reference-to-reference). References
				and pointers are used for similar things, but the the are
				quite different and not interchangeable. A reference is
				is best thought as an alias, or alternate name, for an existing
				variable. If x is an int, then you can simply assign int& y = x
				to create new name y for x. Afterwards, x and y can be used
				interchangeably to refere to the same integer. The two main
				implications of this are that references cannot be NULL(since
				there must be an origiinal variable to reference), and that
				you don't need to use any special operator to get the original
				value (because it's just an alternate name, not a pointer).
				References can also not be reassigned.
*				when used a unary operator performs an operation called
				dereference. This operations is only meaningful on pointgers.
&				when used as a unary operator performs an operation called
				address of.



compiling
g++ -Wall -ggdb3 return_reference.cpp -o return_reference
