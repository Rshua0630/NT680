#ifndef _NVTAUTH_H
#define _NVTAUTH_H
/*
    Copyright (c) 2015~  Novatek Microelectronics Corporation

    @file nvtauth.h

    @version

    @date
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <openssl/md5.h>
#include <nvtcfg_define.h>
#include <nvt_type.h>

#ifdef __cplusplus
extern "C" {
#endif

#define URI_LEN_MAX         1024
#define DOMAIN_NAME_LEN_MAX 128
#define HTTP_RETURN_BUF_LEN 512
#define HTTP_AUTH_NONCE_LEN 32

#define NONCE_DEFAULT_STALE_SEC     120

#define HTTP_STATUS_200     "200 OK"
#define HTTP_STATUS_400     "400 Bad Request"
#define HTTP_STATUS_401     "401 Unauthorized"

typedef enum {
    APPLICATION_PROTOCOL_HTTP,
    APPLICATION_PROTOCOL_RTSP,
    APPLICATION_PROTOCOL_RTSP_OVER_HTTP,
    APPLICATION_PROTOCOL_UNKNOWN,
} APPLICATION_PROTOCOL;

typedef struct {
    CHAR cMethod[64];
    CHAR cURI[URI_LEN_MAX+1];
    CHAR cProtocol[32];
    CHAR cCSeq[16];
    BOOL bAuthInfo;
    CHAR cAuthUserName[USER_NAME_MAX_LEN+1];
    CHAR cAuthRealm[128];
    CHAR cAuthMethod[16];
    CHAR cAuthNonce[HTTP_AUTH_NONCE_LEN+1];
    CHAR cCNonce[HTTP_AUTH_NONCE_LEN*2+1];
    CHAR cNonceCount[HTTP_AUTH_NONCE_LEN+1];
    CHAR cAuthResponse[HTTP_AUTH_NONCE_LEN+1];
    CHAR cAuthURI[URI_LEN_MAX+1];
    BOOL bAuthStale;
    APPLICATION_PROTOCOL eAppProtocol;
    BOOL bAuthSkip;
} HTTPHeaderInfo;

typedef enum {
    NVTAUTH_ER_NONCE_TABLE_FAIL=-7,
    NVTAUTH_ER_TOKEN_NOT_FOUND=-6,
    NVTAUTH_ER_EXCEED_MAX_USER_NUMBER=-5,
    NVTAUTH_ER_USER_NOT_EXIST=-4,
    NVTAUTH_ER_INVALID_INPUT_DATA=-3,
    NVTAUTH_ER_BUFFER_OVERFLOW=-2,
    NVTAUTH_ER_ERR=-1,
    NVTAUTH_ER_OK=0,
    NVTAUTH_ER_HTTP_OK=200,
    NVTAUTH_ER_HTTP_BAD_REQUEST=400,
    NVTAUTH_ER_HTTP_UNAUTHORIZED=401,
} NVTAUTH_ER_CODE;

/**
    Setup authentication domain.

    This API will setup authentication domain string and use as realm in digest authentication.

    @param[in] cDomain            Authentication domain string.

    @note Domain string maximum size is DOMAIN_NAME_LEN_MAX.

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTAUTH_ER_INVALID_INPUT_DATA if domain string is empty or too long

*/
extern NVTRET NVTAuth_SetDomain(const CHAR *cDomain);

/**
    Send HTTP header to authenticate.

    This API will authenticate input HTTP header and return result.

    @param[in]  cHTTPHeader       HTTP header.
    @param[in]  uiHTTPHeaderLen   HTTP header length.
    @param[out] cRetBuf           Return buffer to store return message.
                                  (The end is single "/r/n", must add another "/r/n" before send by yourself)
    @param[in]  uiBufLen          Return buffer size.
    @param[out] eUserLevel        Return user level if authenticate successfully.

    @note Size of cRetBuf is recommend as HTTP_RETURN_BUF_LEN.

    @return
        - @b NVTAUTH_ER_HTTP_OK if authenticate successfully
        - @b NVTAUTH_ER_BUFFER_OVERFLOW if cRetBuf overflow, please increase buffer size
        - @b NVTAUTH_ER_HTTP_BAD_REQUEST if authentication information is incomplete
        - @b NVTAUTH_ER_HTTP_UNAUTHORIZED if authenticate fail

*/
extern NVTRET NVTAuth_HTTPAuth(const CHAR *cHTTPHeader, const UINT32 uiHTTPHeaderLen, CHAR *cRetBuf, UINT32 uiRetBufLen, enum tt__UserLevel *eUserLevel);

/**
    Modify user data.

    This API will modify user data when this user exist and add user when this user not exist.

    @param[in] sUserProfile       User profile include user name, password, and user level.

    @note User list maximum size is USER_NUMBER_MAX.

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTAUTH_ER_INVALID_INPUT_DATA if sUserProfile include incorrect data
        - @b NVTAUTH_ER_EXCEED_MAX_USER_NUMBER if user list is full

*/
extern NVTRET NVTAuth_ModifyUser(const ONVIF_USER sUserProfile);

/**
    Remove user data.

    This API will remove user data.

    @param[in] cUserName          User name to be removed.

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTAUTH_ER_INVALID_INPUT_DATA if cUserName is NULL
        - @b NVTAUTH_ER_USER_NOT_EXIST if user not exist

*/
extern NVTRET NVTAuth_RemoveUser(const CHAR *cUserName);

/**
    Get user list.

    This API will return user list.

    @param[out] sUserProfile      User profile array to store user list.
    @param[out] uiUserNumber      Indicate current user number.

    @return
        - @b NVTUCTRL_ER_OK if success
        - @b NVTAUTH_ER_OK if fail

*/
extern NVTRET NVTAuth_GetUserList(ONVIF_USER sUserProfile[USER_MAX_NUM], UINT32 *uiUserNumber);

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTAUTH_H  */
