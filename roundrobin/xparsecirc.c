/* xparsecfg 
 * tool to parse configuration file.
 * current tool have static predefined parameter.
 * this tool is a layered one.
 * gcc xparsecfg.c parserclass.c filecfgparser.c stduserparser.c -Wall -ggdb3 -o xparsecfg 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parserclass.h" 
#include "stdcircparser.h" 
 
int main (int argc , char **argv)
{
	PMINIPARSER  pstdio; 
	
	pstdio = newminiparser (stdin, " \n", stdin_circparse2);	
	pstdio->parse (pstdio,NULL); 
	exit (0);
}
