/*
 * MACROS.h
 *
 * Created: 11/1/2019 5:31:29 PM
 *  Author:
 */ 


#ifndef MACROS_H_
#define MACROS_H_

#define SETBIT(R,B) (R|=(1<<B))
#define CLRBIT(R,B) (R&=~(1<<B))
#define TGLBIT(R,B) (R^=(1<<B))
#define GETBIT(R,B) ((R&(1<<(B)))>>(B))


/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(R,B) ( R & (1<<B) )

/* Check if a specific bit is cleared in any register and return true if yes */
#define BIT_IS_CLEAR(R,B) ( !(R & (1<<B)) )


#endif /* MACROS_H_ */
