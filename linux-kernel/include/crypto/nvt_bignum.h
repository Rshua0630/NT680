/*
 * big number header file for nvt_crypto
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


#ifndef __NVT_BIGNUM_H__
#define __NVT_BIGNUM_H__

#include <stddef.h>
#include <linux/types.h>

#define CRYPTO_MPI_MAX_LIMBS                             10000
#define CRYPTO_ERR_MPI_BAD_INPUT_DATA                    -0x0004  /**< Bad input parameters to function. */
#define CRYPTO_ERR_MPI_INVALID_CHARACTER                 -0x0006  /**< There is an invalid character in the digit string. */
#define CRYPTO_ERR_MPI_BUFFER_TOO_SMALL                  -0x0008  /**< The buffer is too small to write to. */
#define CRYPTO_ERR_MPI_MALLOC_FAILED                     -0x0010  /**< Memory allocation failed. */
#define CRYPTO_ERR_MPI_NEGATIVE_VALUE                    -0x000A  /**< The input arguments are negative or result in illegal output. */
#define CRYPTO_ERR_MPI_DIVISION_BY_ZERO                  -0x000C  /**< The input argument for division is zero, which is not allowed. */
#define CRYPTO_ERR_MPI_NOT_ACCEPTABLE                    -0x000E  /**< The input arguments are not acceptable. */
#define CRYPTO_MPI_WINDOW_SIZE                           6        /**< Maximum windows size used. */
#define CRYPTO_MPI_MAX_SIZE                              512      /**< Maximum number of bytes for usable MPIs. */

#if defined(__amd64__) || defined (__x86_64__)
typedef uint64_t t_uint;
typedef int64_t t_sint;
#endif

#if defined(__arm__)
typedef uint32_t t_uint;
typedef int32_t t_sint;
#endif

typedef struct
{
    int s;              /*!<  integer sign      */
    size_t n;           /*!<  total # of limbs  */
    t_uint *p;          /*!<  pointer to limbs  */
}
mpi;


#define MPI_CHK(f) do { if( ( ret = f ) != 0 ) goto cleanup; } while( 0 )

void mpi_init( mpi *X );
int mpi_mul_int( mpi *X, const mpi *A, t_sint b );
int mpi_add_int( mpi *X, const mpi *A, t_sint b );
int mpi_sub_int( mpi *X, const mpi *A, t_sint b );
int mpi_mod_int( t_uint *r, const mpi *A, t_sint b );
int mpi_div_int( mpi *Q, mpi *R, const mpi *A, t_sint b );
int mpi_cmp_int( const mpi *X, t_sint z );
int mpi_read_binary( mpi *X, const unsigned char *buf, size_t buflen );
int mpi_cmp_mpi( const mpi *X, const mpi *Y );
void mpi_free( mpi *X );
int mpi_exp_mod( mpi *X, const mpi *A, const mpi *E, const mpi *N, mpi *_RR );
int mpi_sub_mpi( mpi *X, const mpi *A, const mpi *B );
int mpi_mul_mpi( mpi *X, const mpi *A, const mpi *B );
int mpi_mod_mpi( mpi *R, const mpi *A, const mpi *B );
int mpi_add_mpi( mpi *X, const mpi *A, const mpi *B );
int mpi_write_binary( const mpi *X, unsigned char *buf, size_t buflen );
int mpi_read_string( mpi *X, int radix, const char *s );
size_t mpi_msb( const mpi *X );

#endif
