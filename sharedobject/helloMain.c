#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
typedef int (*hello_f)();
hello_f hello;
int main(int argc, char *argv[])
{
   void *dlHandle;
   dlHandle = dlopen("libhello.so",RTLD_NOW);

   if(dlHandle==0)
   {
     printf("[ER] %s\n",dlerror());
     exit(0);
   }
   hello = (hello_f) dlsym(dlHandle,"hello");
   hello(); 
}
