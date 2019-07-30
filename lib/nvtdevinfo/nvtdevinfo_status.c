/* =================================================================
 *
 *      nvtdevinfo_status.c
 *
 *      Device config and inforation library for Novatek NT9666x
 *
 * =================================================================
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include "nvtdevinfo.h"
#include "nvtdevinfo_status.h"


