/*
 * (c) 1997-2010 Netflix, Inc.  All content herein is protected by
 * U.S. copyright and other applicable intellectual property laws and
 * may not be copied without the express permission of Netflix, Inc.,
 * which reserves all rights.  Reuse of any of this content for any
 * purpose without the permission of Netflix, Inc. is strictly
 * prohibited.
 */
#include <asm/byteorder.h>
#include <crypto/mpursa.h>
#include <crypto/mpusha.h>
#include <linux/crypto.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/syscalls.h>

#if defined(CONFIG_CRYPTO_NVT_CRYPT)	
#include <crypto/nvt_sha256.h>
#include <crypto/nvt_bignum.h>
#include <crypto/nvt_rsa.h>
#elif defined(CONFIG_CRYPTO_NVT_CRYPT_V2)
#include <crypto/nvt_crypto.h>
#endif

#include <linux/nfsb.h>

static const char MAGIC[] = {'N','F','S','B'};

static const ssize_t BYTES_PER_SECTOR = 512;

uint8_t nfsb_hash_type(const struct nfsb_header *h)
{
	return h->hash_type;
}

ssize_t nfsb_fs_offset(const struct nfsb_header *h)
{
	return ntohl(h->fs_offset) * BYTES_PER_SECTOR;
}

ssize_t nfsb_fs_size(const struct nfsb_header *h)
{
	return ntohl(h->fs_size) * BYTES_PER_SECTOR;
}

ssize_t nfsb_hash_size(const struct nfsb_header *h)
{
	return ntohl(h->hash_size) * BYTES_PER_SECTOR;
}

uint32_t nfsb_data_blockcount(const struct nfsb_header *h)
{
	return ntohl(h->data_blocks);
}

uint32_t nfsb_data_blocksize(const struct nfsb_header *h)
{
	return ntohl(h->data_blocksize);
}

const char *nfsb_hash_algo(const struct nfsb_header *h)
{
	return h->hash_algo;
}

uint32_t nfsb_hash_blocksize(const struct nfsb_header *h)
{
	return ntohl(h->hash_blocksize);
}

const char *nfsb_verity_salt(const struct nfsb_header *h)
{
	return h->verity_salt;
}

const char *nfsb_verity_hash(const struct nfsb_header *h)
{
	return h->verity_hash;
}

int nfsb_verify(const struct nfsb_header *h, const mpuint *key, const mpuint *n)
{
#if defined(CONFIG_CRYPTO_NVT_CRYPT)	
	unsigned char sha256_result[32] = {0};
	rsa_context rsa;
	int ret = -1;

	nvt_sha256((uint8_t*)h, sizeof(struct nfsb_header) - sizeof(h->rsa_sig), sha256_result);

	nvt_rsa_init( &rsa, RSA_PKCS_V15, 0 );

	if( ( ret = mpi_read_string( &rsa.N , 16, N ) ) != 0 ||
	    ( ret = mpi_read_string( &rsa.E , 16, E ) ) != 0) {
		printk( " failed\n  ! mpi_read_string returned %d\n\n", ret );
		goto out;
	}

	rsa.len = ( mpi_msb( &rsa.N ) + 7 ) >> 3;

	ret = nvt_rsa_verify(&rsa, sha256_result, h->rsa_sig);

out:
	return ret;
#elif defined(CONFIG_CRYPTO_NVT_CRYPT_V2)	
	unsigned char sha256_result[32] = {0};
	int ret = -1;
	mbedtls_pk_context pk;
	size_t key_len;

	mbedtls_pk_init( &pk );
	mbedtls_sha256((uint8_t*)h, sizeof(struct nfsb_header) - sizeof(h->rsa_sig), sha256_result, 0);

	key_len = strlen(pub_key);
	key_len++;
	ret = mbedtls_pk_parse_public_key(&pk, (const unsigned char*)pub_key, key_len);
	if(ret != 0) {
		printk( " failed\n  ! mpi_read_string returned %d\n\n", ret );
		goto out;
	}

	if(!mbedtls_pk_can_do(&pk, MBEDTLS_PK_RSA)) {
		printk(" failed\n ! invalid key \n\n");
		goto out;
	}

	mbedtls_rsa_set_padding( mbedtls_pk_rsa( pk ), MBEDTLS_RSA_PKCS_V21, MBEDTLS_MD_SHA256 );

	ret = mbedtls_pk_verify( &pk, MBEDTLS_MD_SHA256, (const unsigned char*)sha256_result, sizeof(sha256_result), (const unsigned char*)h->rsa_sig, sizeof(h->rsa_sig));

out:
	mbedtls_pk_free( &pk );
	return ret;
#else	
	struct crypto_hash *tfm = NULL;
	struct hash_desc desc;
	uint8_t *digest = NULL;
	struct scatterlist sg[1];
	int ret = -1;
	mpuint *sig = NULL, *msg = NULL, *dgst = NULL;
	unsigned sig_sz = sizeof(h->rsa_sig);
	static unsigned SHA256_SZ = 256 / 8;

	/* Initialize SHA-256. */
	tfm = crypto_alloc_hash("sha256", 0, 0);
	if (IS_ERR(tfm)) {
		printk(KERN_ERR "Failed to load transform for sha256: %ld\n",
			   PTR_ERR(tfm));
		goto failure;
	}
	desc.tfm = tfm;
	desc.flags = 0;

	/* SHA-256 the header, minus the signature memory. */
	digest = kmalloc(crypto_hash_digestsize(tfm), GFP_KERNEL);
	if (!digest) {
		printk(KERN_ERR "Failed to allocate digest memory.\n");
		goto failure;
	}
	ret = crypto_hash_init(&desc);
	if (ret) {
		printk(KERN_ERR "crypto_hash_init failure: %d\n", ret);
		goto failure;
	}
	sg_set_buf(sg, h, sizeof(struct nfsb_header) - sizeof(h->rsa_sig));
	ret = crypto_hash_update(&desc, sg, sizeof(struct nfsb_header) - sizeof(h->rsa_sig));
	if (ret) {
		printk(KERN_ERR "crypto_hash_update failure: %d\n", ret);
		goto failure;
	}
	ret = crypto_hash_final(&desc, digest);
	if (ret) {
		printk(KERN_ERR "crypto_hash_final failure: %d\n", ret);
		goto failure;
	}

	/* Reconstruct the SHA-256 from the header RSA-2048 signature. */
	sig = mpuint_alloc(sig_sz);
	if (!sig) {
		printk(KERN_ERR "Failed to allocate signature memory.\n");
		goto failure;
	}
	sig = mpuint_setbytes(sig, h->rsa_sig, sig_sz);
	if (!sig) {
		printk(KERN_ERR "Failed to set signature bytes.\n");
		goto failure;
	}
	msg = rsa_encryptdecrypt(sig, key, n);
	if (!msg) {
		printk(KERN_ERR "rsa_encryptdecrypt failure\n");
		goto failure;
	}
	msg = mpuint_trim(msg);
	if (!msg) {
		printk(KERN_ERR "mpuint_trim failure\n");
		goto failure;
	}

	/* Compare the digests. */
	dgst = mpuint_alloc(SHA256_SZ);
	if (!dgst) {
		printk(KERN_ERR "Failed to allocate digest integer memory.\n");
		goto failure;
	}
	dgst = mpuint_setbytes(dgst, digest, SHA256_SZ);
	if (!dgst) {
		printk(KERN_ERR "Failed to set digest bytes.\n");
		goto failure;
	}
	if (mpuint_eq(msg, dgst))
		ret = 0;
	else
		ret = -1;

 failure:
	mpuint_delete(dgst);
	mpuint_delete(msg);
	mpuint_delete(sig);
	kfree(digest);
	crypto_free_hash(tfm);
	return ret;
#endif	
}

ssize_t nfsb_read(int fd, struct nfsb_header *h)
{
	/* Once different versions are supported, this would need to change. */
	ssize_t r = 0, total = 0;
	while (total < sizeof(struct nfsb_header))
	{
		r = sys_read(fd, (char*)(h) + r, sizeof(struct nfsb_header) - r);
		if (r == -1) return -1;
		total += r;
	}
	/* Check magic. */
	if (memcmp(h->magic, MAGIC, sizeof(MAGIC)) != 0)
		return -1;
	/* Check version. */
	if (h->version != 3)
		return -1;

	return total;
}
