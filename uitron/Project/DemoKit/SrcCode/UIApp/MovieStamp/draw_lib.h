#ifndef _DRAW_LIB_H
#define _DRAW_LIB_H

#include "Type.h"

typedef struct {
	UINT32      uiOffset;              ///< offset in database.
	UINT16      uiWidth;               ///< icon width.
	UINT16      uiHeight;              ///< icon height.
} ICON_HEADER, *PICON_HEADER;

typedef struct {
	ICON_HEADER const   *pIconHeader;       ///< point tp icon header
	UINT8       const   *pIconData;         ///< point to icon data
	UINT16              uiNumOfIcon;        ///< how many icon in this icon data base
	UINT8               uiBitPerPixel;      ///< bit perpixel of this icon DB, it can be 1, 2 or 4 bits per pixel
	UINT8               uiDrawStrOffset;    ///< Data base first item offset
} ICON_DB, *PICON_DB;

#endif /* _DRAW_LIB_H */
