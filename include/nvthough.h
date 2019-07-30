#ifndef __NVTHOUGH_H__
#define __NVTHOUGH_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <nvtmem.h>

#define MAX_LINE_NUM        (1<<10)
#define MAX_POINT_NUM       (1<<16)
#define MAX_ANGLE_NUM       180
#define MAX_IMAGE_WIDTH     640     //140 //135
#define MAX_IMAGE_HEIGHT    480     //260 //256
#define MAX_RHO_NUM (((MAX_IMAGE_WIDTH + MAX_IMAGE_HEIGHT)<< 1) + 1)
#define HOUGHLINE_BUF_SIZE (((MAX_ANGLE_NUM * MAX_RHO_NUM) + (MAX_LINE_NUM*(4+1)))*sizeof(int) + (MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT)*sizeof(unsigned char))  //2M
#define AVP_LINUX_VER       0x20170825

#define _abs(n) ( (n)>=0 ? (n) : (-(n)) )

typedef struct _MyPoint
{
	int x;
	int y;
}
MyPoint;
typedef struct _MyLine
{
	MyPoint start;
	MyPoint end;
	unsigned int countPoints;
}
MyLine;
typedef struct _MyLineCluster
{
	MyLine* lines;
	int length;
}
MyLineCluster;
typedef struct _MyImage
{
	unsigned char* data;
	int rows;       //height
	int cols;       //width
	int step;       //width step in bytes
}MyImage;
typedef struct _MyMat
{
	int* data;
	int rows;       //height
	int cols;       //width
	int step;       //width step in bytes
}MyIMat;
typedef struct
{
	int  size;
	int *p;
}P_INT;
typedef struct
{
	int size;
	unsigned char* p;
}P_UCHAR;
typedef struct
{
	int    totalsize;
	P_INT  pAccum;
	P_UCHAR  pMask;
	P_INT  pLineClust;
}HOUGH_MEM;

typedef struct _Hough_Parms
{
	int threshold;          //threshold of accum,  the larger the num is, the more difficult the lines to be detected.
	int minLineLength;      //if lines.start.x - lines.end.x>20 && lines.start.y - lines.end.y>20, goodline = 1.  The larger the num is, the more difficult the lines to be detected.
	int maxGap;             //if the gap between two lines< maxGap,the  two lines will be regard as one.
	int linePointsTH;       //if the points on tne line are not enough
	int edgeTH;             //remove the weak pixel
	int ang_res;            // > 0
	int rho_res;            // > 0
	int sAng0;              // >= 0
	int eAng0;              // eAng0 > sAng0
	int sAng1;              // sAng1 > eAng0
	int eAng1;              // eAng1 > sAng1
}Hough_Parms;

typedef struct _HOUGH_IPC_INIT_MSG
{
    unsigned int        version;
    unsigned int        workBuffAddr;
    unsigned int        workBuffSize;
    unsigned int        cacheBuffAddr;
    unsigned int        cacheBuffSize;
}HOUGH_IPC_INIT_MSG;

typedef enum _HOUGH_IPC_CMD
{
	HOUGH_IPC_REQ   = 0,    // uItron request hough active
	HOUGH_IPC_DONE,         // hough is done.
	HOUGH_IPC_REL_REQ,      // release the loop
	HOUGH_IPC_REL_DONE,     // release the loop done
	HOUGH_IPC_INIT_DONE,    // hough thread is init
}HOUGH_IPC_CMD;

typedef struct _HOUGH_IPC_MSG
{
	unsigned int        uiIPCcmd; // IPC command
	unsigned int        uiParmsAddr;
}HOUGH_IPC_MSG;

typedef struct _HOUGH_IPC_PARMS_MSG
{
	unsigned int        EdgeImgAddr;
	unsigned int        HoughParmsAddr;
	unsigned int        CacheBuf;
	unsigned int        LinesAddr;
	unsigned int        HoughPtsAddr;
}HOUGH_IPC_PARMS_MSG;

#ifdef __cplusplus
extern "C" {
#endif

void Nvt_HoughLineP( MyImage *src, Hough_Parms *houghParms, unsigned int pBuff, MyLineCluster *Lines, unsigned int* countImgPoints);
unsigned int HL_CalcCacheBuffSize(void);

#ifdef __cplusplus
}
#endif

#endif
