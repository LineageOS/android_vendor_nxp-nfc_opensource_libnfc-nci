/******************************************************************************
 *  Copyright (c) 2016, The Linux Foundation. All rights reserved.
 *  Not a Contribution.
 *
 *  Copyright (C) 2015 NXP Semiconductors
 *  The original Work has been changed by NXP Semiconductors.
 *
 *  Copyright (C) 2012-2014 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

/******************************************************************************
 *
 *  NFC Hardware Abstraction Layer API
 *
 ******************************************************************************/
#ifndef NFC_HAL_API_H
#define NFC_HAL_API_H
#include <hardware/nfc.h>
#include "data_types.h"

/****************************************************************************
** NFC_HDR header definition for NFC messages
*****************************************************************************/
typedef struct
{
    UINT16          event;
    UINT16          len;
    UINT16          offset;
    UINT16          layer_specific;
} NFC_HDR;
#define NFC_HDR_SIZE (sizeof (NFC_HDR))

/*******************************************************************************
** tHAL_STATUS Definitions
*******************************************************************************/
#define HAL_NFC_STATUS_OK               0
#define HAL_NFC_STATUS_FAILED           1
#define HAL_NFC_STATUS_ERR_TRANSPORT    2
#define HAL_NFC_STATUS_ERR_CMD_TIMEOUT  3
#define HAL_NFC_STATUS_REFUSED          4

typedef UINT8 tHAL_NFC_STATUS;

/*******************************************************************************
** tHAL_HCI_NETWK_CMD Definitions
*******************************************************************************/
#define HAL_NFC_HCI_NO_UICC_HOST    0x00
#define HAL_NFC_HCI_UICC0_HOST      0x01
#define HAL_NFC_HCI_UICC1_HOST      0x02
#define HAL_NFC_HCI_UICC2_HOST      0x04

/*******************************************************************************
** tHAL_NFC_CBACK Definitions
*******************************************************************************/

/* tHAL_NFC_CBACK events */
#define HAL_NFC_OPEN_CPLT_EVT           0x00
#define HAL_NFC_CLOSE_CPLT_EVT          0x01
#define HAL_NFC_POST_INIT_CPLT_EVT      0x02
#define HAL_NFC_PRE_DISCOVER_CPLT_EVT   0x03
#define HAL_NFC_REQUEST_CONTROL_EVT     0x04
#define HAL_NFC_RELEASE_CONTROL_EVT     0x05
#define HAL_NFC_ERROR_EVT               0x06
#if(NXP_EXTNS == TRUE)
typedef struct
{
    uint16_t cmd_len;
    uint8_t *p_cmd;
    uint16_t rsp_len;
    uint8_t *p_cmd_rsp;
} nfc_nci_ExtnCmd_t;

enum {
    HAL_NFC_ENABLE_I2C_FRAGMENTATION_EVT = 0x07,
    HAL_NFC_POST_MIN_INIT_CPLT_EVT  = 0x08
};

enum {
    HAL_NFC_IOCTL_P61_IDLE_MODE = 0,
    HAL_NFC_IOCTL_P61_WIRED_MODE,
    HAL_NFC_IOCTL_P61_PWR_MODE,
    HAL_NFC_IOCTL_P61_DISABLE_MODE,
    HAL_NFC_IOCTL_P61_ENABLE_MODE,
    HAL_NFC_IOCTL_SET_BOOT_MODE,
    HAL_NFC_IOCTL_GET_CONFIG_INFO,
    HAL_NFC_IOCTL_CHECK_FLASH_REQ,
    HAL_NFC_IOCTL_FW_DWNLD,
    HAL_NFC_IOCTL_FW_MW_VER_CHECK,
    HAL_NFC_IOCTL_DISABLE_HAL_LOG,
    HAL_NFC_IOCTL_NCI_TRANSCEIVE,
    HAL_NFC_IOCTL_P61_GET_ACCESS,
    HAL_NFC_IOCTL_P61_REL_ACCESS,
    HAL_NFC_IOCTL_ESE_CHIP_RST,
    HAL_NFC_IOCTL_REL_SVDD_WAIT,
    HAL_NFC_IOCTL_SET_JCP_DWNLD_ENABLE,
    HAL_NFC_IOCTL_SET_JCP_DWNLD_DISABLE
};
#endif
typedef void (tHAL_NFC_STATUS_CBACK) (tHAL_NFC_STATUS status);
typedef void (tHAL_NFC_CBACK) (UINT8 event, tHAL_NFC_STATUS status);
typedef void (tHAL_NFC_DATA_CBACK) (UINT16 data_len, UINT8   *p_data);

/*******************************************************************************
** tHAL_NFC_ENTRY HAL entry-point lookup table
*******************************************************************************/

typedef void (tHAL_API_INITIALIZE) (void);
typedef void (tHAL_API_TERMINATE) (void);
typedef void (tHAL_API_OPEN) (tHAL_NFC_CBACK *p_hal_cback, tHAL_NFC_DATA_CBACK *p_data_cback);
typedef void (tHAL_API_CLOSE) (void);
typedef void (tHAL_API_CORE_INITIALIZED) (UINT8 *p_core_init_rsp_params);
typedef void (tHAL_API_WRITE) (UINT16 data_len, UINT8 *p_data);
typedef BOOLEAN (tHAL_API_PREDISCOVER) (void);
typedef void (tHAL_API_CONTROL_GRANTED) (void);
typedef void (tHAL_API_POWER_CYCLE) (void);
typedef UINT8 (tHAL_API_GET_MAX_NFCEE) (void);
#if(NXP_EXTNS == TRUE)
typedef int (tHAL_API_IOCTL) (long arg, void *p_data);
typedef int (tHAL_API_GET_FW_DWNLD_FLAG)(UINT8 *fwDnldRequest);
#endif

#define NFC_HAL_DM_PRE_SET_MEM_LEN  5
typedef struct
{
    UINT32          addr;
    UINT32          data;
} tNFC_HAL_DM_PRE_SET_MEM;

/* data members for NFC_HAL-HCI */
typedef struct
{
    BOOLEAN nfc_hal_prm_nvm_required;       /* set nfc_hal_prm_nvm_required to TRUE, if the platform wants to abort PRM process without NVM */
    UINT16  nfc_hal_nfcc_enable_timeout;    /* max time to wait for RESET NTF after setting REG_PU to high */
    UINT16  nfc_hal_post_xtal_timeout;      /* max time to wait for RESET NTF after setting Xtal frequency */
#if (defined(NFC_HAL_HCI_INCLUDED) && (NFC_HAL_HCI_INCLUDED == TRUE))
    BOOLEAN nfc_hal_first_boot;             /* set nfc_hal_first_boot to TRUE, if platform enables NFC for the first time after bootup */
    UINT8   nfc_hal_hci_uicc_support;       /* set nfc_hal_hci_uicc_support to Zero, if no UICC is supported otherwise set corresponding bit(s) for every supported UICC(s) */
#endif
} tNFC_HAL_CFG;

typedef struct
{
    tHAL_API_INITIALIZE *initialize;
    tHAL_API_TERMINATE *terminate;
    tHAL_API_OPEN *open;
    tHAL_API_CLOSE *close;
    tHAL_API_CORE_INITIALIZED *core_initialized;
    tHAL_API_WRITE *write;
    tHAL_API_PREDISCOVER *prediscover;
    tHAL_API_CONTROL_GRANTED *control_granted;
    tHAL_API_POWER_CYCLE *power_cycle;
    tHAL_API_GET_MAX_NFCEE *get_max_ee;
#if(NXP_EXTNS == TRUE)
    tHAL_API_IOCTL *ioctl;
    tHAL_API_GET_FW_DWNLD_FLAG *check_fw_dwnld_flag;
#endif
} tHAL_NFC_ENTRY;

#if(NXP_EXTNS == TRUE)
typedef struct
{
    tHAL_NFC_ENTRY *hal_entry_func;
    UINT8 boot_mode;
} tHAL_NFC_CONTEXT;
#endif
/*******************************************************************************
** HAL API Function Prototypes
*******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/* Toolset-specific macro for exporting API funcitons */
#if (defined(NFC_HAL_TARGET) && (NFC_HAL_TARGET == TRUE)) && (defined(_WINDLL))
#define EXPORT_HAL_API  __declspec(dllexport)
#else
#define EXPORT_HAL_API
#endif

/*******************************************************************************
**
** Function         HAL_NfcInitialize
**
** Description      Called when HAL library is loaded.
**
**                  Initialize GKI and start the HCIT task
**
** Returns          void
**
*******************************************************************************/
EXPORT_HAL_API void HAL_NfcInitialize(void);

/*******************************************************************************
**
** Function         HAL_NfcTerminate
**
** Description      Called to terminate NFC HAL
**
** Returns          void
**
*******************************************************************************/
EXPORT_HAL_API void HAL_NfcTerminate(void);

/*******************************************************************************
**
** Function         HAL_NfcOpen
**
** Description      Open transport and intialize the NFCC, and
**                  Register callback for HAL event notifications,
**
**                  HAL_OPEN_CPLT_EVT will notify when operation is complete.
**
** Returns          void
**
*******************************************************************************/
EXPORT_HAL_API void HAL_NfcOpen (tHAL_NFC_CBACK *p_hal_cback, tHAL_NFC_DATA_CBACK *p_data_cback);

/*******************************************************************************
**
** Function         HAL_NfcClose
**
** Description      Prepare for shutdown. A HAL_CLOSE_CPLT_EVT will be
**                  reported when complete.
**
** Returns          void
**
*******************************************************************************/
EXPORT_HAL_API void HAL_NfcClose (void);

/*******************************************************************************
**
** Function         HAL_NfcCoreInitialized
**
** Description      Called after the CORE_INIT_RSP is received from the NFCC.
**                  At this time, the HAL can do any chip-specific configuration,
**                  and when finished signal the libnfc-nci with event
**                  HAL_POST_INIT_CPLT_EVT.
**
** Returns          void
**
*******************************************************************************/
EXPORT_HAL_API void HAL_NfcCoreInitialized (UINT8 *p_core_init_rsp_params);

/*******************************************************************************
**
** Function         HAL_NfcWrite
**
** Description      Send an NCI control message or data packet to the
**                  transport. If an NCI command message exceeds the transport
**                  size, HAL is responsible for fragmenting it, Data packets
**                  must be of the correct size.
**
** Returns          void
**
*******************************************************************************/
EXPORT_HAL_API void HAL_NfcWrite (UINT16 data_len, UINT8 *p_data);

/*******************************************************************************
**
** Function         HAL_NfcPreDiscover
**
** Description      Perform any vendor-specific pre-discovery actions (if needed)
**                  If any actions were performed TRUE will be returned, and
**                  HAL_PRE_DISCOVER_CPLT_EVT will notify when actions are
**                  completed.
**
** Returns          TRUE if vendor-specific pre-discovery actions initialized
**                  FALSE if no vendor-specific pre-discovery actions are needed.
**
*******************************************************************************/
EXPORT_HAL_API BOOLEAN HAL_NfcPreDiscover (void);

/*******************************************************************************
**
** Function         HAL_NfcControlGranted
**
** Description      Grant control to HAL control for sending NCI commands.
**
**                  Call in response to HAL_REQUEST_CONTROL_EVT.
**
**                  Must only be called when there are no NCI commands pending.
**
**                  HAL_RELEASE_CONTROL_EVT will notify when HAL no longer
**                  needs control of NCI.
**
**
** Returns          void
**
*******************************************************************************/
EXPORT_HAL_API void HAL_NfcControlGranted (void);

/*******************************************************************************
**
** Function         HAL_NfcPowerCycle
**
** Description      Restart NFCC by power cyle
**
**                  HAL_OPEN_CPLT_EVT will notify when operation is complete.
**
** Returns          void
**
*******************************************************************************/
EXPORT_HAL_API void HAL_NfcPowerCycle (void);

/*******************************************************************************
**
** Function         HAL_NfcGetMaxNfcee
**
** Description      Retrieve the maximum number of NFCEEs supported by NFCC
**
** Returns          the maximum number of NFCEEs supported by NFCC
**
*******************************************************************************/
EXPORT_HAL_API UINT8 HAL_NfcGetMaxNfcee (void);


#ifdef __cplusplus
}
#endif

#endif /* NFC_HAL_API_H  */
