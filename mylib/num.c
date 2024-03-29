#include "util.h"
#include "bitset.h"
#include "num.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>


#define Deux53   9007199254740992.0  /* 2^53 */
#define Deux17   131072.0            /* 2^17 */
#define UnDeux17   7.62939453125E-6  /* 1 / 2^17 */
#define MASK32  0xffffffffUL

double num_TwoExp[num_MaxTwoExp + 1] = {
	1.0, 2.0, 4.0, 8.0, 1.6e1, 3.2e1,
	6.4e1, 1.28e2, 2.56e2, 5.12e2, 1.024e3,
	2.048e3, 4.096e3, 8.192e3, 1.6384e4, 3.2768e4,
	6.5536e4, 1.31072e5, 2.62144e5, 5.24288e5,
	1.048576e6, 2.097152e6, 4.194304e6, 8.388608e6,
	1.6777216e7, 3.3554432e7, 6.7108864e7,
	1.34217728e8, 2.68435456e8, 5.36870912e8,
	1.073741824e9, 2.147483648e9, 4.294967296e9,
	8.589934592e9, 1.7179869184e10, 3.4359738368e10,
	6.8719476736e10, 1.37438953472e11, 2.74877906944e11,
	5.49755813888e11, 1.099511627776e12, 2.199023255552e12,
	4.398046511104e12, 8.796093022208e12,
	1.7592186044416e13, 3.5184372088832e13,
	7.0368744177664e13, 1.40737488355328e14,
	2.81474976710656e14, 5.62949953421312e14,
	1.125899906842624e15, 2.251799813685248e15,
	4.503599627370496e15, 9.007199254740992e15,
	1.8014398509481984e16, 3.6028797018963968e16,
	7.2057594037927936e16, 1.44115188075855872e17,
	2.88230376151711744e17, 5.76460752303423488e17,
	1.152921504606846976e18, 2.305843009213693952e18,
	4.611686018427387904e18, 9.223372036854775808e18,
	1.8446744073709551616e19
};


double num_TENNEGPOW[] = {
	1.0, 1.0e-1, 1.0e-2, 1.0e-3, 1.0e-4, 1.0e-5, 1.0e-6, 1.0e-7, 1.0e-8,
	1.0e-9, 1.0e-10, 1.0e-11, 1.0e-12, 1.0e-13, 1.0e-14, 1.0e-15, 1.0e-16
};


/*************************************************************************/

int num_IntMin (int x, int y) {
	if (x > y)
		return y;
	return x;
}

int num_IntMax (int x, int y) {
	if (x > y)
		return x;
	return y;
}

double num_DoubleMin (double x, double y) {
	if (x > y)
		return y;
	return x;
}

double num_DoubleMax (double x, double y) {
	if (x > y)
		return x;
	return y;
}

/*************************************************************************/

int num_IsNumber (char S[])
/*********************************************************
*  Returns TRUE if the string S begin with a number     *
*  (with the possibility of spaces and a + or - sign    *
*  before the number).                                  *
*  e.g.                                                 *
*        '  + 2'   returns TRUE                         *
*         '-+ 2'   returns FALSE                        *
*       '4hello'   returns TRUE                         *
*        'hello'   returns FALSE                        *
*********************************************************/
{
	int Max;
	int i;
	int Sign;
	Max = (int) (strlen (S) - 1);
	Sign = 0;
	for (i = 0; i < Max; i++) {
		if (S[i] != ' ') {
			if (S[i] == '+' || S[i] == '-') {
				if (Sign) {
					return 0;
				}
				/* We already saw a sign */
				Sign = 1;
			} else if ((unsigned char) S[i] >= '0' &&
			           (unsigned char) S[i] <= '9') {
				return 1;
			} else {
				return 0;
			}
		}
	}                            /* end for */
	return 1;                    /* There's no digit in S */
}                                    /* end IsNumber() */


void num_IntToStrBase (int64_t k, int b, char S[])
{
	int Sign;                   /* insert a '-' if TRUE */
	long Char0;
	long i;
	long total;
	long uppbound;
	if (b < 2 || b > 10) {
		util_Error ("*** Erreur: num_IntToStrBase demande une b entre 2 et 10 ***");
	}
	Char0 = 48;
	if (k < 0) {
		Sign = 1;
		S[0] = '-';
		k = -k;
	} else {
		if (k == 0) {
			S[0] = '0';
			S[1] = '\0';
			return;
		}
		Sign = 0;
	}
	i = k;
	total = 0;
	while (i > 0) {
		i = (i / b);
		++total;
	}
	if (Sign)
		uppbound = total + 1;
	else
		uppbound = total;
	S[uppbound] = '\0';
	for (i = 0; i <= total - 1; i++) {
		S[(uppbound - i) - 1] =
			(char) ((int) fmod ((double) k, (double) b) + Char0);
		k = (long) (k / b);
	}
}


/*=========================================================================*/

void num_Uint2Uchar (unsigned char *output, uint64_t *input, int L)
{
	int i, j;

	for (i = 0, j = 0; i < L; i++, j += 4) {
		output[j + 3] = (unsigned char) (input[i] & 0xff);
		output[j + 2] = (unsigned char) ((input[i] >> 8) & 0xff);
		output[j + 1] = (unsigned char) ((input[i] >> 16) & 0xff);
		output[j] = (unsigned char) ((input[i] >> 24) & 0xff);
	}
}

/*=========================================================================*/

void num_WriteD (double x, int I, int J, int K)
{
	int PosEntier = 0,        /* Le nombre de positions occupees par la
	                             partie entiere de x */
	    EntierSign,           /* Le nombre de chiffres significatifs
	                             avant le point */
	    Neg = 0;              /* Nombre n'egatif */
	char S[100];
	char *p;

	if (x == 0.0)
		EntierSign = 1;
	else {
		EntierSign = PosEntier = floor (log10 (fabs (x)) + 1);
		if (x < 0.0)
			Neg = 1;
	}
	if (EntierSign <= 0)
		PosEntier = 1;

	if ((x == 0.0) ||
	    (((EntierSign + J) >= K) && (I >= (PosEntier + J + Neg + 1))))
		printf ("%*.*f", I, J, x);

	else {                       /* On doit utiliser la notation
		                        scientifique. */
		sprintf (S, "%*.*e", I, K - 1, x);
		p = strstr (S, "e+0");
		if (NULL == p)
			p = strstr (S, "e-0");

		/* remove the 0 in e-0 and in e+0 */
		if (p) {
			p += 2;
			while ((*p = *(p + 1)))
				p++;
			printf (" "); /* pour utiliser au moins I espaces */
		}
		printf ("%s", S);
	}
}

/***************************************************************************/

void num_WriteBits (uint64_t x, int k)
{
	int i, n = CHAR_BIT * sizeof (unsigned long);
	unsigned long mask = (unsigned long) 1 << (n - 1);
	int spaces;
	lebool flag = FALSE;

	if (k > 0) {
		spaces = k - n;
		for (i = 0; i < spaces; i++)
			printf (" ");
	}
	for (i = 0; i < n; i++) {
		if (x & mask) {
			printf ("1");
			flag = TRUE;
		} else if (flag)
			printf ("0");
		else
			printf (" ");
		mask >>= 1;
	}
	if (k < 0) {
		spaces = -k - n;
		for (i = 0; i < spaces; i++)
			printf (" ");
	}
}

/***************************************************************************/

#define H1   32768
#define H2   2147483648L

int64_t num_MultMod (int64_t a, int64_t s, int64_t c, int64_t m)
/* Suppose que 0 < a < m  et  0 < s < m.   Retourne (a*s + c) % m.   */
/* Cette procedure est tiree de :                                    */
/* L'Ecuyer, P. et Cote, S., A Random Number Package with           */
/* Splitting Facilities, ACM TOMS, 1991.                            */
/* On coupe les entiers en blocs de d bits. H doit etre egal a 2^d.  */
{
	int64_t a0, a1, q, qh, rh, k, p;
	if (a < H2) {
		a0 = a;
		p = 0;
	} else {
		a1 = a / H2;
		a0 = a - H2 * a1;
		qh = m / H2;
		rh = m - H2 * qh;
		if (a1 >= H2) {
			a1 = a1 - H2;
			k = s / qh;
			p = H2 * (s - k * qh) - k * rh;
			if (p < 0)
				p = (p + 1) % m + m - 1;
		} else               /* p = (A2 * s * h) % m.      */
			p = 0;
		if (a1 != 0) {
			q = m / a1;
			k = s / q;
			p -= k * (m - a1 * q);
			if (p > 0)
				p -= m;
			p += a1 * (s - k * q);
			if (p < 0)
				p = (p + 1) % m + m - 1;
		}                    /* p = ((A2 * h + a1) * s) % m. */
		k = p / qh;
		p = H2 * (p - k * qh) - k * rh;
		if (p < 0)
			p = (p + 1) % m + m - 1;
	}                            /* p = ((A2 * h + a1) * h * s) % m  */
	if (a0 != 0) {
		q = m / a0;
		k = s / q;
		p -= k * (m - a0 * q);
		if (p > 0)
			p -= m;
		p += a0 * (s - k * q);
		if (p < 0)
			p = (p + 1) % m + m - 1;
	}
	p = (p - m) + c;
	if (p < 0)
		p += m;
	return p;
}

/*************************************************************************/

double num_MultModDouble (double a, double s, double c, double m)
{
	double V;
	long k;
	V = a * s + c;
	if (V >= Deux53 || -V >= Deux53) {
		k = a * UnDeux17;
		a -= k * Deux17;
		V = k * s;
		k = V / m;
		V -= k * m;
		V = V * Deux17 + a * s + c;
	}
	k = V / m;
	V -= k * m;
	if (V < 0)
		V += m;
	return V;
}

int64_t num_MultModDirect (int64_t a, int64_t s, int64_t c, int64_t m)
{
	int64_t V,k;
	V = a * s + c;
	k = V / m;
	V -= k * m;
	if (V < 0)
		V += m;
	return V;
}

/**************************************************************************/

int64_t num_InvEuclid (int64_t m, int64_t z)
/*
 * Compute the inverse of z mod m by the modified Euclide
 * algorithm (Knuth V2 p. 325).
 */
{
	long u1 = 0, u3 = m, v1 = 1, v3 = z;
	long t1, t3, qq;
	if (z == 0) return 0;

	while (v3 != 0) {
		qq = u3 / v3;
		t1 = u1 - v1 * qq;
		t3 = u3 - v3 * qq;
		u1 = v1;
		v1 = t1;
		u3 = v3;
		v3 = t3;
	}
	if (u1 < 0)
		u1 += m;

	if (u3 != 1) { /* In this case, the inverse does not exist! */
		fprintf (stderr,
		         "ERROR in num_InvEuclid: inverse does not exist:   m = %llu,  z = %llu\n",
		         m, z);
		return 0;
	} else
		return u1;
}

long num_InvEuclid32 (long m, long z)
/*
 * Compute the inverse of z mod m by the modified Euclide
 * algorithm (Knuth V2 p. 325).
 */
{
	long u1 = 0, u3 = m, v1 = 1, v3 = z;
	long t1, t3, qq;
	if (z == 0) return 0;

	while (v3 != 0) {
		qq = u3 / v3;
		t1 = u1 - v1 * qq;
		t3 = u3 - v3 * qq;
		u1 = v1;
		v1 = t1;
		u3 = v3;
		v3 = t3;
	}
	if (u1 < 0)
		u1 += m;

	if (u3 != 1) { /* In this case, the inverse does not exist! */
		fprintf (stderr,
		         "ERROR in num_InvEuclid32: inverse does not exist:   m = %ld,  z = %ld\n",
		         m, z);
		return 0;
	} else
		return u1;
}

/*------------------------------------------------------------------------*/

uint64_t num_InvExpon (int e, uint64_t z)
/*
 * Compute the inverse of Z modulo M = 2^E by exponentiation
 */
{
	int j;
	unsigned long res = z;

	if (z == 0) return 0;
	if (!(z & 1)) {
		fprintf (stderr,
		         "ERROR in num_InvExpon: inverse does not exist:  e = %d, z = %llu\n",
		         e, z);
		return 0;
	}
	for (j = 1; j <= e - 3; j++)
		res = res * res * z;
	return res & bitset_MASK[e];
}

long num_InvExpon32 (int e, long z)
/*
 * Compute the inverse of Z modulo M = 2^E by exponentiation
 */
{
	int j;
	long res = z;

	if (z == 0) return 0;
	if (!(z & 1)) {
		fprintf (stderr,
		         "ERROR in num_InvExpon32: inverse does not exist:  e = %d, z = %ld\n",
		         e, z);
		return 0;
	}
	for (j = 1; j <= e - 3; j++)
		res = res * res * z;
	return res & bitset_MASK[e];
}

/*------------------------------------------------------------------------*/

int64_t num_Round64 (double x)
{
	return (x >= 0) ? (int64_t)(x + 0.5) : (int64_t)(x - 0.5);
}


double num_RoundD (double x)
{
	double z;
	(x >= 0) ? modf(x + 0.5, &z) : modf(x - 0.5, &z);
	return z;
}

/*------------------------------------------------------------------------*/

uint64_t num_gcd (uint64_t a, uint64_t b)
{
	if (a == 0)
		return b;
	return num_gcd(b%a,a);
}

long num_gcd32 (long a, long b)
{
	if (a == 0)
		return b;
	return num_gcd(b%a,a);
}

/*------------------------------------------------------------------------*/

lebool num_isMersennePrime(int e)
{
	if(e==2 || e==3 ||
	   e==5 || e==7 ||
	   e==13 || e==17 ||
	   e==19 || e==31 ||
	   e==61 || e==89 ||
	   e==107 || e==127 ||
	   e==521 || e==607 ||
	   e==1279 || e==2203 ||
	   e==2281 || e==3217 ||
	   e==4253 || e==4423 ||
	   e==9689 || e==9941 ||
	   e==11213 || e==19937 ||
	   e==21701 || e==23209 ||
	   e==44497 || e==86243 ||
	   e==110503) {
		return 1;
	}
	else
		return 0;
}
