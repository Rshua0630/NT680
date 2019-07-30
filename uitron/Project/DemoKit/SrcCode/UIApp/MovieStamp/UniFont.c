
#include "Type.h"
#include "PrjCfg.h"

#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)

#include "UniFont.h"
#include "DemoKit_Font.h"

#define __MODULE__              UniFont
#define __DBGLVL__              2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__              "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

static  UINT8   *g_pGlyphData = 0;
static  UINT32  g_pixelCount = 0;
static  UINT8   g_pixeldata = 0;

static void UniFont_GlyphConcatenate2BitData(UINT8 data)
{

	g_pixeldata |= ((data & 0x03) << (6 - g_pixelCount % 4 * 2));
	g_pixelCount++;
	if (g_pixelCount % 4 == 0) {
		*g_pGlyphData++ = g_pixeldata;
		g_pixeldata = 0;
	}
}

static UINT8 UniFont_GlyphGet2bitData(UINT32 bitsOneRow, UINT32 x, UINT32 y)
{
	UINT32  bitsOffset, bytesOffset, bitsRemainder;

	bitsOffset = bitsOneRow * y + x * 2;
	bitsRemainder = (bitsOffset % 8) >> 1;
	bytesOffset = bitsOffset / 8;

	return (g_pGlyphData[bytesOffset] >> (6 - (bitsRemainder % 4 * 2))) & 0x03;
}

static void UniFont_GlyphSet2bitData(UINT32 bitsOneRow, UINT32 x, UINT32 y, UINT8 frame)
{
	UINT32  bitsOffset, bytesOffset, bitsRemainder;

	bitsOffset = bitsOneRow * y + x * 2;
	bitsRemainder = (bitsOffset % 8) >> 1;
	bytesOffset = bitsOffset / 8;

	g_pGlyphData[bytesOffset] &= ~(0x03 << (6 - (bitsRemainder % 4 * 2)));
	g_pGlyphData[bytesOffset] |= (frame & 0x03) << (6 - bitsRemainder % 4 * 2);
}


static void UniFont_Glyph2bitDataForFrame(UFN_GLYPH *pglyph)
{
	UINT32  width, height;
	UINT32  col, row;
	UINT32  bitsOneRow;
	UINT8   up, down, right, left;

	width = pglyph->GLYPHINFO.wBoxX;
	height = pglyph->GLYPHINFO.wBoxY;

	bitsOneRow = width * 2;
	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			if (UniFont_GlyphGet2bitData(bitsOneRow, col, row) == 0x02) {
				continue;
			}

			if (row == 0) {
				up = 0;
				if (col == 0) {
					left  = 0;
					right = UniFont_GlyphGet2bitData(bitsOneRow, col + 1, row);
					down  = UniFont_GlyphGet2bitData(bitsOneRow, col, row + 1);
				} else if (col == (width - 1)) {
					right = 0;
					left  = UniFont_GlyphGet2bitData(bitsOneRow, col - 1, row);
					down  = UniFont_GlyphGet2bitData(bitsOneRow, col, row + 1);
				} else {
					left  = UniFont_GlyphGet2bitData(bitsOneRow, col - 1, row);
					right = UniFont_GlyphGet2bitData(bitsOneRow, col + 1, row);
					down  = UniFont_GlyphGet2bitData(bitsOneRow, col, row + 1);
				}
			} else if (row == (height - 1)) {
				down = 0;
				if (col == 0) {
					left  = 0;
					right = UniFont_GlyphGet2bitData(bitsOneRow, col + 1, row);
					up    = UniFont_GlyphGet2bitData(bitsOneRow, col, row - 1);
				} else if (col == (width - 1)) {
					left  = UniFont_GlyphGet2bitData(bitsOneRow, col - 1, row);
					right = 0;
					up    = UniFont_GlyphGet2bitData(bitsOneRow, col, row - 1);
				} else {
					left  = UniFont_GlyphGet2bitData(bitsOneRow, col - 1, row);
					right = UniFont_GlyphGet2bitData(bitsOneRow, col + 1, row);
					up    = UniFont_GlyphGet2bitData(bitsOneRow, col, row - 1);
				}
			} else {
				up    = UniFont_GlyphGet2bitData(bitsOneRow, col, row - 1);
				down  = UniFont_GlyphGet2bitData(bitsOneRow, col, row + 1);
				left  = UniFont_GlyphGet2bitData(bitsOneRow, col - 1, row);
				right = UniFont_GlyphGet2bitData(bitsOneRow, col + 1, row);
			}

			// update frame value to pixel data
			if (up == 2 || down == 2 || left == 2 || right == 2) {
				UniFont_GlyphSet2bitData(bitsOneRow, col, row, 0x01);
			}
		}
	}
}

UINT16 UniFont_FontTable1bitDataTo2bit(UINT16 unicode, UFN_GLYPH *pglyph)
{
	UINT32 ChunkIdx = unicode * 8 + 12;
	UINT32 FontOffset = 0;
	UINT32 Size = 0;
	UINT8  BppMode = 0;
	UINT32 i, j;
	UINT8  Data;

	BppMode    = gDemoKit_Font[3];
	FontOffset = ((UINT32)gDemoKit_Font[ChunkIdx + 3] << 24) + ((UINT32)gDemoKit_Font[ChunkIdx + 2] << 16) +
				 ((UINT32)gDemoKit_Font[ChunkIdx + 1] << 8) + (UINT32)gDemoKit_Font[ChunkIdx];
	Size       = ((UINT32)gDemoKit_Font[ChunkIdx + 7] << 24) + ((UINT32)gDemoKit_Font[ChunkIdx + 6] << 16) +
				 ((UINT32)gDemoKit_Font[ChunkIdx + 5] << 8) + (UINT32)gDemoKit_Font[ChunkIdx + 4];

	if (FontOffset == 0xFFFFFFFF) {
		return 0xFFFF;
	}
	FontOffset += 4;                                            // Include GX_IMAGETABLE header (4 bytes)
	Size -= 4;                                                  // Exclude width/height information (4 bytes)
	pglyph->GLYPHINFO.wBoxX = gDemoKit_Font[FontOffset];        // Width
	pglyph->GLYPHINFO.wBoxY = gDemoKit_Font[FontOffset + 2];    // Height
	FontOffset += 4;                                            // Move offset to glyph data start

	for (i = 0 ; i < Size; i ++) {
		Data = gDemoKit_Font[FontOffset + i];
		if (BppMode == 0x02) {                                  // 2Bpp mode
			for (j = 0; j < 4; j ++) {
				UniFont_GlyphConcatenate2BitData(((Data & 0x03) == 0x01) ? 0x02 : 0x00);
				Data >>= 2;
			}
		} else {                                                // 1Bpp mode
			for (j = 0; j < 8; j ++) {
				if (Data & 0x01) {
					UniFont_GlyphConcatenate2BitData(0x02);
				} else {
					UniFont_GlyphConcatenate2BitData(0x00);
				}
				Data >>= 1;
			}
		}
	}
	return unicode;
}

void UniFont_GetGlyphFromUnicode(UINT16 unicode, LINGUAL_INFO *pLingualInfo)
{
	memset(pLingualInfo->GlyphData, 0, GLYPHDATA_LENGTH);
	g_pixelCount = 0;
	g_pixeldata = 0;
	g_pGlyphData = (UINT8 *)pLingualInfo->GlyphData;

	if (UniFont_FontTable1bitDataTo2bit(unicode, &pLingualInfo->glyph) == 0xFFFF) {
		debug_msg("unicode %x not found in FontTable\r\n", unicode);
		return;
	}

	// reset g_pGlyphData buffer pointer after converting 2 bits per pixel.
	g_pGlyphData = (UINT8 *)pLingualInfo->GlyphData;

	// search for Frame pixel in glyph scan data.
	UniFont_Glyph2bitDataForFrame(&pLingualInfo->glyph);
}
#endif

