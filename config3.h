
/*  CONFIGURATION FOR LINUX (CONFIG3)  */

/* To enable this file rename it to CONFIG.H */

/* This headder file contains #define's
   which change for different hardware/software model
*/

/******************************************************************/
/* ### Using STANDARD library or some special libraries           */


/*#define EXIST_ALLOC_H
/* functions farmalloc and farcoreleft */

/*#define EXIST_SYS_TIMEB_H
/* functions to get time with millisecond precision */

/*#define EXIST_CONIO_H
/* functions to input characters from the keyboard immediately; */

/******************************************************************/
/* ### Using sophisticated pointer structure: segment-offset      */

/* #define AT286         segment-offset pointers */

/******************************************************************/
/* ### Memory model definition: SMALL - SPLIT - LARGE - HUGE32
    HUGE32 - use 32-bit heap address,
           otherwise heap address is 16-bit segment pointer;
           element model (see below) is LARGE
    LARGE - use 32-bit pointer to element;
    SPLIT - use 16-bit pointer to element;
            element record is split into 4 arrays
            (maximum 32K elements);
    SMALL - use 16-bit pointer to 8-byte element
            (maximum 8K elements)
                                                                 */
#define HUGE32

/******************************************************************/
/* Function code representation

/* #define CHAIN_FUN         ###   CHAIN/VECTOR function mode */
/* #define NUMB_CODE         ###   NUNB/WORD code */
