/*
 * Copyright (C) 2012-2016 NXP Semiconductors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * NXP NFCC features macros definitions
 */

#ifndef NXP_NFCC_FEATURES_H
#define NXP_NFCC_FEATURES_H
/*Flags common to all chip types*/
#define NXP_NFCC_EMPTY_DATA_PACKET              TRUE
#define GEMALTO_SE_SUPPORT                      TRUE

#if(NFC_NXP_CHIP_TYPE == PN553)
#define NXP_NFCC_I2C_READ_WRITE_IMPROVEMENT     TRUE
#define NXP_NFCC_MIFARE_TIANJIN                 FALSE
#define NXP_NFCC_MW_RCVRY_BLK_FW_DNLD           TRUE
#if(NQ_NFC_DUAL_UICC == FALSE)
#define NXP_NFCC_DYNAMIC_DUAL_UICC              FALSE
#else
#define NXP_NFCC_DYNAMIC_DUAL_UICC              TRUE
#endif
#define NXP_NFCC_FW_WA                          TRUE
#define NXP_NFCC_FORCE_NCI1_0_INIT              FALSE
#define NXP_NFCC_ROUTING_BLOCK_BIT              TRUE
#define NFC_NXP_STAT_DUAL_UICC_EXT_SWITCH       FALSE
#if(NQ_NFC_DUAL_UICC == FALSE)
#define NFC_NXP_STAT_DUAL_UICC_WO_EXT_SWITCH    FALSE
#else
#define NFC_NXP_STAT_DUAL_UICC_WO_EXT_SWITCH    TRUE
#endif
#define NXP_NFCC_SPI_FW_DOWNLOAD_SYNC           TRUE
#define NXP_HW_ANTENNA_LOOP4_SELF_TEST          FALSE
#define NXP_NFCEE_REMOVED_NTF_RECOVERY          TRUE
#define NXP_NFCC_FORCE_FW_DOWNLOAD              TRUE
#define NXP_UICC_CREATE_CONNECTIVITY_PIPE       TRUE
#if(NFC_NXP_ESE == TRUE)
#define NXP_NFCC_SPI_FW_DOWNLOAD_SYNC           TRUE
#define NFA_EE_MAX_EE_SUPPORTED                 4
#else
#define NFA_EE_MAX_EE_SUPPORTED                 3
#endif
#elif(NFC_NXP_CHIP_TYPE == PN551)
#define NXP_NFCC_I2C_READ_WRITE_IMPROVEMENT     TRUE
#define NXP_NFCC_AID_MATCHING_PLATFORM_CONFIG   TRUE
#define NXP_NFCC_DYNAMIC_DUAL_UICC              FALSE
#define NXP_NFCC_ROUTING_BLOCK_BIT_PROP         TRUE
#define NXP_NFCC_MIFARE_TIANJIN                 TRUE
#define NFC_NXP_STAT_DUAL_UICC_EXT_SWITCH       TRUE
#define NFC_NXP_STAT_DUAL_UICC_WO_EXT_SWITCH    FALSE
#define NXP_NFCC_SPI_FW_DOWNLOAD_SYNC           FALSE
#define NXP_HW_ANTENNA_LOOP4_SELF_TEST          TRUE
#define NXP_NFCEE_REMOVED_NTF_RECOVERY          TRUE
#define NXP_NFCC_FORCE_FW_DOWNLOAD              FALSE
#define NXP_UICC_CREATE_CONNECTIVITY_PIPE       FALSE
#if(NFC_NXP_ESE == TRUE)
#define NFA_EE_MAX_EE_SUPPORTED                 3
#else
#define NFA_EE_MAX_EE_SUPPORTED                 2
#endif
#elif(NFC_NXP_CHIP_TYPE == PN548C2)
#define NXP_NFCC_I2C_READ_WRITE_IMPROVEMENT     TRUE
#define NXP_NFCC_AID_MATCHING_PLATFORM_CONFIG   TRUE
#define NXP_NFCC_DYNAMIC_DUAL_UICC              FALSE
#define NXP_NFCC_ROUTING_BLOCK_BIT_PROP         TRUE
#define NXP_NFCC_MIFARE_TIANJIN                 TRUE
#define NFC_NXP_STAT_DUAL_UICC_EXT_SWITCH       TRUE
#define NFC_NXP_STAT_DUAL_UICC_WO_EXT_SWITCH    FALSE
#define NXP_NFCC_SPI_FW_DOWNLOAD_SYNC           FALSE
#define NXP_HW_ANTENNA_LOOP4_SELF_TEST          TRUE
#define NXP_NFCEE_REMOVED_NTF_RECOVERY          TRUE
#define NXP_NFCC_FORCE_FW_DOWNLOAD              FALSE
#define NXP_UICC_CREATE_CONNECTIVITY_PIPE       FALSE
#if(NFC_NXP_ESE == TRUE)
#define NFA_EE_MAX_EE_SUPPORTED                 3
#else
#define NFA_EE_MAX_EE_SUPPORTED                 2
#endif
#elif(NFC_NXP_CHIP_TYPE  == PN547C2)
#define NXP_NFCC_I2C_READ_WRITE_IMPROVEMENT     FALSE
#define NXP_NFCC_AID_MATCHING_PLATFORM_CONFIG   TRUE
#define NXP_NFCC_MIFARE_TIANJIN                 TRUE
#define NXP_NFCC_SPI_FW_DOWNLOAD_SYNC           FALSE
#define NXP_HW_ANTENNA_LOOP4_SELF_TEST          TRUE
#define NXP_NFCEE_REMOVED_NTF_RECOVERY          TRUE
#define NXP_NFCC_FORCE_FW_DOWNLOAD              FALSE
#define NXP_UICC_CREATE_CONNECTIVITY_PIPE       FALSE
#if(NFC_NXP_ESE == TRUE)
#define NFA_EE_MAX_EE_SUPPORTED                 3
#else
#define NFA_EE_MAX_EE_SUPPORTED                 2
#endif
#endif
#endif                          /* end of #ifndef NXP_NFCC_FEATURES_H */
