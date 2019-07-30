#ifndef _H26X_ENCRYPT_H_
#define _H26X_ENCRYPT_H_

// AES context structure
typedef struct 
{
    unsigned int Ek[44];
    unsigned char Nr;
} EncCtx;

int EncCtxIni(EncCtx *pCtx, const unsigned char *pKey);
int Encrypt(EncCtx *pCtx, unsigned char *pt, unsigned char *ct);

extern const unsigned char h26x_enc_key[];
extern const unsigned int h26x_enc_key_size;

#endif //_H26X_ENCRYPT_H_