#ifndef _WIFIAPPCMDMAP_H_
#define _WIFIAPPCMDMAP_H_

#include "Type.h"

typedef struct _WIFIAPPINDEXMAP {
	UINT32              uiIndex;
	char                *string;
} WIFIAPPINDEXMAP;

typedef struct _WIFIAPPMAP {
	UINT32           uiFlag;
	char             *name;
	WIFIAPPINDEXMAP  *maptbl;
} WIFIAPPMAP;


#define  INDEX_END    (0xFFFFFFFF)

WIFIAPPMAP *WifiCmd_GetMapTbl(void);

#endif
