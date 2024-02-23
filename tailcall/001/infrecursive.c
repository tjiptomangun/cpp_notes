int recfunc(unsigned long long k) {

	if (k< 100000000000000000){
		
		return recfunc(k + 1);
	}
	return 0;
}

int main (int argc, char **argv) {
	recfunc(0);
}
