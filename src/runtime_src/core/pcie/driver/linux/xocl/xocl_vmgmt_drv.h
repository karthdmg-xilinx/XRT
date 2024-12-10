/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2016-2022 Xilinx, Inc. All rights reserved.
 * Copyright (C) 2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 */

#ifndef	_VMGMT_XOCL_DRV_H_
#define	_VMGMT_XOCL_DRV_H_

#include "xocl_drv.h"
#include "userpf/common.h"
#define XOCL_PCIE_QDMA_BAR_INDEX		2
#define XOCL_PCIE_QDMA_BAR_IDENTIFIER		0x1FD3
#define XOCL_PCIE_QDMA_BAR_IDENTIFIER_REGOFF	0x0
#define XOCL_PCIE_QDMA_BAR_IDENTIFIER_MASK	GENMASK(31, 16)

/* Device Specific Information */
#define XOCL_BOARD_V70_USER_RAPTOR2_DEVICE_ID	0x50B1
#define XOCL_PCIE_QDMA_BAR_RES_START			0x20200000000
#define XOCL_PCIE_QDMA_BAR_RES_END				0x20207ffffff


#define XOCL_VMGMT_MBX_PROTOCOL_VERSION(xdev_hdl)		\
	(((struct xocl_dev_core *)xdev_hdl)->vmgmt_subdev.vmgmt_mbx_protocol_version)

/* Function definitions for xocl ROM subdev created when vmgmt driver is probed */
#define PLAT_ROM_CTRL_OPS(xdev) \
	(XOCL_GET_DRV_PRI(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev) ? \
	(struct xocl_rom_funcs *)XOCL_GET_DRV_PRI(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev)->ops : \
	NULL)

#define xocl_vmgmt_is_unified(xdev) \
	(PLAT_ROM_CTRL_OPS(xdev) ? \
	 PLAT_ROM_CTRL_OPS(xdev)->is_unified(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev) : true)

#define xocl_vmgmt_mb_mgmt_on(xdev) \
	(PLAT_ROM_CTRL_OPS(xdev) ? \
	 PLAT_ROM_CTRL_OPS(xdev)->mb_mgmt_on(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev) : false)

#define xocl_vmgmt_mb_sched_on(xdev) \
	(PLAT_ROM_CTRL_OPS(xdev) ? \
	 PLAT_ROM_CTRL_OPS(xdev)->mb_sched_on(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev) : false)

#define xocl_vmgmt_cdma_addr(xdev) \
	(PLAT_ROM_CTRL_OPS(xdev) ? \
	 PLAT_ROM_CTRL_OPS(xdev)->cdma_addr(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev) : 0)

#define xocl_vmgmt_clk_scale_on(xdev) \
	(PLAT_ROM_CTRL_OPS(xdev) ? \
	 PLAT_ROM_CTRL_OPS(xdev)->runtime_clk_scale_on(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev) : false)

#define xocl_vmgmt_get_ddr_channel_count(xdev) \
	(PLAT_ROM_CTRL_OPS(xdev) ? \
	 PLAT_ROM_CTRL_OPS(xdev)->get_ddr_channel_count(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev) : 0)

#define xocl_vmgmt_get_ddr_channel_size(xdev) \
	(PLAT_ROM_CTRL_OPS(xdev) ? \
	 PLAT_ROM_CTRL_OPS(xdev)->get_ddr_channel_size(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev) : 0)

#define xocl_vmgmt_is_are(xdev) \
	(PLAT_ROM_CTRL_OPS(xdev) ? \
	 PLAT_ROM_CTRL_OPS(xdev)->is_are(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev) : false)

#define xocl_vmgmt_is_aws(xdev) \
	(PLAT_ROM_CTRL_OPS(xdev) ? \
	 PLAT_ROM_CTRL_OPS(xdev)->is_aws(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev) : false)

#define xocl_vmgmt_verify_timestamp(xdev, ts) \
	(PLAT_ROM_CTRL_OPS(xdev) ? \
	 PLAT_ROM_CTRL_OPS(xdev)->verify_timestamp(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev, ts) : false)

#define xocl_vmgmt_get_raw_header(xdev, header) \
	(PLAT_ROM_CTRL_OPS(xdev) ? \
	 PLAT_ROM_CTRL_OPS(xdev)->get_raw_header(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev, header) : -ENODEV)

#define xocl_vmgmt_rom_load_firmware(xdev, fw, len) \
	(PLAT_ROM_CTRL_OPS(xdev) ? \
	 PLAT_ROM_CTRL_OPS(xdev)->load_firmware(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev, fw, len) : -ENODEV)

#define xocl_vmgmt_passthrough_virtualization_on(xdev) \
	(PLAT_ROM_CTRL_OPS(xdev) ? \
	 PLAT_ROM_CTRL_OPS(xdev)->passthrough_virtualization_on(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev) : false)

#define xocl_vmgmt_rom_get_uuid(xdev) \
	(PLAT_ROM_CTRL_OPS(xdev) ? \
	 PLAT_ROM_CTRL_OPS(xdev)->get_uuid(XDEV(xdev)->vmgmt_subdev.vmgmt_rom_platdev) : NULL)

/* End of ROM Callback Functions */

/* Function definitions for xocl DMA sub device created when vmgmt driver is probed */
#define PLAT_DMA_CTRL_OPS(xdev) \
	(XOCL_GET_DRV_PRI(XDEV(xdev)->vmgmt_subdev.vmgmt_dma_platdev) ? \
	(struct xocl_dma_funcs *)XOCL_GET_DRV_PRI(XDEV(xdev)->vmgmt_subdev.vmgmt_dma_platdev)->ops : \
	NULL)

#define xocl_vmgmt_migrate_bo(xdev, sgt, to_dev, paddr, chan, len) \
	(PLAT_DMA_CTRL_OPS(xdev) ? \
	 PLAT_DMA_CTRL_OPS(xdev)->migrate_bo(XDEV(xdev)->vmgmt_subdev.vmgmt_dma_platdev, sgt, to_dev, paddr, chan, len) : 0)

#define xocl_vmgmt_async_migrate_bo(xdev, sgt, to_dev, paddr, chan, len, cb_fn, ctx_ptr) \
	(PLAT_DMA_CTRL_OPS(xdev) ? \
	 PLAT_DMA_CTRL_OPS(xdev)->async_migrate_bo(XDEV(xdev)->vmgmt_subdev.vmgmt_dma_platdev, sgt, to_dev, paddr, chan, len, cb_fn, ctx_ptr) : 0)

#define xocl_vmgmt_acquire_channel(xdev, dir) \
	(PLAT_DMA_CTRL_OPS(xdev) ? \
	 PLAT_DMA_CTRL_OPS(xdev)->ac_chan(XDEV(xdev)->vmgmt_subdev.vmgmt_dma_platdev, dir) : 0)

#define xocl_vmgmt_release_channel(xdev, dir, chan) \
	(PLAT_DMA_CTRL_OPS(xdev) ? \
	 PLAT_DMA_CTRL_OPS(xdev)->rel_chan(XDEV(xdev)->vmgmt_subdev.vmgmt_dma_platdev, dir, chan) : 0)

#define xocl_vmgmt_get_chan_count(xdev) \
	(PLAT_DMA_CTRL_OPS(xdev) ? \
	 PLAT_DMA_CTRL_OPS(xdev)->get_chan_count(XDEV(xdev)->vmgmt_subdev.vmgmt_dma_platdev) : 0)

#define xocl_vmgmt_get_chan_stat(xdev, chan, write) \
	(PLAT_DMA_CTRL_OPS(xdev) ? \
	 PLAT_DMA_CTRL_OPS(xdev)->get_chan_stat(XDEV(xdev)->vmgmt_subdev.vmgmt_dma_platdev, chan, write) : 0)

#define xocl_vmgmt_dma_intr_config(xdev, irq, en) \
	(PLAT_DMA_CTRL_OPS(xdev) ? \
	 PLAT_DMA_CTRL_OPS(xdev)->user_intr_config(XDEV(xdev)->vmgmt_subdev.vmgmt_dma_platdev, irq, en) : 0)

#define xocl_vmgmt_dma_intr_register(xdev, irq, handler, arg, event_fd) \
	(PLAT_DMA_CTRL_OPS(xdev) ? \
	 PLAT_DMA_CTRL_OPS(xdev)->user_intr_register(XDEV(xdev)->vmgmt_subdev.vmgmt_dma_platdev, irq, handler, arg, event_fd) : 0)

#define xocl_vmgmt_dma_intr_unreg(xdev, irq) \
	(PLAT_DMA_CTRL_OPS(xdev) ? \
	 PLAT_DMA_CTRL_OPS(xdev)->user_intr_unreg(XDEV(xdev)->vmgmt_subdev.vmgmt_dma_platdev, irq) : 0)

/* End of DMA Callback functions */

/* Function definitions for xocl Mailbox sub device created when vmgmt driver is probed */
#define PLAT_MAILBOX_CTRL_OPS(xdev) \
	(XOCL_GET_DRV_PRI(XDEV(xdev)->vmgmt_subdev.vmgmt_mbx_platdev) ? \
	(struct xocl_mailbox_funcs *)XOCL_GET_DRV_PRI(XDEV(xdev)->vmgmt_subdev.vmgmt_mbx_platdev)->ops : \
	NULL)

#define xocl_vmgmt_peer_request(xdev, req, reqlen, resp, resplen, cb, cbarg, rx_timeout, tx_timeout) \
	(PLAT_MAILBOX_CTRL_OPS(xdev) ? \
	 PLAT_MAILBOX_CTRL_OPS(xdev)->request(XDEV(xdev)->vmgmt_subdev.vmgmt_mbx_platdev, req, reqlen, resp, resplen, cb, cbarg, rx_timeout, tx_timeout) : 0)

#define xocl_vmgmt_peer_response(xdev, req, reqid, buf, len) \
	(PLAT_MAILBOX_CTRL_OPS(xdev) ? \
	 PLAT_MAILBOX_CTRL_OPS(xdev)->post_response(XDEV(xdev)->vmgmt_subdev.vmgmt_mbx_platdev, req, reqid, buf, len) : 0)

#define xocl_vmgmt_peer_notify(xdev, req, reqlen) \
	(PLAT_MAILBOX_CTRL_OPS(xdev) ? \
	 PLAT_MAILBOX_CTRL_OPS(xdev)->post_notify(XDEV(xdev)->vmgmt_subdev.vmgmt_mbx_platdev, req, reqlen) : 0)

#define xocl_vmgmt_peer_listen(xdev, cb, cbarg) \
	(PLAT_MAILBOX_CTRL_OPS(xdev) ? \
	 PLAT_MAILBOX_CTRL_OPS(xdev)->listen(XDEV(xdev)->vmgmt_subdev.vmgmt_mbx_platdev, cb, cbarg) : 0)

#define xocl_vmgmt_mailbox_set(xdev, kind, data) \
	(PLAT_MAILBOX_CTRL_OPS(xdev) ? \
	 PLAT_MAILBOX_CTRL_OPS(xdev)->set(XDEV(xdev)->vmgmt_subdev.vmgmt_mbx_platdev, kind, data) : 0)

#define	xocl_vmgmt_mailbox_get(xdev, kind, data)				\
	(PLAT_MAILBOX_CTRL_OPS(xdev) ? \
	 PLAT_MAILBOX_CTRL_OPS(xdev)->get(XDEV(xdev)->vmgmt_subdev.vmgmt_mbx_platdev, kind, data) : 0)

/* End of Mailbox subdev functions */

/* Function definitions for xocl ICAP sub device created when vmgmt driver is probed */
#define PLAT_ICAP_CTRL_OPS(xdev) \
	(XOCL_GET_DRV_PRI(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev) ? \
	(struct xocl_icap_funcs *)XOCL_GET_DRV_PRI(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev)->ops : \
	NULL)

#define xocl_vmgmt_icap_reset_axi_gate(xdev) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->reset_axi_gate(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev) : 0)

#define xocl_vmgmt_icap_reset_bitstream(xdev) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->reset_bitstream(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev) : 0)

#define xocl_vmgmt_icap_download_axlf(xdev, xclbin, slot_id) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->download_bitstream_axlf(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev, xclbin, slot_id) : 0)

#define xocl_vmgmt_icap_download_boot_firmware(xdev) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->download_boot_firmware(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev) : 0)

#define xocl_vmgmt_icap_download_rp(xdev, level, flag) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->download_rp(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev, level, flag) : 0)

#define xocl_vmgmt_icap_post_download_rp(xdev) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->post_download_rp(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev) : 0)

#define xocl_vmgmt_icap_ocl_get_freq(xdev, region, freqs, num) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->ocl_get_freq(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev, region, freqs, num) : 0)

#define xocl_vmgmt_icap_ocl_update_clock_freq_topology(xdev, freqs) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->ocl_update_clock_freq_topology(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev, freqs) : 0)

#define xocl_vmgmt_icap_xclbin_validate_clock_req(xdev, freqs) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->xclbin_validate_clock_req(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev, freqs) : 0)

#define xocl_vmgmt_icap_lock_bitstream(xdev, uuid, slot_id) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->ocl_lock_bitstream(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev, uuid, slot_id) : 0)

#define xocl_vmgmt_icap_unlock_bitstream(xdev, uuid, slot_id) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->ocl_unlock_bitstream(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev, uuid, slot_id) : 0)

#define xocl_vmgmt_icap_bitstream_is_locked(xdev, slot_id) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->ocl_bitstream_is_locked(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev, slot_id) : 0)

#define xocl_vmgmt_icap_refresh_addrs(xdev) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->refresh_addrs(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev) : 0)

#define xocl_vmgmt_icap_get_data(xdev, kind) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->get_data(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev, kind) : 0)

#define xocl_vmgmt_icap_get_xclbin_metadata(xdev, kind, buf, slot_id) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->get_xclbin_metadata(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev, kind, buf, slot_id) : 0)

#define	xocl_vmgmt_icap_put_xclbin_metadata(xdev, slot_id)			\
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->put_xclbin_metadata(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev, slot_id) : 0)

#define xocl_vmgmt_icap_mig_calibration(xdev) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->mig_calibration(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev) : 0)

#define xocl_vmgmt_icap_clean_bitstream(xdev, slot_id) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->clean_bitstream(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev, slot_id) : 0)

#define xocl_vmgmt_icap_clean_bitstream(xdev, slot_id) \
	(PLAT_ICAP_CTRL_OPS(xdev) ? \
	 PLAT_ICAP_CTRL_OPS(xdev)->clean_bitstream(XDEV(xdev)->vmgmt_subdev.vmgmt_icap_platdev, slot_id) : 0)

#define XOCL_VMGMT_GET_MEM_TOPOLOGY(xdev, mem_topo, slot_id)						\
	(xocl_vmgmt_icap_get_xclbin_metadata(xdev, MEMTOPO_AXLF, (void **)&mem_topo, slot_id))
#define XOCL_VMGMT_GET_GROUP_TOPOLOGY(xdev, group_topo, slot_id)					\
	(xocl_vmgmt_icap_get_xclbin_metadata(xdev, GROUPTOPO_AXLF, (void **)&group_topo, slot_id))
#define XOCL_VMGMT_GET_IP_LAYOUT(xdev, ip_layout, slot_id)						\
	(xocl_vmgmt_icap_get_xclbin_metadata(xdev, IPLAYOUT_AXLF, (void **)&ip_layout, slot_id))
#define XOCL_VMGMT_GET_CONNECTIVITY(xdev, conn, slot_id)						\
	(xocl_vmgmt_icap_get_xclbin_metadata(xdev, CONNECTIVITY_AXLF, (void **)&conn, slot_id))
#define XOCL_VMGMT_GET_XCLBIN_ID(xdev, xclbin_id, slot_id)						\
	(xocl_vmgmt_icap_get_xclbin_metadata(xdev, XCLBIN_UUID, (void **)&xclbin_id, slot_id))
#define XOCL_VMGMT_GET_PS_KERNEL(xdev, ps_kernel, slot_id)						\
	(xocl_vmgmt_icap_get_xclbin_metadata(xdev, SOFT_KERNEL, (void **)&ps_kernel, slot_id))

#define XOCL_VMGMT_PUT_MEM_TOPOLOGY(xdev, slot_id)						\
	xocl_vmgmt_icap_put_xclbin_metadata(xdev, slot_id)
#define XOCL_VMGMT_PUT_GROUP_TOPOLOGY(xdev, slot_id)						\
	xocl_vmgmt_icap_put_xclbin_metadata(xdev, slot_id)
#define XOCL_VMGMT_PUT_IP_LAYOUT(xdev, slot_id)						\
	xocl_vmgmt_icap_put_xclbin_metadata(xdev, slot_id)
#define XOCL_VMGMT_PUT_CONNECTIVITY(xdev, slot_id)						\
	xocl_vmgmt_icap_put_xclbin_metadata(xdev, slot_id)
#define XOCL_VMGMT_PUT_XCLBIN_ID(xdev, slot_id)						\
	xocl_vmgmt_icap_put_xclbin_metadata(xdev, slot_id)
#define XOCL_VMGMT_PUT_PS_KERNEL(xdev, slot_id)						\
	xocl_vmgmt_icap_put_xclbin_metadata(xdev, slot_id)
/* End of ICAP function definitions */

/* Function definitions for xocl ERT subdev created when vmgmt driver is probed */
#define PLAT_ERT_CTRL_OPS(xdev) \
	(XOCL_GET_DRV_PRI(XDEV(xdev)->vmgmt_subdev.vmgmt_ert_ctrl_platdev) ? \
	(struct xocl_ert_ctrl_funcs *)XOCL_GET_DRV_PRI(XDEV(xdev)->vmgmt_subdev.vmgmt_ert_ctrl_platdev)->ops : \
	NULL)

#define xocl_vmgmt_ert_ctrl_connect(xdev) \
	(PLAT_ERT_CTRL_OPS(xdev) ? \
	 PLAT_ERT_CTRL_OPS(xdev)->connect(XDEV(xdev)->vmgmt_subdev.vmgmt_ert_ctrl_platdev) : -ENODEV)

#define xocl_vmgmt_ert_ctrl_is_version(xdev, major, minor) \
	(PLAT_ERT_CTRL_OPS(xdev) ? \
	 PLAT_ERT_CTRL_OPS(xdev)->is_version(XDEV(xdev)->vmgmt_subdev.vmgmt_ert_ctrl_platdev, major, minor) : -ENODEV)

#define xocl_vmgmt_ert_ctrl_get_base(xdev) \
	(PLAT_ERT_CTRL_OPS(xdev) ? \
	 PLAT_ERT_CTRL_OPS(xdev)->get_base(XDEV(xdev)->vmgmt_subdev.vmgmt_ert_ctrl_platdev) : -ENODEV)

#define xocl_vmgmt_ert_ctrl_setup_xgq(xdev, id, offset) \
	(PLAT_ERT_CTRL_OPS(xdev) ? \
	 PLAT_ERT_CTRL_OPS(xdev)->setup_xgq(XDEV(xdev)->vmgmt_subdev.vmgmt_ert_ctrl_platdev, id, offset) : -ENODEV)

#define xocl_vmgmt_ert_ctrl_unset_xgq(xdev, xgq) \
	(PLAT_ERT_CTRL_OPS(xdev) ? \
	 PLAT_ERT_CTRL_OPS(xdev)->unset_xgq(XDEV(xdev)->vmgmt_subdev.vmgmt_ert_ctrl_platdev, xgq) : -ENODEV)

#define xocl_vmgmt_ert_ctrl_dump(xdev) \
	(PLAT_ERT_CTRL_OPS(xdev) ? \
	 PLAT_ERT_CTRL_OPS(xdev)->dump_xgq(XDEV(xdev)->vmgmt_subdev.vmgmt_ert_ctrl_platdev) : NULL)

/** End of ERT Callback functions */

#define	XOCL_VMGMT_DDR_COUNT(xdev, slot)			\
	(xocl_vmgmt_is_unified(xdev) ? xocl_vmgmt_ddr_count_unified(xdev, slot) :	\
	xocl_vmgmt_get_ddr_channel_count(xdev))

static inline u32 xocl_vmgmt_ddr_count_unified(xdev_handle_t xdev_hdl,
					 uint32_t slot_id)
{
	struct mem_topology *topo = NULL;
	uint32_t ret = 0;
	int err = XOCL_VMGMT_GET_GROUP_TOPOLOGY(xdev_hdl, topo, slot_id);

	if (err)
		return 0;
	ret = topo ? topo->m_count : 0;
	XOCL_VMGMT_PUT_GROUP_TOPOLOGY(xdev_hdl, slot_id);

	return ret;
}


static inline int xocl_vmgmt_get_pl_slot(xdev_handle_t xdev_hdl, uint32_t *slot_id)
{
	uuid_t *xclbin_id = NULL;
	int ret = 0;

	/* Check if DEFAULT_PL_PS_SLOT has a xclbin loaded */
	ret = XOCL_VMGMT_GET_XCLBIN_ID(xdev_hdl, xclbin_id, DEFAULT_PL_PS_SLOT);
	if (ret)
		return ret;

	/* As of now we have single PL/PS slot and hard coded to slot 0 */
	*slot_id = DEFAULT_PL_PS_SLOT;

	return 0;
}

static inline void xocl_vmgmt_icap_clean_bitstream_all(xdev_handle_t xdev_hdl)
{
	uint32_t slot_id = 0;

	/* Free all the bitstream */
        for (slot_id = 0; slot_id < MAX_SLOT_SUPPORT; slot_id++)
		{
            xocl_vmgmt_icap_clean_bitstream(xdev_hdl, slot_id);
		}
}


/* End of Mailbox subdev functions */

#endif