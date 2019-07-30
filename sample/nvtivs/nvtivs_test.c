#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <fcntl.h>
#include "nvtivs.h"

#include <math.h>
#include <ft2build.h>

//#define DEBUG

#define DBG_WRN(fmtstr, args...) printf("\033[33mWRN:%s(): \033[0m" fmtstr,__func__, ##args)
#define DBG_ERR(fmtstr, args...) printf("\033[31mERR:%s(): \033[0m" fmtstr,__func__, ##args)

#ifdef DEBUG
#define DBG_IND(fmtstr, args...) printf(fmtstr, ##args)
#else
#define DBG_IND(fmtstr, args...)
#endif

#define DBG_DUMP(fmtstr, args...) printf(fmtstr, ##args)

#define HEIGHT  50

typedef enum {
    IVS_TEST_FACE_DETECTION,
    IVS_TEST_MAX,
} IVSTestItem;

static void show_help(void)
{
    DBG_DUMP("usage:\n"
             "  nvtivs_test [test item] [test channel]\n");
    DBG_DUMP("  [test item] : assign test item\n");
    DBG_DUMP("    %d : face detection test\n", IVS_TEST_FACE_DETECTION);
}


static void test_fd(void)
{
#if 1
    UINT32 i,uiFDIdx = 0, testloop = 20;
    IVSFDSetting   fdSetting={0};
    IVSFDData     fdData={0};
    IVSIAObjData *pFace;
    IVSIAObjData targetCoord={0,0,1920,1080};
    BOOL bUpdated;

    // Get fd setting
    NvtIVS_GetFDSetting(uiFDIdx, &fdSetting);
    DBG_DUMP("Current face detection setting:\n");
    DBG_DUMP("Max Face Number = %lu\n", fdSetting.uiMaxFaceNum);
    DBG_DUMP("Face Scale      = %lu\n", fdSetting.uiFaceScale);
    DBG_DUMP("Process Rate    = %lu\n", fdSetting.uiProcessRate);
    DBG_DUMP("\n");

    // Set fd setting
    fdSetting.uiMaxFaceNum = 6;
    fdSetting.uiFaceScale = 14;
    fdSetting.uiProcessRate = 4;
    NvtIVS_SetFDSetting(uiFDIdx, &fdSetting);

    DBG_DUMP("New face detection setting:\n");
    DBG_DUMP("Max Face Number = %lu\n", fdSetting.uiMaxFaceNum);
    DBG_DUMP("Face Scale      = %lu\n", fdSetting.uiFaceScale);
    DBG_DUMP("Process Rate    = %lu\n", fdSetting.uiProcessRate);
    // Start FD
    NvtIVS_StartFD(uiFDIdx);

    // Get FD result
    while (testloop--)
    {
        sleep(1);

        NvtIVS_GetFDData(uiFDIdx, &fdData,&targetCoord, &bUpdated);
        pFace = fdData.psFDObjData;
        DBG_DUMP("uiFaceNum  =%lu, bUpdated =%lu\n",fdData.uiFaceNum, bUpdated);
        for (i=0;i<fdData.uiFaceNum;i++)
        {
            DBG_DUMP("face %02d , x=%04d, y=%04d, w=%04d, h=%04d\n",i,pFace->uiCoordinateX,pFace->uiCoordinateY,pFace->uiWidth,pFace->uiHeight);
            pFace++;
        }
        NvtIVS_ReleaseFDData(uiFDIdx, &fdData);
    }
    // Stop FD
    NvtIVS_StopFD(uiFDIdx);

    DBG_DUMP("Face detection test terminated!\n");
#endif
}


int main(int argc, char *argv[])
{
    int testItem=IVS_TEST_MAX;

    if ((argc != 2) || (testItem = atoi(argv[1])) >= IVS_TEST_MAX)
    {
        show_help();
        return 0;
    }


    switch (testItem)
    {
        case IVS_TEST_FACE_DETECTION:
            test_fd();
            break;
        default:
            break;
    }

    return 0;
}


