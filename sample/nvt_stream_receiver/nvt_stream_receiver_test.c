#include <protected/nvtstreamreceiver_protected.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include "nvtipc.h"

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

int hDev = 0;
int pc = 0;

unsigned int audSR = 32000;
signed short gAudio_Buf[32000] = {0};

void UserAudio_GenData(int is_data)
{
    float amplitude = 0xFFFF/2;
    float frequency = 500;
    unsigned int n;

    if (is_data) {
		for (n=0; n<audSR; n++)
		{
	        *(gAudio_Buf+n) = (signed short)(amplitude * sin((2 * M_PI * (float)n * frequency) / audSR));
		}
	}
}

void signal_cb(int signo)
{
    static int cn=0;
    static int ret=0;
    char* addr;
    int size;
    switch (signo)
    {
    case SIGALRM:
        addr = (char*)(gAudio_Buf+3200*cn);
        size = sizeof(signed short)*3200;
        DBG_DUMP("[PutAStream] - addr=0x%x,size=0x%x ..... ", addr, size);
        ret = NvtStreamReceiver_PutAStream(hDev, addr, &size);
        DBG_DUMP("ret = %d, rcvsize=0x%x \r\n", ret, size);
        cn++; if(cn==10)cn=0;
        pc++;
        break;
    }
    return;
}

void test_audio(int is_data)
{
	UserAudio_GenData(is_data);

    {
        struct itimerval timer={0};

        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = 100*1000;
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = 100*1000;

        signal(SIGALRM, signal_cb);
        setitimer(ITIMER_REAL, &timer, NULL);
    }

    while(pc<1000*1000)
        sleep(1);


#if 0
		    {//audio
		        ASTREAM_S AStream;
		        if(NVTSTREAM_RET_OK != NvtStream_GetAStream(hDev, ChanId, 0, &AStream))
		        {
		            DBG_ERR("GetAStream\r\n");
		            return -1;
		        }
		        DBG_DUMP("A%d(%d): Addr=0x%X, Size=0x%X, ItemSN=%lld\r\n",
		            ChanId, mypid, AStream.Addr, AStream.Size, AStream.ItemSN);
		    }
#endif
}

void test_vidio(unsigned int addr, int size)
{
	float amplitude = 0xFFFF/2;
	float frequency = 500;
	unsigned int n;
	int ret = 0;
	char* inaddr;

	inaddr = (char *)NvtIPC_GetNonCacheAddr(addr);

	for (n=0; n<audSR/2; n++)
	{
        *(((signed short*)inaddr)+n) = (signed short)(amplitude * sin((2 * M_PI * (float)n * frequency) / audSR));
	}

    while(pc < 30) {
		ret = NvtStreamReceiver_PutVStream(hDev, inaddr, &size);
		usleep(499500);
		pc++;
		if (ret < 0) {
			DBG_ERR("Put video stream failed\r\n");
			break;
		}
	}
}

int main(int argc, char *argv[])
{
    //pid_t mypid;
    int is_data = 0;
	int testItem = 0;
#if 0
    unsigned int addr = 0;
    unsigned int size = 0;

    //mypid = getpid();
    if(argc < 3)
    {
        DBG_ERR("argc < 3\r\n");
        return -1;
    }
    DBG_DUMP("arg = %s %s\r\n", argv[1], argv[2]);
    addr = strtoul(argv[1], (char**)0, 0);
    size = strtoul(argv[2], (char**)0, 0);
    DBG_DUMP("[Buffer] - addr = %x, size = %x\r\n", addr, size);
#endif

	if (argc < 2) {
		DBG_ERR("Parameter error\r\n");
		return 0;
	}

	testItem = atoi(argv[1]);

	DBG_DUMP("[Open]\r\n");
    //hDev = NvtStreamReceiver_Open((char*)addr, (int)size);
    hDev = NvtStreamReceiver_Open();
    if(-1 == hDev)
    {
        DBG_ERR("Open\r\n");
        return -1;
    }

	DBG_DUMP("testItem=%d\t\n", testItem);

	switch (testItem)
    {
        case 0:
		{
			if(argc >= 3)
			{
				is_data = atoi(argv[2]);
			}
			test_audio(is_data);
			break;
        }
		case 1:
		{
			NVTSTREAMRECEIVER_VIDEO_INFO info = {0};

			if (argc < 6) {
				DBG_ERR("Parameter error\r\n");
				break;
			}

			info.CodecType = atoi(argv[2]);
			info.FrameRate = atoi(argv[3]);
			info.Width     = atoi(argv[4]);
			info.Height    = atoi(argv[5]);

			DBG_DUMP("codectype=%d, framrate=%d, width=%d, height=%d\r\n", info.CodecType, info.FrameRate, info.Width, info.Height);

			NvtStreamReceiver_SetVideoInfo(hDev, &info);
			break;
		}
		case 2:
		{
			int size = atoi(argv[3]);
			unsigned int inaddr;
			static void *g_strmrcv_test_addr = NULL;

			inaddr = atoi(argv[2]);

			g_strmrcv_test_addr = NvtIPC_mmap(NVTIPC_MEM_TYPE_NONCACHE, inaddr, size);

			test_vidio(inaddr, size);

			NvtIPC_munmap(g_strmrcv_test_addr, size);
			break;
		}
	}

	DBG_DUMP("[Close]\r\n");

	if(NVTSTREAMRECEIVER_ER_OK != NvtStreamReceiver_Close(hDev))
    {
        DBG_ERR("Close\r\n");
        return -1;
    }

    return 0;
}
