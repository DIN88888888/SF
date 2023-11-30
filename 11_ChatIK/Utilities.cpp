#include "Const.h"
#include "Predef.h"

time_t gettime() {
	time_t t;
	time(&t);
	return t;
}