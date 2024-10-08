///////////////////////////////////////////////////////////////////////////////
//
//    (C) Copyright 2021 OSR Open Systems Resources, Inc.
//    Copyright (C) 2019-2021 Xilinx, Inc.
//    (C) Copyright 2019-2021 OSR Open Systems Resources, Inc.
//    All Rights Reserved
//
//    ABSTRACT:
//
//       IOCTL interface definitions for the Xilinx XOCL driver
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

//
// Device interface GUID for the BME280 device
//
#include <initguid.h>
#include <stdint.h>
#include "xclbin.h"
#include "mailbox_proto.h"

// {45A6FFCA-EF63-4933-9983-F63DEC5816EB}
DEFINE_GUID(GUID_DEVINTERFACE_XOCL_USER,
    0x45a6ffca, 0xef63, 0x4933, 0x99, 0x83, 0xf6, 0x3d, 0xec, 0x58, 0x16, 0xeb);

#define FILE_DEVICE_XOCL_USER   ((ULONG)0x8879)   // "XO"

//
// Constant string for the symbolic link associated with the device
//
#define XOCL_USER_BASE_DEVICE_NAME               L"XOCL_USER-"

//
// Device name space names
//
#define XOCL_USER_DEVICE_BUFFER_OBJECT_NAMESPACE L"\\Buffer"
#define XOCL_USER_DEVICE_DEVICE_NAMESPACE        L"\\Device"

//
// IOCTL Codes and structures supported by XoclUser
//

typedef enum _XOCL_BUFFER_SYNC_DIRECTION {

    XOCL_BUFFER_DIRECTION_TO_DEVICE = 0,
    XOCL_BUFFER_DIRECTION_FROM_DEVICE

} XOCL_BUFFER_SYNC_DIRECTION, *PXOCL_BUFFER_SYNC_DIRECTION;

typedef enum _XOCL_BUFFER_TYPE {

    XOCL_BUFFER_TYPE_NONE = 0,
    XOCL_BUFFER_TYPE_NORMAL = 0x3323,
    XOCL_BUFFER_TYPE_USERPTR,
    XOCL_BUFFER_TYPE_IMPORT,
    XOCL_BUFFER_TYPE_CMA,   // NOT USED
    XOCL_BUFFER_TYPE_P2P,
    XOCL_BUFFER_TYPE_EXECBUF,
    XOCL_BUFFER_TYPE_HOST_ONLY,
    XOCL_BUFFER_TYPE_DEVICE_ONLY

} XOCL_BUFFER_TYPE, *PXOCL_BUFFER_TYPE;

#define XOCL_MAX_DDR_BANKS    4

//
// Creating and using Buffer Objects
//
// Instantiating Buffer Objects is a two step process:
//
// 1) An empty Buffer Object is created on the device, using CreateFile
//      specifying XOCL device's buffer namespace (e.g. "XOCL_USER-0\Buffer").
//      The File Handle returned from the successful CreateFile operation is
//      the handle to the newly created empty Buffer Object.
//
// 2) An IOCTL is sent, via the File Handle of the empty Buffer Object
//      created in step 1 above, to complete the creation of the Buffer Object.
//      The IOCTL will be either IOCTL_XOCL_CREATE_BO or IOCTL_XOCL_USERPTR_BO.
//
// After a Buffer Object has been created as described above, Sync, Map (if
// appropriate), PREAD, PWRITE, INFO, and EXECBUF (if appropriate) operations
// can be performed using the Buffer Object, by sending the associated IOCTL
// on the File Handle of the Buffer Object.  For EXECBUF, dependent File Handles
// may optionally be specified in the provided dependency buffer.
//
// To destroy the buffer object simply call CloseHandle on the HANDLE returned
// by the CreateFile call
//

//
// IOCTL_XOCL_CREATE_BO
//
// InBuffer = XOCL_CREATE_BO_ARGS
// OutBuffer = (not used)
//
#define IOCTL_XOCL_CREATE_BO        CTL_CODE(FILE_DEVICE_XOCL_USER, 2070, METHOD_BUFFERED, FILE_READ_DATA)
//
typedef struct _XOCL_CREATE_BO_ARGS {
    ULONGLONG               Size;           // IN: Size in bytes of Buffer
    ULONG                   BankNumber;     // IN: Zero-based DDR bank number to use
    XOCL_BUFFER_TYPE        BufferType;     // IN: Which type of Buffer Object is being created
                                            //     Must be "NORMAL" or "EXECBUF"
} XOCL_CREATE_BO_ARGS, *PXOCL_CREATE_BO_ARGS;

//
// IOCTL_XOCL_USERPTR_BO
//
// InBuffer  = XOCL_USERPTR_BO_ARGS
// OutBuffer = (not used)
//
#define IOCTL_XOCL_USERPTR_BO       CTL_CODE(FILE_DEVICE_XOCL_USER, 2071, METHOD_BUFFERED, FILE_READ_DATA)
//
typedef struct _XOCL_USERPTR_BO_ARGS {
    PVOID                   Address;        // IN: User VA of buffer for driver to use
    ULONGLONG               Size;           // IN: Size in bytes of buffer
    ULONG                   BankNumber;     // IN: Zero-based DDR bank number to use
    XOCL_BUFFER_TYPE        BufferType;     // IN: Which type of Buffer Object is being created
                                            //     Must be "USERPTR"
} XOCL_USERPTR_BO_ARGS, *PXOCL_USERPTR_BO_ARGS;

//
// IOCTL_XOCL_MAP_BO
//
// InBuffer =  (not used)
// OutBuffer = XOCL_MAP_BO_RESULT
//
#define IOCTL_XOCL_MAP_BO           CTL_CODE(FILE_DEVICE_XOCL_USER, 2072, METHOD_BUFFERED, FILE_READ_DATA)
//
typedef struct _XOCL_MAP_BO_RESULT {
    PVOID       MappedUserVirtualAddress;         // OUT: User VA of mapped buffer
} XOCL_MAP_BO_RESULT, *PXOCL_MAP_BO_RESULT;

//
// IOCTL_XOCL_SYNC_BO
//
// InBuffer =  XOCL_SYNC_BO_ARGS
// OutBuffer =  (not used)
//
#define IOCTL_XOCL_SYNC_BO          CTL_CODE(FILE_DEVICE_XOCL_USER, 2073, METHOD_BUFFERED, FILE_READ_DATA)
//
typedef struct _XOCL_SYNC_BO_ARGS {
    ULONGLONG   Size;           // IN: Bytes to read or write
    ULONGLONG   Offset;         // IN: DDR offset, in bytes, for sync operation
    XOCL_BUFFER_SYNC_DIRECTION Direction;  // IN: Sync direction (FROM device or TO device)
} XOCL_SYNC_BO_ARGS, *PXOCL_SYNC_BO_ARGS;

//
// IOCTL_XOCL_INFO_BO
//
// InBuffer =  (not used)
// OutBuffer = XOCL_INFO_BO_RESULT
//
#define IOCTL_XOCL_INFO_BO          CTL_CODE(FILE_DEVICE_XOCL_USER, 2075, METHOD_BUFFERED, FILE_READ_DATA)
//
typedef struct _XOCL_INFO_BO_RESULT {
    ULONGLONG           Size;           // OUT: Size in bytes of the buffer
    ULONGLONG           Paddr;          // OUT: Physical address of associated DDR
    XOCL_BUFFER_TYPE    BufferType;     // OUT: Buffer Type
} XOCL_INFO_BO_RESULT, *PXOCL_INFO_BO_RESULT;


#define	ICAP_XCLBIN_V2		"xclbin2"

/**
 * struct argument_info - Kernel argument information
 *
 * @name:	argument name
 * @offset:	argument offset in CU
 * @size:	argument size in bytes
 * @dir:	input or output argument for a CU
 */
struct argument_info {
    char        name[64];
    size_t      offset;
    size_t      size;
    uint32_t    dir;
};

/**
 * struct kernel_info - Kernel information
 *
 * @name:	kernel name
 * @range:	kernel register range
 * @anums:	number of argument
 * @args:	argument array
 */
struct kernel_info {
    char                    name[64];
    size_t                  range;
    size_t                  anums;
    struct argument_info    args[1];
};

struct xocl_kds{
    uint32_t slot_size;
    uint32_t ert : 1;
    uint32_t polling : 1;
    uint32_t cu_dma : 1;
    uint32_t cu_isr : 1;
    uint32_t cq_int : 1;
    uint32_t dataflow : 1;
    uint32_t rw_shared : 1;
    uint32_t unused : 25;
};

//
// IOCTL_XOCL_READ_AXLF
//
// InBuffer =  User data buffer pointer and size (containing AXLF File)
// OutBuffer = XOCL_READ_AXLF_ARGS
//
typedef struct _XOCL_READ_AXLF_ARGS {
    struct xocl_kds  kds_cfg;    // IN: KDS user configuration
    size_t           ksize;      // IN: size of kernels in bytes
    CHAR             kernels[1]; // IN: pointer of kernel_info array
} XOCL_READ_AXLF_ARGS, * PXOCL_READ_AXLF_ARGS;

#define IOCTL_XOCL_READ_AXLF        CTL_CODE(FILE_DEVICE_XOCL_USER, 2076, METHOD_IN_DIRECT, FILE_READ_DATA)


//
// IOCTL_XOCL_MAP_BAR
//
// InBuffer =  XOCL_MAP_BAR_ARGS
// OutBuffer = XOCL_MAP_BAR_RESULT
//
#define IOCTL_XOCL_MAP_BAR      CTL_CODE(FILE_DEVICE_XOCL_USER, 2077, METHOD_BUFFERED, FILE_READ_DATA)

#define XOCL_MAP_BAR_TYPE_USER     0
#define XOCL_MAP_BAR_TYPE_CONFIG   1
#define XOCL_MAP_BAR_TYPE_BYPASS   2
#define XOCL_MAP_BAR_TYPE_MAX      3

typedef struct _XOCL_MAP_BAR_ARGS {
    ULONG BarType; // IN: XOCL_MAP_BAR_TYPE_XXX
} XOCL_MAP_BAR_ARGS, *PXOCL_MAP_BAR_ARGS;

typedef struct _XOCL_MAP_BAR_RESULT {
    PVOID           Bar;           // OUT: User VA of mapped buffer
    ULONGLONG       BarLength;     // OUT: Length of mapped buffer
} XOCL_MAP_BAR_RESULT, *PXOCL_MAP_BAR_RESULT;

//
// IOCTL_XOCL_STAT
//
// InBuffer =  XOCL_STAT_CLASS_ARGS
// OutBuffer = Varies
//
#define IOCTL_XOCL_STAT      CTL_CODE(FILE_DEVICE_XOCL_USER, 2078, METHOD_BUFFERED, FILE_READ_DATA)

typedef enum _XOCL_STAT_CLASS {

    XoclStatDevice = 0xCC,
    XoclStatMemTopology,
    XoclStatMemRaw,
    XoclStatIpLayout,
    XoclStatKds,
    XoclStatKdsCU,
    XoclStatRomInfo,
    XoclStatDebugIpLayout,
    XoclStatTempByMemTopology,
    XoclStatGroupTopology,
    XoclStatMemStatRaw,
    XoclStatMemStat
} XOCL_STAT_CLASS, *PXOCL_STAT_CLASS;

typedef struct _XOCL_STAT_CLASS_ARGS {

    XOCL_STAT_CLASS StatClass;

} XOCL_STAT_CLASS_ARGS, *PXOCL_STAT_CLASS_ARGS;

//
// XoclStatDevice
//
typedef struct _XOCL_DEVICE_INFORMATION {
    ULONG  DeviceNumber;
    USHORT Vendor;
    USHORT Device;
    USHORT SubsystemVendor;
    USHORT SubsystemDevice;
    ULONG  DmaEngineVersion;
    ULONG  DriverVersion;
    ULONG  PciSlot;
    ULONG  MaximumLinkWidth;
    ULONG  LinkWidth;
    ULONG  MaximumLinkSpeed;
    ULONG  LinkSpeed;

} XOCL_DEVICE_INFORMATION, *PXOCL_DEVICE_INFORMATION;

//
// XoclStatMemRaw
//
typedef struct _XOCL_MEM_RAW {
    ULONGLONG               MemoryUsage;
    ULONGLONG               BOCount;
} XOCL_MEM_RAW, *PXOCL_MEM_RAW;

typedef struct _XOCL_MEM_RAW_INFORMATION {

    ULONG        MemRawCount;
    XOCL_MEM_RAW MemRaw[XOCL_MAX_DDR_BANKS];

} XOCL_MEM_RAW_INFORMATION, *PXOCL_MEM_RAW_INFORMATION;

#if 0
//
// XoclStatIpInfo
//
enum IP_TYPE {
    IP_MB = 0,
    IP_KERNEL, //kernel instance
    IP_DNASC,
    IP_DDR4_CONTROLLER
};
#endif

//
// XoclStatKds
//
typedef struct _XOCL_KDS_INFORMATION {
    xuid_t    XclBinUuid;
    ULONG     OutstandingExecs;
    ULONGLONG TotalExecs;
    ULONG     ClientCount;
    ULONG     CDMACount;
    ULONG     CuCount;
} XOCL_KDS_INFORMATION, *PXOCL_KDS_INFORMATION;

//
// XoclStatKdsCU
//
typedef struct _XOCL_KDS_CU {

    ULONG BaseAddress;
    ULONG Usage;

} XOCL_KDS_CU, *PXOCL_KDS_CU;

typedef struct _XOCL_KDS_CU_INFORMATION {

    ULONG       CuCount;
    XOCL_KDS_CU CuInfo[1];

} XOCL_KDS_CU_INFORMATION, *PXOCL_KDS_CU_INFORMATION;

//
// XoclStatRomInfo
//
typedef struct _XOCL_ROM_INFORMATION {
    UCHAR FPGAPartName[64];
    UCHAR VBNVName[64];
    uint8_t DDRChannelCount;
    uint8_t DDRChannelSize;
} XOCL_ROM_INFORMATION, *PXOCL_ROM_INFORMATION;

//
// IOCTL_XOCL_PREAD_BO
//
// Inbuffer =  XOCL_PREAD_BO
// OutBuffer = User data buffer pointer and size (Direct I/O)
//             The OutBuffer length indicates requested size of the read.
//
#define IOCTL_XOCL_PREAD_BO         CTL_CODE(FILE_DEVICE_XOCL_USER, 2100, METHOD_OUT_DIRECT, FILE_READ_DATA)

typedef struct _XOCL_PREAD_BO_ARGS {
    ULONGLONG   Offset;     // IN: BO offset to read from
} XOCL_PREAD_BO_ARGS, *PXOCL_PREAD_BO_ARGS;


//
// IOCTL_XOCL_PWRITE_BO
//
// Inbuffer =  XOCL_PWRITE_BO
// OutBuffer = User data buffer pointer and size (Direct I/O)
//             The OutBuffer length indicates requested size of the write.
//
#define IOCTL_XOCL_PWRITE_BO        CTL_CODE(FILE_DEVICE_XOCL_USER, 2101, METHOD_IN_DIRECT, FILE_READ_DATA)

typedef struct _XOCL_PWRITE_BO_ARGS {
    ULONGLONG   Offset;     // IN: BI offset to write to
} XOCL_PWRITE_BO_ARGS, *PXOCL_PWRITE_BO_ARGS;


//
// IOCTL_XOCL_CTX
//
// Inbuffer = XOCL_CTX_ARGS
// OutBuffer = (not used)
//
#define IOCTL_XOCL_CTX              CTL_CODE(FILE_DEVICE_XOCL_USER, 2102, METHOD_BUFFERED, FILE_READ_DATA)

typedef enum _XOCL_CTX_OPERATION {

    XOCL_CTX_OP_ALLOC_CTX,
    XOCL_CTX_OP_FREE_CTX

}XOCL_CTX_OPERATION, *PXOCL_CTX_OPERATION;

/* Context properties */
#define XOCL_CTX_PROP_MASK         0x0F
#define XOCL_CTX_FLAG_SHARED       0x00
#define XOCL_CTX_FLAG_EXCLUSIVE    0x01

//
// M2 NOTE
// Is this used?
//
/* Virtual CU index
 * This is useful when there is no need to open a context on hardware CU,
 * but still need to lockdown the xclbin.
 */
#define	CU_CTX_VIRT_CU		0xffffffff

typedef struct _XOCL_CTX_ARGS {
    XOCL_CTX_OPERATION Operation;   // IN: Alloc or free context
    xuid_t             XclBinUuid;  // IN: XCLBIN to acquire a context on
    ULONG              CuIndex;     // IN: Compute unit for the request
    ULONG              Flags;       // IN: XOCL_CTX_FLAG_XXX values
} XOCL_CTX_ARGS, *PXOCL_CTX_ARGS;

//
// IOCTL_XOCL_EXECBUF
//
// Inbuffer = XOCL_EXECBUF_ARGS
// OutBuffer = (not used)
//
#define IOCTL_XOCL_EXECBUF          CTL_CODE(FILE_DEVICE_XOCL_USER, 2103, METHOD_BUFFERED, FILE_READ_DATA)

typedef struct _XOCL_EXECBUF_ARGS {
    HANDLE      ExecBO;
} XOCL_EXECBUF_ARGS, *PXOCL_EXECBUF_ARGS;

//
// IOCTL_XOCL_EXECPOLL
//
// Inbuffer = XOCL_EXECPOLL_ARGS
// OutBuffer = (not used)
//
#define IOCTL_XOCL_EXECPOLL          CTL_CODE(FILE_DEVICE_XOCL_USER, 2104, METHOD_BUFFERED, FILE_READ_DATA)

typedef struct _XOCL_EXECPOLL_ARGS {
    ULONG DelayInMS;        // IN: Poll delay in microseconds
} XOCL_EXECPOLL_ARGS, *PXOCL_EXECPOLL_ARGS;

//
// XOCL_PREAD_PWRITE_UNMGD_ARGS
//Read IOCTL to unmanaged DDR memory
// InputBuffer -  XOCL_PREAD_PWRITE_BO_UNMGD_ARGS
//OutputBuffer - (not used)

#define IOCTL_XOCL_PREAD_UNMGD         CTL_CODE(FILE_DEVICE_XOCL_USER, 2105, METHOD_BUFFERED, FILE_READ_DATA)

typedef struct _XOCL_PREAD_PWRITE_UNMGD_ARGS {
    uint32_t address_space; //must be 0. We must to keep it to make the structure compatible with Linux code
    uint32_t pad;           //Currently is unused. We must to keep it to make the structure compatible with Linux code
    uint64_t paddr;         //Physical address in the specified address space
    uint64_t size;          //Length of data to write
    uint64_t data_ptr;      //User's pointer(virtual address) to write the data to
}XOCL_PREAD_PWRITE_UNMGD_ARGS, *PXOCL_PREAD_PWRITE_UNMGD_ARGS;

//
// IOCTL_XOCL_PWRITE_UNMGD
//Write IOCTL to unmanaged DDR memory
// InputBuffer -  XOCL_PREAD_PWRITE_UNMGD_ARGS
//OutputBuffer - (not used)

#define IOCTL_XOCL_PWRITE_UNMGD        CTL_CODE(FILE_DEVICE_XOCL_USER, 2106, METHOD_BUFFERED, FILE_WRITE_DATA)

//
// IOCTL_XOCL_SENSOR_INFO
// Get sensor info
// Inbuffer = (not used)
// OutBuffer = struct xcl_sensor
//
#define IOCTL_XOCL_SENSOR_INFO          CTL_CODE(FILE_DEVICE_XOCL_USER, 2107, METHOD_BUFFERED, FILE_READ_DATA)

//
// IOCTL_XOCL_ICAP_INFO
// Get sensor info
// Inbuffer = (not used)
// OutBuffer = struct xcl_pr_region
//
#define IOCTL_XOCL_ICAP_INFO          CTL_CODE(FILE_DEVICE_XOCL_USER, 2108, METHOD_BUFFERED, FILE_READ_DATA)

//
// IOCTL_XOCL_BOARD_INFO
// Get board info
// Inbuffer = (not used)
// OutBuffer = struct xcl_board_info
//
#define IOCTL_XOCL_BOARD_INFO          CTL_CODE(FILE_DEVICE_XOCL_USER, 2109, METHOD_BUFFERED, FILE_READ_DATA)

//
// IOCTL_XOCL_MIG_ECC_INFO
// Get MIG ECC info
// Inbuffer = (not used)
// OutBuffer = sizeof(struct xcl_sensor) * MAX_M_COUNT
//
#define IOCTL_XOCL_MIG_ECC_INFO          CTL_CODE(FILE_DEVICE_XOCL_USER, 2110, METHOD_BUFFERED, FILE_READ_DATA)

#define MAX_M_COUNT      64

//
// IOCTL_XOCL_FIREWALL_INFO
// Get firewall info
// Inbuffer = (not used)
// OutBuffer = sizeof(struct xcl_firewall)
//
#define IOCTL_XOCL_FIREWALL_INFO          CTL_CODE(FILE_DEVICE_XOCL_USER, 2111, METHOD_BUFFERED, FILE_READ_DATA)


//
// IOCTL_XOCL_MAILBOX_INFO
// Get sensor info
// Inbuffer = (not used)
// OutBuffer = struct xcl_mailbox
//
#define IOCTL_XOCL_MAILBOX_INFO          CTL_CODE(FILE_DEVICE_XOCL_USER, 2112, METHOD_BUFFERED, FILE_READ_DATA)

/**
 * struct xcl_mailbox -  Data structure used to fetch mailbox group
 */
struct xcl_mailbox {
	/* recv metrics */
	uint64_t          mbx_recv_raw_bytes;
	uint64_t          mbx_recv_req[XCL_MAILBOX_REQ_MAX];
};

//
// IOCTL_XOCL_ALLOC_HOST_MEM
//
// InBuffer = XOCL_ALLOC_HOST_MEM_ARGS
// OutBuffer = (not used)
//
//
#define IOCTL_XOCL_ALLOC_HOST_MEM       CTL_CODE(FILE_DEVICE_XOCL_USER, 2201, METHOD_BUFFERED, FILE_READ_DATA)

typedef struct _XOCL_ALLOC_HOST_MEM_ARGS {
    ULONGLONG   SizeToAllocate;     // IN: Size, in bytes, to allocate
} XOCL_ALLOC_HOST_MEM_ARGS, *PXOCL_ALLOC_HOST_MEM_ARGS;

//
// IOCTL_XOCL_FREE_HOST_MEM
//
// InBuffer = (not used)
// OutBuffer = (not used)
//
//
#define IOCTL_XOCL_FREE_HOST_MEM       CTL_CODE(FILE_DEVICE_XOCL_USER, 2202, METHOD_BUFFERED, FILE_READ_DATA)


//
// IOCTL_XOCL_GET_XCLBIN_SW_CHANNEL
//
// InBuffer = (not used)
// OutBuffer = Message to be sent to Management (opaque to the caller)
//
#define IOCTL_XOCL_GET_XCLBIN_SW_CHANNEL       CTL_CODE(FILE_DEVICE_XOCL_USER, 2203, METHOD_OUT_DIRECT, FILE_READ_DATA)


struct drm_xocl_mm_stat {
    size_t memory_usage;
    unsigned int bo_count;
};

