#include <math.h>
#include <float.h>
const float d = 0.0000000001f;
float cSum(float A, float B)
{
	return A + B;
}

float cDif(float A, float B)
{
	return A - B;
}

float cPro(float A, float B)
{
	return A * B;
}

float cDiv(float A, float B)
{
	if (fabs(B) > d)
		return A / B;
	else
		return FLT_MAX;
}

long long cFak(short V)
{
	if (V <= 1)
		return 1;
	return cFak(V - 1) * V;
}

float cPow(float V, short P)
{	
	if (P == 0)
		return 1;
	return cPow(V, P - 1) * V;
}