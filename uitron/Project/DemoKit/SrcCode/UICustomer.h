#ifndef _UICUSTOMER_H
#define _UICUSTOMER_H
#include "SysCfg.h"




//display DPOF menu item or not
#ifndef DPOF
#define DPOF                 ENABLE
#endif

//display quick view item or not,if disable,after capture,quick view always on
#ifndef QUICK_VIEW
#define QUICK_VIEW           ENABLE
#endif

//support North Eurpo Area or not
#ifndef SUP_NORTH_EUROP
#define SUP_NORTH_EUROP         DISABLE
#endif

//display printer item in USB menu or not
#ifndef SUP_PRINTER
#define SUP_PRINTER           ENABLE
#endif

//define flash default setting in Auto mode
#ifndef FLASH_DEF_MODE
#define FLASH_DEF_MODE           CUST_FLASH_OFF
#endif

//support menu select function or not ,ex:select item in delete ,protect menu,
//in printer "select" item would display "single" (please refer to customer version table)
#ifndef MENU_SELECT
#define MENU_SELECT           ENABLE
#endif

//if Enable, Exif version and down key would show customer version
//shutter key would display real version
//otherwise, customer version,real version Exif version would be the same
#ifndef SUP_CUSTOMER_VERSION
#define SUP_CUSTOMER_VERSION           DISABLE
#endif

#ifndef SUP_CUSTOMER_VERSION
#define ANIMATION_LOGO        DISABLE
#endif

#ifndef PWROFF_LOGO
#define PWROFF_LOGO          DISABLE
#endif



#if (SUP_NORTH_EUROP == ENABLE)  //if support north europ ,disable not standard language

#define SUP_PORTUGUESE        DISABLE
#define SUP_ARABIC            DISABLE
#define SUP_RUSSIA            DISABLE
#define SUP_HOLLAND           DISABLE
#define SUP_TCHINESE          DISABLE
#define SUP_SCHINESE          DISABLE
#define SUP_JAPANESE          DISABLE

#define SUP_CZECHESKY         ENABLE
#define SUP_DANISHDANSK       ENABLE
#define SUP_GREEK             ENABLE
#define SUP_HUNGARIANMAGYAR   ENABLE
#define SUP_NORSENORSK        ENABLE
#define SUP_SWEDISHSVENSKA    ENABLE
#define SUP_FINNISHSUOMI      ENABLE
#define SUP_POLISHPOLSKI      ENABLE
#define SUP_TURKISHTRKE       ENABLE

#endif


//define different default language

//most version  support Portuguese
//Portuguese and Brasil would support only at the same time
//support Portuguese or not
#ifndef SUP_PORTUGUESE
#define SUP_PORTUGUESE       ENABLE
#endif

//support Arabic
#ifndef SUP_ARABIC
#define SUP_ARABIC           DISABLE
#endif

#ifndef SUP_RUSSIA
#define SUP_RUSSIA           ENABLE
#endif

//most version  support Holland
//support Holland or not
#ifndef SUP_HOLLAND
#define SUP_HOLLAND           DISABLE
#endif

#ifndef SUP_TCHINESE
#define SUP_TCHINESE          ENABLE
#endif

#ifndef SUP_SCHINESE
#define SUP_SCHINESE          ENABLE
#endif

//support Japanese or not
#ifndef SUP_JAPANESE
#define SUP_JAPANESE         DISABLE
#endif

#ifndef SUP_CZECHESKY
#define SUP_CZECHESKY        DISABLE
#endif

#ifndef SUP_DANISHDANSK
#define SUP_DANISHDANSK      DISABLE
#endif

#ifndef SUP_GREEK
#define SUP_GREEK               DISABLE
#endif

#ifndef SUP_HUNGARIANMAGYAR
#define SUP_HUNGARIANMAGYAR     DISABLE
#endif

#ifndef SUP_NORSENORSK
#define SUP_NORSENORSK           DISABLE
#endif

#ifndef SUP_SWEDISHSVENSKA
#define SUP_SWEDISHSVENSKA       DISABLE
#endif

#ifndef SUP_FINNISHSUOMI
#define SUP_FINNISHSUOMI         DISABLE
#endif

#ifndef SUP_POLISHPOLSKI
#define SUP_POLISHPOLSKI         DISABLE
#endif

#ifndef SUP_TURKISHTRKE
#define SUP_TURKISHTRKE          DISABLE
#endif

#ifndef ANIMATION_LOGO
#define ANIMATION_LOGO          DISABLE
#endif

#endif
