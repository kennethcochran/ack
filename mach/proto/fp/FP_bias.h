/*
	include file for floating point package
*/

#ifdef	IEEEFORMAT
		/*	FLOAT FORMAT EXPONENT BIAS	*/

#define	SGL_BIAS	 127	/* excess  128 notation used	*/
#define	DBL_BIAS	1023	/* excess 1024 notation used	*/
#define	EXT_BIAS	   0	/* 2s-complement notation used	*/
				/* this is possible because the	*/
				/* sign is in a seperate word	*/
		
		/*	VARIOUS MAX AND MIN VALUES	*/
		/*	1) FOR THE DIFFERENT FORMATS	*/

#define	SGL_MAX		   255	/*	standard definition	*/
#define	SGL_MIN		     0	/*	standard definition	*/
#define	DBL_MAX		  2047	/*	standard definition	*/
#define	DBL_MIN		     0	/*	standard definition	*/
#define EXT_MAX		 16384	/*	standard minimum	*/
#define EXT_MIN		-16383	/*	standard minimum	*/
#else

		/*	FLOAT FORMAT EXPONENT BIAS	*/

#define	SGL_BIAS	 127	/* excess  128 notation used	*/
#define	DBL_BIAS	127	/* excess 128 notation used	*/
#define	EXT_BIAS	   0	/* 2s-complement notation used	*/
				/* this is possible because the	*/
				/* sign is in a seperate word	*/
		
		/*	VARIOUS MAX AND MIN VALUES	*/
		/*	1) FOR THE DIFFERENT FORMATS	*/

#define	SGL_MAX		   255	/*	standard definition	*/
#define	SGL_MIN		     0	/*	standard definition	*/
#define	DBL_MAX		  255	/*	standard definition	*/
#define	DBL_MIN		     0	/*	standard definition	*/
#define EXT_MAX		 16384	/*	standard minimum	*/
#define EXT_MIN		-16383	/*	standard minimum	*/
#endif
