#include <stdio.h>
#include <stdlib.h>
#include "nvtuctrl.h"
#include "nvt_type.h"
#include "nvtipc.h"
#include "string.h"
#include <errno.h>
#include <unistd.h>
//#include "protected/nvtstreamreceiver_protected.h"


#define DEBUG


#ifdef DEBUG
#define DBG_IND(fmtstr, args...) printf(fmtstr, ##args)
#else
#define DBG_IND(fmtstr, args...)
#endif



static int nvtget_buf(char *name,int *size, unsigned int *phy_addr){

    int hdl = 0;
    char cmd[32]={0};
    char RetString[32] ={0};
    sprintf(cmd,"usys -bufget %s %d",name,*size);
    
    if ((hdl = NvtUctrl_Open()) >= 0)
    {
        if(NvtUctrl_CmdSnd(hdl, cmd, RetString, sizeof(RetString))< 0){

            printf("query buf from uitron error\r\n");
            return -1;
        }
        NvtUctrl_Close(hdl);
    }
    else{
        printf("get buf fail\r\n");
    }
    sscanf(RetString,"%u %d",phy_addr, size);
    printf("addr:%x \r\n",*phy_addr);
    printf("size:%d\r\n",*size);
    

    return 0;
}


static void * g_SnapMapAddr = NULL;
UINT32 g_SnapSize = 0;

static NVTRET nvtbuf_mmap(unsigned int phy_addr, int size){


    g_SnapMapAddr = NvtIPC_mmap(NVTIPC_MEM_TYPE_NONCACHE, phy_addr, size);
    if(g_SnapMapAddr == NULL){
        printf("NvtIPC_mmap error!\r\n");
        return ER_GET_DATA_FAIL;
    }
    g_SnapSize = size;


    return ER_SUCCESS;
}

static NVTRET nvtbuf_munmap(void){


    if(g_SnapMapAddr != NULL){
        if(NvtIPC_munmap(g_SnapMapAddr, g_SnapSize) != 0){
            printf("NvtIPC_munmap error!\r\n");
            return ER_GET_DATA_FAIL;
        }
    }
    return ER_SUCCESS;


}



#define BUF_SIZE 1500


static int malloc_buf(char **buf, int size){

    *buf = (char *)malloc( size );
    
    memset(*buf, 0,size);
    if(buf == NULL){
        printf("malloc buf fail!\r\n");
        return -1;
    }
    
    return 0;

}

static int check_start_code(char *buf, int size){

    int i=0;
    for(i=0; i< (size-3); i++){
        if(buf[i]==0 && buf[i+1]==0 && buf[i+2]==0 && buf[i+3]==1){
            return i;
        }
    } 
    return -1;
}

static int check_frame_end(char *buf, int size){

    ////first 4 bytes should be 0 0 0 1
    if(buf[0] != 0 || buf[1] != 0 || buf[2] != 0 || buf[3] != 1){
        printf("check first 4 bytes start code fail\r\n");
        return -1;
    }

    int i=0;
    for(i=4; i< size; i++){

        if((buf[i] == 0) && (buf[i+1]==0) && (buf[i+2] == 0) && (buf[i+3] == 1)){
            return i;
        }

    }
    return -1;

}

static int parse_frame(FILE *fp, char *queue,int *size){

    typedef enum {   MALLOC_BUF,
                     FILE_READ,
                     CHECK_START_CODE,
                     CHECK_FRAME_END,
                     REALLOCATE_BUF,
                     ERROR_STATE
                }PARSE_STATE;


    static PARSE_STATE parse_state = MALLOC_BUF;
    static char *TmpBuf = NULL;
    static char *TmpBuf2 = NULL;
    static int  TmpBufOffset = 0;
    static int TmpBufSize = 0;
    static int  TmpBufOffsetEnd = 0;
    static int TmpBufDataSize =0;
    static int FileReadBufOffset =0;
    int ret=0;
    if(fp == NULL){
        printf("FP NULL");
        return -1;
    }
    while(1){
        switch(parse_state){

            case MALLOC_BUF:
                DBG_IND("==MALLOC_BUF\r\n");
                
                if( malloc_buf(&TmpBuf,BUF_SIZE)!= 0 ){
                    printf("MALLOC_BUF state error\r\n");
                    parse_state = ERROR_STATE;    
                }
                else{
                    parse_state = FILE_READ;
                }
                TmpBufSize = BUF_SIZE;
                break;

            case FILE_READ:
                DBG_IND("==FILE_READ\r\n");
                ret = fread(TmpBuf+ FileReadBufOffset, 1, BUF_SIZE, fp);
                if(ret < BUF_SIZE){
                    printf("read finish ret:%d\r\n",ret);
                    ////charlie need to do
                    return 0;
                }
                else if(ret == BUF_SIZE){
                    parse_state = CHECK_START_CODE;
                }
                else{
                    printf("fread error ret:%d\r\n",ret);
                    parse_state = ERROR_STATE;
                }
                TmpBufDataSize = TmpBufDataSize + ret;

                break;
            case CHECK_START_CODE:
                DBG_IND("==CHECK_START_CODE :%x\r\n",TmpBuf + TmpBufOffset);
                TmpBufOffset = check_start_code(TmpBuf + TmpBufOffset,TmpBufSize-TmpBufOffset);
                if(TmpBufOffset == -1){
                    printf("%x %x %x %x %X\r\n",*((char *)(TmpBuf + TmpBufOffset)),*((char *)(TmpBuf + TmpBufOffset+1))
                                ,*((char *)(TmpBuf + TmpBufOffset+2)),*((char *)(TmpBuf + TmpBufOffset+3)));
                    //can not find start code, need fread from file again
                    parse_state = REALLOCATE_BUF;
                }
                else{
                    parse_state = CHECK_FRAME_END;
                }
                break;
            case CHECK_FRAME_END:
                DBG_IND("==CHECK_FRAME_END:%x %d\r\n",TmpBuf+TmpBufOffset,TmpBufOffset);
                TmpBufOffsetEnd = check_frame_end(TmpBuf+TmpBufOffset, TmpBufDataSize - TmpBufOffset); 
                if(TmpBufOffsetEnd < 0){
                    //not find frame end, need to do
                    parse_state = REALLOCATE_BUF;
                }
                else{
                    memcpy(queue, (char*)(TmpBuf+TmpBufOffset), TmpBufOffsetEnd);
                    TmpBufOffset = TmpBufOffset + TmpBufOffsetEnd;
                    TmpBufSize = TmpBufSize - TmpBufOffsetEnd;
                    TmpBufDataSize = TmpBufDataSize - TmpBufOffsetEnd;
                    parse_state = CHECK_START_CODE;

                    /////re-allocate buf
                    TmpBuf2 = TmpBuf;
                    if( malloc_buf(&TmpBuf,TmpBufSize)!= 0 ){
                        printf("CHECK_FRAME_END MALLOC_BUF state error\r\n");
                        parse_state = ERROR_STATE;
                    }
                    else{
                        memcpy(TmpBuf,TmpBuf2+ TmpBufOffset,TmpBufDataSize);
#if 0
                        printf("TmpBuf:%x %x %x %x\r\n",(*(char *)(TmpBuf + 0)),(*(char *)(TmpBuf + 1)),(*(char *)(TmpBuf + 2)),
                                        (*(char *)(TmpBuf + 3)));
                        printf("TmpBuf2:%x %x %x %x\r\n",(*(char *)(TmpBuf2 + 0 + TmpBufOffset)),
                                        (*(char *)(TmpBuf2 + 1+ TmpBufOffset)),(*(char *)(TmpBuf2 + 2+ TmpBufOffset)),
                                        (*(char *)(TmpBuf2 + 3+ TmpBufOffset)));
#endif
                        free(TmpBuf2);
                    }
                    TmpBufOffset = 0;
                    *size = TmpBufOffsetEnd; 
                    return 0;
                }

                break;
            case REALLOCATE_BUF:
                DBG_IND("==REALLOCATE_BUF\r\n");
                TmpBuf2 = TmpBuf;

                if( malloc_buf(&TmpBuf,BUF_SIZE+TmpBufSize)!= 0 ){
                    printf("MALLOC_BUF state error\r\n");
                    parse_state = ERROR_STATE;
                }
                else{
                    memcpy(TmpBuf,TmpBuf2,TmpBufDataSize);
                    free(TmpBuf2);
                    FileReadBufOffset = TmpBufSize;
                    parse_state = FILE_READ;
                }

                TmpBufSize = BUF_SIZE + TmpBufSize;
                printf("TmpBufSize:%d\r\n",TmpBufSize);
                break;
            case ERROR_STATE:
                DBG_IND("==ERROR_STATE\r\n");
                printf("error, need stop\r\n");
                return -1;
                break;

            default:
                printf("error state:%d\r\n",parse_state);
                break;


        }
    }

}

static int check_h264NalType(char *pQueueBuf,int size){

    int nal_type = pQueueBuf[4] & 0x1F;

    if(nal_type == 5){

        return 1;// I frame
    }        
    else if(nal_type == 8){
        
        return 2;// PPS
    }
    else if(nal_type == 7){
        return 3; // SPS
    }
    else{
        return 4;//other
    }


    return -1;

}
static void video_test(void){

    int queue_size = 500*1024;
    unsigned int phy_addr= 0;
    unsigned int queue_buf =0;
//    NVTSTREAMRECEIVER_VIDEO_INFO videoInfo={0};
    if(nvtget_buf("linux_pool",&queue_size,&phy_addr) < 0){

        printf("get buf error\r\n");
        return;
    }
    
    nvtbuf_mmap(phy_addr, queue_size);

    queue_buf = NvtIPC_GetNonCacheAddr(phy_addr);
    printf("queue_buf:%x size:%d\r\n",queue_buf,queue_size);
    if(queue_buf == 0){

        printf("queue buf error\r\n");
        nvtbuf_munmap();
        return;
    }
    
#if 1
    char Path[128]={0};
    FILE *fp = NULL;
    printf("Enter Video file path:\n");
    scanf(" %s",Path);
    printf("\r\npath:%s\r\n",Path);
    printf("fp:%x\r\n",fp);
    fp = fopen(Path, "rb");
    if(fp == NULL){

        printf("open file %s error\r\n",Path);
        return;
    }
#if 0
    printf("Enter codec Type:\r\n");
    printf(" 0) h264\r\n");
    printf(" 1) h265\r\n");
    scanf(" %u",&videoInfo.CodecType);
    if(videoInfo.CodecType >1){

        printf("codec type %d error\r\n",videoInfo.CodecType);
        return;
    }

    printf("Enter Width:\r\n");
    scanf(" %u",&videoInfo.Width);
    printf("Enter Height:\r\n");
    scanf(" %u",&videoInfo.Height);

    printf("Enter FPS:\r\n");
    scanf(" %u",&videoInfo.FrameRate);

    int videohDev = -1;
    int ret =0;
    videohDev =  NvtStreamReceiver_Open();
    if(videohDev < 0){
        printf("NvtStreamReceiver_Open %d error\r\n",videohDev);
        return;
    }


    ret = NvtStreamReceiver_SetVideoInfo(videohDev, videoInfo);
    if(ret < 0){
        printf("NvtStreamReceiver_SetVideoInfo error ret:%d\r\n",ret);
        return;
    }
#endif
    static int i=0;
    int count=10;
    int size=0;
    int nal_type=-1;
    char *pQueueBuf = (char *)queue_buf;
    char SPSBuf[128]={0};
    int SPSSize = 0;
    char PPSBuf[128]={0};
    int PPSSize = 0;
    int DelayTime = 0;
    int code_type=0;//charlie test
    //DelayTime = 1000000/videoInfo.FrameRate;
    while(i< count){
        parse_frame(fp, pQueueBuf, &size);


        if(code_type == 0){
            //h264
            nal_type = check_h264NalType(pQueueBuf, size);
            if(nal_type < 0){
                printf("get nal type %d error\r\n",nal_type);
                return;
            }
            else if(nal_type == 3){//SPS
                memcpy(SPSBuf,pQueueBuf,size);
                SPSSize = size;
            }
            else if(nal_type == 2){//PPS
                memcpy(PPSBuf,pQueueBuf,size);
                PPSSize = size;
            }
            else if(nal_type == 1){//I
                
                char *TmpBuf  = (char *)malloc( size );
                memcpy(TmpBuf,pQueueBuf,size);
                
                ///I frame need inlcude SPS and PPS
                memcpy(pQueueBuf, SPSBuf, SPSSize);
                memcpy(pQueueBuf + SPSSize, PPSBuf, PPSSize);
                memcpy(pQueueBuf + SPSSize + PPSSize, TmpBuf, size);
                //NvtStreamReceiver_PutVStream(videohDev,pQueueBuf,size);
                usleep(DelayTime);
            }
            else if(nal_type == 4){
               // NvtStreamReceiver_PutVStream(videohDev,pQueueBuf,size);
                usleep(DelayTime);
            }
            else{
                printf("get code type %d error\r\n",code_type);
                return;
            }


        }
        else if(code_type == 1){
            //h265

        }
        else{
            printf("code_type %lu error\r\n", code_type);
            return;
        }
        


#if 0
        char save_name[128]={0};
        FILE *fp_write = NULL;
        memset(save_name,0,sizeof(save_name));
        sprintf(save_name,"frame%d",i);
        fp_write = fopen(save_name,"ab");
        if(fp_write== NULL){
            printf("open file %s error\r\n",Path);
            return;
        }

        fwrite( pQueueBuf,1,size,fp_write);
        fclose(fp_write);
#endif
        i++;
        printf("i %d size %d : %x %x %x %x %x %x\r\n",
                  i,size,*pQueueBuf,*(pQueueBuf+1),*(pQueueBuf+2),*(pQueueBuf+3),*(pQueueBuf+4),*(pQueueBuf+5));
    }
    fclose(fp);
#endif
    return;
}


int main(int argc, char *argv[]){

    

    printf("\r\n");
    printf("====This test for IPCAMPB mode & only for NOVA encode file=======\r\n");
    printf("test Audio out or Video out\r\n");
    printf("Please select Option:\r\n");
    printf("0) Video test\r\n");

    int testItem=0;

    scanf(" %d",&testItem);


    switch(testItem){

        case 0:


            video_test();
            break;
        default:
            printf("Option %d not support now!!\r\n",testItem);
            break;

    }

    
    return 0;
}
