#include <dlfcn.h>
#include <stdio.h>
#include <time.h>
//https://balau82.wordpress.com/2010/10/06/trace-and-profile-function-calls-with-gcc/
//https://unix.stackexchange.com/questions/386747/tool-to-print-out-functions-being-called-during-run-time
 
static FILE *fp_trace;
 
void
__attribute__ ((constructor))
trace_begin (void)
{
 fp_trace = fopen("trace.log", "w");
}
 
void
__attribute__ ((destructor))
trace_end (void)
{
 if(fp_trace != NULL) {
 fclose(fp_trace);
 }
}
 
void
__cyg_profile_func_enter (void *func,  void *caller)
{
 Dl_info info_f;
 Dl_info info_c;
 if(fp_trace != NULL) {
	if(dladdr(func, &info_f) && dladdr(func, &info_c))
	 fprintf(fp_trace, "e %p [%s] [%s] %p [%s] %lu\n", 
		func, 
		info_f.dli_fname ?  info_f.dli_fname : "?",
		info_f.dli_sname ?  info_f.dli_sname : "?",
	   caller, 
		info_c.dli_sname ?  info_c.dli_sname : "?",
		time(NULL) );
 }
}
 
void
__cyg_profile_func_exit (void *func, void *caller)
{
 if(fp_trace != NULL) {
 fprintf(fp_trace, "x %p %p %lu\n", func, caller, time(NULL));
 }
}
