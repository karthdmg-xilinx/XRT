// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2019 Xilinx, Inc
// Copyright (C) 2022 Advanced Micro Devices, Inc. All rights reserved.
#ifndef _AWS_DEV_H_
#define _AWS_DEV_H_

#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <string>
#include "xclhal2.h"
#include "core/pcie/driver/linux/include/mailbox_proto.h"
#include "core/pcie/driver/linux/include/mgmt-ioctl.h"
#include "core/pcie/linux/pcidev.h"
#include "core/pcie/linux/shim.h"
#include "../mpd_plugin.h"
#include "../common.h"
#include "../sw_msg.h"
#include "../pciefunc.h"

#ifdef INTERNAL_TESTING_FOR_AWS
#include "core/pcie/driver/linux/include/xocl_ioctl.h"
#endif

#ifndef INTERNAL_TESTING_FOR_AWS
#include "fpga_pci.h"
#include "fpga_mgmt.h"
#include "fpga_clkgen.h"
#include "hal/fpga_common.h"
#endif

#define DEFAULT_GLOBAL_AFI "agfi-069ddd533a748059b" // 1.4 shell
#define AWS_UserPF_DEVICE_ID 0x1042     //userPF device on AWS F1 & Pegasus
#define AWS_MgmtPF_DEVICE_ID 0x1040     //mgmtPF device on Pegasus (mgmtPF not visible on AWS)
#define AWS_UserPF_DEVICE_ID_SDx 0xf010 //userPF device on AWS F1 after downloading xclbin into FPGA (SHELL 1.4)

/*
 * This class is used to handle ioctl access to mgmt PF of AWS specific FPGA.
 *
 * Since AWS has its own FPGA mgmt hardware/driver, any mgmt HW access request from xocl driver
 * will be forwarded by sw mailbox and be intercepted and interpreted by this mpd plugin. And
 * with the plugin, those special requests will be translated to API calls into the libmgmt.so
 * (or .a) provided by AWS
 *
 * This class will *only* handle AWS specific parts. Those not implemented in AWS hardware
 * so far will be returned as NOTSUPPORTED
 */
class AwsDev
{
public:
    AwsDev(size_t index, const char *logfileName);
    ~AwsDev();

    static int init(std::map<std::string, size_t>& index_map)
    {
#ifndef INTERNAL_TESTING_FOR_AWS
        int domain, bus, dev, func;
        if (fpga_mgmt_init() || fpga_pci_init() ) {
            std::cout << "ERROR: failed to initialized fpga libraries" << std::endl;
            return -1;
        }
        fpga_slot_spec spec_array[16];
        std::memset(spec_array, 0, sizeof(fpga_slot_spec) * 16);
        if (fpga_pci_get_all_slot_specs(spec_array, 16)) {
            std::cout << "ERROR: failed at fpga_pci_get_all_slot_specs" << std::endl;
            return -1;
        }

        for (unsigned short i = 0; i < 16; i++) {
            if (spec_array[i].map[FPGA_APP_PF].vendor_id == 0)
                break;

            domain = spec_array[i].map[FPGA_APP_PF].domain;
            bus = spec_array[i].map[FPGA_APP_PF].bus;
            dev = spec_array[i].map[FPGA_APP_PF].dev;
            func = spec_array[i].map[FPGA_APP_PF].func;

            std::stringstream domain_str;
            std::stringstream bus_str;
            std::stringstream dev_str;
            //Note: Below works with stringstream only for integers and not for uint8, etc.
            domain_str << std::setw(4) << std::setfill('0') << domain;
            bus_str << std::setw(2) << std::setfill('0') << std::hex << bus;
            dev_str << std::setw(2) << std::setfill('0') << std::hex << dev;
            std::string func_str = std::to_string(func);//stringstream is giving minimum of two chars

            std::string sysfs_name = domain_str.str() + ":" + bus_str.str() + ":" + dev_str.str() + "." + func_str;
            index_map[sysfs_name] = i;
        }
#endif
        return 0;
    };
    int awsGetIcap(xcl_pr_region *resp);
    int awsGetSensor(xcl_sensor *resp);
    int awsGetBdinfo(xcl_board_info *resp);
    int awsGetMig(char *resp, size_t resp_len);
    int awsGetFirewall(xcl_mig_ecc *resp);
    int awsGetDna(xcl_dna *resp);
    int awsGetSubdev(char *resp, size_t resp_len);
    // Bitstreams
    int awsLoadXclBin(const xclBin *buffer);
    //int xclBootFPGA();
    int awsResetDevice();
    int awsReClock2(const xclmgmt_ioc_freqscaling *obj);
    int awsProgramShell();
    int awsReadP2pBarAddr(const xcl_mailbox_p2p_bar_addr *addr);
    int awsUserProbe(xcl_mailbox_conn_resp *resp);
    bool isGood();
private:
    int mBoardNumber;
    std::ofstream mLogStream;
#ifdef INTERNAL_TESTING_FOR_AWS
    int mMgtHandle;
#else
    int sleepUntilLoaded( const std::string &afi, fpga_mgmt_image_info* new_afi );
    char* get_afi_from_axlf(const axlf * buffer);
    const clock_freq_topology* get_clock_freq_from_axlf(const axlf *buffer);
    int index;
#endif
};

int get_remote_msd_fd(size_t index, int* fd);
int mb_notify(size_t index, int fd, bool online);
int awsLoadXclBin(size_t index, const axlf *xclbin, int *resp);
int awsGetIcap(size_t index, xcl_pr_region *resp);
int awsGetSensor(size_t index, xcl_sensor *resp);
int awsGetBdinfo(size_t index, xcl_board_info *resp);
int awsGetMig(size_t index, char *resp, size_t resp_len);
int awsGetFirewall(size_t index, xcl_mig_ecc *resp);
int awsGetDna(size_t index, xcl_dna *resp);
int awsGetSubdev(size_t index, char *resp, size_t resp_len);
int awsResetDevice(size_t index, int *resp);
int awsReClock2(size_t index, const xclmgmt_ioc_freqscaling *obj, int *resp);
int awsUserProbe(size_t index, xcl_mailbox_conn_resp *resp);
int awsProgramShell(size_t index, int *resp);
int awsReadP2pBarAddr(size_t index, const xcl_mailbox_p2p_bar_addr *addr, int *resp);
#endif
