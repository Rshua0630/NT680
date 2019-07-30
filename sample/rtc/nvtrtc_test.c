#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>
#include <fcntl.h>

static void usage(void)
{
	printf(
		"\n"
		"Usage: nvtrtc_test [Option] [Parameter]\n"
		"Option:\n"
		"	-g	show hardware clock time\n"
		"	-l	show local time\n"
		"	-u	show GMT time\n"
		"	-s	set hardware clock time (2015-11-4 11:00:00)\n"
		"	-h	show help\n\n"
		);
}

static int open_rtc(char *devname)
{
	int fd = 0;

	fd = open(devname, O_RDWR);
	if (fd == -1) {
		perror("Requested device cannot be opened!");
		_exit(errno);
	}

	return fd;
}

static void close_rtc(int fd)
{
	close(fd);
}

static int get_gmtime(void)
{
	time_t rawtime;
	struct tm * ptm;

	time(&rawtime);
	ptm = gmtime(&rawtime);

	printf("\nGMT time: %d:%d:%d %d-%d-%d\n",
					ptm->tm_hour,
					ptm->tm_min,
					ptm->tm_sec,
					ptm->tm_year,
					(ptm->tm_mon + 1),
					ptm->tm_mday
					);
	return 0;
}

static int get_localtime(void)
{
	time_t rawtime;
	struct tm * ptm;

	time(&rawtime);
	ptm = localtime(&rawtime);

	printf("\nLocal time: %d:%d:%d %d-%d-%d\n",
					ptm->tm_hour,
					ptm->tm_min,
					ptm->tm_sec,
					ptm->tm_year,
					(ptm->tm_mon + 1),
					ptm->tm_mday
					);
	return 0;
}

static int get_time_from_rtc(void)
{
	int fd = 0, ret = 0;
	struct rtc_time rtc_tm;

	fd = open_rtc("/dev/rtc0");
	ret = ioctl(fd, RTC_RD_TIME, &rtc_tm);
	if (ret == -1) {
		printf("\nFailed getting RTC Date/Time!");
		perror("ioctl");
		_exit(errno);
	}
	printf("\nRTC time: %d:%d:%d %d-%d-%d\n",
					rtc_tm.tm_hour,
					rtc_tm.tm_min,
					rtc_tm.tm_sec,
					rtc_tm.tm_year,
					rtc_tm.tm_mon,
					rtc_tm.tm_mday
					);

	close_rtc(fd);

	return 0;
}

static int set_time_to_rtc(void)
{
	int fd = 0, ret = 0;
	struct rtc_time rtc_tm;

	/* 2015-11-4 11:35:00 */
	rtc_tm.tm_sec = 0;	/* second */
	rtc_tm.tm_min = 0;	/* minute */
	rtc_tm.tm_hour = 11;	/* hour */
	rtc_tm.tm_mday = 4;	/* day of the month */
	rtc_tm.tm_mon = 10;	/* month Jan=0, Feb=1 ... */
	rtc_tm.tm_year = 115;	/* = year - epoch */

	fd = open_rtc("/dev/rtc0");
	ret = ioctl(fd, RTC_SET_TIME, &rtc_tm);
	if (ret == -1) {
		printf("\nFailed getting RTC Date/Time!");
		perror("ioctl");
		_exit(errno);
	}

	printf("\nRTC time set with: %d:%d:%d %d-%d-%d\n",
						rtc_tm.tm_hour,
						rtc_tm.tm_min,
						rtc_tm.tm_sec,
						rtc_tm.tm_year,
						(rtc_tm.tm_mon + 1),
						rtc_tm.tm_mday
						);

	close_rtc(fd);

	return 0;
}

int main(int argc, char **argv)
{
	int cmd_opt = 0;;

	if (argc < 2) {
		fprintf(stderr, "Without options\n");
		usage();
	}

	while(1) {
		cmd_opt = getopt(argc, argv, "ghslu");

		if (cmd_opt == -1) {
			break;
		}

		switch (cmd_opt) {
			case 'g':
				fprintf(stderr, "get time from rtc\n");
				get_time_from_rtc();
				break;
			case 'u':
				fprintf(stderr, "get gmt time\n");
				get_gmtime();
				break;
			case 's':
				fprintf(stderr, "set time to rtc %s\n", optarg);
				set_time_to_rtc();
				break;
			case 'l':
				fprintf(stderr, "get local time\n");
				get_localtime();
				break;
			case 'h':
				usage();
				break;
			case '?':
				fprintf(stderr, "Illegal option:-%c\n", optopt);
				usage();
				break;
		}
	}

	return 0;
}
