/*
 * (c) 1997-2010 Netflix, Inc.  All content herein is protected by
 * U.S. copyright and other applicable intellectual property laws and
 * may not be copied without the express permission of Netflix, Inc.,
 * which reserves all rights.  Reuse of any of this content for any
 * purpose without the permission of Netflix, Inc. is strictly
 * prohibited.
 */
#include <linux/device-mapper.h>
#include <linux/random.h>

#include "do_mounts.h"
#include "../drivers/md/dm.h"
#include <linux/nfsb.h>

static char *DEV_LINEAR = "/dev/linear";
static char *DEV_NFSB = "/dev/nfsb";

/* RSA-2048 modulus, big-endian. Replace with your real value. */
static uint8_t rsa_modulus[256] =
{
	   0xb0, 0x74, 0x93, 0x2a, 0x08, 0xae, 0x9a, 0xbe, 
	   0x67, 0x58, 0x8d, 0x6f, 0x45, 0xe8, 0x6f, 0x46, 
	   0xc1, 0x98, 0x59, 0x78, 0xdd, 0xc1, 0x77, 0x4a, 
	   0x0b, 0xfc, 0x69, 0xcd, 0x26, 0x9e, 0x8a, 0x90, 
	   0x4d, 0x8f, 0x53, 0xda, 0xc6, 0x84, 0x3c, 0xc2, 
	   0x58, 0x59, 0x94, 0xf1, 0xe8, 0x77, 0x83, 0xb1, 
	   0x75, 0xb6, 0xf3, 0x9a, 0x91, 0xaa, 0x10, 0x5e, 
	   0x0d, 0xb8, 0xed, 0xaf, 0xb8, 0x77, 0xfd, 0x23, 
	   0x7d, 0x3a, 0x66, 0x88, 0x4f, 0x5f, 0x5a, 0x57, 
	   0x26, 0x9c, 0x0e, 0x0d, 0x03, 0x6e, 0xe5, 0xee, 
	   0x81, 0x84, 0xfe, 0x1e, 0x46, 0x08, 0xbf, 0x2d, 
	   0x1b, 0xf3, 0x68, 0xc7, 0x79, 0x02, 0x77, 0x29, 
	   0x12, 0x18, 0xbf, 0x43, 0x95, 0x0f, 0x6a, 0x32, 
	   0x2e, 0x8e, 0x0e, 0x2c, 0x7f, 0x7b, 0x63, 0xd7, 
	   0xb3, 0xf8, 0x77, 0x15, 0xba, 0xd3, 0x78, 0x83, 
	   0x92, 0x85, 0xcc, 0xe1, 0x26, 0x5c, 0xfb, 0x13, 
	   0xe7, 0x7d, 0xd0, 0x3c, 0x04, 0x9e, 0x73, 0xe6, 
	   0x5a, 0xdf, 0x52, 0xd0, 0xaf, 0xc9, 0x57, 0x80, 
	   0xfa, 0x4a, 0x63, 0x5d, 0x86, 0x3f, 0xdb, 0xd9, 
	   0x5e, 0xb3, 0x8a, 0x41, 0x1f, 0x84, 0x3c, 0x1b, 
	   0xa9, 0x62, 0x59, 0xd6, 0x92, 0x7c, 0x73, 0xd2, 
	   0xc2, 0x92, 0xf9, 0xf7, 0xdc, 0x66, 0xf5, 0x2e, 
	   0xec, 0xe6, 0x04, 0x12, 0x8c, 0x82, 0x14, 0xaa, 
	   0x31, 0xcf, 0xe8, 0xde, 0x98, 0xcb, 0x17, 0xf8, 
	   0x91, 0x55, 0x39, 0x0a, 0xb1, 0x74, 0xeb, 0xc6, 
	   0xb8, 0x73, 0xe3, 0xd2, 0x3d, 0xc2, 0x6d, 0xdf, 
	   0x6f, 0xc2, 0xda, 0x87, 0x7b, 0x8c, 0xbf, 0x9d, 
	   0x47, 0x91, 0xe8, 0x6b, 0xe8, 0xb8, 0xd8, 0xa8, 
	   0xf9, 0x3d, 0x80, 0x5d, 0x5d, 0x59, 0x64, 0x25, 
	   0x7e, 0x82, 0x26, 0x28, 0x05, 0xc7, 0xc1, 0x2a, 
	   0x24, 0x96, 0x8b, 0x13, 0x28, 0xd0, 0x1c, 0x7b, 
	   0x89, 0x75, 0x71, 0x52, 0x7a, 0x2b, 0xda, 0x13

};


/* RSA-2048 public key exponent, big-endian. Replace with your real value. */
static uint8_t rsa_pubkey[256] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01
};

static int __init dm_check_nfsb(struct nfsb_header *hdr, dev_t dev)
{
#if defined(CONFIG_CRYPTO_NVT_CRYPT) || defined(CONFIG_CRYPTO_NVT_CRYPT_V2)
	int err = 0;
	int root_fd = -1;
	ssize_t sz = -1;

	/* Create a temporary root device. */
	err = create_dev(DEV_NFSB, dev);
	if (err < 0) {
		printk(KERN_ERR "nfsb: create_dev failed\n");
		goto create_fail;
	}
	
	/* Try to read an NFSB header from the root device. */
	root_fd = sys_open(DEV_NFSB, O_RDONLY, 0);
	if (root_fd < 0) {
		printk(KERN_ERR "nfsb: open %s failed\n", DEV_NFSB);
		goto open_fail;
	}
	sz = nfsb_read(root_fd, hdr);
	if (sz == -1)
		goto read_fail;

	/* Verify the NFSB header. */

	if (nfsb_verify(hdr, NULL, NULL) != 0) {
		printk(KERN_ERR "nfsb: header verification failed\n");
		goto verify_fail;
	}
	printk(KERN_INFO "NFSB header found and verified.\n");

	sys_close(root_fd);
	sys_unlink(DEV_NFSB);
	return 1;

read_fail:
verify_fail:
	sys_close(root_fd);
open_fail:
	sys_unlink(DEV_NFSB);
create_fail:
	return 0;
#else	
	int err = 0;
	int root_fd = -1;
	ssize_t sz = -1;
	mpuint *pubkey = NULL, *modulus = NULL;

	/* Create a temporary root device. */
	err = create_dev(DEV_NFSB, dev);
	if (err < 0) {
		printk(KERN_ERR "nfsb: create_dev failed\n");
		goto create_fail;
	}
	
	/* Try to read an NFSB header from the root device. */
	root_fd = sys_open(DEV_NFSB, O_RDONLY, 0);
	if (root_fd < 0) {
		printk(KERN_ERR "nfsb: open %s failed\n", DEV_NFSB);
		goto open_fail;
	}
	sz = nfsb_read(root_fd, hdr);
	if (sz == -1)
		goto read_fail;

	/* Verify the NFSB header. */
	pubkey = mpuint_alloc(sizeof(rsa_pubkey));
	if (!pubkey) {
		printk(KERN_ERR "out of memory\n");
		goto pubkey_fail;
	}
	pubkey = mpuint_setbytes(pubkey, rsa_pubkey, sizeof(rsa_pubkey));
	if (!pubkey) {
		printk(KERN_ERR "mpuint_setbytes failed\n");
		goto pubkey_fail;
	}
	modulus = mpuint_alloc(sizeof(rsa_modulus));
	if (!modulus) {
		printk(KERN_ERR "out of memory\n");
		goto modulus_fail;
	}
	modulus = mpuint_setbytes(modulus, rsa_modulus, sizeof(rsa_modulus));
	if (!modulus) {
		printk(KERN_ERR "mpuint_setbytes failed\n");
		goto modulus_fail;
	}
	if (nfsb_verify(hdr, pubkey, modulus) != 0) {
		printk(KERN_ERR "nfsb: header verification failed\n");
		goto verify_fail;
	}
	printk(KERN_INFO "NFSB header found and verified.\n");

	/* Success. */
	mpuint_delete(modulus);
	mpuint_delete(pubkey);
	sys_close(root_fd);
	sys_unlink(DEV_NFSB);
	return 1;

verify_fail:
modulus_fail:
	mpuint_delete(modulus);
pubkey_fail:
	mpuint_delete(pubkey);
read_fail:
	sys_close(root_fd);
open_fail:
	sys_unlink(DEV_NFSB);
create_fail:
	return 0;
#endif	
}

/* From drivers/md/dm.h */
#define DM_SUSPEND_NOFLUSH_FLAG         (1 << 1)
void dm_destroy(struct mapped_device *md);
int dm_table_alloc_md_mempools(struct dm_table *t);
void dm_table_destroy(struct dm_table *t);
int dm_table_set_type(struct dm_table *t);

static int __init dm_setup_linear(struct nfsb_header *hdr, dev_t *dev)
{
	int linear_fd = -1;
	unsigned long linear_size = 0;
	struct mapped_device *md = NULL;
	struct dm_table *table = NULL, *old_map = NULL;
	char *target_params = NULL;
	ssize_t fs_offset = to_sector(nfsb_fs_offset(hdr));

	/* Create linear device. */
	if (create_dev(DEV_LINEAR, *dev) < 0) {
		printk(KERN_ERR "Failed to create linear device\n");
		goto create_dev_fail;
	}

	/* Size block device. */
	linear_fd = sys_open(DEV_LINEAR, O_RDONLY, 0);
	if (linear_fd < 0) {
		printk(KERN_ERR "Failed to open linear device\n");
		goto sys_open_fail;
	}
	if (sys_ioctl(linear_fd, BLKGETSIZE, (unsigned long)&linear_size) < 0) {
		printk(KERN_ERR "Failed to get linear device size\n");
		goto sys_ioctl_fail;
	}

	/* Create a new mapped device. */
	if (dm_create(DM_ANY_MINOR, &md)) {
		printk(KERN_ERR "Failed to create mapped device\n");
		goto create_fail;
	}

	/* Explicitly set read-only. */
	set_disk_ro(dm_disk(md), 1);

	/* Create a mapped device table. */
	if (dm_table_create(&table, FMODE_READ, 1, md)) {
		printk(KERN_ERR "Failed to create table\n");
		goto table_create_fail;
	}

	dm_lock_md_type(md);
	/* Add the linear target to the table. */
	target_params = kmalloc(128, GFP_KERNEL);
	snprintf(target_params, 128, "%s %u", DEV_LINEAR, fs_offset);
	if (dm_table_add_target(table, "linear", 0, linear_size - fs_offset, target_params)) {
		printk(KERN_ERR "Failed to add linear target to the table\n");
		goto add_target_fail;
	}

	/* Table is complete. */
	if (dm_table_complete(table)) {
		printk(KERN_ERR "Failed to complete NFSB table\n");
		goto table_complete_fail;
	}

	if (dm_get_md_type(md) == DM_TYPE_NONE) {
		dm_set_md_type(md, dm_table_get_type(table));
		if (dm_setup_md_queue(md)) {
			pr_warn("unable to set up device queue for new table.");
			goto setup_md_queue_fail;
		}
	} else if (dm_get_md_type(md) != dm_table_get_type(table)) {
		pr_warn("can't change device type after initial table load.");
		goto setup_md_queue_fail;
        }


	/* Suspend the device so that we can bind it to the table. */
	if (dm_suspend(md, DM_SUSPEND_NOFLUSH_FLAG)) {
		printk(KERN_ERR "Failed to suspend the device pre-bind\n");
		goto suspend_fail;
	}

	/* Bind the table to the device. This is the only way to associate
	 * md->map with the table and set the disk capacity. Ouch. */
	old_map = dm_swap_table(md, table);
	if (IS_ERR(old_map)) {
		printk(KERN_ERR "Failed to bind the device to the table\n");
		dm_table_destroy(table);
		goto swap_table_fail;
	}
	
	/* Finally, resume and the device should be ready. */
	if (dm_resume(md)) {
		printk(KERN_ERR "Failed to resume the device\n");
		goto resume_fail;
	}

	/* Success. */
	*dev = disk_devt(dm_disk(md));
	printk(KERN_INFO "dm: target linear of size %llu on %s(%i:%i) is ready\n",
		   dm_table_get_size(table), dm_device_name(md),
		   MAJOR(*dev), MINOR(*dev));
	if (old_map)
		dm_table_destroy(old_map);
	dm_unlock_md_type(md);
	kfree(target_params);
	sys_close(linear_fd);
	return 1;

resume_fail:
	if (old_map)
		dm_table_destroy(old_map);
swap_table_fail:
suspend_fail:
setup_md_queue_fail:
table_complete_fail:
add_target_fail:
	dm_unlock_md_type(md);
	kfree(target_params);
	dm_table_destroy(table);
table_create_fail:
	dm_put(md);
	dm_destroy(md);
create_fail:
sys_ioctl_fail:
	sys_close(linear_fd);
sys_open_fail:
create_dev_fail:
	return 0;
}

static dev_t __init dm_setup_nfsb(struct nfsb_header *hdr, dev_t *dev)
{
	struct mapped_device *md = NULL;
	struct dm_table *table = NULL, *old_map = NULL;
	char *target_params = NULL;
	ssize_t fs_size = nfsb_fs_size(hdr);
	uint32_t data_blocksize = nfsb_data_blocksize(hdr);
	uint32_t hash_blocksize = nfsb_hash_blocksize(hdr);

	/* Verify data and hash start block indexes. */
	if (fs_size % hash_blocksize) {
		printk(KERN_ERR "Hash start is not a multiple of the hash block size.\n");
		goto hash_start_fail;
	}

	/* Create NFSB device. */
	if (create_dev(DEV_NFSB, *dev) < 0) {
		printk(KERN_ERR "Failed to create NFSB root device\n");
		goto create_dev_fail;
	}
	
	/* Create a new mapped device. */
	if (dm_create(DM_ANY_MINOR, &md)) {
		printk(KERN_ERR "Failed to create mapped device\n");
		goto create_fail;
	}

	/* Explicitly set read-only. */
	set_disk_ro(dm_disk(md), 1);

	/* Create a mapped device table. */
	if (dm_table_create(&table, FMODE_READ, 1, md)) {
		printk(KERN_ERR "Failed to create table\n");
		goto table_create_fail;
	}

	dm_lock_md_type(md);

	/* Add the NFSB verity target to the table. */
	target_params = kmalloc(384, GFP_KERNEL);
	snprintf(target_params, 384, "%u %s %s %u %u %u %u %s %s %s",
			 nfsb_hash_type(hdr), DEV_NFSB, DEV_NFSB,
			 data_blocksize, hash_blocksize, nfsb_data_blockcount(hdr),
			 fs_size / hash_blocksize, nfsb_hash_algo(hdr),
			 nfsb_verity_hash(hdr), nfsb_verity_salt(hdr));
	if (dm_table_add_target(table, "verity", 0, to_sector(fs_size), target_params)) {
		printk(KERN_ERR "Failed to add NFSB target to the table\n");
		goto add_target_fail;
	}

	/* Table is complete. */
	if (dm_table_complete(table)) {
		printk(KERN_ERR "Failed to complete NFSB table\n");
		goto table_complete_fail;
	}

	if (dm_get_md_type(md) == DM_TYPE_NONE) {
		dm_set_md_type(md, dm_table_get_type(table));
		if (dm_setup_md_queue(md)) {
			pr_warn("unable to set up device queue for new table.");
			goto setup_md_queue_fail;
		}
	} else if (dm_get_md_type(md) != dm_table_get_type(table)) {
		pr_warn("can't change device type after initial table load.");
		goto setup_md_queue_fail;
        }

	/* Suspend the device so that we can bind it to the table. */
	if (dm_suspend(md, DM_SUSPEND_NOFLUSH_FLAG)) {
		printk(KERN_ERR "Failed to suspend the device pre-bind\n");
		goto suspend_fail;
	}

	/* Bind the table to the device. This is the only way to associate
	 * md->map with the table and set the disk capacity. Ouch. */
	old_map = dm_swap_table(md, table);
	if (IS_ERR(old_map)) {
		printk(KERN_ERR "Failed to bind the device to the table\n");
		dm_table_destroy(table);
		goto swap_table_fail;
	}
	
	/* Finally, resume and the device should be ready. */
	if (dm_resume(md)) {
		printk(KERN_ERR "Failed to resume the device\n");
		goto resume_fail;
	}

	/* Success. */
	*dev = disk_devt(dm_disk(md));
	printk(KERN_INFO "dm: target verity of size %llu on %s(%i:%i) is ready\n",
		   dm_table_get_size(table), dm_device_name(md),
		   MAJOR(*dev), MINOR(*dev));
	if (old_map)
		dm_table_destroy(old_map);
	dm_unlock_md_type(md);
	kfree(target_params);
	return 1;

resume_fail:
	if (old_map)
		dm_table_destroy(old_map);
swap_table_fail:
suspend_fail:
setup_md_queue_fail:
table_complete_fail:
add_target_fail:
	dm_unlock_md_type(md);
	kfree(target_params);
	dm_table_destroy(table);
table_create_fail:
	dm_put(md);
	dm_destroy(md);
create_fail:
create_dev_fail:
hash_start_fail:
	return 0;
}

dev_t __init dm_mount_nfsb(dev_t dev)
{
	struct nfsb_header *hdr = NULL;

	hdr = kmalloc(sizeof(struct nfsb_header), GFP_KERNEL);
	if (!hdr)
		panic("Failed to allocate NFSB header struct.");

	printk(KERN_INFO "Checking for NFSB mount.\n");
	if (!dm_check_nfsb(hdr, dev))
		panic("No NFSB image found.");
	if (!dm_setup_linear(hdr, &dev))
		panic("Linear setup failed.");
	if (!dm_setup_nfsb(hdr, &dev)) {
		sys_unlink(DEV_LINEAR);
		panic("NFSB setup failed.");
	}

	kfree(hdr);
	return dev;
}

static unsigned int nfsb_dev_id = 0;

int nvt_dm_check_nfsb(struct nfsb_header *hdr, const char* dev_path)
{
#if defined(CONFIG_CRYPTO_NVT_CRYPT) || defined(CONFIG_CRYPTO_NVT_CRYPT_V2)	
	int nfsb_fd = -1;
	ssize_t sz = -1;
	mm_segment_t oldfs = get_fs();

	set_fs(KERNEL_DS);

	if(dev_path == NULL)
		goto create_fail;

	/* Try to read an NFSB header from the root device. */
	nfsb_fd = sys_open(dev_path, O_RDONLY, 0);
	if (nfsb_fd < 0) {
		printk(KERN_ERR "nfsb: open %s failed(%d)\n", dev_path, nfsb_fd);
		goto open_fail;
	}
	sz = nfsb_read(nfsb_fd, hdr);
	if (sz == -1)
		goto read_fail;

	/* Verify the NFSB header. */

	if (nfsb_verify(hdr, NULL, NULL) != 0) {
		printk(KERN_ERR "nfsb: header verification failed\n");
		goto verify_fail;
	}
	printk(KERN_INFO "NFSB header found and verified.\n");

	sys_close(nfsb_fd);
	set_fs(oldfs);
	return 1;

verify_fail:
read_fail:
	sys_close(nfsb_fd);
open_fail:
create_fail:
	set_fs(oldfs);
	return 0;
#else	
	int err = 0;
	int nfsb_fd = -1;
	ssize_t sz = -1;
	mpuint *pubkey = NULL, *modulus = NULL;
	mm_segment_t oldfs = get_fs();

	set_fs(KERNEL_DS);

	if(dev_path == NULL)
		goto create_fail;
	
	/* Try to read an NFSB header from the root device. */
	nfsb_fd = sys_open(dev_path, O_RDONLY, 0);
	if (nfsb_fd < 0) {
		printk(KERN_ERR "nfsb: open %s failed(%d)\n", dev_path, nfsb_fd);
		goto open_fail;
	}
	sz = nfsb_read(nfsb_fd, hdr);
	if (sz == -1)
		goto read_fail;

	/* Verify the NFSB header. */
	pubkey = mpuint_alloc(sizeof(rsa_pubkey));
	if (!pubkey) {
		printk(KERN_ERR "out of memory\n");
		goto pubkey_fail;
	}
	pubkey = mpuint_setbytes(pubkey, rsa_pubkey, sizeof(rsa_pubkey));
	if (!pubkey) {
		printk(KERN_ERR "mpuint_setbytes failed\n");
		goto pubkey_fail;
	}
	modulus = mpuint_alloc(sizeof(rsa_modulus));
	if (!modulus) {
		printk(KERN_ERR "out of memory\n");
		goto modulus_fail;
	}
	modulus = mpuint_setbytes(modulus, rsa_modulus, sizeof(rsa_modulus));
	if (!modulus) {
		printk(KERN_ERR "mpuint_setbytes failed\n");
		goto modulus_fail;
	}
	if (nfsb_verify(hdr, pubkey, modulus) != 0) {
		printk(KERN_ERR "nfsb: header verification failed\n");
		goto verify_fail;
	}
	printk(KERN_INFO "NFSB header found and verified.\n");

	/* Success. */
	mpuint_delete(modulus);
	mpuint_delete(pubkey);
	sys_close(nfsb_fd);
	set_fs(oldfs);
	return 1;

verify_fail:
modulus_fail:
	mpuint_delete(modulus);
pubkey_fail:
	mpuint_delete(pubkey);
read_fail:
	sys_close(nfsb_fd);
open_fail:
create_fail:
	set_fs(oldfs);
	return 0;
#endif	
}

EXPORT_SYMBOL(nvt_dm_check_nfsb);

int nvt_dm_setup_linear(struct nfsb_header *hdr, dev_t *dev)
{
	int linear_fd = -1;
	unsigned long linear_size = 0;
	struct mapped_device *md = NULL;
	struct dm_table *table = NULL, *old_map = NULL;
	char *target_params = NULL;
	ssize_t fs_offset = to_sector(nfsb_fs_offset(hdr));
	mm_segment_t oldfs = get_fs();
	char linear_path[64] = {0};

	set_fs(KERNEL_DS);

	/* Create linear device. */
	sprintf(linear_path,"%s-%d", DEV_LINEAR, nfsb_dev_id + 2);
	if (create_dev(linear_path, *dev) < 0) {
		printk(KERN_ERR "Failed to create linear device\n");
		goto create_dev_fail;
	}

	/* Size block device. */
	linear_fd = sys_open(linear_path, O_RDONLY, 0);
	if (linear_fd < 0) {
		printk(KERN_ERR "Failed to open linear device(%d)\n",linear_fd);
		goto sys_open_fail;
	}
	if (sys_ioctl(linear_fd, BLKGETSIZE, (unsigned long)&linear_size) < 0) {
		printk(KERN_ERR "Failed to get linear device size\n");
		goto sys_ioctl_fail;
	}

	/* Create a new mapped device. */
	if (dm_create(DM_ANY_MINOR, &md)) {
		printk(KERN_ERR "Failed to create mapped device\n");
		goto create_fail;
	}

	/* Explicitly set read-only. */
	set_disk_ro(dm_disk(md), 1);

	/* Create a mapped device table. */
	if (dm_table_create(&table, FMODE_READ, 1, md)) {
		printk(KERN_ERR "Failed to create table\n");
		goto table_create_fail;
	}

	dm_lock_md_type(md);

	/* Add the linear target to the table. */
	target_params = kmalloc(128, GFP_KERNEL);
	snprintf(target_params, 128, "%s %u", linear_path, fs_offset);
	if (dm_table_add_target(table, "linear", 0, linear_size - fs_offset, target_params)) {
		printk(KERN_ERR "Failed to add linear target to the table\n");
		goto add_target_fail;
	}

	/* Table is complete. */
	if (dm_table_complete(table)) {
		printk(KERN_ERR "Failed to complete NFSB table\n");
		goto table_complete_fail;
	}

	if (dm_get_md_type(md) == DM_TYPE_NONE) {
		dm_set_md_type(md, dm_table_get_type(table));
		if (dm_setup_md_queue(md)) {
			pr_warn("unable to set up device queue for new table.");
			goto setup_md_queue_fail;
		}
	} else if (dm_get_md_type(md) != dm_table_get_type(table)) {
		pr_warn("can't change device type after initial table load.");
		goto setup_md_queue_fail;
        }


	/* Suspend the device so that we can bind it to the table. */
	if (dm_suspend(md, DM_SUSPEND_NOFLUSH_FLAG)) {
		printk(KERN_ERR "Failed to suspend the device pre-bind\n");
		goto suspend_fail;
	}

	/* Bind the table to the device. This is the only way to associate
	 * md->map with the table and set the disk capacity. Ouch. */
	old_map = dm_swap_table(md, table);
	if (IS_ERR(old_map)) {
		printk(KERN_ERR "Failed to bind the device to the table\n");
		dm_table_destroy(table);
		goto swap_table_fail;
	}
	
	/* Finally, resume and the device should be ready. */
	if (dm_resume(md)) {
		printk(KERN_ERR "Failed to resume the device\n");
		goto resume_fail;
	}

	/* Success. */
	*dev = disk_devt(dm_disk(md));
	printk(KERN_INFO "dm: target linear of size %llu on %s(%i:%i) is ready\n",
		   dm_table_get_size(table), dm_device_name(md),
		   MAJOR(*dev), MINOR(*dev));
	if (old_map)
		dm_table_destroy(old_map);
	dm_unlock_md_type(md);
	kfree(target_params);
	sys_close(linear_fd);
	set_fs(oldfs);
	return 1;

resume_fail:
	if (old_map)
		dm_table_destroy(old_map);
swap_table_fail:
suspend_fail:
setup_md_queue_fail:
table_complete_fail:
add_target_fail:
	dm_unlock_md_type(md);
	kfree(target_params);
	dm_table_destroy(table);
table_create_fail:
	dm_put(md);
	dm_destroy(md);
create_fail:
sys_ioctl_fail:
	sys_close(linear_fd);
sys_open_fail:
create_dev_fail:
	sys_unlink(linear_path);
	set_fs(oldfs);
	return 0;
}

EXPORT_SYMBOL(nvt_dm_setup_linear);

int nvt_dm_setup_nfsb(struct nfsb_header *hdr, dev_t *dev, const char* dir_name, char* mapper_path)
{
	struct mapped_device *md = NULL;
	struct dm_table *table = NULL, *old_map = NULL;
	char *target_params = NULL;
	ssize_t fs_size = nfsb_fs_size(hdr);
	uint32_t data_blocksize = nfsb_data_blocksize(hdr);
	uint32_t hash_blocksize = nfsb_hash_blocksize(hdr);
	char nfsb_path[256] = {0};
	char tmp_dir[256] = {0};
	mm_segment_t oldfs = get_fs();
	char *ptr;

	set_fs(KERNEL_DS);

	/* Verify data and hash start block indexes. */
	if (fs_size % hash_blocksize) {
		printk(KERN_ERR "Hash start is not a multiple of the hash block size.\n");
		goto hash_start_fail;
	}

	/* Create NFSB device. */
	sprintf(nfsb_path, "%s-%d", DEV_NFSB, nfsb_dev_id + 2);
	if (create_dev(nfsb_path, *dev) < 0) {
		printk(KERN_ERR "Failed to create NFSB root device\n");
		goto create_dev_fail;
	}
	
	/* Create a new mapped device. */
	if (dm_create(DM_ANY_MINOR, &md)) {
		printk(KERN_ERR "Failed to create mapped device\n");
		goto create_fail;
	}

	/* Explicitly set read-only. */
	set_disk_ro(dm_disk(md), 1);

	/* Create a mapped device table. */
	if (dm_table_create(&table, FMODE_READ, 1, md)) {
		printk(KERN_ERR "Failed to create table\n");
		goto table_create_fail;
	}

	dm_lock_md_type(md);

	/* Add the NFSB verity target to the table. */
	target_params = kmalloc(384, GFP_KERNEL);
	snprintf(target_params, 384, "%u %s %s %u %u %u %u %s %s %s",
			 nfsb_hash_type(hdr), nfsb_path, nfsb_path,
			 data_blocksize, hash_blocksize, nfsb_data_blockcount(hdr),
			 fs_size / hash_blocksize, nfsb_hash_algo(hdr),
			 nfsb_verity_hash(hdr), nfsb_verity_salt(hdr));
	if (dm_table_add_target(table, "verity", 0, to_sector(fs_size), target_params)) {
		printk(KERN_ERR "Failed to add NFSB target to the table\n");
		goto add_target_fail;
	}

	/* Table is complete. */
	if (dm_table_complete(table)) {
		printk(KERN_ERR "Failed to complete NFSB table\n");
		goto table_complete_fail;
	}

	if (dm_get_md_type(md) == DM_TYPE_NONE) {
		dm_set_md_type(md, dm_table_get_type(table));
		if (dm_setup_md_queue(md)) {
			pr_warn("unable to set up device queue for new table.");
			goto setup_md_queue_fail;
		}
	} else if (dm_get_md_type(md) != dm_table_get_type(table)) {
		pr_warn("can't change device type after initial table load.");
		goto setup_md_queue_fail;
        }


	/* Suspend the device so that we can bind it to the table. */
	if (dm_suspend(md, DM_SUSPEND_NOFLUSH_FLAG)) {
		printk(KERN_ERR "Failed to suspend the device pre-bind\n");
		goto suspend_fail;
	}

	/* Bind the table to the device. This is the only way to associate
	 * md->map with the table and set the disk capacity. Ouch. */
	old_map = dm_swap_table(md, table);
	if (IS_ERR(old_map)) {
		printk(KERN_ERR "Failed to bind the device to the table\n");
		dm_table_destroy(table);
		goto swap_table_fail;
	}
	
	/* Finally, resume and the device should be ready. */
	if (dm_resume(md)) {
		printk(KERN_ERR "Failed to resume the device\n");
		goto resume_fail;
	}

	/* Success. */
	*dev = disk_devt(dm_disk(md));
	printk(KERN_INFO "dm: target verity of size %llu on %s(%i:%i) is ready\n",
		   dm_table_get_size(table), dm_device_name(md),
		   MAJOR(*dev), MINOR(*dev));
	strcpy(tmp_dir, dir_name);
	ptr = strrchr(tmp_dir, '/');
	if(ptr == NULL)
		sprintf(nfsb_path, "/dev/mapper/%s", dir_name);
	else {
		if(strlen(ptr) == 1) {
			*ptr = '\0';
			ptr = strrchr(tmp_dir, '/');
		} 

		sprintf(nfsb_path, "/dev/mapper/%s",(ptr+1));
		
	}

	//generate_random_uuid(uuid);
	//sprintf(nfsb_path,"/dev/mapper/");
	//for(i=0; i < sizeof(uuid);i++) {
	//	sprintf(tmp_buf,"%02X",uuid[i]);
	//	strcat(nfsb_path,tmp_buf);
	//}
	create_dev(nfsb_path, *dev);
	strncpy(mapper_path, nfsb_path, sizeof(nfsb_path));
	if (old_map)
		dm_table_destroy(old_map);
	dm_unlock_md_type(md);
	kfree(target_params);
	set_fs(oldfs);
	nfsb_dev_id++;
	return 1;

resume_fail:
	if (old_map)
		dm_table_destroy(old_map);
swap_table_fail:
suspend_fail:
setup_md_queue_fail:
table_complete_fail:
add_target_fail:
	dm_unlock_md_type(md);
	kfree(target_params);
	dm_table_destroy(table);
table_create_fail:
	dm_put(md);
	dm_destroy(md);
create_fail:
create_dev_fail:
hash_start_fail:
	set_fs(oldfs);
	return 0;
}

EXPORT_SYMBOL(nvt_dm_setup_nfsb);
