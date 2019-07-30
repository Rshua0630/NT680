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
#include <linux/fcntl.h>
#include <pthread.h>
#include <nvtpstore.h>

#define THREAD_DESTROY(thread_handle) if (thread_handle)pthread_cancel(thread_handle)
#define THREAD_RETURN(thread_handle) thread_handle = 0;pthread_detach(pthread_self());pthread_exit(NULL);

#define SEM_HANDLE pthread_mutex_t
#define SEM_CREATE(handle,init_cnt) pthread_mutex_init(&handle,NULL);if(init_cnt==0)pthread_mutex_lock(&handle)
#define SEM_SIGNAL(handle) pthread_mutex_unlock(&handle)
#define SEM_WAIT(handle) pthread_mutex_lock(&handle)
#define SEM_DESTROY(handle) pthread_mutex_destroy(&handle)

#define DBG_WRN(fmtstr, args...) printf("\033[33mWRN:%s(): \033[0m" fmtstr,__func__, ##args)
#define DBG_ERR(fmtstr, args...) printf("\033[31mERR:%s(): \033[0m" fmtstr,__func__, ##args)

#if 0
#define DBG_IND(fmtstr, args...)
#define DBG_MSG(fmtstr, args...)
#else
#define DBG_IND(fmtstr, args...) printf(fmtstr, ##args)//printf("%s(): " fmtstr, __func__, ##args)
#define DBG_MSG(fmtstr, args...) printf(fmtstr, ##args)
#endif

#define DBG_DUMP(fmtstr, args...) printf(fmtstr, ##args)

#define RANDOM_RANGE(n)     (rand() % (n))

static pthread_t ps_thread1;
static pthread_t ps_thread2;
static int       ps_thread1_ret = 0;
static int       ps_thread2_ret = 0;
static pthread_mutex_t     semID1,semID2;


static int test_pstore_main(char *psname, char* dataW);
static void* ps_thread1_main(void* arg)
{
    char dataW[100]= "09876543210987654321098765432109876543210987654321098765432109876543210987654321";
    ps_thread1_ret = test_pstore_main("ps2",dataW);
    SEM_SIGNAL(semID1);
    pthread_detach(pthread_self());
    pthread_exit(NULL);
}

static void* ps_thread2_main(void* arg)
{
    char dataW[100]= "09876543210987654321098765432109876543210987654321098765432109876543210987654321";
    ps_thread2_ret = test_pstore_main("ps3",dataW);
    SEM_SIGNAL(semID2);
    pthread_detach(pthread_self());
    pthread_exit(NULL);
}


static int  test_pstore_main(char *psname, char* dataW)
{
    int psHandle, ret = 0;
    psHandle=nvtpstore_open(psname,PS_CREATE|PS_RDWR);
    DBG_IND("psHandle = %d\r\n",psHandle);
    if (psHandle <=0)
    {
        DBG_ERR("open pstore error\n");
        psHandle = 0;
        return -1;
    }
    //char dataW[100]= "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
    char dataR[100];
    int  i;
    int  datelen[10]={50,30,20,10,24,48,72};
    int  offset[10]={1,2,3,4,5,6,7};
    int  dumpCount = 0;

    #if 1
    {
        #define TEMP_BUF_SIZE  0x200000
        #define ALIGN_FLOOR(value, base)  ((value) & ~((base)-1))                   ///< Align Floor
        #define ALIGN_ROUND(value, base)  ALIGN_FLOOR((value) + ((base)/2), base)   ///< Align Round
        #define ALIGN_CEIL(value, base)   ALIGN_FLOOR((value) + ((base)-1), base)   ///< Align Ceil


        int    Size,Size2,i,j,k,compareFail=0;
        int    Wsize,Rsize;
        char   *tempbuf , *readBuf;
        char   *pTmp,*pTmp2;
        unsigned int    pBuf,randNumber,randSize;
        int    buffalign=4,pos=0;
        int    datelen[10]={200020,410001,630003,140002,100005,130006,150007,170008};
        int    offset[10]={1,2,3,4,5,6,7,8};
        int64_t    timeMarkB,timeMarkE;
        int64_t    timeWrite,timeRead;
        struct     timeval tv;


        pBuf = (int)malloc(TEMP_BUF_SIZE);
        if (!pBuf)
        {
            printf("alloc buffer fail\r\n");
            ret = -1;
            goto ps_err1;
        }
        //coverity[dont_call]
        randSize = RANDOM_RANGE(20000);
        for (k=0;k<8;k++)
        {
            Wsize = datelen[k]+randSize;
            Rsize = Wsize;
            pos = offset[k];
            Size = Wsize;
            Size2 = Rsize;
            tempbuf = (char * )(pBuf+buffalign);
            readBuf = (char * )ALIGN_CEIL(((unsigned int)tempbuf + Wsize)+ buffalign,32);

            DBG_IND("WriteBuf = 0x%x, ReadBuf = 0x%x \r\n",tempbuf,readBuf);
            if (Wsize+Rsize> TEMP_BUF_SIZE )
            {
                DBG_ERR("Wsize+Rsize %d > tmp buf size %d\r\n",Wsize+Rsize,TEMP_BUF_SIZE);
                goto ps_err2;
            }
            //coverity[dont_call]
            randNumber = RANDOM_RANGE(47);
            j=randNumber;
            for (i=0;i<Wsize;i++)
            {
                tempbuf[i]=j++;
                if (j==47)
                    j=0;
            }
            DBG_IND("%s Wsize = %d, Rsize=%d , pos =%d\r\n",psname,Wsize,Rsize,pos);
            // write data
            gettimeofday(&tv,NULL);
            timeMarkB = (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;

            if(lseek(psHandle, pos, SEEK_SET) == -1)
                printf("fail to seek file\r\n");

            ret = write(psHandle, tempbuf, Wsize);
            if (ret < 0) {
                DBG_ERR("write error -%d\r\n",errno);
                ret = -1;
                goto ps_err2;
            }
            gettimeofday(&tv,NULL);
            timeMarkE = (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;
            timeWrite = timeMarkE-timeMarkB;

            // read data
            gettimeofday(&tv,NULL);
            timeMarkB = (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;

            if(lseek(psHandle, pos, SEEK_SET) == -1)
                printf("fail to seek\r\n");

            ret = read(psHandle, readBuf, Rsize);
            if (ret < 0) {
                DBG_ERR("read error -%d\r\n",errno);
                ret = -1;
                goto ps_err2;
            }
            gettimeofday(&tv,NULL);
            timeMarkE = (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;
            timeRead = timeMarkE-timeMarkB;
            // compare data
            pTmp =  tempbuf;
            pTmp2 = readBuf;
            for(i=0;i<Rsize;i++)
            {
                if (pTmp[i] != pTmp2[i])
                {
                    compareFail = 1;
                    dumpCount++;
                    if (dumpCount > 100)
                        break;
                    DBG_ERR("PS Data compare fail i=%d, *(0x%X)=0x%x, *(0x%X)=0x%x\r\n", i, &pTmp[i], pTmp[i], &pTmp2[i], pTmp2[i]);
                }
            }
            DBG_IND("%s Wsize = %d, Rsize=%d , pos =%d, WriteT=%lld ms, ReadT=%lld ms\r\n",psname,Wsize,Rsize,pos,timeWrite/1000,timeRead/1000);
            if (compareFail)
            {
                ret= -1;
                goto ps_err2;
            }
        }
        // test error case
        #if 0
        printf("test read/write over size begin\r\n");
        Wsize = 0x500001;
        Rsize = 0x500001;
        write(psHandle,tempbuf,Wsize);
        read(psHandle,tempbuf,Rsize);
        printf("%s Wsize = %d, Rsize=%d \r\n",psname,Wsize,Rsize);
        printf("test read/write over size end\r\n");
        #endif
        printf("test for %s OK\r\n", psname);
ps_err2:
        free((char*)pBuf);
    }
    #endif

ps_err1:
    nvtpstore_close(psHandle);
    nvtpstore_remove(psname);
    return ret;
}
static void test_pstore(int testloop)
{
    int i,j;
    char dataW[100]= "12345678901234567890123456789012345678901234567890123456789012345678901234567890";

    DBG_IND("test_pstore begin\r\n");
    SEM_CREATE(semID1,1);
    SEM_CREATE(semID2,1);

    ps_thread1_ret = 0;
    ps_thread2_ret = 0;

    if (testloop == 0)
        testloop = 1;
    for (j=0;j<testloop;j++)
    {
        DBG_IND("loop %d\r\n",j);
        SEM_WAIT(semID1);
        SEM_WAIT(semID2);
        if (ps_thread1_ret < 0 || ps_thread2_ret < 0)
            goto test_ps_err;
        if(pthread_create(&ps_thread1,NULL,ps_thread1_main,NULL)){
            printf("fail to create ps_thread1_main thread\r\n");
            break;
        }
        if(pthread_create(&ps_thread2,NULL,ps_thread2_main,NULL)){
            printf("fail to create ps_thread1_main thread\r\n");
            break;
        }
        // test error case
        #if 0
        printf("test open/remove name over size begin\r\n");
        pstore_open("ps12345678901",PS_CREATE|PS_RDWR);
        pstore_remove("ps12345678901");
        pstore_remove("ps12345678");
        printf("test open/remove name over size end\r\n");
        #endif
    }
    DBG_IND("loop end\r\n");
    //coverity[double_lock]
    SEM_WAIT(semID1);
    //coverity[double_lock]
    SEM_WAIT(semID2);
test_ps_err:
    //coverity[locked_destroy]
    SEM_DESTROY(semID1);
    //coverity[locked_destroy]
    SEM_DESTROY(semID2);
    DBG_IND("test_pstore end\r\n");
}

static void test_nand_main(void)
{
    char       *filepath = "/var/www/tmp.tmp";
    int        fd, ret = 0;
    int        flags = O_RDWR|O_CREAT|O_TRUNC;
    char       *pBuf;

    DBG_IND("test_nand_main begin\r\n");
    fd = open(filepath, flags, 0777);
    if (fd < 0)
    {
        printf("create file /var/www/tmp.tmp fail errno=%d\r\n",errno);
        return;
    }
    #if 1
    {
        #define TEMP_BUF_SIZE  0x200000
        #define ALIGN_FLOOR(value, base)  ((value) & ~((base)-1))                   ///< Align Floor
        #define ALIGN_ROUND(value, base)  ALIGN_FLOOR((value) + ((base)/2), base)   ///< Align Round
        #define ALIGN_CEIL(value, base)   ALIGN_FLOOR((value) + ((base)-1), base)   ///< Align Ceil


        int    Size,Size2,i,j,k,compareFail=0;
        int    Wsize,Rsize;
        char   *tempbuf , *readBuf;
        char   *pTmp,*pTmp2;
        unsigned int    pBuf,randNumber,randSize;
        int    buffalign=4,pos=0;
        int    datelen[10]={200020,410001,630003,140002,100005,130006,150007,170008};
        int    offset[10]={1,2,3,4,5,6,7,8};
        int64_t    timeMarkB,timeMarkE;
        int64_t    timeWrite,timeRead;
        struct     timeval tv;
        int        dumpCount = 0;


        pBuf = (int)malloc(TEMP_BUF_SIZE);
        if (!pBuf)
        {
            DBG_ERR("alloc buffer fail\r\n");
            ret = -1;
            goto nand_err1;
        }
        //coverity[dont_call]
        randSize = RANDOM_RANGE(20000);
        for (k=0;k<8;k++)
        {
            Wsize = datelen[k]+randSize;
            Rsize = Wsize;
            pos = offset[k];
            Size = Wsize;
            Size2 = Rsize;
            tempbuf = (char * )(pBuf+buffalign);
            readBuf = (char * )ALIGN_CEIL(((unsigned int)tempbuf + Wsize)+ buffalign,32);

            DBG_IND("WriteBuf = 0x%x, ReadBuf = 0x%x \r\n",tempbuf,readBuf);

            if (Wsize+Rsize> TEMP_BUF_SIZE )
            {
                DBG_ERR("Wsize+Rsize %d > tmp buf size %d\r\n",Wsize+Rsize,TEMP_BUF_SIZE);
                ret = -1;
                goto nand_err2;
            }
            //coverity[dont_call]
            randNumber = RANDOM_RANGE(47);
            j=randNumber;
            for (i=0;i<Wsize;i++)
            {
                tempbuf[i]=j++;
                if (j==47)
                    j=0;
            }
            // write data
            gettimeofday(&tv,NULL);
            timeMarkB = (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;

            if(lseek(fd, pos, SEEK_SET) == -1)
                printf("fail to seek\r\n");

            ret = write(fd, tempbuf, Wsize);
            sync();
            if (ret < 0)
            {
                DBG_ERR("write error -%d\r\n",errno);
                ret = -1;
                goto nand_err2;
            }
            gettimeofday(&tv,NULL);
            timeMarkE = (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;
            timeWrite = timeMarkE-timeMarkB;

            // read data
            gettimeofday(&tv,NULL);
            timeMarkB = (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;

            if(lseek(fd, pos, SEEK_SET) == -1)
                printf("fail to seek\r\n");

            ret = read(fd, readBuf, Rsize);
            if (ret < 0) {
                DBG_ERR("read error -%d\r\n",errno);
                ret = -1;
                goto nand_err2;
            }
            gettimeofday(&tv,NULL);
            timeMarkE = (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;
            timeRead = timeMarkE-timeMarkB;
            // compare data
            pTmp =  tempbuf;
            pTmp2 = readBuf;
            for(i=0;i<Rsize;i++)
            {
                if (pTmp[i] != pTmp2[i])
                {
                    compareFail = 1;
                    dumpCount++;
                    if (dumpCount > 100)
                        break;
                    DBG_ERR("Nand Data compare fail i=%d, *(0x%X)=0x%x, *(0x%X)=0x%x\r\n", i, &pTmp[i], pTmp[i], &pTmp2[i], pTmp2[i]);
                }
            }
            DBG_IND("%s Wsize = %d, Rsize=%d , pos =%d, WriteT=%lld ms, ReadT=%lld ms\r\n",filepath,Wsize,Rsize,pos,timeWrite/1000,timeRead/1000);
            if (compareFail)
            {
                ret= -1;
                goto nand_err2;
            }
        }
nand_err2:
        free((char*)pBuf);
    }
    #endif
nand_err1:
    close(fd);
    DBG_IND("test_nand_main end\r\n");
}


static void test_nand(int testloop)
{
    int i,j;

    DBG_IND("test_nand begin");
    if (testloop == 0)
        testloop = 1;
    for (j=0;j<testloop;j++)
    {
        DBG_IND("test loop %d\r\n",j);
        for (i=0;i<3;i++)
        {
            test_nand_main();
        }
    }
    DBG_IND("test_nand end");
}

static void test_pstore_and_nand(int testloop)
{
    int i,j;
    char dataW[100]= "12345678901234567890123456789012345678901234567890123456789012345678901234567890";


    DBG_IND("test_pstore_and_nand begin");
    SEM_CREATE(semID1,1);
    SEM_CREATE(semID2,1);

    ps_thread1_ret = 0;
    ps_thread2_ret = 0;

    if (testloop == 0)
        testloop = 1;
    for (j=0;j<testloop;j++)
    {
        DBG_IND("loop %d\r\n",j);
        SEM_WAIT(semID1);
        SEM_WAIT(semID2);
        if (ps_thread1_ret < 0 || ps_thread2_ret < 0)
            goto test_ps_err;
        if(pthread_create(&ps_thread1, NULL, ps_thread1_main, NULL)){
            printf("fail to create ps_thread1_main thread\r\n");
            break;
        }
        if(pthread_create(&ps_thread2, NULL, ps_thread2_main, NULL)){
            printf("fail to create ps_thread2_main thread\r\n");
            break;
        }
        for (i=0; i<3; i++) {
            test_nand_main();
        }
        // test error case
        #if 0
        printf("test open/remove name over size begin\r\n");
        pstore_open("ps12345678901",PS_CREATE|PS_RDWR);
        pstore_remove("ps12345678901");
        pstore_remove("ps12345678");
        printf("test open/remove name over size end\r\n");
        #endif
    }
    //coverity[double_lock]
    SEM_WAIT(semID1);
    //coverity[double_lock]
    SEM_WAIT(semID2);

test_ps_err:
    //coverity[locked_destroy]
    SEM_DESTROY(semID1);
    //coverity[locked_destroy]
    SEM_DESTROY(semID2);
    DBG_IND("test_pstore_and_nand end");
}

int main(int argc, char *argv[])
{
    int testloop = 10, i = 0, testmode=0;

    while (i < argc)
    {
        // loopcount
        if (!strcmp(argv[i], "-l"))
        {
            i++;
            if (argv[i])
            {
                testloop = atoi(argv[i]);
            }
        }
        // test mode
        else if (!strcmp(argv[i], "-m"))
        {
            i++;
            if (argv[i])
            {
                testmode = atoi(argv[i]);
            }
        }
        i++;
    }
    if (testmode == 0)
    {
	//coverity[tainted_data]
        test_pstore(testloop);
    }
    else if (testmode == 1)
    {
	//coverity[tainted_data]
        test_nand(testloop);
    }
    else if (testmode == 2)
    {
	//coverity[tainted_data]
        test_pstore_and_nand(testloop);
    }

	return 0;
}
