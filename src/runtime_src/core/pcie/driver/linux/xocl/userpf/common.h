/*
 * Copyright (C) 2016-2022 Xilinx, Inc. All rights reserved.
 *
 * Authors:
 * 		Lizhi Hou <lizhi.hou@xilinx.com>
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _USERPF_COMMON_H
#define	_USERPF_COMMON_H

#include "../xocl_drv.h"
#include "xocl_bo.h"
#include "../xocl_drm.h"
#include "xocl_ioctl.h"
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0)
#include <linux/hashtable.h>
#endif
#include "kds_core.h"

#define XOCL_DRIVER_DESC        "Xilinx PCIe Accelerator Device Manager"
#define XOCL_DRIVER_DATE        "20180612"
#define XOCL_DRIVER_MAJOR       2018
#define XOCL_DRIVER_MINOR       2
#define XOCL_DRIVER_PATCHLEVEL  8

#define XOCL_DRIVER_VERSION                             \
	__stringify(XOCL_DRIVER_MAJOR) "."              \
	__stringify(XOCL_DRIVER_MINOR) "."              \
	__stringify(XOCL_DRIVER_PATCHLEVEL)

#define XOCL_DRIVER_VERSION_NUMBER                              \
	((XOCL_DRIVER_MAJOR)*1000 + (XOCL_DRIVER_MINOR)*100 +   \
	XOCL_DRIVER_PATCHLEVEL)

#define userpf_err(d, args...)                     \
	xocl_err(&XDEV(d)->pdev->dev, ##args)
#define userpf_info(d, args...)                    \
	xocl_info(&XDEV(d)->pdev->dev, ##args)
#define userpf_dbg(d, args...)                     \
	xocl_dbg(&XDEV(d)->pdev->dev, ##args)
#define userpf_info_once(d, args...)               \
({                                                 \
	 static bool __info_once __read_mostly;    \
						   \
	 if (!__info_once) {                       \
		 __info_once = true;               \
		 userpf_info(d, ##args);           \
	 }                                         \
 })

#define xocl_get_root_dev(dev, root)		\
	for (root = dev; root->bus && root->bus->self; root = root->bus->self)

#define	XOCL_RESET_DELAY		2000
#define	XOCL_PROGRAM_SHELL_DELAY	2000

#define	XOCL_USER_PROC_HASH_SZ		256

#define XOCL_U32_MASK 0xFFFFFFFF

#define	MAX_SLOTS	128
#define MAX_CUS		128
#define MAX_U32_SLOT_MASKS (((MAX_SLOTS-1)>>5) + 1)
#define MAX_U32_CU_MASKS (((MAX_CUS-1)>>5) + 1)
//#define MAX_DEPS        8 moved to ert.h; as needed in user space code as well

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 13, 0)
#define XOCL_DRM_FREE_MALLOC
#elif defined(RHEL_RELEASE_CODE)
#if RHEL_RELEASE_CODE > RHEL_RELEASE_VERSION(7, 4)
#define XOCL_DRM_FREE_MALLOC
#endif
#endif

enum {
	XOCL_FLAGS_SYSFS_INITIALIZED = (1 << 0),
	XOCL_FLAGS_PERSIST_SYSFS_INITIALIZED = (1 << 1),
};

struct xocl_dev	{
	struct xocl_dev_core	core;

	bool			is_legacy_ctx;
	bool			reset_ert_cus;
	int                 	ps_slot_id;
	struct list_head	ctx_list;

	/*
	 * Per xdev lock protecting client list and all client contexts in the
	 * list. Any operation which requires client status, such as xclbin
	 * downloading or validating exec buf, should hold this lock.
	 */
	struct mutex		dev_lock;
	unsigned int		needs_reset; /* bool aligned */
	atomic_t		outstanding_execs;
	atomic64_t		total_execs;

	struct xocl_subdev	*dyn_subdev_store;
	int			dyn_subdev_num;

	unsigned int		mbx_offset;

	uint64_t		mig_cache_expire_secs;
	ktime_t			mig_cache_expires;

	u32			flags;
	struct xocl_cma_bank	*cma_bank;
	struct xocl_pci_info	pci_stat;
	atomic_t		dev_hotplug_done;
};

/**
 * struct client_ctx: Manage user space client attached to device
 *
 * @link: Client context is added to list in device
 * @trigger: Poll wait counter for number of completed exec buffers
 * @outstanding_execs: Counter for number outstanding exec buffers
 * @abort: Flag to indicate that this context has detached from user space (ctrl-c)
 * @num_cus: Number of resources (CUs) explcitly aquired
 * @lock: Mutex lock for exclusive access
 * @cu_bitmap: CUs reserved by this context, may contain implicit resources
 * @virt_cu_ref: ref count for implicit resources reserved by this context.
 */
struct client_ctx {
	struct list_head	link;
	unsigned int            abort;
	unsigned int            num_cus;
	atomic_t		trigger;
	atomic_t                outstanding_execs;
	struct xocl_dev        *xdev;
	DECLARE_BITMAP		(cu_bitmap, MAX_CUS);
	struct pid             *pid;
	unsigned int		virt_cu_ref;
};
#define	CLIENT_NUM_CU_CTX(client) ((client)->num_cus + (client)->virt_cu_ref)

/* ioctl functions */
int xocl_info_ioctl(struct drm_device *dev, void *data,
	struct drm_file *filp);
int xocl_execbuf_ioctl(struct drm_device *dev, void *data,
	struct drm_file *filp);
int xocl_hw_ctx_execbuf_ioctl(struct drm_device *dev, void *data,
	struct drm_file *filp);
int xocl_ctx_ioctl(struct drm_device *dev, void *data,
	struct drm_file *filp);
int xocl_create_hw_ctx_ioctl(struct drm_device *dev, void *data,
	struct drm_file *filp);
int xocl_destroy_hw_ctx_ioctl(struct drm_device *dev, void *data,
	struct drm_file *filp);
int xocl_open_cu_ctx_ioctl(struct drm_device *dev, void *data,
	struct drm_file *filp);
int xocl_close_cu_ctx_ioctl(struct drm_device *dev, void *data,
	struct drm_file *filp);
int xocl_user_intr_ioctl(struct drm_device *dev, void *data,
	struct drm_file *filp);
int xocl_read_axlf_ioctl(struct drm_device *dev, void *data,
	struct drm_file *filp);
int xocl_hot_reset_ioctl(struct drm_device *dev, void *data,
	struct drm_file *filp);
int xocl_reclock_ioctl(struct drm_device *dev, void *data,
	struct drm_file *filp);
int xocl_alloc_cma_ioctl(struct drm_device *dev, void *data,
	struct drm_file *filp);
int xocl_free_cma_ioctl(struct drm_device *dev, void *data,
	struct drm_file *filp);
int xocl_set_cu_read_only_range_ioctl(struct drm_device *dev, void *data,
	struct drm_file *filp);
int xocl_command_ioctl(struct xocl_dev *xdev, void *data,
	struct drm_file *filp, bool in_kernel);

/* sysfs functions */
int xocl_init_sysfs(struct xocl_dev *xdev);
void xocl_fini_sysfs(struct xocl_dev *xdev);
int xocl_init_persist_sysfs(struct xocl_dev *xdev);
void xocl_fini_persist_sysfs(struct xocl_dev *xdev);

/* helper functions */
enum {
	XOCL_RESET_FORCE = 1,
	XOCL_RESET_SHUTDOWN = 2,
	XOCL_RESET_NO = 4,
};
int xocl_hot_reset(struct xocl_dev *xdev, u32 flag);
void xocl_p2p_fini(struct xocl_dev *xdev);
int xocl_p2p_init(struct xocl_dev *xdev);
int xocl_hwmon_sdm_init(struct xocl_dev *xdev);
void xocl_reset_notify(struct pci_dev *pdev, bool prepare);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 13, 0)
void user_pci_reset_prepare(struct pci_dev *pdev);
void user_pci_reset_done(struct pci_dev *pdev);
#endif

int xocl_refresh_subdevs(struct xocl_dev *xdev);

u32 get_live_clients(struct xocl_dev *xdev, pid_t **pid_list);
void reset_notify_client_ctx(struct xocl_dev *xdev);

void get_pcie_link_info(struct xocl_dev	*xdev,
	unsigned short *link_width, unsigned short *link_speed, bool is_cap);
uint64_t xocl_get_data(struct xocl_dev *xdev, enum data_kind kind);
int xocl_reclock(struct xocl_dev *xdev, void *data);

void xocl_update_mig_cache(struct xocl_dev *xdev);

int xocl_config_pci(struct xocl_dev *xdev);

int xocl_cma_bank_alloc(struct xocl_dev *xdev, struct drm_xocl_alloc_cma_info *cma_info);
void xocl_cma_bank_free(struct xocl_dev *xdev);

static inline u64 xocl_pci_rebar_size_to_bytes(int size)
{
	return 1ULL << (size + 20);
}

int xocl_read_axlf_helper(struct xocl_drm *drm_p, struct drm_xocl_axlf *axlf_ptr,
	                     uint32_t qos, uint32_t *slot_id);

/* KDS functions */
int xocl_init_sched(struct xocl_dev *xdev);
void xocl_fini_sched(struct xocl_dev *xdev);
int xocl_create_client(struct xocl_dev *xdev, void **priv);
void xocl_destroy_client(struct xocl_dev *xdev, void **priv);
int xocl_client_ioctl(struct xocl_dev *xdev, int op, void *data,
		      struct drm_file *filp);
/* New hw context support functions */
int xocl_get_slot_id_by_hw_ctx_id(struct xocl_dev *xdev,
		struct drm_file *filp, uint32_t hw_ctx_id);
int xocl_create_hw_context(struct xocl_dev *xdev, struct drm_file *filp,
                struct drm_xocl_create_hw_ctx *hw_ctx_args, uint32_t slot_id);
int xocl_destroy_hw_context(struct xocl_dev *xdev, struct drm_file *filp,
                struct drm_xocl_destroy_hw_ctx *hw_ctx_args);
int xocl_open_cu_context(struct xocl_dev *xdev, struct drm_file *filp,
                struct drm_xocl_open_cu_ctx *drm_cu_args);
int xocl_close_cu_context(struct xocl_dev *xdev, struct drm_file *filp,
                struct drm_xocl_close_cu_ctx *drm_cu_args);
int xocl_hw_ctx_command(struct xocl_dev *xdev, void *data,
		      struct drm_file *filp);
/* End of new hw context support functions */

int xocl_poll_client(struct file *filp, poll_table *wait, void *priv);
int xocl_kds_stop(struct xocl_dev *xdev);
int xocl_kds_reset(struct xocl_dev *xdev, const xuid_t *xclbin_id);
int xocl_kds_reconfig(struct xocl_dev *xdev);
int xocl_cu_map_addr(struct xocl_dev *xdev, u32 cu_idx,
		     struct drm_file *filp, unsigned long size, u32 *addrp);
u32 xocl_kds_live_clients(struct xocl_dev *xdev, pid_t **plist);
u32 xocl_kds_get_open_clients(struct xocl_dev *xdev, pid_t **plist);
int xocl_kds_update(struct xocl_dev *xdev, struct drm_xocl_kds kds_cfg);
void xocl_kds_cus_enable(struct xocl_dev *xdev);
void xocl_kds_cus_disable(struct xocl_dev *xdev);
int xocl_kds_register_cus(struct xocl_dev *xdev, int slot_hd, xuid_t *uuid,
			  struct ip_layout *ip_layout,
			  struct ps_kernel_node *ps_kernel);
int xocl_kds_unregister_cus(struct xocl_dev *xdev, int slot_hd);
int xocl_kds_xgq_query_mem(struct xocl_dev *xdev, struct mem_data *mem_data);
int xocl_kds_set_cu_read_range(struct xocl_dev *xdev, u32 cu_idx,
			       u32 start, u32 size);

#endif
