#ifndef _MOVIESTAMPINT_H
#define _MOVIESTAMPINT_H

#include "Type.h"

// Flag
#define FLGMOVIESTAMP_IDLE          FLGPTN_BIT(0)
#define FLGMOVIESTAMP_UPDATE        FLGPTN_BIT(1)
#define FLGMOVIESTAMP_ALL           0xFFFFFFFF

extern UINT32 MOVIESTAMPTSK_ID;
extern UINT32 FLG_ID_MOVIESTAMP;

extern void MovieStampTsk(void);

#endif //_MOVIESTAMPINT_H
