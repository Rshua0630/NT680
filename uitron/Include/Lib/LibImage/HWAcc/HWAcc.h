/**
    Header file for HW acceleration

    This file is the header file that define the API and data type
    for image HW accleration

    @file       HWAcc.h
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#ifndef _HWACC_DEF_H
#define _HWACC_DEF_H

#include "Type.h"

/**
    HW Acc Input Format Selection.

\n  Used for nt96650_blockNR()
*/
//@{
typedef enum _BLOCK_NR_FORMAT_SEL

{
	INPUT_YCC_444  = 0,         ///< YCC444 separate planar format for input
	INPUT_YCCP_422 = 1,         ///< YCC422 UV-packing format for input
	INPUT_YCCP_420 = 2,         ///< YCC420 UV-packing format for input
	INPUT_Y_ONLY   = 3,         ///< Only Y channel for input
	ENUM_DUMMY4WORD(BLOCK_NR_FORMAT_SEL)

} BLOCK_NR_FORMAT_SEL;
//@}

/*
    Alpha Blending for YUV422

    Blends two images (SRC, DST) into one output image by the following formula.
        DST(i, j) = ( ALPHA(i, j) * SRC(i, j) + (255 - ALPHA(i, j)) * DST(i, j) ) / 256
        Assume Morphor will invert internal ALPHA value
        i: horizontal pixel position,  j: vertical pixel position
        ALPHA is a 8bit per-pixel alpha value map.

    @param[in] p_dst_image  Pointer to a destination image buffer. (byte alignment)
    @param[in] p_src_image  Pointer to a source image buffer. (byte alignment)
    @param[in] p_alpha_map  Pointer to an alpha map. (byte alignment)
    @param[in] p_alpha_tmp  Pointer to a temporal buffer for UV packed alpha map
                            (Buffer size must at least 3 * width*height / 2)
                            (byte alignment)
    @param[in] width        Image width in pixel. (range: 4~16383)
    @param[in] height       Image height in pixel. (range: 1~16383)
    @param[in] lineoffset_y     lineoffset of y plane (word alignment)
    @param[in] lineoffset_uv    lineoffset of uv packed plane (word alignment)

    @return:
        - @b 0: OK
 */
extern INT32 nt96650_hw_alpha_blend_for_YUV422(
	UINT8 *p_dst_image,
	UINT8 *p_src_image,
	UINT8 *p_alpha_map,
	UINT8 *p_alpha_tmp,
	INT32 width,
	INT32 height,
	INT32 lineoffset_y,
	INT32 lineoffset_uv
);

/*
    Alpha Blending for YUV420

    Same as the one for YUV422.
        DST(i, j) = ( ALPHA(i, j) * SRC(i, j) + (255 - ALPHA(i, j)) * DST(i, j) ) / 256

    @param[in] p_dst_image  Pointer to a destination image buffer. (byte alignment)
    @param[in] p_src_image  Pointer to a source image buffer. (byte alignment)
    @param[in] p_alpha_map  Pointer to an alpha map. (byte alignment)
    @param[in] p_alpha_tmp  Pointer to a temporal buffer for UV packed alpha map
                            (Buffer size must at least 3*width*height/4)
                            (byte alignment)
    @param[in] width        Image width in pixel. (range: 4~16383)
    @param[in] height       Image height in pixel. (range: 1~16383)
    @param[in] lineoffset_y     lineoffset of y plane (word alignment)
    @param[in] lineoffset_uv    lineoffset of uv packed plane (word alignment)

    @return:
        -@b 0: OK
 */
extern INT32 nt96650_hw_alpha_blend_for_YUV420(
	UINT8 *p_dst_image,
	UINT8 *p_src_image,
	UINT8 *p_alpha_map,
	UINT8 *p_alpha_tmp,
	INT32 width,
	INT32 height,
	INT32 lineoffset_y,
	INT32 lineoffset_uv
);

/*
    Alpha Blending for Y plane

    Same as the one for Y plane.
        DST(i, j) = ( ALPHA(i, j) * SRC(i, j) + (255 - ALPHA(i, j)) * DST(i, j) ) / 256

    @param[in] p_dst_image  Pointer to a destination image buffer. (byte alignment)
    @param[in] p_src_image  Pointer to a source image buffer. (byte alignment)
    @param[in] p_alpha_map  Pointer to an alpha map. (byte alignment)
    @param[in] p_alpha_tmp  Don't care, just pass NULL
    @param[in] width        Image width in pixel. (range: 4~16383)
    @param[in] height       Image height in pixel. (range: 4~16383)
    @param[in] lineoffset_y     lineoffset of y plane (word alignment)
    @param[in] lineoffset_uv    lineoffset of uv packed plane (word alignment)

    @return:
        -@b 0: OK
*/
extern INT32 nt96650_hw_alpha_blend_for_Y(
	UINT8 *p_dst_image,
	UINT8 *p_src_image,
	UINT8 *p_alpha_map,
	UINT8 *p_alpha_tmp,
	INT32 width,
	INT32 height,
	INT32 lineoffset_y,
	INT32 lineoffset_uv
);

/*
    Affine Transform for YUV422

    Transforms a source image to a destination image by a specified affine matrix.
    If the transforming area contains a region outside of the source image,
    any value is acceptable for the target pixels on the destination image,
    but the function MUST run with NO ERROR.
    And for valid pixels, bilinear interpolation is required.

    @note Support rotation angle -15 ~ +15
        Shear/Scaling is NOT supported

    @param[in] p_dst_image_y    Pointer to a destination image Y buffer. (word alignment)
    @param[in] p_dst_image_uv   Pointer to a destination image UV buffer. (word alignment)
    @param[in] p_src_image_y    Pointer to a source image Y buffer. (word alignment)
    @param[in] p_src_image_uv   Pointer to a source image UV buffer. (word alignment)
    @param[in] width        Image width in pixel. (range: 16~16384) (16 bytes alignment)
    @param[in] height       Image height in pixel. (range: 8~8192) (8 lines alignment)
    @param[in] lineoffset_y     lineoffset of y plane (word alignment)
    @param[in] lineoffset_uv    lineoffset of uv packed plane (word alignment)
    @param[in] coeff_a      Affine coeff A in float format. Should be cos(theta). (range: -1.99 ~ +1.99)
    @param[in] coeff_b      Affine coeff B in float format. Should be -sin(theta). (range: -1.99 ~ +1.99)
    @param[in] coeff_c      Affine coeff C in float format. (range: -16384.0 ~ +16384.0)
    @param[in] coeff_d      Affine coeff D in float format. Should be sin(theta). (range: -1.99 ~ +1.99)
    @param[in] coeff_e      Affine coeff E in float format. Should be cos(theta). (range: -1.99 ~ +1.99)
    @param[in] coeff_f      Affine coeff F in float format. (range: -16384.0 ~ +16384.0)

    @return:
        - @b E_OK: OK
        - @b E_NOSPT: affine coefficients not support, may not pure rotation
 */
extern INT32 nt96650_hw_affine_transform_for_YUV422(
	UINT8 *p_dst_image_y,
	UINT8 *p_dst_image_uv,
	UINT8 *p_src_image_y,
	UINT8 *p_src_image_uv,
	INT32 width,
	INT32 height,
	INT32 lineoffset_y,
	INT32 lineoffset_uv,
	float coeff_a,
	float coeff_b,
	float coeff_c,
	float coeff_d,
	float coeff_e,
	float coeff_f
);

/*
    Affine Transform for YUV420

    Same as the one for YUV422.

    @note Support rotation angle -15 ~ +15
        Shear/Scaling is NOT supported

    @param[in] p_dst_image_y    Pointer to a destination image Y buffer. (word alignment)
    @param[in] p_dst_image_uv   Pointer to a destination image UV buffer. (word alignment)
    @param[in] p_src_image_y    Pointer to a source image Y buffer. (word alignment)
    @param[in] p_src_image_uv   Pointer to a source image UV buffer. (word alignment)
    @param[in] width        Image width in pixel. (range: 16~16384) (16 bytes alignment)
    @param[in] height       Image height in pixel. (range: 16~8192) (16 lines alignment)
    @param[in] lineoffset_y     lineoffset of y plane (word alignment)
    @param[in] lineoffset_uv    lineoffset of uv packed plane (word alignment)
    @param[in] coeff_a      Affine coeff A in float format. Should be cos(theta). (range: -1.99 ~ +1.99)
    @param[in] coeff_b      Affine coeff B in float format. Should be -sin(theta). (range: -1.99 ~ +1.99)
    @param[in] coeff_c      Affine coeff C in float format. (range: -16384.0 ~ +16384.0)
    @param[in] coeff_d      Affine coeff D in float format. Should be sin(theta). (range: -1.99 ~ +1.99)
    @param[in] coeff_e      Affine coeff E in float format. Should be cos(theta). (range: -1.99 ~ +1.99)
    @param[in] coeff_f      Affine coeff F in float format. (range: -16384.0 ~ +16384.0)

    @return:
        - @b E_OK: OK
        - @b E_NOSPT: affine coefficients not support, may not pure rotation
 */
extern INT32 nt96650_hw_affine_transform_for_YUV420(
	UINT8 *p_dst_image_y,
	UINT8 *p_dst_image_uv,
	UINT8 *p_src_image_y,
	UINT8 *p_src_image_uv,
	INT32 width,
	INT32 height,
	INT32 lineoffset_y,
	INT32 lineoffset_uv,
	float coeff_a,
	float coeff_b,
	float coeff_c,
	float coeff_d,
	float coeff_e,
	float coeff_f
);

/*
    Y-Plane Normalization

    Converts pixel value by following equation.
        DST(i, j) = A * SRC(i, j) + B

    @note If DST(i, j) larger than 0xFF or smaller than 0, it will be saturated to 0xFF/0.

    @param[in] p_image  Pointer to a source-destination-shared Y-plane buffer (byte alignment)
    @param[in] coeff_a  coefficient A. (range: 0x00~0xFF)
    @param[in] coeff_b  coefficient B. (range: 0x00~0xFF)
    @param[in] width    image width (range: 4~16383)
    @param[in] height   image height (range: 1~16383)
    @param[in] lineoffset   image lineoffset (word alignment)

    @return:
        - @b 0: OK
 */
extern INT32 nt96650_hw_y_normalization(
	UINT8 *p_image,
	INT32 coeff_a,
	INT32 coeff_b,
	INT32 width,
	INT32 height,
	INT32 lineoffset
);

/*
  Y-Plane Scaling

  Scales up/down an input Y-plane to an output Y-plane by specified input/outpu sizes.
  Max scaling ratio is 16x to 1/16x
  Bilinear interpolation is used for scaling.

  @param  uiInputAddr      Source buffer DRAM address of a 8bit (Y-plane, Must be word-aligned)
  @param  uiInputWidth     Source image width in pixels (Must be multiple of 4)
  @param  uiInputHeight    Source image height in lines (Must be multiple of 2)
  @param  uiInputLineOfs   Source buffer DRAM line offset in bytes (Must be word-aligned)
  @param  uiOutputAddr     Destination buffer DRAM address of a 8bit (Y-plane, Must be word-aligned)
  @param  uiOutputWidth    Destination image width in pixels (Must be multiple of 2)
  @param  uiOutputHeight   Destination image height in lines (Must be multiple of 2)
  @param  uiOutputLineOfs  Destination buffer DRAM line offset in bytes (Must be word-aligned)
  @return status. 0 if OK.
 */
extern INT32 nt96650_scaleY(
	UINT32 uiInputAddr,
	UINT32 uiInputWidth,
	UINT32 uiInputHeight,
	UINT32 uiInputLineOfs,
	UINT32 uiOutputAddr,
	UINT32 uiOutputWidth,
	UINT32 uiOutputHeight,
	UINT32 uiOutputLineOfs
);

/*
  Block NR

  Block NR of a single 8 bit input plane

  @param  uiYUVFormat       Define process format YUV444/422/420 or Y only
  @param  uiYBlockWidth     Processing block width in pixels(must be multiples of 32)
  @param  uiYBlockHeight    Processing block height in lines(must be multiples of 32)
  @param  uiNRIntensity     NR intensity for this block(0:NO NR, 255: Max NR)
  @param  uiYInputAddr      Source buffer DRAM address of a 8bit Y input (word-aligned)
  @param  uiYInputLineOfs   Source buffer DRAM line offset in bytes (word-aligned)
  @param  uiYOutputAddr     Destination buffer DRAM address of a 8bit Y output (word-aligned)
  @param  uiYOutputLineOfs  Destination buffer DRAM line offset in bytes (word-aligned)
  @param  uiUVInputAddr     Source buffer DRAM address of a 8bit UV input (word-aligned)
  @param  uiUVInputLineOfs  Source buffer DRAM line offset in bytes (word-aligned)
  @param  uiUVOutputAddr    Destination buffer DRAM address of a 8bit UV output (word-aligned)
  @param  uiUVOutputLineOfs Destination buffer DRAM line offset in bytes (word-aligned)
  @return status. 0 if OK.
 */
extern INT32 nt96650_blockNR(
	BLOCK_NR_FORMAT_SEL uiYUVFormat,
	UINT32 uiYBlockWidth,
	UINT32 uiYBlockHeight,
	UINT32 uiNRIntensity,
	UINT32 uiYInputAddr,
	UINT32 uiYInputLineOfs,
	UINT32 uiYOutputAddr,
	UINT32 uiYOutputLineOfs,
	UINT32 uiUVInputAddr,
	UINT32 uiUVInputLineOfs,
	UINT32 uiUVOutputAddr,
	UINT32 uiUVOutputLineOfs
);

#endif
