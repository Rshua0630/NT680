#include <stdio.h>
#include <string.h>
#include "nvtmemhotplug.h"
#include "nvtuctrl.h"

int main(int argc, char *argv[]){

    int ret=0;
    printf("ready hot plug memory from uITRON\r\n");

    ///stop ipcam function and release memory
#if 1
    int hdl;
    char cmd[128]={0};   
    if ((hdl = NvtUctrl_Open()) >= 0){
     
        memset(cmd, 0, sizeof(cmd));
        sprintf(cmd,"umode -c main");
        ret = NvtUctrl_CmdSnd(hdl, cmd, NULL, 0);
        NvtUctrl_Close(hdl);     
        
        if (ret != ER_SUCCESS){
            printf("change main mode fail!\r\n");
            return ret;
        }


    }

#endif
    ////start memory hot plug
    ret = MemHotPlug();
    if(ret != 0){
        printf("mem hot plug fail!!\r\n");
    }
    printf("finish hot plug memory from uITRON\r\n");
    return 0;

}
