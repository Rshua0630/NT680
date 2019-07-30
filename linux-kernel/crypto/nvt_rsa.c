/*
 * rsa sign and verify functions for nvt_crypto
 *
 * Copyright (c) 2014, Novatek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */


#include <linux/string.h>
#include <crypto/nvt_bignum.h>
#include <crypto/nvt_rsa.h>
#include <crypto/nvt_oid.h>

#define CRYPTO_ERR_ASN1_OUT_OF_DATA                      -0x0060  /**< Out of data when parsing an ASN1 data structure. */
#define CRYPTO_ERR_ASN1_UNEXPECTED_TAG                   -0x0062  /**< ASN1 tag was of an unexpected value. */
#define CRYPTO_ERR_ASN1_INVALID_LENGTH                   -0x0064  /**< Error when trying to determine the length or invalid length. */
#define CRYPTO_ERR_ASN1_LENGTH_MISMATCH                  -0x0066  /**< Actual length differs from expected length. */
#define CRYPTO_ERR_ASN1_INVALID_DATA                     -0x0068  /**< Data is invalid. (not used) */
#define CRYPTO_ERR_ASN1_MALLOC_FAILED                    -0x006A  /**< Memory allocation failed */
#define CRYPTO_ERR_ASN1_BUF_TOO_SMALL                    -0x006C  /**< Buffer too small when writing ASN.1 data structure. */



static void rsa_set_padding( rsa_context *ctx, int padding, int hash_id )
{
    ctx->padding = padding;
    ctx->hash_id = hash_id;
}


void nvt_rsa_init( rsa_context *ctx, int padding, int hash_id )
{
    memset( ctx, 0, sizeof( rsa_context ) );

    rsa_set_padding( ctx, padding, hash_id );

}
static int asn1_get_len( unsigned char **p, const unsigned char *end, size_t *len )
{
    if( ( end - *p ) < 1 )
        return( CRYPTO_ERR_ASN1_OUT_OF_DATA );

    if( ( **p & 0x80 ) == 0 )
        *len = *(*p)++;
    else
    {
        switch( **p & 0x7F )
        {
        case 1:
            if( ( end - *p ) < 2 )
                return( CRYPTO_ERR_ASN1_OUT_OF_DATA );

            *len = (*p)[1];
            (*p) += 2;
            break;

        case 2:
            if( ( end - *p ) < 3 )
                return( CRYPTO_ERR_ASN1_OUT_OF_DATA );

            *len = ( (*p)[1] << 8 ) | (*p)[2];
            (*p) += 3;
            break;

        case 3:
            if( ( end - *p ) < 4 )
                return( CRYPTO_ERR_ASN1_OUT_OF_DATA );

            *len = ( (*p)[1] << 16 ) | ( (*p)[2] << 8 ) | (*p)[3];
            (*p) += 4;
            break;

        case 4:
            if( ( end - *p ) < 5 )
                return( CRYPTO_ERR_ASN1_OUT_OF_DATA );

            *len = ( (*p)[1] << 24 ) | ( (*p)[2] << 16 ) | ( (*p)[3] << 8 ) |
                   (*p)[4];
            (*p) += 5;
            break;

        default:
            return( CRYPTO_ERR_ASN1_INVALID_LENGTH );
        }
    }

    if( *len > (size_t) ( end - *p ) )
        return( CRYPTO_ERR_ASN1_OUT_OF_DATA );

    return( 0 );
}

static int asn1_get_tag( unsigned char **p, const unsigned char *end, size_t *len, int tag )
{
    if( ( end - *p ) < 1 )
        return( CRYPTO_ERR_ASN1_OUT_OF_DATA );

    if( **p != tag )
        return( CRYPTO_ERR_ASN1_UNEXPECTED_TAG );

    (*p)++;

    return( asn1_get_len( p, end, len ) );
}


static int rsa_private( rsa_context *ctx, const unsigned char *input, unsigned char *output )
{
    int ret;
    size_t olen;
    mpi T, T1, T2;

    /*
     * When using the Chinese Remainder Theorem, we use blinding values.
     * Without threading, we just read them directly from the context,
     * otherwise we make a local copy in order to reduce locking contention.
     */

    mpi_init( &T ); mpi_init( &T1 ); mpi_init( &T2 );

    MPI_CHK( mpi_read_binary( &T, input, ctx->len ) );
    if( mpi_cmp_mpi( &T, &ctx->N ) >= 0 )
    {
        mpi_free( &T );
        return( CRYPTO_ERR_RSA_BAD_INPUT_DATA );
    }

    /*
     * faster decryption using the CRT
     *
     * T1 = input ^ dP mod P
     * T2 = input ^ dQ mod Q
     */
    MPI_CHK( mpi_exp_mod( &T1, &T, &ctx->DP, &ctx->P, &ctx->RP ) );
    MPI_CHK( mpi_exp_mod( &T2, &T, &ctx->DQ, &ctx->Q, &ctx->RQ ) );

    /*
     * T = (T1 - T2) * (Q^-1 mod P) mod P
     */
    MPI_CHK( mpi_sub_mpi( &T, &T1, &T2 ) );
    MPI_CHK( mpi_mul_mpi( &T1, &T, &ctx->QP ) );
    MPI_CHK( mpi_mod_mpi( &T, &T1, &ctx->P ) );

    /*
     * T = T2 + T * Q
     */
    MPI_CHK( mpi_mul_mpi( &T1, &T, &ctx->Q ) );
    MPI_CHK( mpi_add_mpi( &T, &T2, &T1 ) );

    olen = ctx->len;
    MPI_CHK( mpi_write_binary( &T, output, olen ) );

cleanup:
    mpi_free( &T ); mpi_free( &T1 ); mpi_free( &T2 );

    if( ret != 0 )
        return( CRYPTO_ERR_RSA_PRIVATE_FAILED + ret );

    return( 0 );
}

static int rsa_public( rsa_context *ctx, const unsigned char *input, unsigned char *output )
{
    int ret;
    size_t olen;
    mpi T;

    mpi_init( &T );

    MPI_CHK( mpi_read_binary( &T, input, ctx->len ) );

    if( mpi_cmp_mpi( &T, &ctx->N ) >= 0 )
    {
        mpi_free( &T );
        return( CRYPTO_ERR_RSA_BAD_INPUT_DATA );
    }

    olen = ctx->len;
    MPI_CHK( mpi_exp_mod( &T, &T, &ctx->E, &ctx->N, &ctx->RN ) );
    MPI_CHK( mpi_write_binary( &T, output, olen ) );

cleanup:

    mpi_free( &T );

    if( ret != 0 )
        return( CRYPTO_ERR_RSA_PUBLIC_FAILED + ret );

    return( 0 );
}



int nvt_rsa_sign( rsa_context *ctx, const unsigned char *hash, unsigned char *sig )
{
	size_t nb_pad, olen, oid_size = 0;
	unsigned char *p = sig;
	const char *oid;
	unsigned int hashlen;


	olen = ctx->len;
	nb_pad = olen - 3;


	if( oid_get_oid_by_md( CRYPTO_MD_SHA256, &oid, &oid_size ) != 0 )
		return( CRYPTO_ERR_RSA_BAD_INPUT_DATA );

	nb_pad -= 10 + oid_size;

	hashlen = 32;

	nb_pad -= hashlen;

	if( ( nb_pad < 8 ) || ( nb_pad > olen ) )
		return( CRYPTO_ERR_RSA_BAD_INPUT_DATA );

	*p++ = 0;
	*p++ = RSA_SIGN;
	memset( p, 0xFF, nb_pad );
	p += nb_pad;
	*p++ = 0;

	{
		/*
		 * DigestInfo ::= SEQUENCE {
		 *   digestAlgorithm DigestAlgorithmIdentifier,
		 *   digest Digest }
		 *
		 * DigestAlgorithmIdentifier ::= AlgorithmIdentifier
		 *
		 * Digest ::= OCTET STRING
		 */
		*p++ = ASN1_SEQUENCE | ASN1_CONSTRUCTED;
		*p++ = (unsigned char) ( 0x08 + oid_size + hashlen );
		*p++ = ASN1_SEQUENCE | ASN1_CONSTRUCTED;
		*p++ = (unsigned char) ( 0x04 + oid_size );
		*p++ = ASN1_OID;
		*p++ = oid_size & 0xFF;
		memcpy( p, oid, oid_size );
		p += oid_size;
		*p++ = ASN1_NULL;
		*p++ = 0x00;
		*p++ = ASN1_OCTET_STRING;
		*p++ = hashlen;
		memcpy( p, hash, hashlen );
	}

	return rsa_private( ctx, sig, sig );
}

int nvt_rsa_verify( rsa_context *ctx, const unsigned char *hash, const unsigned char *sig )
{
    int ret;
    size_t len, siglen, asn1_len;
    unsigned char *p, *end;
    unsigned char buf[CRYPTO_MPI_MAX_SIZE];
    md_type_t msg_md_alg;
    asn1_buf oid;
    unsigned int hashlen;


    siglen = ctx->len;


    ret = rsa_public(  ctx, sig, buf );

    if( ret != 0 )
        return( ret );

    p = buf;

    if( *p++ != 0 || *p++ != RSA_SIGN )
        return( CRYPTO_ERR_RSA_INVALID_PADDING );

    while( *p != 0 )
    {
        if( p >= buf + siglen - 1 || *p != 0xFF )
            return( CRYPTO_ERR_RSA_INVALID_PADDING );
        p++;
    }
    p++;

    len = siglen - ( p - buf );

    hashlen = 32;

    end = p + len;

    // Parse the ASN.1 structure inside the PKCS#1 v1.5 structure
    //
    if( ( ret = asn1_get_tag( &p, end, &asn1_len,
            ASN1_CONSTRUCTED | ASN1_SEQUENCE ) ) != 0 )
        return( CRYPTO_ERR_RSA_VERIFY_FAILED );

    if( asn1_len + 2 != len )
        return( CRYPTO_ERR_RSA_VERIFY_FAILED );

    if( ( ret = asn1_get_tag( &p, end, &asn1_len,
            ASN1_CONSTRUCTED | ASN1_SEQUENCE ) ) != 0 )
        return( CRYPTO_ERR_RSA_VERIFY_FAILED );

    if( asn1_len + 6 + hashlen != len )
        return( CRYPTO_ERR_RSA_VERIFY_FAILED );

    if( ( ret = asn1_get_tag( &p, end, &oid.len, ASN1_OID ) ) != 0 )
        return( CRYPTO_ERR_RSA_VERIFY_FAILED );

    oid.p = p;
    p += oid.len;

    if( oid_get_md_alg( &oid, &msg_md_alg ) != 0 )
        return( CRYPTO_ERR_RSA_VERIFY_FAILED );

    if( CRYPTO_MD_SHA256 != msg_md_alg )
        return( CRYPTO_ERR_RSA_VERIFY_FAILED );

    /*
     * assume the algorithm parameters must be NULL
     */
    if( ( ret = asn1_get_tag( &p, end, &asn1_len, ASN1_NULL ) ) != 0 )
        return( CRYPTO_ERR_RSA_VERIFY_FAILED );

    if( ( ret = asn1_get_tag( &p, end, &asn1_len, ASN1_OCTET_STRING ) ) != 0 )
        return( CRYPTO_ERR_RSA_VERIFY_FAILED );

    if( asn1_len != hashlen )
        return( CRYPTO_ERR_RSA_VERIFY_FAILED );

    if( memcmp( p, hash, hashlen ) != 0 )
        return( CRYPTO_ERR_RSA_VERIFY_FAILED );

    p += hashlen;

    if( p != end )
        return( CRYPTO_ERR_RSA_VERIFY_FAILED );

    return( 0 );
}

