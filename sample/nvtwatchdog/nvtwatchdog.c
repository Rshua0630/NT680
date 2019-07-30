#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>
#include <nvtuctrl.h>

//#define DEBUG
#define PRINT_OUTFILE stderr
#define DBG_WRN(fmtstr, args...)  fprintf(PRINT_OUTFILE, "\033[33m%s(): \033[0m" fmtstr,__func__, ##args)

#define DBG_ERR(fmtstr, args...)  fprintf(PRINT_OUTFILE, "\033[31m%s(): \033[0m" fmtstr,__func__, ##args)
#ifdef DEBUG
#define DBG_IND(fmtstr, args...)  fprintf(PRINT_OUTFILE, "%s(): " fmtstr, __func__, ##args)
#else
#define DBG_IND(fmtstr, args...)
#endif
#define DBG_DUMP(fmtstr, args...) fprintf(PRINT_OUTFILE,fmtstr, ##args)

#define WATCHDOG_DEVICE "/dev/watchdog"

int cpu1_check_alive(void)
{
    int  hdl, ret=0;
    char retString[128];

    if ((hdl = NvtUctrl_Open()) >= 0)
    {
        if (NvtUctrl_CmdSnd(hdl, "usys -core2fwupflag", retString, sizeof(retString)) == NVTUCTRL_ER_OK)
            ret = 1;
        NvtUctrl_Close(hdl);
    }
    else
        DBG_ERR("NvtUctrl_Open fail!\n");

    if (ret != 1)
        DBG_ERR("cpu1 no response!\n");

    return ret;
}

int process_check_alive(void)
{
    return 1;
}

int main(int argc, char *argv[])
{
	int fd=-1, cmdOpt = 0;;
	int ret = 0, timeout, sleep_time, retry=10;
    int daemon_flag=0;

    while(1) {
        cmdOpt = getopt(argc, argv, "d");

        /* End condition */
        if (cmdOpt == -1) {
            break;
        }

        switch (cmdOpt) {
            case 'd':
                daemon_flag = 1;
                break;
            default:
                break;
        }
    }

    if (daemon_flag == 1)
    {
        DBG_DUMP("%s run as daemon...\n", argv[0]);
        daemon(0, 1);
    }

    // Use retry to wait watchdog driver ready
    while (retry > 0)
    {
    	if ((fd = open(WATCHDOG_DEVICE, O_WRONLY)) >= 0)
            break;

        sleep(1);
        retry--;
    }

    if (fd < 0)
    {
        perror("Watchdog");
  		exit(EXIT_FAILURE);
    }

    if (ioctl(fd, WDIOC_GETTIMEOUT, &timeout) < 0)
		timeout = 20;
    sleep_time = timeout / 4;
    DBG_IND("Watchdog timeout = %d sec, sleep time = %d sec\n", timeout, sleep_time);

	while (1)
    {
        if (cpu1_check_alive() == 1 && process_check_alive() == 1)
        {
            DBG_IND("Watchdog tick...\n");
    		ret = write(fd, "\0", 1);
    		if (ret != 1)
            {
                DBG_ERR("Watchdog tick fail!\n");
    			ret = -1;
    			break;
    		}
        }

		sleep(sleep_time);
	}
	close(fd);
	return ret;
}
