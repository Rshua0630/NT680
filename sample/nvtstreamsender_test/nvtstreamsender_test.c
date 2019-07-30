#include <protected/nvtstreamsender_protected.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

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

#define TIMEOUT_MS 10

#define USE_LOCK_API 0

static int g_delay_range_ms = 55;
static int g_max_count = 100;

void print_data(unsigned int addr, unsigned int size)
{
	unsigned int i;
	char *p_byte = (char *)addr;

	for (i = 0; i < size; i++) {
		DBG_DUMP("[%d]=0x%X\r\n", i, p_byte[i]);
	}
}

int check_vinto(unsigned int ChanId, VIDEO_INFO_S *p_vinfo)
{
	char *pSPS = (char *)p_vinfo->Addr;

	if (*(pSPS) != 0 ||
		*(pSPS+1) != 0 ||
		*(pSPS+2) != 0 ||
		*(pSPS+3) != 1) {
		DBG_ERR("CodecType %d, start code error: %x %x %x %x, time:%d.%d\r\n",
			p_vinfo->CodecType,
			*pSPS ,*(pSPS+1),*(pSPS+2),*(pSPS+3));
		return -1;
	}

	return 0;
}

void print_vinfo(unsigned int ChanId, VIDEO_INFO_S *p_vinfo)
{
	char *p_byte = (char *)p_vinfo->Addr;
	int i;

	DBG_DUMP("CodecType %d, Size %d, SPS: ", p_vinfo->CodecType, p_vinfo->Size);

	for (i = 0; i < (int)p_vinfo->Size; i++) {
		DBG_DUMP("%02X ", p_byte[i]);
	}
	DBG_DUMP("\r\n");
}

void video_test(int hDev, unsigned int ChanId)
{
	unsigned long long old_sn = 0;
	VSTREAM_S VStream;
	VIDEO_INFO_S Video_Info;
	pid_t mypid;
	int ret;
	int count = 0;

	mypid = getpid();

	while (count++ < g_max_count) {
		if (g_delay_range_ms) {
			int delay_ms;
			delay_ms = rand()%g_delay_range_ms;
			usleep(delay_ms*1000);
			DBG_DUMP("delay_ms %d\r\n", delay_ms);
		}
L_GET_VSTRM_AGAIN:
#if USE_LOCK_API
		ret = NvtStreamSender_GetnLockVStrm(hDev, ChanId, TIMEOUT_MS, &VStream);
#else
		ret = NvtStreamSender_GetVStream(hDev, ChanId, TIMEOUT_MS, &VStream);
#endif
		if (NVTSTREAMSENDER_RET_ETIME == ret ||
			NVTSTREAMSENDER_RET_EAGAIN == ret) {
			goto L_GET_VSTRM_AGAIN;
		} else if (NVTSTREAMSENDER_RET_EINTR == ret) {
			DBG_WRN("terminated by users\r\n");
			return;
		} else if (ret < 0) {
			DBG_ERR("Get VStrm failed, ret %d\r\n", ret);
			return;
		}

		if (old_sn != 0 && !VStream.is_new && VStream.ItemSN != (old_sn + 1)) {
			DBG_ERR("old_sn %llu, cur_sn %llu\r\n", old_sn, VStream.ItemSN);
			return;
		} else {
			old_sn = VStream.ItemSN;
		}

		//print vstream
		DBG_DUMP("V%d(%d): ItemSN %lld, MaxSN %lld, Addr=0x%X, Size=%d, Type=%d\r\n",
				 ChanId, mypid, VStream.ItemSN, VStream.MaxSN, VStream.Addr, VStream.Size, VStream.FrameType);

		ret = NvtStreamSender_GetVideoInfo(hDev, ChanId, TIMEOUT_MS, &Video_Info);
		if (NVTSTREAMSENDER_RET_EINTR == ret) {
			DBG_WRN("terminated by users\r\n");
			return;
		} else if (ret < 0) {
			DBG_ERR("Get VideoInfo failed, ret %d\r\n", ret);
			return;
		}


		//print_vinfo(ChanId, &Video_Info);

		if (0 != check_vinto(ChanId, &Video_Info)) {
			DBG_ERR("check_vinfo %d failed\r\n", ChanId);
			return;
		}

#if USE_LOCK_API
		if (NVTSTREAMSENDER_RET_OK != NvtStreamSender_ReleaseVStrm(hDev, ChanId)) {
			DBG_ERR("ReleaseVStrm failed\r\n");
			return;
		}
#endif
	}
}

void audio_test(int hDev, unsigned int ChanId)
{
	unsigned long long old_sn = 0;
	ASTREAM_S AStream;
	AUDIO_INFO_S Audio_Info;
	pid_t mypid;
	int ret;
	int count = 0;

	mypid = getpid();

	while (count++ < g_max_count) {
		if (g_delay_range_ms) {
			int delay_ms;
			delay_ms = rand()%g_delay_range_ms;
			usleep(delay_ms*1000);
			DBG_DUMP("delay_ms %d\r\n", delay_ms);
		}
L_GET_ASTRM_AGAIN:
#if USE_LOCK_API
		ret = NvtStreamSender_GetnLockAStrm(hDev, ChanId, TIMEOUT_MS, &AStream);
#else
		ret = NvtStreamSender_GetAStream(hDev, ChanId, TIMEOUT_MS, &AStream);
#endif
		if (NVTSTREAMSENDER_RET_ETIME == ret ||
			NVTSTREAMSENDER_RET_EAGAIN == ret) {
			goto L_GET_ASTRM_AGAIN;
		} else if (NVTSTREAMSENDER_RET_EINTR == ret) {
			DBG_WRN("terminated by users\r\n");
			return;
		} else if (ret < 0) {
			DBG_ERR("Get AStrm failed, ret %d\r\n", ret);
			return;
		}

		if (old_sn != 0 && !AStream.is_new && AStream.ItemSN != (old_sn + 1)) {
			DBG_ERR("old_sn %llu, cur_sn %llu\r\n", old_sn, AStream.ItemSN);
			return;
		} else {
			old_sn = AStream.ItemSN;
		}

		DBG_DUMP("A%d(%d): ItemSN %lld, MaxSN %lld, Addr=0x%X, Size=%d\r\n",
				 ChanId, mypid, AStream.ItemSN, AStream.MaxSN, AStream.Addr, AStream.Size);

		ret = NvtStreamSender_GetAudioInfo(hDev, ChanId, TIMEOUT_MS, &Audio_Info);
		if (NVTSTREAMSENDER_RET_EINTR == ret) {
			DBG_WRN("terminated by users\r\n");
			return;
		} else if (ret < 0) {
			DBG_ERR("Get AudioInfo failed, ret %d\r\n", ret);
			return;
		}

		DBG_DUMP("CodecType=%d, ChannelCnt=%d, BitsPerSample=%d, SampePerSecond=%d\r\n",
				 Audio_Info.CodecType, Audio_Info.ChannelCnt, Audio_Info.BitsPerSample, Audio_Info.SampePerSecond);

#if USE_LOCK_API
		if (NVTSTREAMSENDER_RET_OK != NvtStreamSender_ReleaseAStrm(hDev, ChanId)) {
			DBG_ERR("ReleaseAStrm failed\r\n");
			return;
		}
#endif

	}
}

int main(int argc, char *argv[])
{
	int hDev;

	unsigned int bIsAudio;
	unsigned int ChanId;

	if (argc < 3) {
		DBG_ERR("argc < 3\r\n");
		return -1;
	}
	bIsAudio = atoi(argv[1]);
	ChanId = atoi(argv[2]);

	if (argc > 3) {
		g_max_count = atoi(argv[3]);
	}

	if (argc > 4) {
		g_delay_range_ms = atoi(argv[4]);
	}

	DBG_DUMP("USE_LOCK_API %d\r\n", USE_LOCK_API);
	DBG_DUMP("bIsAudio %d, ChanId %d, max_count %d, delay_range_ms %d\r\n", bIsAudio, ChanId, g_max_count, g_delay_range_ms);

	hDev = NvtStreamSender_Open();
	if (-1 == hDev) {
		DBG_ERR("Open\r\n");
		return -1;
	}

	if (bIsAudio) {
		audio_test(hDev, ChanId);
	} else {
		video_test(hDev, ChanId);
	}

	if (NVTSTREAMSENDER_RET_OK != NvtStreamSender_Close(hDev)) {
		DBG_ERR("Close\r\n");
		return -1;
	}

	return 0;
}
