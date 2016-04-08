                            /* AttrChr.h */

#ifndef __ATTRCHR_H
#define __ATTRCHR_H

#define uchar    unsigned char

#define ushort   unsigned short

typedef ushort   attrint;

struct attrchr
         { uchar chr;
           uchar attr;
         };
typedef struct attrchr   attrchr;
typedef struct attrchr * attrptr;

struct fbgrchr
         { unsigned chr:8;
           unsigned fgr:4;
           unsigned bgr:3;
           unsigned blk:1;
         };
typedef struct fbgrchr   fbgrchr;

enum attr_corr_ops
       { /* BitBlt options */
          NOT_ATTR = 16,  /* NOT */
          NGT_ATTR = 32   /* NGT */
       };

/**********************************************************************/
void attr_fill (attrptr aptr, int len, int attr);
void attr_or   (attrptr aptr, int len, int attr);
void attr_and  (attrptr aptr, int len, int attr);
void attr_xor  (attrptr aptr, int len, int attr);
void attr_ngt  (attrptr aptr, int len);
/**********************************************************************/
typedef void (*attr_func) (attrptr aptr, int len, int attr);

#endif  /* __ATTRCHR_H */
