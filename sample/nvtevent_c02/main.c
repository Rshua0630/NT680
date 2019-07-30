#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "nvtevent.h"

static char msg[EVENT_BUF_SIZE] = {0}; // temp buf for nvtevent copy.

int main(int argc, char *argv[])
{
    int er;
    int i;
    NVTEVENT_HANDLE hdl = NULL;
    // initiate for create message queue internal.
    er = nvtevent_initiate(&hdl);
    if (er < 0) {
        return er;
    }
    // subscribe the channels you want.
    NVTEVENT_CHANNEL channel;
    NVTEVENT_NOTIFY notify;
    memset(&channel, 0, sizeof(channel));
    channel.event0 = 1;
    er = nvtevent_subscribe(hdl, &channel);
    if (er < 0) {
        return -1;
    }

    // loop to wait message delivery
    memset(&notify, 0, sizeof(notify));
	notify.p_tmp = msg;
	notify.tmp_size = sizeof(msg);
	notify.timeout_ms = -1; // -1 for timeout infinite
    while (1) {
        if ((er=nvtevent_wait(hdl, &notify)) < 0) {
            break;
        }
		if (er == ENOMSG) {
			printf("timeout, no msg got.\n");
			continue;
		}
        if (notify.package.channel.event0) {
            /*
            //FD event
            IPCAM_FD_INFO *fd_info = (IPCAM_FD_INFO *)msg;
            printf("face num=%lu\n",fd_info->uiFaceNumber);
            for(i=0;i< (int)fd_info->uiFaceNumber;i++){
                printf("x:%lu\ty:%lu\tw:%lu\th:%lu\r\n",
                             fd_info->info[i].x,
                             fd_info->info[i].y,
                             fd_info->info[i].w,
                             fd_info->info[i].h
                );

            }
            */
        }
        if (notify.package.channel.event1) {
            printf("c02:event1: %s\n",msg);
        }
        if (notify.package.channel.event2) {
            printf("c02:event2: %s\n",msg);
        }
        if (notify.package.channel.event3) {
            printf("c02:event3: %s\n",msg);
        }
    }
    // unsubscribe for quit
    er = nvtevent_unsubscribe(hdl);
    if (er < 0) {
        return er;
    }
    // release message queue
    er = nvtevent_exit(hdl);
    if (er < 0) {
        return er;
    }

    return 0;
}
