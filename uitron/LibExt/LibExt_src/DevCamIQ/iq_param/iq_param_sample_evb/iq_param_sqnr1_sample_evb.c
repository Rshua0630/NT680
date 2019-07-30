#ifndef _IQ_PARAM_SQNR1_SAMPLE_EVB_C
#define _IQ_PARAM_SQNR1_SAMPLE_EVB_C

//ISO100
static UINT32 snr_ratio_1x[6] =	 {0x7f,0x1c,0x18,0x1b,0x20,0x64};// set SNR ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 tnr_ratio_1x[6] =	{0x2e,0x2e,0x28,0x26,0x23,0x26};// set tnr ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 luma_gain_1x[8] =	 {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10};// SQUARE strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 freq_1x[7] =	 {0x5a,0x55,0x50,0x4b,0xc,0x46,0x3c};// set diff ratio in diff freq, 7 elements, range = [0, 127]
static UINT32 snr_freq_1x[4] =	 {0x0,0x0,0x1,0x1};// snr ratio depend on frequency, 4 elements, range = [0, 255]
static UINT32 detail_penalty_1x[6] =	 {0xa,0xa,0x8,0xb,0xb,0xa};//  detail penalty, 6 elements, range = [0, 15]
static UINT32 sad_penalty_1x[8] =	 {0x2c,0x2c,0x14,0x28,0x38,0x38,0x3c,0x72};//  sad penalty, 8 elements, range = [0, 15]
static UINT32 periodic_sad_similar_1x[4] =	 {0x2,0x0,0x0,0x0};//  Periodic sad similar, 4 elements
static UINT32 candiate_pos_x_1x[7] =	 {5,1,0,6,2,5,1};// mv position for horizontal direction, 7 elements
static UINT32 candiate_pos_y_1x[7] = 	{5,5,2,2,2,5,5};// mv position for vertical direction, 7 elements
static UINT32 motion_tensor_valmap_1x[4] =	 {0x0,0x33,0x0,0x8};// tensor remapping, 4 elements, range = [0, 63]
static UINT32 motion_map_1x[5] =	 {0x2c,0x23,0x19,0xc,0x8};// decide number of full search patches, 5 elements, range = [0, 63]

static IME_HAL_3DNR_LUMA_PARAM sq_l_info_1x =
{
	0x8,0x2400,0x0,0x3,0x1f0,0x0,0x0,
	{//
		&detail_penalty_1x[0],&sad_penalty_1x[0],
		0x0,0x0,0x4,0x10,
		0x1,0x0,0x1,0x20,&periodic_sad_similar_1x[0],
		0x0,0x0,0x1,0x0,
		0x0,0x0,&candiate_pos_x_1x[0], &candiate_pos_y_1x[0],
	},
	{// Motion
		0x0,0xa,0x8c,0x3,
		0x4,0x30,
		&motion_tensor_valmap_1x[0],&motion_map_1x[0],
	},
	&snr_ratio_1x[0], &tnr_ratio_1x[0],&luma_gain_1x[0],&freq_1x[0],&snr_freq_1x[0],
};

static UINT32 chroma_info_luma_gain_1x[8] =	{0x10,0x10,0x10,0x10,0x20,0x30,0x38,0x40};// TNR strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 chroma_info_motion_ctrl_1x[16] = // mapped motion value of Xth motion level, 16 elements, range = [0, 255]
	{0x6b,0x5f,0x55,0x4b,0x41,0x37,0x32,0x2d,0x28,0x23,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e};
static UINT32 chroma_info_map2d_weight_1x[4] = 	{0x0,0x0,0x80,0x8};// 4 elements, puiMap2DWeights[0]: chroma threshold for still; puiMap2DWeights[1]: chroma weighting for HSNR when still; puiMap2DWeights[2]: chroma threshold for motion; puiMap2DWeights[3]: chroma curve slope
static UINT32 chroma_info_map3D_weight_1x[4] = 	{0x0,0x40,0x80,0xf8};// 4 elements, puiMap3DWeights[0]: chroma threshold for still; puiMap3DWeights[1]: chroma weighting for HSNR when still; puiMap3DWeights[2]: chroma threshold for motion; puiMap3DWeights[3]: chroma curve slope
static UINT32 chroma_info_mix_th_1x[4] =	 {0x46,0x3,0x46,0x3};// luma threshold to mix the motion detect value, and chroma threshold to mix the motion detect value, 4 elements
static UINT32 chroma_info_level_map_1x[16] = // Memc level mapping for motion noise, 16 elements
    {0x0,0x3,0x7,0xc,0x11,0x18,0x20,0x29,0x34,0x42,0x51,0x64,0x7a,0x95,0xb4,0xd9};

static IME_HAL_3DNR_CHROMA_PARAM sq_c_info_1x =
{
	0x0,0x8,0x0,
	0x20,0x60,0x8,0x8,0x8,0x0,0x0,0x8,
	&chroma_info_luma_gain_1x[0],&chroma_info_motion_ctrl_1x[0],
	&chroma_info_map2d_weight_1x[0],&chroma_info_map3D_weight_1x[0],
	&chroma_info_mix_th_1x[0],&chroma_info_level_map_1x[0],
};

static UINT32 chroma_y_info_map2d_weight_1x[4] = {0x0, 0x0, 0x80, 0x8};
static UINT32 chroma_y_info_map3D_weight_1x[4] = {0x0, 0x40, 0x80, 0xf8};
static IME_HAL_3DNR_Y_PARAM sq_c_y_info_1x =
{
    0x0,
    &chroma_y_info_map2d_weight_1x[0], &chroma_y_info_map3D_weight_1x[0],
};

//ISO200
static UINT32 snr_ratio_2x[6] =	 {0x7f,0x1c,0x18,0x1b,0x20,0x64};// set SNR ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 tnr_ratio_2x[6] =	{0x2e,0x2e,0x28,0x26,0x23,0x26};// set tnr ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 luma_gain_2x[8] =	 {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10};// SQUARE strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 freq_2x[7] =	 {0x5a,0x55,0x50,0x4b,0xc,0x46,0x3c};// set diff ratio in diff freq, 7 elements, range = [0, 127]
static UINT32 snr_freq_2x[4] =	 {0x0,0x0,0x1,0x1};// snr ratio depend on frequency, 4 elements, range = [0, 255]
static UINT32 detail_penalty_2x[6] =	 {0xa,0xa,0x8,0xb,0xb,0xa};//  detail penalty, 6 elements, range = [0, 15]
static UINT32 sad_penalty_2x[8] =	 {0x2c,0x2c,0x14,0x28,0x38,0x38,0x3c,0x72};//  sad penalty, 8 elements, range = [0, 15]
static UINT32 periodic_sad_similar_2x[4] =	 {0x2,0x0,0x0,0x0};//  Periodic sad similar, 4 elements
static UINT32 candiate_pos_x_2x[7] =	 {5,1,0,6,2,5,1};// mv position for horizontal direction, 7 elements
static UINT32 candiate_pos_y_2x[7] = 	{5,5,2,2,2,5,5};// mv position for vertical direction, 7 elements
static UINT32 motion_tensor_valmap_2x[4] =	 {0x0,0x33,0x0,0xa};// tensor remapping, 4 elements, range = [0, 63]
static UINT32 motion_map_2x[5] =	 {0x2c,0x23,0x19,0xc,0x8};// decide number of full search patches, 5 elements, range = [0, 63]

static IME_HAL_3DNR_LUMA_PARAM sq_l_info_2x =
{
	0xc,0x2400,0x0,0x3,0x1f0,0x0,0x0,
	{//
		&detail_penalty_2x[0],&sad_penalty_2x[0],
		0x0,0x0,0x4,0x10,
		0x1,0x0,0x1,0x20,&periodic_sad_similar_2x[0],
		0x0,0x0,0x1,0x0,
		0x0,0x0,&candiate_pos_x_2x[0], &candiate_pos_y_2x[0],
	},
	{// Motion
		0x0,0xa,0x8c,0x3,
		0x4,0x30,
		&motion_tensor_valmap_2x[0],&motion_map_2x[0],
	},
	&snr_ratio_2x[0], &tnr_ratio_2x[0],&luma_gain_2x[0],&freq_2x[0],&snr_freq_2x[0],
};

static UINT32 chroma_info_luma_gain_2x[8] =	{0x10,0x10,0x10,0x10,0x20,0x30,0x38,0x40};// TNR strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 chroma_info_motion_ctrl_2x[16] = // mapped motion value of Xth motion level, 16 elements, range = [0, 255]
	{0x6b,0x5f,0x55,0x4b,0x41,0x37,0x32,0x2d,0x28,0x23,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e};
static UINT32 chroma_info_map2d_weight_2x[4] = 	{0x0,0x0,0x80,0x8};// 4 elements, puiMap2DWeights[0]: chroma threshold for still; puiMap2DWeights[1]: chroma weighting for HSNR when still; puiMap2DWeights[2]: chroma threshold for motion; puiMap2DWeights[3]: chroma curve slope
static UINT32 chroma_info_map3D_weight_2x[4] = 	{0x0,0x40,0x80,0xf8};// 4 elements, puiMap3DWeights[0]: chroma threshold for still; puiMap3DWeights[1]: chroma weighting for HSNR when still; puiMap3DWeights[2]: chroma threshold for motion; puiMap3DWeights[3]: chroma curve slope
static UINT32 chroma_info_mix_th_2x[4] =	 {0x46,0x3,0x46,0x3};// luma threshold to mix the motion detect value, and chroma threshold to mix the motion detect value, 4 elements
static UINT32 chroma_info_level_map_2x[16] = // Memc level mapping for motion noise, 16 elements
    {0x0,0x3,0x7,0xc,0x11,0x18,0x20,0x29,0x34,0x42,0x51,0x64,0x7a,0x95,0xb4,0xd9};

static IME_HAL_3DNR_CHROMA_PARAM sq_c_info_2x =
{
	0x0,0xa,0x0,
	0x20,0x60,0x8,0x8,0x8,0x0,0x0,0x8,
	&chroma_info_luma_gain_2x[0],&chroma_info_motion_ctrl_2x[0],
	&chroma_info_map2d_weight_2x[0],&chroma_info_map3D_weight_2x[0],
	&chroma_info_mix_th_2x[0],&chroma_info_level_map_2x[0],
};

static UINT32 chroma_y_info_map2d_weight_2x[4] = {0x0, 0x0, 0x80, 0x8};
static UINT32 chroma_y_info_map3D_weight_2x[4] = {0x0, 0x40, 0x80, 0xf8};
static IME_HAL_3DNR_Y_PARAM sq_c_y_info_2x =
{
    0x0,
    &chroma_y_info_map2d_weight_2x[0], &chroma_y_info_map3D_weight_2x[0],
};

//ISO400
static UINT32 snr_ratio_4x[6] =	 {0x7f,0x1c,0x18,0x1b,0x20,0x64};// set SNR ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 tnr_ratio_4x[6] =	{0x2e,0x2e,0x28,0x26,0x23,0x26};// set tnr ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 luma_gain_4x[8] =	 {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10};// SQUARE strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 freq_4x[7] =	 {0x5a,0x55,0x50,0x4b,0xc,0x46,0x3c};// set diff ratio in diff freq, 7 elements, range = [0, 127]
static UINT32 snr_freq_4x[4] =	 {0x0,0x0,0x1,0x1};// snr ratio depend on frequency, 4 elements, range = [0, 255]
static UINT32 detail_penalty_4x[6] =	 {0xa,0xa,0x8,0xb,0xb,0xa};//  detail penalty, 6 elements, range = [0, 15]
static UINT32 sad_penalty_4x[8] =	 {0x2c,0x2c,0x14,0x28,0x38,0x38,0x3c,0x72};//  sad penalty, 8 elements, range = [0, 15]
static UINT32 periodic_sad_similar_4x[4] =	 {0x2,0x0,0x0,0x0};//  Periodic sad similar, 4 elements
static UINT32 candiate_pos_x_4x[7] =	 {5,1,0,6,2,5,1};// mv position for horizontal direction, 7 elements
static UINT32 candiate_pos_y_4x[7] = 	{5,5,2,2,2,5,5};// mv position for vertical direction, 7 elements
static UINT32 motion_tensor_valmap_4x[4] =	 {0x0,0x33,0x0,0xc};// tensor remapping, 4 elements, range = [0, 63]
static UINT32 motion_map_4x[5] =	 {0x2c,0x23,0x19,0xc,0x8};// decide number of full search patches, 5 elements, range = [0, 63]

static IME_HAL_3DNR_LUMA_PARAM sq_l_info_4x =
{
	0x10,0x2400,0x0,0x3,0x1f0,0x0,0x0,
	{//
		&detail_penalty_4x[0],&sad_penalty_4x[0],
		0x0,0x0,0x4,0x10,
		0x1,0x0,0x1,0x20,&periodic_sad_similar_4x[0],
		0x0,0x0,0x1,0x0,
		0x0,0x0,&candiate_pos_x_4x[0], &candiate_pos_y_4x[0],
	},
	{// Motion
		0x0,0xa,0x8c,0x3,
		0x4,0x34,
		&motion_tensor_valmap_4x[0],&motion_map_4x[0],
	},
	&snr_ratio_4x[0], &tnr_ratio_4x[0],&luma_gain_4x[0],&freq_4x[0],&snr_freq_4x[0],
};

static UINT32 chroma_info_luma_gain_4x[8] =	{0x10,0x10,0x10,0x10,0x20,0x30,0x38,0x40};// TNR strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 chroma_info_motion_ctrl_4x[16] = // mapped motion value of Xth motion level, 16 elements, range = [0, 255]
	{0x6b,0x5f,0x55,0x4b,0x41,0x37,0x32,0x2d,0x28,0x23,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e};
static UINT32 chroma_info_map2d_weight_4x[4] = 	{0x0,0x0,0x80,0x8};// 4 elements, puiMap2DWeights[0]: chroma threshold for still; puiMap2DWeights[1]: chroma weighting for HSNR when still; puiMap2DWeights[2]: chroma threshold for motion; puiMap2DWeights[3]: chroma curve slope
static UINT32 chroma_info_map3D_weight_4x[4] = 	{0x0,0x40,0x80,0xf8};// 4 elements, puiMap3DWeights[0]: chroma threshold for still; puiMap3DWeights[1]: chroma weighting for HSNR when still; puiMap3DWeights[2]: chroma threshold for motion; puiMap3DWeights[3]: chroma curve slope
static UINT32 chroma_info_mix_th_4x[4] =	 {0x41,0x3,0x41,0x3};// luma threshold to mix the motion detect value, and chroma threshold to mix the motion detect value, 4 elements
static UINT32 chroma_info_level_map_4x[16] = // Memc level mapping for motion noise, 16 elements
    {0x0,0x3,0x7,0xc,0x11,0x18,0x20,0x29,0x34,0x42,0x51,0x64,0x7a,0x95,0xb4,0xd9};

static IME_HAL_3DNR_CHROMA_PARAM sq_c_info_4x =
{
	0x0,0xc,0x0,
	0x20,0x60,0x8,0x8,0x8,0x0,0x0,0x8,
	&chroma_info_luma_gain_4x[0],&chroma_info_motion_ctrl_4x[0],
	&chroma_info_map2d_weight_4x[0],&chroma_info_map3D_weight_4x[0],
	&chroma_info_mix_th_4x[0],&chroma_info_level_map_4x[0],
};

static UINT32 chroma_y_info_map2d_weight_4x[4] = {0x0, 0x0, 0x80, 0x8};
static UINT32 chroma_y_info_map3D_weight_4x[4] = {0x0, 0x40, 0x80, 0xf8};
static IME_HAL_3DNR_Y_PARAM sq_c_y_info_4x =
{
    0x0,
    &chroma_y_info_map2d_weight_4x[0], &chroma_y_info_map3D_weight_4x[0],
};

//ISO800
static UINT32 snr_ratio_8x[6] =	 {0x7f,0x1c,0x18,0x1b,0x20,0x64};// set SNR ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 tnr_ratio_8x[6] =	{0x2e,0x2e,0x28,0x26,0x23,0x26};// set tnr ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 luma_gain_8x[8] =	 {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10};// SQUARE strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 freq_8x[7] =	 {0x5a,0x55,0x50,0x4b,0xc,0x46,0x3c};// set diff ratio in diff freq, 7 elements, range = [0, 127]
static UINT32 snr_freq_8x[4] =	 {0x0,0x0,0x1,0x1};// snr ratio depend on frequency, 4 elements, range = [0, 255]
static UINT32 detail_penalty_8x[6] =	 {0xa,0xa,0x8,0xb,0xb,0xa};//  detail penalty, 6 elements, range = [0, 15]
static UINT32 sad_penalty_8x[8] =	 {0x2c,0x2c,0x14,0x28,0x38,0x38,0x3c,0x72};//  sad penalty, 8 elements, range = [0, 15]
static UINT32 periodic_sad_similar_8x[4] =	 {0x2,0x0,0x0,0x0};//  Periodic sad similar, 4 elements
static UINT32 candiate_pos_x_8x[7] =	 {5,1,0,6,2,5,1};// mv position for horizontal direction, 7 elements
static UINT32 candiate_pos_y_8x[7] = 	{5,5,2,2,2,5,5};// mv position for vertical direction, 7 elements
static UINT32 motion_tensor_valmap_8x[4] =	 {0x0,0x33,0x0,0xf};// tensor remapping, 4 elements, range = [0, 63]
static UINT32 motion_map_8x[5] =	 {0x2c,0x23,0x19,0xc,0x8};// decide number of full search patches, 5 elements, range = [0, 63]

static IME_HAL_3DNR_LUMA_PARAM sq_l_info_8x =
{
	0x18,0x2400,0x0,0x3,0x1f0,0x0,0x0,
	{//
		&detail_penalty_8x[0],&sad_penalty_8x[0],
		0x0,0x0,0x4,0x10,
		0x1,0x0,0x1,0x20,&periodic_sad_similar_8x[0],
		0x0,0x0,0x1,0x0,
		0x0,0x0,&candiate_pos_x_8x[0], &candiate_pos_y_8x[0],
	},
	{// Motion
		0x0,0xa,0x8c,0x4,
		0x4,0x38,
		&motion_tensor_valmap_8x[0],&motion_map_8x[0],
	},
	&snr_ratio_8x[0], &tnr_ratio_8x[0],&luma_gain_8x[0],&freq_8x[0],&snr_freq_8x[0],
};

static UINT32 chroma_info_luma_gain_8x[8] =	{0x10,0x10,0x10,0x10,0x20,0x30,0x38,0x40};// TNR strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 chroma_info_motion_ctrl_8x[16] = // mapped motion value of Xth motion level, 16 elements, range = [0, 255]
	{0x6b,0x5f,0x55,0x4b,0x41,0x37,0x32,0x2d,0x28,0x23,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e};
static UINT32 chroma_info_map2d_weight_8x[4] = 	{0x0,0x0,0x80,0x8};// 4 elements, puiMap2DWeights[0]: chroma threshold for still; puiMap2DWeights[1]: chroma weighting for HSNR when still; puiMap2DWeights[2]: chroma threshold for motion; puiMap2DWeights[3]: chroma curve slope
static UINT32 chroma_info_map3D_weight_8x[4] = 	{0x0,0x40,0x80,0xf8};// 4 elements, puiMap3DWeights[0]: chroma threshold for still; puiMap3DWeights[1]: chroma weighting for HSNR when still; puiMap3DWeights[2]: chroma threshold for motion; puiMap3DWeights[3]: chroma curve slope
static UINT32 chroma_info_mix_th_8x[4] =	 {0x3c,0x3,0x3c,0x3};// luma threshold to mix the motion detect value, and chroma threshold to mix the motion detect value, 4 elements
static UINT32 chroma_info_level_map_8x[16] = // Memc level mapping for motion noise, 16 elements
    {0x0,0x3,0x7,0xc,0x11,0x18,0x20,0x29,0x34,0x42,0x51,0x64,0x7a,0x95,0xb4,0xd9};

static IME_HAL_3DNR_CHROMA_PARAM sq_c_info_8x =
{
	0x0,0x10,0x0,
	0x20,0x60,0x8,0x8,0x8,0x0,0x0,0x8,
	&chroma_info_luma_gain_8x[0],&chroma_info_motion_ctrl_8x[0],
	&chroma_info_map2d_weight_8x[0],&chroma_info_map3D_weight_8x[0],
	&chroma_info_mix_th_8x[0],&chroma_info_level_map_8x[0],
};

static UINT32 chroma_y_info_map2d_weight_8x[4] = {0x0, 0x0, 0x80, 0x8};
static UINT32 chroma_y_info_map3D_weight_8x[4] = {0x0, 0x40, 0x80, 0xf8};
static IME_HAL_3DNR_Y_PARAM sq_c_y_info_8x =
{
    0x0,
    &chroma_y_info_map2d_weight_8x[0], &chroma_y_info_map3D_weight_8x[0],
};

//1600
static UINT32 snr_ratio_16x[6] =	 {0x7f,0x1c,0x18,0x1b,0x20,0x64};// set SNR ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 tnr_ratio_16x[6] =	{0x2e,0x2e,0x28,0x26,0x23,0x26};// set tnr ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 luma_gain_16x[8] =	 {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10};// SQUARE strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 freq_16x[7] =	 {0x5a,0x55,0x50,0x4b,0xc,0x46,0x3c};// set diff ratio in diff freq, 7 elements, range = [0, 127]
static UINT32 snr_freq_16x[4] =	 {0x0,0x0,0x1,0x1};// snr ratio depend on frequency, 4 elements, range = [0, 255]
static UINT32 detail_penalty_16x[6] =	 {0xa,0xa,0x8,0xb,0xb,0xa};//  detail penalty, 6 elements, range = [0, 15]
static UINT32 sad_penalty_16x[8] =	 {0x2c,0x2c,0x14,0x28,0x38,0x38,0x3c,0x72};//  sad penalty, 8 elements, range = [0, 15]
static UINT32 periodic_sad_similar_16x[4] =	 {0x2,0x0,0x0,0x0};//  Periodic sad similar, 4 elements
static UINT32 candiate_pos_x_16x[7] =	 {5,1,0,6,2,5,1};// mv position for horizontal direction, 7 elements
static UINT32 candiate_pos_y_16x[7] = 	{5,5,2,2,2,5,5};// mv position for vertical direction, 7 elements
static UINT32 motion_tensor_valmap_16x[4] =	 {0x0,0x33,0x0,0x10};// tensor remapping, 4 elements, range = [0, 63]
static UINT32 motion_map_16x[5] =	 {0x2c,0x23,0x19,0xc,0x8};// decide number of full search patches, 5 elements, range = [0, 63]

static IME_HAL_3DNR_LUMA_PARAM sq_l_info_16x =
{
	0x20,0x2400,0x0,0x3,0x1f0,0x0,0x0,
	{//
		&detail_penalty_16x[0],&sad_penalty_16x[0],
		0x0,0x0,0x4,0x10,
		0x1,0x0,0x1,0x20,&periodic_sad_similar_16x[0],
		0x0,0x0,0x1,0x0,
		0x0,0x0,&candiate_pos_x_16x[0], &candiate_pos_y_16x[0],
	},
	{// Motion
		0x0,0xa,0x8c,0x4,
		0x4,0x3d,
		&motion_tensor_valmap_16x[0],&motion_map_16x[0],
	},
	&snr_ratio_16x[0], &tnr_ratio_16x[0],&luma_gain_16x[0],&freq_16x[0],&snr_freq_16x[0],
};

static UINT32 chroma_info_luma_gain_16x[8] =	{0x10,0x10,0x10,0x10,0x20,0x30,0x38,0x40};// TNR strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 chroma_info_motion_ctrl_16x[16] = // mapped motion value of Xth motion level, 16 elements, range = [0, 255]
	{0x6b,0x5f,0x55,0x4b,0x41,0x37,0x32,0x2d,0x28,0x23,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e};
static UINT32 chroma_info_map2d_weight_16x[4] = 	{0x0,0x0,0x80,0x8};// 4 elements, puiMap2DWeights[0]: chroma threshold for still; puiMap2DWeights[1]: chroma weighting for HSNR when still; puiMap2DWeights[2]: chroma threshold for motion; puiMap2DWeights[3]: chroma curve slope
static UINT32 chroma_info_map3D_weight_16x[4] = 	{0x0,0x40,0x80,0xf8};// 4 elements, puiMap3DWeights[0]: chroma threshold for still; puiMap3DWeights[1]: chroma weighting for HSNR when still; puiMap3DWeights[2]: chroma threshold for motion; puiMap3DWeights[3]: chroma curve slope
static UINT32 chroma_info_mix_th_16x[4] =	 {0x37,0x3,0x37,0x3};// luma threshold to mix the motion detect value, and chroma threshold to mix the motion detect value, 4 elements
static UINT32 chroma_info_level_map_16x[16] = // Memc level mapping for motion noise, 16 elements
    {0x0,0x3,0x7,0xc,0x11,0x18,0x20,0x29,0x34,0x42,0x51,0x64,0x7a,0x95,0xb4,0xd9};

static IME_HAL_3DNR_CHROMA_PARAM sq_c_info_16x =
{
	0x0,0x14,0x0,
	0x20,0x60,0x8,0x8,0x8,0x0,0x0,0x8,
	&chroma_info_luma_gain_16x[0],&chroma_info_motion_ctrl_16x[0],
	&chroma_info_map2d_weight_16x[0],&chroma_info_map3D_weight_16x[0],
	&chroma_info_mix_th_16x[0],&chroma_info_level_map_16x[0],
};

static UINT32 chroma_y_info_map2d_weight_16x[4] = {0x0, 0x0, 0x80, 0x8};
static UINT32 chroma_y_info_map3D_weight_16x[4] = {0x0, 0x40, 0x80, 0xf8};
static IME_HAL_3DNR_Y_PARAM sq_c_y_info_16x =
{
    0x0,
    &chroma_y_info_map2d_weight_16x[0], &chroma_y_info_map3D_weight_16x[0],
};

//ISO3200
static UINT32 snr_ratio_32x[6] =	 {0x7f,0x1c,0x18,0x1b,0x20,0x64};// set SNR ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 tnr_ratio_32x[6] =	{0x2e,0x2e,0x28,0x26,0x23,0x26};// set tnr ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 luma_gain_32x[8] =	 {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10};// SQUARE strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 freq_32x[7] =	 {0x5a,0x55,0x50,0x4b,0xc,0x46,0x3c};// set diff ratio in diff freq, 7 elements, range = [0, 127]
static UINT32 snr_freq_32x[4] =	 {0x0,0x0,0x1,0x1};// snr ratio depend on frequency, 4 elements, range = [0, 255]
static UINT32 detail_penalty_32x[6] =	 {0xb,0xb,0x8,0xc,0xc,0xb};//  detail penalty, 6 elements, range = [0, 15]
static UINT32 sad_penalty_32x[8] =	 {0x2c,0x2c,0x28,0x28,0x38,0x38,0x3c,0x72};//  sad penalty, 8 elements, range = [0, 15]
static UINT32 periodic_sad_similar_32x[4] =	 {0x2,0x0,0x0,0x0};//  Periodic sad similar, 4 elements
static UINT32 candiate_pos_x_32x[7] =	 {5,1,0,6,2,5,1};// mv position for horizontal direction, 7 elements
static UINT32 candiate_pos_y_32x[7] = 	{5,5,2,2,2,5,5};// mv position for vertical direction, 7 elements
static UINT32 motion_tensor_valmap_32x[4] =	 {0x0,0x33,0x0,0x12};// tensor remapping, 4 elements, range = [0, 63]
static UINT32 motion_map_32x[5] =	 {0x2c,0x23,0x19,0xc,0x8};// decide number of full search patches, 5 elements, range = [0, 63]

static IME_HAL_3DNR_LUMA_PARAM sq_l_info_32x =
{
	0x30,0x2400,0x0,0x3,0x1f0,0x0,0x0,
	{//
		&detail_penalty_32x[0],&sad_penalty_32x[0],
		0x0,0x0,0x4,0x10,
		0x1,0x0,0x1,0x20,&periodic_sad_similar_32x[0],
		0x0,0x0,0x1,0x0,
		0x0,0x0,&candiate_pos_x_32x[0], &candiate_pos_y_32x[0],
	},
	{// Motion
		0x0,0xa,0x8c,0x6,
		0x4,0x41,
		&motion_tensor_valmap_32x[0],&motion_map_32x[0],
	},
	&snr_ratio_32x[0], &tnr_ratio_32x[0],&luma_gain_32x[0],&freq_32x[0],&snr_freq_32x[0],
};

static UINT32 chroma_info_luma_gain_32x[8] =	{0x10,0x10,0x10,0x10,0x20,0x30,0x38,0x40};// TNR strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 chroma_info_motion_ctrl_32x[16] = // mapped motion value of Xth motion level, 16 elements, range = [0, 255]
	{0x6b,0x5f,0x55,0x4b,0x41,0x37,0x32,0x2d,0x28,0x23,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e};
static UINT32 chroma_info_map2d_weight_32x[4] = 	{0x0,0x0,0x80,0x8};// 4 elements, puiMap2DWeights[0]: chroma threshold for still; puiMap2DWeights[1]: chroma weighting for HSNR when still; puiMap2DWeights[2]: chroma threshold for motion; puiMap2DWeights[3]: chroma curve slope
static UINT32 chroma_info_map3D_weight_32x[4] = 	{0x0,0x40,0x80,0xf8};// 4 elements, puiMap3DWeights[0]: chroma threshold for still; puiMap3DWeights[1]: chroma weighting for HSNR when still; puiMap3DWeights[2]: chroma threshold for motion; puiMap3DWeights[3]: chroma curve slope
static UINT32 chroma_info_mix_th_32x[4] =	 {0x32,0x3,0x32,0x3};// luma threshold to mix the motion detect value, and chroma threshold to mix the motion detect value, 4 elements
static UINT32 chroma_info_level_map_32x[16] = // Memc level mapping for motion noise, 16 elements
    {0x0,0x3,0x7,0xc,0x11,0x18,0x20,0x29,0x34,0x42,0x51,0x64,0x7a,0x95,0xb4,0xd9};

static IME_HAL_3DNR_CHROMA_PARAM sq_c_info_32x =
{
	0x0,0x19,0x0,
	0x20,0x60,0x8,0x8,0x8,0x0,0x0,0x8,
	&chroma_info_luma_gain_32x[0],&chroma_info_motion_ctrl_32x[0],
	&chroma_info_map2d_weight_32x[0],&chroma_info_map3D_weight_32x[0],
	&chroma_info_mix_th_32x[0],&chroma_info_level_map_32x[0],
};

static UINT32 chroma_y_info_map2d_weight_32x[4] = {0x0, 0x0, 0x80, 0x8};
static UINT32 chroma_y_info_map3D_weight_32x[4] = {0x0, 0x40, 0x80, 0xf8};
static IME_HAL_3DNR_Y_PARAM sq_c_y_info_32x =
{
    0x0,
    &chroma_y_info_map2d_weight_32x[0], &chroma_y_info_map3D_weight_32x[0],
};

//ISO6400
static UINT32 snr_ratio_64x[6] =	 {0x7f,0x1c,0x18,0x1b,0x20,0x64};// set SNR ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 tnr_ratio_64x[6] =	{0x2e,0x2e,0x28,0x26,0x23,0x26};// set tnr ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 luma_gain_64x[8] =	 {0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10};// SQUARE strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 freq_64x[7] =	 {0x5a,0x55,0x50,0x4b,0xc,0x46,0x3c};// set diff ratio in diff freq, 7 elements, range = [0, 127]
static UINT32 snr_freq_64x[4] =	 {0x0,0x0,0x1,0x1};// snr ratio depend on frequency, 4 elements, range = [0, 255]
static UINT32 detail_penalty_64x[6] =	 {0xb,0xb,0x9,0xc,0xc,0xb};//  detail penalty, 6 elements, range = [0, 15]
static UINT32 sad_penalty_64x[8] =	 {0x2c,0x2c,0x14,0x28,0x38,0x38,0x3c,0x72};//  sad penalty, 8 elements, range = [0, 15]
static UINT32 periodic_sad_similar_64x[4] =	 {0x2,0x0,0x0,0x0};//  Periodic sad similar, 4 elements
static UINT32 candiate_pos_x_64x[7] =	 {5,1,0,6,2,5,1};// mv position for horizontal direction, 7 elements
static UINT32 candiate_pos_y_64x[7] = 	{5,5,2,2,2,5,5};// mv position for vertical direction, 7 elements
static UINT32 motion_tensor_valmap_64x[4] =	 {0x0,0x33,0x0,0x14};// tensor remapping, 4 elements, range = [0, 63]
static UINT32 motion_map_64x[5] =	 {0x2c,0x23,0x19,0xc,0x8};// decide number of full search patches, 5 elements, range = [0, 63]

static IME_HAL_3DNR_LUMA_PARAM sq_l_info_64x =
{
	0x40,0x2400,0x0,0x3,0x1f0,0x0,0x0,
	{//
		&detail_penalty_64x[0],&sad_penalty_64x[0],
		0x0,0x0,0x4,0x10,
		0x1,0x0,0x1,0x20,&periodic_sad_similar_64x[0],
		0x0,0x0,0x1,0x0,
		0x0,0x0,&candiate_pos_x_64x[0], &candiate_pos_y_64x[0],
	},
	{// Motion
		0x0,0xa,0x8c,0x6,
		0x4,0x46,
		&motion_tensor_valmap_64x[0],&motion_map_64x[0],
	},
	&snr_ratio_64x[0], &tnr_ratio_64x[0],&luma_gain_64x[0],&freq_64x[0],&snr_freq_64x[0],
};

static UINT32 chroma_info_luma_gain_64x[8] =	{0x10,0x10,0x10,0x10,0x20,0x30,0x38,0x40};// TNR strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 chroma_info_motion_ctrl_64x[16] = // mapped motion value of Xth motion level, 16 elements, range = [0, 255]
	{0x6b,0x5f,0x55,0x4b,0x41,0x37,0x32,0x2d,0x28,0x23,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e};
static UINT32 chroma_info_map2d_weight_64x[4] = 	{0x0,0x0,0x80,0x8};// 4 elements, puiMap2DWeights[0]: chroma threshold for still; puiMap2DWeights[1]: chroma weighting for HSNR when still; puiMap2DWeights[2]: chroma threshold for motion; puiMap2DWeights[3]: chroma curve slope
static UINT32 chroma_info_map3D_weight_64x[4] = 	{0x0,0x40,0x80,0xf8};// 4 elements, puiMap3DWeights[0]: chroma threshold for still; puiMap3DWeights[1]: chroma weighting for HSNR when still; puiMap3DWeights[2]: chroma threshold for motion; puiMap3DWeights[3]: chroma curve slope
static UINT32 chroma_info_mix_th_64x[4] =	 {0x32,0x3,0x32,0x3};// luma threshold to mix the motion detect value, and chroma threshold to mix the motion detect value, 4 elements
static UINT32 chroma_info_level_map_64x[16] = // Memc level mapping for motion noise, 16 elements
    {0x0,0x3,0x7,0xc,0x11,0x18,0x20,0x29,0x34,0x42,0x51,0x64,0x7a,0x95,0xb4,0xd9};

static IME_HAL_3DNR_CHROMA_PARAM sq_c_info_64x =
{
	0x0,0x1e,0x0,
	0x20,0x60,0x8,0x8,0x8,0x0,0x0,0x8,
	&chroma_info_luma_gain_64x[0],&chroma_info_motion_ctrl_64x[0],
	&chroma_info_map2d_weight_64x[0],&chroma_info_map3D_weight_64x[0],
	&chroma_info_mix_th_64x[0],&chroma_info_level_map_64x[0],
};

static UINT32 chroma_y_info_map2d_weight_64x[4] = {0x0, 0x0, 0x80, 0x8};
static UINT32 chroma_y_info_map3D_weight_64x[4] = {0x0, 0x40, 0x80, 0xf8};
static IME_HAL_3DNR_Y_PARAM sq_c_y_info_64x =
{
    0x0,
    &chroma_y_info_map2d_weight_64x[0], &chroma_y_info_map3D_weight_64x[0],
};

//ISO12800
static UINT32 snr_ratio_128x[6] =	 {0x3c, 0x10, 0xc, 0x8, 0x4, 0x0};// set SNR ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 tnr_ratio_128x[6] =	{0x3c, 0x23, 0x1e, 0x1c, 0x1d, 0x1f};// set tnr ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 luma_gain_128x[8] =	 {0x19, 0x19, 0x16, 0x14, 0x13, 0x12, 0x10, 0xf};// SQUARE strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 freq_128x[7] =	 {127, 108, 64, 51, 20, 80, 60};// set diff ratio in diff freq, 7 elements, range = [0, 127]
static UINT32 snr_freq_128x[4] =	 {0, 4, 4, 12};// snr ratio depend on frequency, 4 elements, range = [0, 255]
static UINT32 detail_penalty_128x[6] =	 {0x6,0x6,0x3,0x6,0x6,0x6};//  detail penalty, 6 elements, range = [0, 15]
static UINT32 sad_penalty_128x[8] =	 {30, 30, 40, 35, 30, 35, 80, 160};//  sad penalty, 8 elements, range = [0, 15]
static UINT32 periodic_sad_similar_128x[4] =	 {0x2,0x0,0x0,0x0};//  Periodic sad similar, 4 elements
static UINT32 candiate_pos_x_128x[7] =	 {5,1,0,6,2,5,1};// mv position for horizontal direction, 7 elements
static UINT32 candiate_pos_y_128x[7] = 	{5,5,2,2,2,5,5};// mv position for vertical direction, 7 elements
static UINT32 motion_tensor_valmap_128x[4] =	 {12, 51, 0, 15};// tensor remapping, 4 elements, range = [0, 63]
static UINT32 motion_map_128x[5] =	 {0x32,0x2c,0x23,0x16,0xc};// decide number of full search patches, 5 elements, range = [0, 63]

static IME_HAL_3DNR_LUMA_PARAM sq_l_info_128x =
{
	0x2c,0x2400,0x0,0x3,0x320,0x0,0x0,
	{//
		&detail_penalty_128x[0],&sad_penalty_128x[0],
		0x1,0x0,0x8,0x14,
		0x1,0x0,0x1,0x20,&periodic_sad_similar_128x[0],
		0x0,0x0,0x1,0x0,
		0x0,0x0,&candiate_pos_x_128x[0], &candiate_pos_y_128x[0],
	},
	{// Motion
		0x0,0xa,0x8c,0x5,
		25,48,
		&motion_tensor_valmap_128x[0],&motion_map_128x[0],
	},
	&snr_ratio_128x[0], &tnr_ratio_128x[0],&luma_gain_128x[0],&freq_128x[0],&snr_freq_128x[0],
};

static UINT32 chroma_info_luma_gain_128x[8] =	{0x10,0x10,0x10,0x10,0x20,0x30,0x38,0x40};// TNR strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 chroma_info_motion_ctrl_128x[16] = // mapped motion value of Xth motion level, 16 elements, range = [0, 255]
	{0x6b,0x55,0x40,0x2b,0x20,0x1a,0x15,0x12,0x10,0xe,0xd,0xc,0xb,0xa,0x9,0x8};
static UINT32 chroma_info_map2d_weight_128x[4] = 	{0x0,0x0,0x80,0x8};// 4 elements, puiMap2DWeights[0]: chroma threshold for still; puiMap2DWeights[1]: chroma weighting for HSNR when still; puiMap2DWeights[2]: chroma threshold for motion; puiMap2DWeights[3]: chroma curve slope
static UINT32 chroma_info_map3D_weight_128x[4] = 	{0x0,0x40,0x80,0xf8};// 4 elements, puiMap3DWeights[0]: chroma threshold for still; puiMap3DWeights[1]: chroma weighting for HSNR when still; puiMap3DWeights[2]: chroma threshold for motion; puiMap3DWeights[3]: chroma curve slope
static UINT32 chroma_info_mix_th_128x[4] =	 {0x46,0x3,0x46,0x3};// luma threshold to mix the motion detect value, and chroma threshold to mix the motion detect value, 4 elements
static UINT32 chroma_info_level_map_128x[16] = // Memc level mapping for motion noise, 16 elements
    {0x0,0x3,0x7,0xc,0x11,0x18,0x20,0x29,0x34,0x42,0x51,0x64,0x7a,0x95,0xb4,0xd9};

static IME_HAL_3DNR_CHROMA_PARAM sq_c_info_128x =
{
	0x0,0x0,0x0,
	0x20,0x60,0x8,0x8,0x8,0x0,0x0,0x8,
	&chroma_info_luma_gain_128x[0],&chroma_info_motion_ctrl_128x[0],
	&chroma_info_map2d_weight_128x[0],&chroma_info_map3D_weight_128x[0],
	&chroma_info_mix_th_128x[0],&chroma_info_level_map_128x[0],
};

static UINT32 chroma_y_info_map2d_weight_128x[4] = {0x0, 0x0, 0x80, 0x8};
static UINT32 chroma_y_info_map3D_weight_128x[4] = {0x0, 0x40, 0x80, 0xf8};
static IME_HAL_3DNR_Y_PARAM sq_c_y_info_128x =
{
    0x0,
    &chroma_y_info_map2d_weight_128x[0], &chroma_y_info_map3D_weight_128x[0],
};

//ISO25600
static UINT32 snr_ratio_256x[6] =	 {60, 12, 12, 8, 4, 0};// set SNR ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 tnr_ratio_256x[6] =	{60, 25, 25, 28, 29, 35};// set tnr ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 luma_gain_256x[8] =	 {25, 25, 22, 20, 19, 18, 16, 15};// SQUARE strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 freq_256x[7] =	 {127, 108, 64, 51, 20, 80, 60};// set diff ratio in diff freq, 7 elements, range = [0, 127]
static UINT32 snr_freq_256x[4] =	 {0, 4, 4, 12};// snr ratio depend on frequency, 4 elements, range = [0, 255]
static UINT32 detail_penalty_256x[6] =	 {0x6,0x6,0x3,0x6,0x6,0x6};//  detail penalty, 6 elements, range = [0, 15]
static UINT32 sad_penalty_256x[8] =	 {30, 30, 50, 35, 30, 35, 80, 160};//  sad penalty, 8 elements, range = [0, 15]
static UINT32 periodic_sad_similar_256x[4] =	 {0x2,0x0,0x0,0x0};//  Periodic sad similar, 4 elements
static UINT32 candiate_pos_x_256x[7] =	 {5,1,0,6,2,5,1};// mv position for horizontal direction, 7 elements
static UINT32 candiate_pos_y_256x[7] = 	{5,5,2,2,2,5,5};// mv position for vertical direction, 7 elements
static UINT32 motion_tensor_valmap_256x[4] =	 {0xc,0x33,0x0,0xf};// tensor remapping, 4 elements, range = [0, 63]
static UINT32 motion_map_256x[5] =	 {0x32,0x2c,0x23,0x16,0xc};// decide number of full search patches, 5 elements, range = [0, 63]

static IME_HAL_3DNR_LUMA_PARAM sq_l_info_256x =
{
	60,0x2400,0x0,0x3,0x320,0x0,0x0,
	{//
		&detail_penalty_256x[0],&sad_penalty_256x[0],
		0x1,0x0,0x8,0x14,
		0x1,0x0,0x1,0x20,&periodic_sad_similar_256x[0],
		0x0,0x0,0x1,0x0,
		0x0,0x0,&candiate_pos_x_256x[0], &candiate_pos_y_256x[0],
	},
	{// Motion
		0x0,0xa,0x8c,0x5,
		25, 48,
		&motion_tensor_valmap_256x[0],&motion_map_256x[0],
	},
	&snr_ratio_256x[0], &tnr_ratio_256x[0],&luma_gain_256x[0],&freq_256x[0],&snr_freq_256x[0],
};

static UINT32 chroma_info_luma_gain_256x[8] =	{0x10,0x10,0x10,0x10,0x20,0x30,0x38,0x40};// TNR strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 chroma_info_motion_ctrl_256x[16] = // mapped motion value of Xth motion level, 16 elements, range = [0, 255]
	{0x6b,0x55,0x40,0x2b,0x20,0x1a,0x15,0x12,0x10,0xe,0xd,0xc,0xb,0xa,0x9,0x8};
static UINT32 chroma_info_map2d_weight_256x[4] = 	{0x0,0x0,0x80,0x8};// 4 elements, puiMap2DWeights[0]: chroma threshold for still; puiMap2DWeights[1]: chroma weighting for HSNR when still; puiMap2DWeights[2]: chroma threshold for motion; puiMap2DWeights[3]: chroma curve slope
static UINT32 chroma_info_map3D_weight_256x[4] = 	{0x0,0x40,0x80,0xf8};// 4 elements, puiMap3DWeights[0]: chroma threshold for still; puiMap3DWeights[1]: chroma weighting for HSNR when still; puiMap3DWeights[2]: chroma threshold for motion; puiMap3DWeights[3]: chroma curve slope
static UINT32 chroma_info_mix_th_256x[4] =	 {0x46,0x3,0x46,0x3};// luma threshold to mix the motion detect value, and chroma threshold to mix the motion detect value, 4 elements
static UINT32 chroma_info_level_map_256x[16] = // Memc level mapping for motion noise, 16 elements
    {0x0,0x3,0x7,0xc,0x11,0x18,0x20,0x29,0x34,0x42,0x51,0x64,0x7a,0x95,0xb4,0xd9};

static IME_HAL_3DNR_CHROMA_PARAM sq_c_info_256x =
{
	0x0,0x0,0x0,
	0x20,0x60,0x8,0x8,0x8,0x0,0x0,0x8,
	&chroma_info_luma_gain_256x[0],&chroma_info_motion_ctrl_256x[0],
	&chroma_info_map2d_weight_256x[0],&chroma_info_map3D_weight_256x[0],
	&chroma_info_mix_th_256x[0],&chroma_info_level_map_256x[0],
};

static UINT32 chroma_y_info_map2d_weight_256x[4] = {0x0, 0x0, 0x80, 0x8};
static UINT32 chroma_y_info_map3D_weight_256x[4] = {0x0, 0x40, 0x80, 0xf8};
static IME_HAL_3DNR_Y_PARAM sq_c_y_info_256x =
{
    0x0,
    &chroma_y_info_map2d_weight_256x[0], &chroma_y_info_map3D_weight_256x[0],
};

//ISO51200
static UINT32 snr_ratio_512x[6] =	 {60, 10, 10, 7, 6, 4};// set SNR ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 tnr_ratio_512x[6] =	{60, 25, 25, 28, 29, 35};// set tnr ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 luma_gain_512x[8] =	 {24, 24, 22, 21, 20, 19, 18, 16};// SQUARE strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 freq_512x[7] =	 {0x7f,0x6c,0x40,0x33,0x14,0x40,0x3c};// set diff ratio in diff freq, 7 elements, range = [0, 127]
static UINT32 snr_freq_512x[4] =	 {3, 8, 8, 12};// snr ratio depend on frequency, 4 elements, range = [0, 255]
static UINT32 detail_penalty_512x[6] =	 {6, 6, 3, 6, 6, 6};//  detail penalty, 6 elements, range = [0, 15]
static UINT32 sad_penalty_512x[8] =	 {0x1e, 0x1e, 0x50, 0x23, 0x1e, 0x23, 0x50, 0xa0};//  sad penalty, 8 elements, range = [0, 15]
static UINT32 periodic_sad_similar_512x[4] =	 {0x2,0x0,0x0,0x0};//  Periodic sad similar, 4 elements
static UINT32 candiate_pos_x_512x[7] =	 {5,1,0,6,2,5,1};// mv position for horizontal direction, 7 elements
static UINT32 candiate_pos_y_512x[7] = 	{5,5,2,2,2,5,5};// mv position for vertical direction, 7 elements
static UINT32 motion_tensor_valmap_512x[4] =	 {8, 48, 0, 10};// tensor remapping, 4 elements, range = [0, 63]
static UINT32 motion_map_512x[5] =	 {0x32,0x2c,0x23,0x16,0xc};// decide number of full search patches, 5 elements, range = [0, 63]

static IME_HAL_3DNR_LUMA_PARAM sq_l_info_512x =
{
	0x40,0x2400,0x0,0x3,0x320,0x0,0x0,
	{//
		&detail_penalty_512x[0],&sad_penalty_512x[0],
		0x1,0x0,0x8,0x14,
		0x1,0x0,0x1,0x20,&periodic_sad_similar_512x[0],
		0x0,0x0,0x1,0x0,
		0x0,0x0,&candiate_pos_x_512x[0], &candiate_pos_y_512x[0],
	},
	{// Motion
		0x0,0xa,0x78,0x6,
		25, 20,
		&motion_tensor_valmap_512x[0],&motion_map_512x[0],
	},
	&snr_ratio_512x[0], &tnr_ratio_512x[0],&luma_gain_512x[0],&freq_512x[0],&snr_freq_512x[0],
};

static UINT32 chroma_info_luma_gain_512x[8] =	{0x10,0x10,0x10,0x10,0x20,0x30,0x38,0x40};// TNR strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 chroma_info_motion_ctrl_512x[16] = // mapped motion value of Xth motion level, 16 elements, range = [0, 255]
	{0x6b,0x55,0x40,0x2b,0x20,0x1a,0x15,0x12,0x10,0xe,0xd,0xc,0xb,0xa,0x9,0x8};
static UINT32 chroma_info_map2d_weight_512x[4] = 	{0x0,0x0,0x80,0x8};// 4 elements, puiMap2DWeights[0]: chroma threshold for still; puiMap2DWeights[1]: chroma weighting for HSNR when still; puiMap2DWeights[2]: chroma threshold for motion; puiMap2DWeights[3]: chroma curve slope
static UINT32 chroma_info_map3D_weight_512x[4] = 	{0x0,0x40,0x80,0xf8};// 4 elements, puiMap3DWeights[0]: chroma threshold for still; puiMap3DWeights[1]: chroma weighting for HSNR when still; puiMap3DWeights[2]: chroma threshold for motion; puiMap3DWeights[3]: chroma curve slope
static UINT32 chroma_info_mix_th_512x[4] =	 {0x46,0x3,0x46,0x3};// luma threshold to mix the motion detect value, and chroma threshold to mix the motion detect value, 4 elements
static UINT32 chroma_info_level_map_512x[16] = // Memc level mapping for motion noise, 16 elements
    {0x0,0x3,0x7,0xc,0x11,0x18,0x20,0x29,0x34,0x42,0x51,0x64,0x7a,0x95,0xb4,0xd9};

static IME_HAL_3DNR_CHROMA_PARAM sq_c_info_512x =
{
	0x0,0x0,0x0,
	0x20,0x60,0x8,0x8,0x8,0x0,0x0,0x8,
	&chroma_info_luma_gain_512x[0],&chroma_info_motion_ctrl_512x[0],
	&chroma_info_map2d_weight_512x[0],&chroma_info_map3D_weight_512x[0],
	&chroma_info_mix_th_512x[0],&chroma_info_level_map_512x[0],
};

static UINT32 chroma_y_info_map2d_weight_512x[4] = {0x0, 0x0, 0x80, 0x8};
static UINT32 chroma_y_info_map3D_weight_512x[4] = {0x0, 0x40, 0x80, 0xf8};
static IME_HAL_3DNR_Y_PARAM sq_c_y_info_512x =
{
    0x0,
    &chroma_y_info_map2d_weight_512x[0], &chroma_y_info_map3D_weight_512x[0],
};

//ISO102400
static UINT32 snr_ratio_1024x[6] =	 {50, 2, 1, 1, 1, 0};// set SNR ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 tnr_ratio_1024x[6] =	{60, 25, 25, 28, 29, 50};// set tnr ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 luma_gain_1024x[8] =	 {24, 24, 22, 21, 20, 19, 18, 16};// SQUARE strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 freq_1024x[7] =	 {0x7f,0x6c,0x40,0x33,0x14,0x40,0x3c};// set diff ratio in diff freq, 7 elements, range = [0, 127]
static UINT32 snr_freq_1024x[4] =	 {1, 4, 4, 12};// snr ratio depend on frequency, 4 elements, range = [0, 255]
static UINT32 detail_penalty_1024x[6] =	 {5, 5, 3, 5, 5, 5};//  detail penalty, 6 elements, range = [0, 15]
static UINT32 sad_penalty_1024x[8] =	 {40, 40, 70, 45, 40, 45, 80, 160};//  sad penalty, 8 elements, range = [0, 15]
static UINT32 periodic_sad_similar_1024x[4] =	 {0x2,0x0,0x0,0x0};//  Periodic sad similar, 4 elements
static UINT32 candiate_pos_x_1024x[7] =	 {5,1,0,6,2,5,1};// mv position for horizontal direction, 7 elements
static UINT32 candiate_pos_y_1024x[7] = 	{5,5,2,2,2,5,5};// mv position for vertical direction, 7 elements
static UINT32 motion_tensor_valmap_1024x[4] =	 {4, 48, 0, 10};// tensor remapping, 4 elements, range = [0, 63]
static UINT32 motion_map_1024x[5] =	 {0x32,0x2c,0x23,0x16,0xc};// decide number of full search patches, 5 elements, range = [0, 63]

static IME_HAL_3DNR_LUMA_PARAM sq_l_info_1024x =
{
	0x48,0x2400,0x0,0x3,0x320,0x0,0x0,
	{//
		&detail_penalty_1024x[0],&sad_penalty_1024x[0],
		0x1,0x0,0xc,21,
		0x1,0x0,0x1,0x20,&periodic_sad_similar_1024x[0],
		0x0,0x0,0x1,0x0,
		0x0,0x0,&candiate_pos_x_1024x[0], &candiate_pos_y_1024x[0],
	},
	{// Motion
		0x0,0xa,0x64,0x6,
		20, 20,
		&motion_tensor_valmap_1024x[0],&motion_map_1024x[0],
	},
	&snr_ratio_1024x[0], &tnr_ratio_1024x[0],&luma_gain_1024x[0],&freq_1024x[0],&snr_freq_1024x[0],
};

static UINT32 chroma_info_luma_gain_1024x[8] =	{0x10,0x10,0x10,0x10,0x20,0x30,0x38,0x40};// TNR strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 chroma_info_motion_ctrl_1024x[16] = // mapped motion value of Xth motion level, 16 elements, range = [0, 255]
	{0x6b,0x55,0x40,0x2b,0x20,0x1a,0x15,0x12,0x10,0xe,0xd,0xc,0xb,0xa,0x9,0x8};
static UINT32 chroma_info_map2d_weight_1024x[4] = 	{0x0,0x0,0x80,0x8};// 4 elements, puiMap2DWeights[0]: chroma threshold for still; puiMap2DWeights[1]: chroma weighting for HSNR when still; puiMap2DWeights[2]: chroma threshold for motion; puiMap2DWeights[3]: chroma curve slope
static UINT32 chroma_info_map3D_weight_1024x[4] = 	{0x0,0x40,0x80,0xf8};// 4 elements, puiMap3DWeights[0]: chroma threshold for still; puiMap3DWeights[1]: chroma weighting for HSNR when still; puiMap3DWeights[2]: chroma threshold for motion; puiMap3DWeights[3]: chroma curve slope
static UINT32 chroma_info_mix_th_1024x[4] =	 {0x46,0x3,0x46,0x3};// luma threshold to mix the motion detect value, and chroma threshold to mix the motion detect value, 4 elements
static UINT32 chroma_info_level_map_1024x[16] = // Memc level mapping for motion noise, 16 elements
    {0x0,0x3,0x7,0xc,0x11,0x18,0x20,0x29,0x34,0x42,0x51,0x64,0x7a,0x95,0xb4,0xd9};

static IME_HAL_3DNR_CHROMA_PARAM sq_c_info_1024x =
{
	0x0,0x0,0x0,
	0x20,0x60,0x8,0x8,0x8,0x0,0x0,0x8,
	&chroma_info_luma_gain_1024x[0],&chroma_info_motion_ctrl_1024x[0],
	&chroma_info_map2d_weight_1024x[0],&chroma_info_map3D_weight_1024x[0],
	&chroma_info_mix_th_1024x[0],&chroma_info_level_map_1024x[0],
};

static UINT32 chroma_y_info_map2d_weight_1024x[4] = {0x0, 0x0, 0x80, 0x8};
static UINT32 chroma_y_info_map3D_weight_1024x[4] = {0x0, 0x40, 0x80, 0xf8};
static IME_HAL_3DNR_Y_PARAM sq_c_y_info_1024x =
{
    0x0,
    &chroma_y_info_map2d_weight_1024x[0], &chroma_y_info_map3D_weight_1024x[0],
};

//ISO204800
static UINT32 snr_ratio_2048x[6] =	 {50, 2, 1, 1, 1, 0};// set SNR ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 tnr_ratio_2048x[6] =	{60, 25, 25, 28, 29, 50};// set tnr ratio in diff full search patch, 6 elements, range = [0, 127]
static UINT32 luma_gain_2048x[8] =	 {24, 24, 22, 21, 20, 19, 18, 16};// SQUARE strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 freq_2048x[7] =	 {0x7f,0x6c,0x40,0x33,0x14,0x40,0x3c};// set diff ratio in diff freq, 7 elements, range = [0, 127]
static UINT32 snr_freq_2048x[4] =	 {1, 4, 4, 12};// snr ratio depend on frequency, 4 elements, range = [0, 255]
static UINT32 detail_penalty_2048x[6] =	 {5, 5, 3, 5, 5, 5};//  detail penalty, 6 elements, range = [0, 15]
static UINT32 sad_penalty_2048x[8] =	 {40, 40, 70, 45, 40, 45, 80, 160};//  sad penalty, 8 elements, range = [0, 15]
static UINT32 periodic_sad_similar_2048x[4] =	 {0x2,0x0,0x0,0x0};//  Periodic sad similar, 4 elements
static UINT32 candiate_pos_x_2048x[7] =	 {5,1,0,6,2,5,1};// mv position for horizontal direction, 7 elements
static UINT32 candiate_pos_y_2048x[7] = 	{5,5,2,2,2,5,5};// mv position for vertical direction, 7 elements
static UINT32 motion_tensor_valmap_2048x[4] =	 {4, 48, 0, 10};// tensor remapping, 4 elements, range = [0, 63]
static UINT32 motion_map_2048x[5] =	 {0x32,0x2c,0x23,0x16,0xc};// decide number of full search patches, 5 elements, range = [0, 63]

static IME_HAL_3DNR_LUMA_PARAM sq_l_info_2048x =
{
	0x48,0x2400,0x0,0x3,0x320,0x0,0x0,
	{//
		&detail_penalty_2048x[0],&sad_penalty_2048x[0],
		0x1,0x0,0xc,21,
		0x1,0x0,0x1,0x20,&periodic_sad_similar_2048x[0],
		0x0,0x0,0x1,0x0,
		0x0,0x0,&candiate_pos_x_2048x[0], &candiate_pos_y_2048x[0],
	},
	{// Motion
		0x0,0xa,0x64,0x6,
		20, 20,
		&motion_tensor_valmap_2048x[0],&motion_map_2048x[0],
	},
	&snr_ratio_2048x[0], &tnr_ratio_2048x[0],&luma_gain_2048x[0],&freq_2048x[0],&snr_freq_2048x[0],
};

static UINT32 chroma_info_luma_gain_2048x[8] =	{0x10,0x10,0x10,0x10,0x20,0x30,0x38,0x40};// TNR strength adjust at luma, 8 elements, range = [0, 255]
static UINT32 chroma_info_motion_ctrl_2048x[16] = // mapped motion value of Xth motion level, 16 elements, range = [0, 255]
	{0x6b,0x55,0x40,0x2b,0x20,0x1a,0x15,0x12,0x10,0xe,0xd,0xc,0xb,0xa,0x9,0x8};
static UINT32 chroma_info_map2d_weight_2048x[4] = 	{0x0,0x0,0x80,0x8};// 4 elements, puiMap2DWeights[0]: chroma threshold for still; puiMap2DWeights[1]: chroma weighting for HSNR when still; puiMap2DWeights[2]: chroma threshold for motion; puiMap2DWeights[3]: chroma curve slope
static UINT32 chroma_info_map3D_weight_2048x[4] = 	{0x0,0x40,0x80,0xf8};// 4 elements, puiMap3DWeights[0]: chroma threshold for still; puiMap3DWeights[1]: chroma weighting for HSNR when still; puiMap3DWeights[2]: chroma threshold for motion; puiMap3DWeights[3]: chroma curve slope
static UINT32 chroma_info_mix_th_2048x[4] =	 {0x46,0x3,0x46,0x3};// luma threshold to mix the motion detect value, and chroma threshold to mix the motion detect value, 4 elements
static UINT32 chroma_info_level_map_2048x[16] = // Memc level mapping for motion noise, 16 elements
    {0x0,0x3,0x7,0xc,0x11,0x18,0x20,0x29,0x34,0x42,0x51,0x64,0x7a,0x95,0xb4,0xd9};

static IME_HAL_3DNR_CHROMA_PARAM sq_c_info_2048x =
{
	0x0,0x0,0x0,
	0x20,0x60,0x8,0x8,0x8,0x0,0x0,0x8,
	&chroma_info_luma_gain_2048x[0],&chroma_info_motion_ctrl_2048x[0],
	&chroma_info_map2d_weight_2048x[0],&chroma_info_map3D_weight_2048x[0],
	&chroma_info_mix_th_2048x[0],&chroma_info_level_map_2048x[0],
};

static UINT32 chroma_y_info_map2d_weight_2048x[4] = {0x0, 0x0, 0x80, 0x8};
static UINT32 chroma_y_info_map3D_weight_2048x[4] = {0x0, 0x40, 0x80, 0xf8};
static IME_HAL_3DNR_Y_PARAM sq_c_y_info_2048x =
{
    0x0,
    &chroma_y_info_map2d_weight_2048x[0], &chroma_y_info_map3D_weight_2048x[0],
};

static IME_HAL_3DNR_LUMA_PARAM* sq_luma_param[TOTAL_GAIN_NUM]=
{
    &sq_l_info_1x,
    &sq_l_info_2x,
	&sq_l_info_4x,
    &sq_l_info_8x,
	&sq_l_info_16x,
    &sq_l_info_32x,
	&sq_l_info_64x,
	&sq_l_info_128x,
	&sq_l_info_256x,
	&sq_l_info_512x,
	&sq_l_info_1024x,
	&sq_l_info_2048x,
};

static IME_HAL_3DNR_CHROMA_PARAM* sq_chroma_param[TOTAL_GAIN_NUM]=
{
    &sq_c_info_1x,
    &sq_c_info_2x,
    &sq_c_info_4x,
    &sq_c_info_8x,
    &sq_c_info_16x,
    &sq_c_info_32x,
    &sq_c_info_64x,
    &sq_c_info_128x,
    &sq_c_info_256x,
    &sq_c_info_512x,
    &sq_c_info_1024x,
    &sq_c_info_2048x,
};

static IME_HAL_3DNR_Y_PARAM* sq_chroma_y_param[TOTAL_GAIN_NUM]=
{
    &sq_c_y_info_1x,
    &sq_c_y_info_2x,
    &sq_c_y_info_4x,
    &sq_c_y_info_8x,
    &sq_c_y_info_16x,
    &sq_c_y_info_32x,
    &sq_c_y_info_64x,
    &sq_c_y_info_128x,
    &sq_c_y_info_256x,
    &sq_c_y_info_512x,
    &sq_c_y_info_1024x,
    &sq_c_y_info_2048x,
};

#endif
