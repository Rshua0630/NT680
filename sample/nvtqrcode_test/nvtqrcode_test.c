#include <sys/types.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zbar.h>
#include <nvtuctrl.h>

//#define DEBUG

#define DBG_WRN(fmtstr, args...) printf("\033[33mWRN:%s(): \033[0m" fmtstr,__func__, ##args)
#define DBG_ERR(fmtstr, args...) printf("\033[31mERR:%s(): \033[0m" fmtstr,__func__, ##args)

#ifdef DEBUG
#define DBG_IND(fmtstr, args...) printf(fmtstr, ##args)
#else
#define DBG_IND(fmtstr, args...)
#endif

#define DBG_DUMP(fmtstr, args...) printf(fmtstr, ##args)


zbar_image_scanner_t *scanner = NULL;


static int get_data(const char *fileName,
                    const unsigned int readSize,
                    void *buf)
{
    FILE *file = NULL;

	DBG_IND("%s %d\r\n",fileName,readSize);

	if(!buf){
        DBG_ERR("no buf %x \n", buf);
		return -1;
	}

    if ((file = fopen(fileName, "rb")) == NULL) {
        DBG_ERR("%s open fail\n", fileName);
		return -1;
	}

	if(readSize != fread(buf,1,readSize,file))
    {
        DBG_ERR("fread() failed\r\n");
        return -1;
    }

	fclose(file);

	return 0;
}

static void show_help(void)
{
    DBG_DUMP("usage:\n"
             "  zbar_test [filename] [width] [height]\n");
}

#if 0  // test uctrl.
static void test_uctrlSnd(void)
{
    int  hdl;
    char cmd[100];
    char retString[50];
    char retBufSize=50;

    hdl = NvtUctrl_Open();
    // change to movie mode
    strcpy(cmd,"umode -c movie");
    NvtUctrl_CmdSnd(hdl, cmd, retString, retBufSize);
    printf("retString = %s\r\n",retString);    
    NvtUctrl_Close(hdl);
}

static void test_uctrlRecv(void)
{
    char strBuf[512];
    int hdl= NvtUctrl_Open();
    while(1)
    {
        NvtUctrl_CmdRcv(hdl,strBuf,512);
        printf("%s\r\n",strBuf);
    }
    NvtUctrl_Close(hdl);
    return 0;
}
#endif

int main (int argc, char **argv)
{
    int  hdl;
    char cmd[100];
    char retString[50], szQrCode[3 * 128] = { 0 };
    char retBufSize=50;

    int width = 0, height = 0;
    void *raw = NULL;

    DBG_DUMP("qrcode decode test!\r\n");

    //test_uctrlSnd();
    //test_uctrlRecv();
    
    if (argc < 4)
    {
        show_help();
        return 0;
    }

    width = atoi(argv[2]);
    height = atoi(argv[3]);

	DBG_IND("%s %d %d\r\n",argv[1],width,height);

    /* create a reader */
    scanner = zbar_image_scanner_create();

    /* configure the reader */
    zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);

	raw = malloc(width*height);
    /* obtain image data */
    if(get_data(argv[1],width*height, raw)!=0) {
    	free(raw);
        goto clean_up2;
    }

    /* wrap image data */
    zbar_image_t *image = zbar_image_create();
    zbar_image_set_format(image, *(int*)"Y800");
    zbar_image_set_size(image, width, height);
    /* data would be free in "zbar_image_free_data" */
    zbar_image_set_data(image, raw, width * height, zbar_image_free_data);

    /* scan the image for barcodes */
    int n = zbar_scan_image(scanner, image);

	if(!n) {
		DBG_DUMP("no symbol\r\n");
        goto clean_up1;
	}

    /* extract results */
    const zbar_symbol_t *symbol = zbar_image_first_symbol(image);
    for(; symbol; symbol = zbar_symbol_next(symbol)) {
        /* do something useful with results */
        zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
        const char *data = zbar_symbol_get_data(symbol);
        DBG_DUMP("decoded %s symbol \"%s\"\n",
               zbar_get_symbol_name(typ), data);
               
        strncpy(szQrCode, data, sizeof(szQrCode));
    }
    
    hdl = NvtUctrl_Open();
    // set weight of auto exposure
    strcpy(cmd,"umovie -livestreamqrcode");
    NvtUctrl_CmdSetConfig(hdl, cmd, (void *)szQrCode, sizeof(szQrCode), retString, retBufSize);
    printf("retString = %s\r\n",retString);    
    NvtUctrl_Close(hdl);

clean_up1:
    /* clean up */
    zbar_image_destroy(image);
clean_up2:
    zbar_image_scanner_destroy(scanner);
    return(0);
}
