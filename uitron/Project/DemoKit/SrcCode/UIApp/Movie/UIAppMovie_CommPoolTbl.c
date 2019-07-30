#include "SysCfg.h" //for memory POOL_ID
#include "nvtmpp.h"

#define TEST_510_64MB       DISABLE


#if (SENSOR_CAPS_COUNT == 1)

#if (defined(_CHIP_96683_) && (defined(_MODEL_WiFiSDV_) || defined(_MODEL_FIREFLY_)|| defined(_MODEL_B5_) || defined(_MODEL_CarDV_)) )
NVT_VB_CPOOL_S g_movie_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 3840 * 2160 * 3 / 2 + 1024, 3, NVTMPP_DDR_1 }, // record
	{ 1920 * 1080 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 }, // display
	{  848 *  480 * 3 / 2 + 1024, 2, NVTMPP_DDR_1 }, // wifi
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};

NVT_VB_CPOOL_S g_movie_hdmi4k_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 3840 * 2160 * 3 / 2 + 1024, 3, NVTMPP_DDR_1 }, // record
	{  640 *  528 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 }, // display
	{  848 *  480 * 3 / 2 + 1024, 2, NVTMPP_DDR_1 }, // wifi
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};

#elif defined(_MODEL_DVCAM2_EVB_)
NVT_VB_CPOOL_S g_movie_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 3840 * 2160 * 3 / 2 + 1024, 3, NVTMPP_DDR_1 }, // record
	{  960 *  240 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 }, // display
	{  848 *  480 * 3 / 2 + 1024, 2, NVTMPP_DDR_1 }, // wifi
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};

NVT_VB_CPOOL_S g_movie_hdmi4k_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 3840 * 2160 * 3 / 2 + 1024, 3, NVTMPP_DDR_1 }, // record
	{  960 *  240 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 }, // display
	{  848 *  480 * 3 / 2 + 1024, 2, NVTMPP_DDR_1 }, // wifi
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};


#else
NVT_VB_CPOOL_S g_movie_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 3840 * 2160 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 },
	{ 1920 * 1080 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 },
	{ 1280 *  720 * 3 / 2 + 1024, 2, NVTMPP_DDR_1 },
	{  640 *  480 * 3 / 2 + 1024, 2, NVTMPP_DDR_1 },
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};

NVT_VB_CPOOL_S g_movie_hdmi4k_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 3840 * 2160 * 3 / 2 + 1024, 5, NVTMPP_DDR_1 },
	{ 1920 * 1080 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 },
	{ 1280 *  720 * 3 / 2 + 1024, 2, NVTMPP_DDR_1 },
	{  640 *  480 * 3 / 2 + 1024, 2, NVTMPP_DDR_1 },
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};
#endif

#if (TEST_510_64MB == ENABLE)
NVT_VB_CPOOL_S g_movie_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 1920 * 1080 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 },
	{  720 *  576 * 3 / 2 + 1024, 2, NVTMPP_DDR_1 },
	{  640 *  480 * 3 / 2 + 1024, 3, NVTMPP_DDR_1 },
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};

NVT_VB_CPOOL_S g_movie_hdmi4k_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 1920 * 1080 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 },
	{  720 *  576 * 3 / 2 + 1024, 2, NVTMPP_DDR_1 },
	{  640 *  480 * 3 / 2 + 1024, 3, NVTMPP_DDR_1 },
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};

#endif

#endif

#if (SENSOR_CAPS_COUNT == 2)
NVT_VB_CPOOL_S g_movie_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 3840 * 2160 * 3 / 2 + 1024, 3, NVTMPP_DDR_1 },
	{ 1920 * 1080 * 3 / 2 + 1024, 3, NVTMPP_DDR_1 },
	{ 848 *   480 * 3 / 2 + 1024, 12, NVTMPP_DDR_1 },
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};

NVT_VB_CPOOL_S g_movie_hdmi4k_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 3840 * 2160 * 3 / 2 + 1024, 10, NVTMPP_DDR_1 },
	{  960 *  240 * 3 / 2 + 1024, 12, NVTMPP_DDR_1 },
	{ 1080 *  720 * 3 / 2 + 1024, 12, NVTMPP_DDR_1 },
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};

#endif
#if (SENSOR_CAPS_COUNT == 4)
NVT_VB_CPOOL_S g_movie_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 1920 * 1080 * 3 / 2 + 1024, 10, NVTMPP_DDR_1 },
	{  960 *  240 * 3 / 2 + 1024, 10, NVTMPP_DDR_1 },
	{  864 *  480 * 3 / 2 + 1024, 10, NVTMPP_DDR_1 },
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};

NVT_VB_CPOOL_S g_movie_hdmi4k_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 1920 * 1080 * 3 / 2 + 1024, 16, NVTMPP_DDR_1 },
	{  864 *  480 * 3 / 2 + 1024, 10, NVTMPP_DDR_1 },
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};

#endif



