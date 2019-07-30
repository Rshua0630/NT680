#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "nvtevent.h"

static char msg[128] = {0}; // temp buf for nvtevent copy.

int main(int argc, char *argv[])
{
	int er;
	int cnt = 0;
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
	channel.event1 = 1;
	channel.event3 = 1;
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
			printf("c13:event0:  byte[0]=0x%02X\n",msg[0]);
		}
		if (notify.package.channel.event1) {
			printf("c13:event1:  byte[0]=0x%02X\n",msg[0]);
		}
		if (notify.package.channel.event2) {
			printf("c13:event2:  byte[0]=0x%02X\n",msg[0]);
		}
		if (notify.package.channel.event3) {
			printf("c13:event3:  byte[0]=0x%02X\n",msg[0]);
		}
		cnt++;
		if (cnt == 10) {
			//change subscribe to event 0,2
			channel.event0 = 1;
			channel.event1 = 0;
			channel.event2 = 1;
			channel.event3 = 0;
			er = nvtevent_subscribe(hdl, &channel);
			if (er < 0) {
				return -1;
			}
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
