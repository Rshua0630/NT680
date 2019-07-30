
#ifndef _UNIFONT_H
#define _UNIFONT_H

//#pragma pack(1)

#include "Type.h"
#include "Debug.h"
#include "GxGfx.h"

#define GLYPHDATA_LENGTH            (8 * 64 * 2)    // Max gpyph is 64x64, so we need 8*64 bytes for a glyph data
// and since we will extend data to 2-bit mode, so we need 8 * 64 * 2 for a glyph.

typedef struct tagUFN_GLYPHINFO {
	UINT8       wBoxX;              // Width
	UINT8       wBoxY;              // Height
} UFN_GLYPHINFO, *PUFN_GLYPHINFO;

typedef struct _GLYPH {
	UFN_GLYPHINFO GLYPHINFO;
} UFN_GLYPH, *PUFN_GLYPH;

typedef struct {
	UFN_GLYPH     glyph;
	UINT8         GlyphData[GLYPHDATA_LENGTH];
} LINGUAL_INFO, *PLINGUAL_INFO;

typedef struct tagStringTable {
	UINT16    GX_signature1;
	UINT8      Format1;
	UINT8      BPS;
	UINT16    GX_signature2;
	UINT8      Format2;
	UINT8      Revd;
	UINT16    Start;
	UINT16    Count;
} StringTable, *PStringTable;

typedef struct tagStringIndexChunk {
	UINT32     Offset;
	UINT32     Size;
} StringIndexChunk, *PStringIndexChunk;

extern void     UniFont_GetGlyphFromUnicode(UINT16 unicode, LINGUAL_INFO *pLingualInfo);

#endif // _UNIFONT_H
