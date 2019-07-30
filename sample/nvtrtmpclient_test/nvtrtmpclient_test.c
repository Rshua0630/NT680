#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#include <nvtrtmpclient.h>

#define DBG_COLOR_RED "^R"
#define DBG_COLOR_BLUE "^B"
#define DBG_COLOR_YELLOW "^Y"

#define CHKPNT      printf("\033[37mCHK: %d, %s\033[0m\r\n",__LINE__,__func__)
#define DBGD(x)     printf("\033[0;35m%s=%d\033[0m\r\n",#x,x)                  ///< Show a color sting of variable name and variable deciaml value
#define DBGH(x)     printf("\033[0;35m%s=0x%08X\033[0m\r\n",#x,x)
#define DBG_ERR(fmtstr, args...) printf("\033[0;31mERR:%s() \033[0m" fmtstr, __func__, ##args)
#define DBG_WRN(fmtstr, args...) printf("\033[0;33mWRN:%s() \033[0m" fmtstr, __func__, ##args)
#define DBG_DUMP(fmtstr, args...) printf(fmtstr, ##args)

#if 1 //debug
#define DBG_IND(fmtstr, args...) printf("%s(): " fmtstr, __func__, ##args)
#else //release
#define DBG_IND(...)
#endif


int gbExit = 0;

//-----------------------------------------------------
// This function handles the SIGINT (Ctrl-C) signal.
//-----------------------------------------------------
void INThandler(int sig)
{
    printf("\n<CTRL-C>\n");
    gbExit = 1;
}

int main(int argc, char *argv[])
{
    NVTRTMPCLIENT_USEROBJ sRtmpClientObj;
    NVTRTMPCLIENT_HANDLER hRtmpClient;

    if(argc < 3) {
        DBG_ERR("argc < 3\r\nUsage   : %s [streamsender channel] [rtmpURL]\r\nexample : %s 0 rtmp://192.168.0.20/myapp/mystream\r\n", argv[0], argv[0]);
        return -1;
    }
    sRtmpClientObj.channel = atoi(argv[1]);
    sRtmpClientObj.rtmpURL = argv[2];

    signal(SIGINT,  INThandler);    // install Ctrl-C handler
    signal(SIGPIPE, SIG_IGN);       // ignore SIGPIPE


    if (ER_SUCCESS != NvtRtmpClient_Open(&sRtmpClientObj, &hRtmpClient)) {
        DBG_ERR("Open fail !!!\n");
        return (-1);
    }

    if (ER_SUCCESS != NvtRtmpClient_Start(hRtmpClient)) {
        DBG_ERR("Start fail !!!\n");
        return (-1);
    }

    // wait until user hit <CTRL-C>
    printf("Enter <CTRL-C> to quit\n");
    while (gbExit==0) {
        usleep(50000);
    }

    if (ER_SUCCESS != NvtRtmpClient_Stop(hRtmpClient)) {
        DBG_ERR("Stop fail !!!\n");
        return (-1);
    }

    if (ER_SUCCESS != NvtRtmpClient_Close(hRtmpClient)) {
        DBG_ERR("Close fail !!!\n");
        return (-1);
    }

    return 0;
}
