#ifndef __AUD_SRC_H__
#define __AUD_SRC_H__

// -------------------------------------------------
// Description : Get version number (Ex. 0x0001030c means version 1.3.c
// Return :
// -------------------------------------------------
int Aud_SRC_Ver(void);

// -------------------------------------------------
// Description : retrun the size of required memory allocation.
// Return : integer.
// -------------------------------------------------
int Aud_SRC_PreInit(
	int ch,
	int inCount,
	int outCount,
	int OneFrameMode
);

// -------------------------------------------------
// Description : initialize Aud_SRC_Main library, mainly
//          for memory allocation.
// Return : 0 (Success)
//          others (Error)
// -------------------------------------------------
int Aud_SRC_Init(
	int *handle,
	int ch,
	int inCount,
	int outCount,
	int OneFrameMode,
	short * OutMem      // Memory Address used in ASRC
);

// -------------------------------------------------
// Description : main program
//
// Return : 0 (Success)
//          others (error)
// -------------------------------------------------
int Aud_SRC_Main(
	int handle,
	void *  pInBuf,   /*Input Buffer pointer*/
	void *  pOutBuf   /*Output Buffer pointer*/
);

// -------------------------------------------------
// Description : Ends Aud_SRC_Main library, mainly for
//              releasing allocated memory space.
// Return :
// -------------------------------------------------
void Aud_SRC_Destroy(
	int handle
);

#endif

