/*
	SUBTRACT TWO FLOATS - DOUBLE Precision
*/

#include	"FP_types.h"

extern	_double	adf8();

_double
sbf8(s2,s1)
_double	s1,s2;
{
				/* changing the sign directly	*/
				/* is faster than the code line	*/
				/*	s2 = -s2;		*/
	char unsigned *p;		/* sufficient to access sign bit */

#ifdef	PRT_EXT
	fprintf(stderr,"SBF8 ():\n");
#endif
	p = (char unsigned *) &s2;
	*p ^= 0x80;	/* change sign of s2 */
	s1 = adf8(s2,s1);	/* add and return result */
	return(s1);
}

