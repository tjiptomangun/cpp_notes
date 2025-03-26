#include <iostream>
#include <getopt.h>
#include "Trace.h"
int main(int argc, char **argv) {
	int c;
	while(1) {
		int option_index = 0;
		static struct option long_options[] = {
			{"i", required_argument, 0, 'i'},
			{0, 0, 0, 0}
		};

		c  = getopt_long(argc, argv, "i:", long_options, &option_index);
		if (c == -1){
			break;
		}

		switch (c) {
			case 'i' :
				ReadSceneFile(optarg);
				GenImage();
				break;
			default:
				break;
		}
	}
}
