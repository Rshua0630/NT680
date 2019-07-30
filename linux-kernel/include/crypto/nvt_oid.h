/*
 * oid header file for nvt_crypto
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


#ifndef __NVT_OID_H__
#define __NVT_OID_H__

#if defined(__GLIBC__)
#include <stdlib.h>
#endif

typedef enum {
    CRYPTO_MD_NONE=0,
    CRYPTO_MD_MD2,
    CRYPTO_MD_MD4,
    CRYPTO_MD_MD5,
    CRYPTO_MD_SHA1,
    CRYPTO_MD_SHA224,
    CRYPTO_MD_SHA256,
    CRYPTO_MD_SHA384,
    CRYPTO_MD_SHA512,
    CRYPTO_MD_RIPEMD160,
} md_type_t;


typedef struct {
    const char *asn1;               /*!< OID ASN.1 representation       */
    size_t asn1_len;                /*!< length of asn1                 */
    const char *name;               /*!< official name (e.g. from RFC)  */
    const char *description;        /*!< human friendly description     */
} oid_descriptor_t;

typedef struct _asn1_buf
{
    int tag;                /**< ASN1 type, e.g. ASN1_UTF8_STRING. */
    size_t len;             /**< ASN1 length, e.g. in octets. */
    unsigned char *p;       /**< ASN1 data, e.g. in ASCII. */
}
asn1_buf;


#define CRYPTO_ERR_OID_NOT_FOUND                         -0x002E  /**< OID is not found. */

#define OID_ISO_ITU_COUNTRY             "\x60"          /* {joint-iso-itu-t(2) country(16)} */
#define OID_COUNTRY_US                  "\x86\x48"      /* {us(840)} */
#define OID_ORGANIZATION                "\x01"          /* {organization(1)} */
#define OID_ISO_ITU_US_ORG              OID_ISO_ITU_COUNTRY OID_COUNTRY_US OID_ORGANIZATION /* {joint-iso-itu-t(2) country(16) us(840) organization(1)} */
#define OID_ORG_GOV                     "\x65"          /* {gov(101)} */
#define OID_GOV                         OID_ISO_ITU_US_ORG OID_ORG_GOV /* {joint-iso-itu-t(2) country(16) us(840) organization(1) gov(101)} */
#define OID_DIGEST_ALG_SHA256           OID_GOV "\x03\x04\x02\x01" /**< id-sha256 OBJECT IDENTIFIER ::= { joint-iso-itu-t(2) country(16) us(840) organization(1) gov(101) csor(3) nistalgorithm(4) hashalgs(2) 1 } */


int oid_get_oid_by_md( md_type_t md_alg, const char **oid, size_t *olen );
int oid_get_md_alg( const asn1_buf *oid, md_type_t *md_alg );
#endif
