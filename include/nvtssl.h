/**

    This header file of nvtssl APIs.

    @file       nvtssl.h
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

*/

#ifndef _NVTSSL_H
#define _NVTSSL_H
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned int          NVTSSL_CTX;
typedef unsigned int          NVTSSL;

typedef struct {
    unsigned int data[32];
} NVTMD5_CTX;


#define NVTSSL_DISPLAY_CERTS  1
#define NVTSSL_OK             0
#define NVTSSL_FAIL          -1

int nvtssl_init(void);

NVTSSL_CTX* nvtssl_ctx_new(unsigned int options, int num_sessions);

ssize_t nvtssl_read(NVTSSL* ssl, char *buf, size_t buffersize);

ssize_t nvtssl_write(NVTSSL* ssl, char *buf, size_t buffersize);

ssize_t nvtssl_recv(NVTSSL* ssl, char *buf, size_t buffersize, int flags);

ssize_t nvtssl_send(NVTSSL* ssl, char *buf, size_t buffersize, int flags);


/**
 * @brief (server only) Establish a new SSL connection to an SSL client.
 *
 * It is up to the application to establish the logical connection (whether it
 * is  a socket, serial connection etc).
 * @param ssl_ctx [in] The server context.
 * @param client_fd [in] The client's file descriptor.
 * @return An SSL object reference.
 */
NVTSSL* nvtssl_server_new(NVTSSL_CTX* ctx,int fd);


int nvtssl_accept(NVTSSL* ssl);

void nvtssl_free(NVTSSL* ssl);

void nvtssl_ctx_free(NVTSSL_CTX* ctx);

const char*  nvtssl_get_tls_version(NVTSSL* ssl);

const char*  nvtssl_get_current_cipher(NVTSSL* ssl);

int nvtssl_MD5_Init(NVTMD5_CTX *md5ctx);

int nvtssl_MD5_Update(NVTMD5_CTX *md5ctx, const uint8_t *msg, int len);

int nvtssl_MD5_Final(NVTMD5_CTX *md5ctx, uint8_t *digest);

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTSSL_H  */
