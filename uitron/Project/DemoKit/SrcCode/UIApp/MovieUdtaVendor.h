#ifndef _MOVIE_UDTA_VENDOR_H
#define _MOVIE_UDTA_VENDOR_H

// Maker User Data
extern BOOL MovieUdta_MakeVendorUserData(UINT32 *UDtaAddr, UINT32 *UDtaSize);
// Parse User Data & Check Model/Maker Name
extern BOOL MovieUdta_ParseVendorUserData(UINT32 UDtaAddr, UINT32 UDtaSize);

#endif//_MOVIE_UDTA_VENDOR_H
