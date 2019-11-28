//http://almostvalid.blogspot.com/2013/05/curried-functions-in-c.html

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

typedef int (*fun_t)(int);

int bar(int a){
	return a * 5;
}

fun_t foo(void){
	return bar;
}

void some_fun(void){
	foo()(5);
}

#define DECLARE_CURRIED_FUNCTION(name, type, op)\
	typedef type (*_curry_##name##_t) (type); \
	type _curried_a_##name; \
	type _curried_ret_##name(type b) {\
		type a = _curried_a_##name; \
		op; \
	}\
	_curry_##name##_t name(type a) {\
		_curried_a_##name = a; \
		return _curried_ret_##name; \
	}

#define DECLARE_CURRIED_2_FUNCTION(name, ret_type, p1_type, p2_type, op)\
	typedef ret_type (*_curry_##name##_t) (p2_type); \
	p1_type _curried_a_##name; \
	ret_type _curried_ret_##name(p2_type b) {\
		p1_type a = _curried_a_##name; \
		op; \
	}\
	_curry_##name##_t name(p1_type a) {\
		_curried_a_##name = a; \
		return _curried_ret_##name; \
	} 

	
void closure_0(){
	int (*(* fun1 (char *in)) (float))(int) {
		char curried_1_fun_var[10] = {0};
		strcpy(curried_1_fun_var, in);
		fprintf(stdout, "&curried_1_fun_var = %lu \n", &curried_1_fun_var);
		fprintf(stdout, "curried_1_fun_var = %s\n", curried_1_fun_var);
		int (* fun2 (float a))(int) {
				float curried_2_fun_var = a;
				fprintf(stdout, "&curried_1_fun_var = %lu \n", &curried_1_fun_var);
				fprintf(stdout, "curried_1_fun_var = %s curried_2_fun_var = %f\n", curried_1_fun_var, curried_2_fun_var);
				fprintf(stdout, "curried_1_fun_var = %s curried_2_fun_var = %f\n", curried_1_fun_var, curried_2_fun_var);
				int fun_3(int curried_3_fun_var) {
					fprintf(stdout, "&curried_1_fun_var = %lu \n", &curried_1_fun_var);
					fprintf(stdout, "curried_1_fun_var = %s curried_2_fun_var = %f curried_3_fun_var = %d\n", curried_1_fun_var, curried_2_fun_var, curried_3_fun_var);
					int try_3 (char *p1, double p2, int p3){
						return (int) p2 + atoi(p1) - p3;
					}
					return try_3(curried_1_fun_var, curried_2_fun_var, curried_3_fun_var);
				} 
				return fun_3;
		}
		return fun2; 
	} 
#ifdef _COMPILE_ONLY_
	fprintf (stdout, "fun1(\"10\"): %s\n", fun1("10"));
	fprintf (stdout, "fun1(\"10\")(2.4): %s\n", fun1("10")(2.4));
	fprintf (stdout, "fun1(\"10\")(2.4)(3): %s\n", fun1("10")(2.4)(3));
#endif
	fprintf (stdout, "fun1(\"10\")(2.4)(3) {p2 + atoi(p1) - p3}: %d\n", fun1("10")(2.4)(3));

}
int main (int argc, char **argv) {
	DECLARE_CURRIED_FUNCTION(add, int, return a + b);
	DECLARE_CURRIED_2_FUNCTION(add_str, int, char *, char *, return atoi(a) + atoi(b));
	//DECLARE_CURRIED_3_FUNCTION(add_str, int, char *, float, int, return (int)b + atoi(a) - c);

	int j = add(5)(3);

	printf("j = %d\n", j);

	j = add_str("11")("23");

	printf("j_str = %d\n", j);

	closure_0();

/*
	int (*(* fun1 (char *in)) (float))(int) {
		char curried_1_fun_var[10] = {0};
		strcpy(curried_1_fun_var, in);
		fprintf(stdout, "&curried_1_fun_var = %lu \n", &curried_1_fun_var);
		fprintf(stdout, "curried_1_fun_var = %s\n", curried_1_fun_var);
		int (* fun2 (float a))(int) {
				float curried_2_fun_var = a;
				fprintf(stdout, "&curried_1_fun_var = %lu \n", &curried_1_fun_var);
				fprintf(stdout, "curried_1_fun_var = %s curried_2_fun_var = %f\n", curried_1_fun_var, curried_2_fun_var);
				fprintf(stdout, "curried_1_fun_var = %s curried_2_fun_var = %f\n", curried_1_fun_var, curried_2_fun_var);
				int fun_3(int curried_3_fun_var) {
					fprintf(stdout, "&curried_1_fun_var = %lu \n", &curried_1_fun_var);
					fprintf(stdout, "curried_1_fun_var = %s curried_2_fun_var = %f curried_3_fun_var = %d\n", curried_1_fun_var, curried_2_fun_var, curried_3_fun_var);
					int try_3 (char *p1, double p2, int p3){
						return (int) p2 + atoi(p1) - p3;
					}
					return try_3(curried_1_fun_var, curried_2_fun_var, curried_3_fun_var);
				} 
				return fun_3;
		}
		return fun2; 
	} 
#ifdef _COMPILE_ONLY_
	fprintf (stdout, "fun1(\"10\"): %s\n", fun1("10"));
	fprintf (stdout, "fun1(\"10\")(2.4): %s\n", fun1("10")(2.4));
	fprintf (stdout, "fun1(\"10\")(2.4)(3): %s\n", fun1("10")(2.4)(3));
#endif
	fprintf (stdout, "fun1(\"10\")(2.4)(3) {p2 + atoi(p1) - p3}: %d\n", fun1("10")(2.4)(3));
*/
/*
	int (*(*prim1 (int a)) (float)) (char){
		int curried_fun_var_1 = a;
		fprintf(stdout, "curried_fun_var_1 = %d\n", curried_fun_var_1);
		int (* prim2(float b))(char){
			fprintf(stdout, "curried_fun_var_1 = %d\n", curried_fun_var_1);
			float curried_fun_var_2 = b;
			fprintf(stdout, "curried_fun_var_1 = %d curried_fun_var_2 = %f\n", curried_fun_var_1, curried_fun_var_2); 
			int prim3(char c) {
				char curried_fun_var_3 = c;
				int prim_print(int v1, float v2, char v3){
					return (int) v2 + (int) v3 - v1; 
				}
				return prim_print(curried_fun_var_1, curried_fun_var_2, curried_fun_var_3);
			}
			return prim3;
		}
		return prim2;
	} 
	//fprintf (stdout, "prim1(100) :%s\n" , prim1(100)); 
	fprintf (stdout, "prim1(100)(31.1)) :%s\n" , prim1(100)(31.1)); 
	//fprintf (stdout, "prim1(100)(31.1)('c') :%s\n" , prim1(100)(31.1)('c')); 
*/
	int (*(*prim1 (int a)) (int)) (char){
		int curried_var_1 = a;
		fprintf(stdout, "curried_var_1 = %d\n", curried_var_1);
		int (* prim2(int b))(char){
			float curried_var_2 = b;
			fprintf(stdout, "curried_var_1 = %d curried_var_2 = %f\n", curried_var_1, curried_var_2); 
			int prim3(char c) {
				char curried_var_3 = c;
			fprintf(stdout, "curried_var_1 = %d curried_var_2 = %f curried_var_3 = %d\n", curried_var_1, curried_var_2, curried_var_3); 
				int prim_print(int v1, int v2, char v3){
					return (int) v2 + (int) v3 - v1; 
				}
				return prim_print(curried_var_1, curried_var_2, curried_var_3);
			}
			return prim3;
		}
		return prim2;
	} 
	//fprintf (stdout, "prim1(100) :%s\n" , prim1(100)); 
	//fprintf (stdout, "prim1(100)(31.1)) :%s\n" , prim1(100)(31.1)); 
	fprintf (stdout, "prim1(100)(31.1)('c') :%d\n" , prim1(100)(31.1)('c')); 
	return 0;
}
	
