/******************************************************************************
 *  Copyright (c) 2016, The Linux Foundation. All rights reserved.
 *  Not a Contribution.
 *
 *  Copyright (C) 2011-2012 Broadcom Corporation
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
 *  The original Work has been changed by NXP Semiconductors.
 *
 *  Copyright (C) 2015 NXP Semiconductors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
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

 /**
  * @file phNxpConfig.cpp
  * @date 24 Aug 2016
  * @brief File containing code for dynamic selection of config files based on target.
  *
  * The target device has to be configured with some primary setting while booting.So a
  * config file will be picked while the target is booted. Here based on the target device
  * a configuration file will be selected dynamically and the device will be configured.
  */

#include <phNxpConfig.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <sys/stat.h>
#include <stdlib.h>

#include <phNxpLog.h>
#include <cutils/log.h>
#include <cutils/properties.h>
#include <errno.h>

#if GENERIC_TARGET
const char alternative_config_path[] = "/data/nfc/";
#else
const char alternative_config_path[] = "";
#endif

#if 1
const char transport_config_path[] = "/etc/";
const char transit_config_path[] = "/data/nfc/libnfc-nxpTransit.conf";
#else
const char transport_config_path[] = "res/";
#endif

#define config_name             "libnfc-nxp.conf"
#if (NXP_EXTNS == TRUE)
#define extra_config_base       "libnfc-"
#else
#define extra_config_base       "libnfc-nxp-"
#endif
#define extra_config_ext        ".conf"
#define     IsStringValue       0x80000000

const char rf_config_timestamp_path[] = "/data/nfc/libnfc-nxpRFConfigState.bin";
const char tr_config_timestamp_path[] = "/data/nfc/libnfc-nxpTransitConfigState.bin";
const char config_timestamp_path[] = "/data/nfc/libnfc-nxpConfigState.bin";
const char default_nxp_config_path[] = "/etc/libnfc-nxp.conf";
const char nxp_rf_config_path[] = "/system/vendor/libnfc-nxp_RF.conf";

/**
 *  @brief target platform ID values.
 */

typedef enum
{
  CONFIG_GENERIC                         = 0x00,
  MTP_TYPE_DEFAULT                       = 0x01, /**< default MTP config. DC DC ON */
  QRD_TYPE_DEFAULT                       = 0x02, /**< default QRD config DC DC OFF */
  MTP_TYPE_1                             = 0x03, /**< mtp config type1 : newer chip */
  MTP_TYPE_2                             = 0x04, /**< mtp config type2 TBD */
  QRD_TYPE_1                             = 0x05, /**< qrd config type1 DC DC ON*/
  QRD_TYPE_2                             = 0x06, /**< qrd config type2  Newer chip */
  DEFAULT_CONFIG                         = QRD_TYPE_DEFAULT, /**< default is qrd default config */
  CONFIG_INVALID                         = 0xFF
} CONFIGIDVALUE;

/**
 *  @brief Defines the soc_id values for different targets.
 */

typedef enum
{
  TARGET_GENERIC                       = 0x00,/**< new targets */
  TARGET_MSM8952                       = 264, /**< 8952 target */
  TARGET_MSM8976                       = 278, /**< 8976 target */
  TARGET_MSM8937                       = 294, /**< 8937 target */
  TARGET_MSM8953                       = 293, /**< 8953 target */
  TARGET_MSM8996                       = 246, /**< 8996 target*/
  TARGET_MSM8909                       = 245, /**< 8909w target */
  TARGET_MSM8998                       = 292, /**< 8998 target */
  TARGET_MSM8997                       = 306, /**< 8997 target */
  TARGET_MSM8917                       = 303, /**< 8917 target */
  TARGET_MSM8940                       = 313, /**< 8940 target */
  TARGET_DEFAULT                       = TARGET_GENERIC, /**< new targets */
  TARGET_INVALID                       = 0xFF
} TARGETTYPE;

using namespace::std;

namespace nxp {

void readOptionalConfig(const char* optional);

class CNfcParam : public string
{
public:
    CNfcParam();
    CNfcParam(const char* name, const string& value);
    CNfcParam(const char* name, unsigned long value);
    virtual ~CNfcParam();
    unsigned long numValue() const {return m_numValue;}
    const char*   str_value() const {return m_str_value.c_str();}
    size_t        str_len() const   {return m_str_value.length();}
private:
    string          m_str_value;
    unsigned long   m_numValue;
};

class CNfcConfig : public vector<const CNfcParam*>
{
public:
    virtual ~CNfcConfig();
    static CNfcConfig& GetInstance();
    friend void readOptionalConfig(const char* optional);
    int updateTimestamp();
    int checkTimestamp(const char* fileName,const char* fileTimeStamp);

    bool    getValue(const char* name, char* pValue, size_t& len) const;
    bool    getValue(const char* name, unsigned long& rValue) const;
    bool    getValue(const char* name, unsigned short & rValue) const;
    bool    getValue(const char* name, char* pValue, unsigned long len, long* readlen) const;
    const CNfcParam*    find(const char* p_name) const;
    void    readNxpTransitConfig(const char* fileName) const;
    void    readNxpRFConfig(const char* fileName) const;
    void    clean();
private:
    CNfcConfig();
    bool    readConfig(const char* name, bool bResetContent);
    int     file_exist (const char* filename);
    int     getconfiguration_id (char * config_file);
    void    moveFromList();
    void    moveToList();
    void    add(const CNfcParam* pParam);
    void    dump();
    bool    isAllowed(const char* name);
    list<const CNfcParam*> m_list;
    bool    mValidFile;
    bool    mDynamConfig;
    unsigned long m_timeStamp;
    unsigned long m_timeStampRF;
    unsigned long m_timeStampTransit;
    string  mCurrentFile;

    unsigned long   state;

    inline bool Is(unsigned long f) {return (state & f) == f;}
    inline void Set(unsigned long f) {state |= f;}
    inline void Reset(unsigned long f) {state &= ~f;}
};

/**
 * @brief This function reads the hardware information from the given path.
 *
 * This function receives the path and then reads the hardware information
 * from the file present in the given path. It reads the details like whether
 * it is QRD or MTP. It reads the data from that file and stores in buffer.
 * It also receives a count which tells the number of characters to be read
 * Finally the length of the buffer is returned.
 *
 * @param path The path where the file containing hardware details to be read.
 * @param buff The hardware details that is read from that path will be stored here.
 * @param count It represents the number of characters to be read from that file.
 * @return It returns the length of the buffer.
 */

static int read_line_from_file(const char *path, char *buf, size_t count)
{
    char *fgets_ret = NULL;
    FILE *fd = NULL;
    int rv = 0;

    // opens the file to read the HW_PLATFORM detail of the target
    fd = fopen(path, "r");
    if (fd == NULL)
        return -1;

    // stores the data that is read from the given path into buf
    fgets_ret = fgets(buf, (int)count, fd);
    if (NULL != fgets_ret)
        rv = (int)strlen(buf);
    else
        rv = ferror(fd);

    fclose(fd);

    return rv;
}

/**
 * @brief This function gets the source information from the file.
 *
 * This function receives a buffer variable to store the read information
 * and also receives two different path. The hardware information may be
 * present in any one of the received path. So this function checks in
 * both the paths. This function internally uses read_line_from_file
 * function to read the check and read the hardware details in each path.
 *
 * @param buf hardware details that is read will be stored.
 * @param soc_node_path1 The first path where the file may be present.
 * @param soc_node_path2 The second path where the file may be present.
 * @return Returns the length of buffer.
 */

static int get_soc_info(char *buf, const char *soc_node_path1,
            const char *soc_node_path2)
{
    int ret = 0;

    // checks whether the hw platform detail is present in this path
    ret = read_line_from_file(soc_node_path1, buf, MAX_SOC_INFO_NAME_LEN);
    if (ret < 0) {
        // if the hw platform detail is not present in the former path it checks here
        ret = read_line_from_file(soc_node_path2, buf, MAX_SOC_INFO_NAME_LEN);
        if (ret < 0) {
            ALOGE("getting socinfo(%s, %d) failed.\n", soc_node_path1, ret);
            return ret;
        }
    }
    if (ret && buf[ret - 1] == '\n')
        buf[ret - 1] = '\0';

    return ret;
}

/**
 * @brief finds the cofiguration id value for the particular target.
 *
 * This function reads the target board platform detail and hardware
 * platform detail from the target device and generate a generic
 * config file name.If that config file is present then it will be
 * used for configuring that target. If not then based on the target
 * information a config file will be assigned.
 *
 * @param config_file The generic config file name will be stored.
 * @return it returns the config id for the target.
 */

int CNfcConfig::getconfiguration_id (char * config_file)
{
    int config_id = QRD_TYPE_DEFAULT;
    char target_type[MAX_SOC_INFO_NAME_LEN] = {'\0'};
    char soc_info[MAX_SOC_INFO_NAME_LEN] = {'\0'};
    string strPath;
    int rc = 0;
    int idx = 0;

    rc = get_soc_info(soc_info, SYSFS_SOCID_PATH1, SYSFS_SOCID_PATH2);
    if (rc < 0) {
        ALOGE("get_soc_info(SOC_ID) fail!\n");
        return DEFAULT_CONFIG;
    }
    idx = atoi(soc_info);

    rc = get_soc_info(target_type, SYSFS_HW_PLATFORM_PATH1, SYSFS_HW_PLATFORM_PATH2);
    if (rc < 0) {
        ALOGE("get_soc_info(HW_PLATFORM) fail!\n");
        return DEFAULT_CONFIG;
    }

    // Converting the HW_PLATFORM detail that is read from target to lowercase
    for (int i=0;target_type[i];i++)
        target_type[i] = tolower(target_type[i]);

    // generating a generic config file name based on the target details
    snprintf(config_file, MAX_DATA_CONFIG_PATH_LEN, "libnfc-%s_%s.conf",
            soc_info, target_type);

    strPath.assign(transport_config_path);
    strPath += config_file;
    if (file_exist(strPath.c_str()))
        idx = 0;

    if (DEBUG)
        ALOGI("id:%d, config_file_name:%s\n", idx, config_file);

    // if target is QRD platform then config id is assigned here
    if (0 == strncmp(target_type, QRD_HW_PLATFORM, MAX_SOC_INFO_NAME_LEN)) {
        switch (idx)
        {
        case TARGET_GENERIC:
            config_id = CONFIG_GENERIC;
            break;
        case TARGET_MSM8952:
        case TARGET_MSM8953:
        case TARGET_MSM8937:
        case TARGET_MSM8909:
        case TARGET_MSM8917:
        case TARGET_MSM8940:
            config_id = QRD_TYPE_DEFAULT;
            strlcpy(config_file, config_name_qrd, MAX_DATA_CONFIG_PATH_LEN);
            break;
        case TARGET_MSM8976:
        case TARGET_MSM8996:
            strlcpy(config_file, config_name_qrd1, MAX_DATA_CONFIG_PATH_LEN);
            config_id = QRD_TYPE_1;
            break;
        case TARGET_MSM8998:
        case TARGET_MSM8997:
            config_id = QRD_TYPE_2;
            strlcpy(config_file, config_name_qrd2, MAX_DATA_CONFIG_PATH_LEN);
            break;
        default:
            config_id = QRD_TYPE_DEFAULT;
            strlcpy(config_file, config_name_qrd, MAX_DATA_CONFIG_PATH_LEN);
            break;
        }
    }
    // if target is MTP platform then config id is assigned here
    else if (0 == strncmp(target_type, MTP_HW_PLATFORM, MAX_SOC_INFO_NAME_LEN)) {
        switch (idx)
        {
        case TARGET_GENERIC:
            config_id = CONFIG_GENERIC;
            break;
        case TARGET_MSM8998:
        case TARGET_MSM8997:
            config_id = MTP_TYPE_1;
            strlcpy(config_file, config_name_mtp1, MAX_DATA_CONFIG_PATH_LEN);
            break;
        default:
            config_id = MTP_TYPE_DEFAULT;
            strlcpy(config_file, config_name_mtp, MAX_DATA_CONFIG_PATH_LEN);
            break;
        }
    }
    if (DEBUG)
        ALOGI("platform config id:%d, config_file_name:%s\n", config_id, config_file);

    return config_id;
}

/*******************************************************************************
**
** Function:    isPrintable()
**
** Description: determine if 'c' is printable
**
** Returns:     1, if printable, otherwise 0
**
*******************************************************************************/
inline bool isPrintable(char c)
{
    return  (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') ||
            c == '/' || c == '_' || c == '-' || c == '.';
}

/*******************************************************************************
**
** Function:    isDigit()
**
** Description: determine if 'c' is numeral digit
**
** Returns:     true, if numerical digit
**
*******************************************************************************/
inline bool isDigit(char c, int base)
{
    if ('0' <= c && c <= '9')
        return true;
    if (base == 16)
    {
        if (('A' <= c && c <= 'F') ||
            ('a' <= c && c <= 'f') )
            return true;
    }
    return false;
}

/*******************************************************************************
**
** Function:    getDigitValue()
**
** Description: return numerical value of a decimal or hex char
**
** Returns:     numerical value if decimal or hex char, otherwise 0
**
*******************************************************************************/
inline int getDigitValue(char c, int base)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    if (base == 16)
    {
        if ('A' <= c && c <= 'F')
            return c - 'A' + 10;
        else if ('a' <= c && c <= 'f')
            return c - 'a' + 10;
    }
    return 0;
}

/*******************************************************************************
**
** Function:    CNfcConfig::readConfig()
**
** Description: read Config settings and parse them into a linked list
**              move the element from linked list to a array at the end
**
** Returns:     1, if there are any config data, 0 otherwise
**
*******************************************************************************/
bool CNfcConfig::readConfig(const char* name, bool bResetContent)
{
    enum {
        BEGIN_LINE = 1,
        TOKEN,
        STR_VALUE,
        NUM_VALUE,
        BEGIN_HEX,
        BEGIN_QUOTE,
        END_LINE
    };

    FILE*   fd;
    struct stat buf;
    string  token;
    string  strValue;
    unsigned long    numValue = 0;
    CNfcParam* pParam = NULL;
    int     i = 0;
    int     base = 0;
    char    c;
    int     bflag = 0;
    mCurrentFile = name;

    state = BEGIN_LINE;

    // open config file, read it into a buffer
    if ((fd = fopen(name, "rb")) == NULL)
    {
        ALOGE("%s Cannot open config file %s\n", __func__, name);
        if (bResetContent)
        {
            ALOGE("%s Using default value for all settings\n", __func__);
            mValidFile = false;
        }
        return false;
    }
    ALOGD("%s Opened %s config %s\n", __func__, (bResetContent ? "base" : "optional"), name);
    stat(name, &buf);

    if(mDynamConfig)
        m_timeStamp = (unsigned long)buf.st_mtime;
    else {
        if(strcmp(default_nxp_config_path, name) == 0)
            m_timeStamp = (unsigned long)buf.st_mtime;
    }
    if(strcmp(nxp_rf_config_path, name) == 0)
    {
        m_timeStampRF = (unsigned long)buf.st_mtime;
    }
    if(strcmp(transit_config_path, name) == 0)
    {
        m_timeStampTransit = (unsigned long)buf.st_mtime;
    }
    mValidFile = true;
    if (size() > 0)
    {
        if (bResetContent)
        clean();
        else
            moveToList();
    }

    for (;;)
    {
        if (feof(fd) || fread(&c, 1, 1, fd) != 1)
        {
            if (state == BEGIN_LINE)
                break;
            /**
              * got to the EOF but not in BEGIN_LINE state so the file
              * probably does not end with a newline, so the parser has
              * not processed current line, simulate a newline in the file
              */
            c = '\n';
        }

        switch (state & 0xff)
        {
        case BEGIN_LINE:
            if (c == '#')
                state = END_LINE;
            else if (isPrintable(c))
            {
                i = 0;
                token.erase();
                strValue.erase();
                state = TOKEN;
                token.push_back(c);
            }
            break;
        case TOKEN:
            if (c == '=')
            {
                token.push_back('\0');
                state = BEGIN_QUOTE;
            }
            else if (isPrintable(c))
                token.push_back(c);
            else
                state = END_LINE;
            break;
        case BEGIN_QUOTE:
            if (c == '"')
            {
                state = STR_VALUE;
                base = 0;
            }
            else if (c == '0')
                state = BEGIN_HEX;
            else if (isDigit(c, 10))
            {
                state = NUM_VALUE;
                base = 10;
                numValue = getDigitValue(c, base);
                i = 0;
            }
            else if (c == '{')
            {
                state = NUM_VALUE;
                bflag = 1;
                base = 16;
                i = 0;
                Set(IsStringValue);
            }
            else
                state = END_LINE;
            break;
        case BEGIN_HEX:
            if (c == 'x' || c == 'X')
            {
                state = NUM_VALUE;
                base = 16;
                numValue = 0;
                i = 0;
                break;
            }
            else if (isDigit(c, 10))
            {
                state = NUM_VALUE;
                base = 10;
                numValue = getDigitValue(c, base);
                break;
            }
            else if (c != '\n' && c != '\r')
            {
                state = END_LINE;
                break;
            }
            // fall through to numValue to handle numValue

        case NUM_VALUE:
            if (isDigit(c, base))
            {
                numValue *= base;
                numValue += getDigitValue(c, base);
                ++i;
            }
            else if(bflag == 1 && (c == ' ' || c == '\r' || c=='\n' || c=='\t'))
            {
                break;
            }
            else if (base == 16 && (c== ','|| c == ':' || c == '-' || c == ' ' || c == '}'))
            {

                if( c=='}' )
                {
                    bflag = 0;
                }
                if (i > 0)
                {
                    int n = (i+1) / 2;
                    while (n-- > 0)
                    {
                        numValue = numValue >> (n * 8);
                        unsigned char c = (numValue)  & 0xFF;
                        strValue.push_back(c);
                    }
                }

                Set(IsStringValue);
                numValue = 0;
                i = 0;
            }
            else
            {
                if (c == '\n' || c == '\r')
                {
                    if(bflag == 0 )
                    {
                        state = BEGIN_LINE;
                    }
                }
                else
                {
                    if( bflag == 0)
                    {
                        state = END_LINE;
                    }
                }
                if (Is(IsStringValue) && base == 16 && i > 0)
                {
                    int n = (i+1) / 2;
                    while (n-- > 0)
                        strValue.push_back(((numValue >> (n * 8))  & 0xFF));
                }
                if (strValue.length() > 0)
                    pParam = new CNfcParam(token.c_str(), strValue);
                else
                    pParam = new CNfcParam(token.c_str(), numValue);
                add(pParam);
                strValue.erase();
                numValue = 0;
            }
            break;
        case STR_VALUE:
            if (c == '"')
            {
                strValue.push_back('\0');
                state = END_LINE;
                pParam = new CNfcParam(token.c_str(), strValue);
                add(pParam);
            }
            else if (isPrintable(c))
                strValue.push_back(c);
            break;
        case END_LINE:
            if (c == '\n' || c == '\r')
                state = BEGIN_LINE;
            break;
        default:
            break;
        }
    }

    fclose(fd);

    moveFromList();
    return size() > 0;
}

/*******************************************************************************
**
** Function:    CNfcConfig::CNfcConfig()
**
** Description: class constructor
**
** Returns:     none
**
*******************************************************************************/
CNfcConfig::CNfcConfig() :
    mValidFile(true),
    mDynamConfig(true),
    m_timeStamp(0),
    m_timeStampRF(0),
    m_timeStampTransit(0),
    state(0)
{
}

/*******************************************************************************
**
** Function:    CNfcConfig::~CNfcConfig()
**
** Description: class destructor
**
** Returns:     none
**
*******************************************************************************/
CNfcConfig::~CNfcConfig()
{
}

/**
 * @brief checks whether the given file exist.
 *
 * This function gets the file name and checks whether the given file
 * exist in the particular path.Internaly it uses stat system call to
 * find the existance.
 *
 * @param filename The name of the file whose existance has to be checked.
 * @return it returns true if the given file name exist.
 */
int CNfcConfig::file_exist (const char* filename)
{
    struct stat   buffer;
    return (stat (filename, &buffer) == 0);
}

/*******************************************************************************
**
** Function:    CNfcConfig::GetInstance()
**
** Description: get class singleton object
**
** Returns:     none
**
*******************************************************************************/
CNfcConfig& CNfcConfig::GetInstance()
{
    static CNfcConfig theInstance;
    int gconfigpathid=0;
    char config_name_generic[MAX_DATA_CONFIG_PATH_LEN] = {'\0'};

    if (theInstance.size() == 0 && theInstance.mValidFile)
    {
        string strPath;
        if (alternative_config_path[0] != '\0')
        {
            strPath.assign(alternative_config_path);
            strPath += config_name;
            theInstance.readConfig(strPath.c_str(), true);
            if (!theInstance.empty())
            {
                return theInstance;
            }
        }
        strPath.assign(transport_config_path);
        strPath += config_name;
        //checks whether the default config file is present in th target
        if (theInstance.file_exist(strPath.c_str())) {
            ALOGI("default config file exists = %s, disables dynamic selection", strPath.c_str());
            theInstance.mDynamConfig = false;
            theInstance.readConfig(strPath.c_str(), true);
            /*
             * if libnfc-nxp.conf exists then dynamic selection will
             * be turned off by default we will not have this file.
             */
            return theInstance;
        }
        strPath.assign(transport_config_path);

        gconfigpathid = theInstance.getconfiguration_id(config_name_generic);
        strPath += config_name_generic;
        if (!(theInstance.file_exist(strPath.c_str()))) {
            ALOGI("no matching file found, using default file for stability\n");
            strPath.assign(transport_config_path);
            strPath += config_name_default;
        }
        ALOGI("config file used = %s\n",strPath.c_str());
        theInstance.readConfig(strPath.c_str(), true);
#if(NXP_EXTNS == TRUE)
        readOptionalConfig("brcm");
        theInstance.readNxpTransitConfig(transit_config_path);
        theInstance.readNxpRFConfig(nxp_rf_config_path);
#endif
    }
    return theInstance;
}

/*******************************************************************************
**
** Function:    CNfcConfig::getValue()
**
** Description: get a string value of a setting
**
** Returns:     true if setting exists
**              false if setting does not exist
**
*******************************************************************************/
bool CNfcConfig::getValue(const char* name, char* pValue, size_t& len) const
{
    const CNfcParam* pParam = find(name);
    if (pParam == NULL)
        return false;

    if (pParam->str_len() > 0)
    {
        memset(pValue, 0, len);
        if (len > pParam->str_len() - 1)
            len  = pParam->str_len() - 1;
        memcpy(pValue, pParam->str_value(), len);
        return true;
    }
    return false;
}

bool CNfcConfig::getValue(const char* name, char* pValue, unsigned long len, long* readlen) const
{
    const CNfcParam* pParam = find(name);
    if (pParam == NULL)
        return false;

    if (pParam->str_len() > 0)
    {
        if(pParam->str_len() <= len)
        {
            memset(pValue, 0, len);
            memcpy(pValue, pParam->str_value(), pParam->str_len());
            *readlen = pParam->str_len();
        }
        else
        {
            *readlen = -1;
        }

        return true;
    }
    return false;
}

/*******************************************************************************
**
** Function:    CNfcConfig::getValue()
**
** Description: get a long numerical value of a setting
**
** Returns:     true if setting exists
**              false if setting does not exist
**
*******************************************************************************/
bool CNfcConfig::getValue(const char* name, unsigned long& rValue) const
{
    const CNfcParam* pParam = find(name);
    if (pParam == NULL)
        return false;

    if (pParam->str_len() == 0)
    {
        rValue = static_cast<unsigned long>(pParam->numValue());
        return true;
    }
    return false;
}

/*******************************************************************************
**
** Function:    CNfcConfig::getValue()
**
** Description: get a short numerical value of a setting
**
** Returns:     true if setting exists
**              false if setting does not exist
**
*******************************************************************************/
bool CNfcConfig::getValue(const char* name, unsigned short& rValue) const
{
    const CNfcParam* pParam = find(name);
    if (pParam == NULL)
        return false;

    if (pParam->str_len() == 0)
    {
        rValue = static_cast<unsigned short>(pParam->numValue());
        return true;
    }
    return false;
}

/*******************************************************************************
**
** Function:    CNfcConfig::find()
**
** Description: search if a setting exist in the setting array
**
** Returns:     pointer to the setting object
**
*******************************************************************************/
const CNfcParam* CNfcConfig::find(const char* p_name) const
{
    if (size() == 0)
        return NULL;

    for (const_iterator it = begin(), itEnd = end(); it != itEnd; ++it)
    {
        if (**it < p_name)
        {
            continue;
        }
        else if (**it == p_name)
        {
            if((*it)->str_len() > 0)
            {
                NXPLOG_EXTNS_D("%s found %s=%s\n", __func__, p_name, (*it)->str_value());
            }
            else
            {
                NXPLOG_EXTNS_D("%s found %s=(0x%lx)\n", __func__, p_name, (*it)->numValue());
            }
            return *it;
        }
        else
            break;
    }
    return NULL;
}

/*******************************************************************************
**
** Function:    CNfcConfig::readNxpTransitConfig()
**
** Description: read Config settings from transit conf file
**
** Returns:     none
**
*******************************************************************************/
void CNfcConfig::readNxpTransitConfig(const char* fileName) const
{
    ALOGD("readNxpTransitConfig-Enter..Reading %s",fileName);
    CNfcConfig::GetInstance().readConfig(fileName, false);
}

/*******************************************************************************
**
** Function:    CNfcConfig::readNxpRFConfig()
**
** Description: read Config settings from RF conf file
**
** Returns:     none
**
*******************************************************************************/
void CNfcConfig::readNxpRFConfig(const char* fileName) const
{
    ALOGD("readNxpRFConfig-Enter..Reading %s",fileName);
    CNfcConfig::GetInstance().readConfig(fileName, false);
}

/*******************************************************************************
**
** Function:    CNfcConfig::clean()
**
** Description: reset the setting array
**
** Returns:     none
**
*******************************************************************************/
void CNfcConfig::clean()
{
    if (size() == 0)
        return;

    for (iterator it = begin(), itEnd = end(); it != itEnd; ++it)
        delete *it;
    clear();
}

/*******************************************************************************
**
** Function:    CNfcConfig::Add()
**
** Description: add a setting object to the list
**
** Returns:     none
**
*******************************************************************************/
void CNfcConfig::add(const CNfcParam* pParam)
{
    if (m_list.size() == 0)
    {
        m_list.push_back(pParam);
        return;
    }
    if((mCurrentFile.find("nxpTransit") != std::string::npos) && !isAllowed(pParam->c_str()))
    {
        ALOGD("%s Token restricted. Returning", __func__);
        return;
    }
    for (list<const CNfcParam*>::iterator it = m_list.begin(), itEnd = m_list.end(); it != itEnd; ++it)
    {
        if (**it < pParam->c_str())
            continue;
        if (**it == pParam->c_str())
            m_list.insert(m_list.erase(it), pParam);
        else
            m_list.insert(it, pParam);

        return;
    }
    m_list.push_back(pParam);
}
/*******************************************************************************
**
** Function:    CNfcConfig::dump()
**
** Description: prints all elements in the list
**
** Returns:     none
**
*******************************************************************************/
void CNfcConfig::dump()
{
    ALOGD("%s Enter", __func__);

    for (list<const CNfcParam*>::iterator it = m_list.begin(), itEnd = m_list.end(); it != itEnd; ++it)
    {
        if((*it)->str_len()>0)
            ALOGD("%s %s \t= %s", __func__, (*it)->c_str(),(*it)->str_value());
        else
            ALOGD("%s %s \t= (0x%0lX)\n", __func__,(*it)->c_str(),(*it)->numValue());
    }
}
/*******************************************************************************
**
** Function:    CNfcConfig::isAllowed()
**
** Description: checks if token update is allowed
**
** Returns:     true if allowed else false
**
*******************************************************************************/
bool CNfcConfig::isAllowed(const char* name)
{
    string token(name);
    bool stat = false;
    if((token.find("P2P_LISTEN_TECH_MASK") != std::string::npos)        ||
            (token.find("HOST_LISTEN_TECH_MASK") != std::string::npos)  ||
            (token.find("UICC_LISTEN_TECH_MASK") != std::string::npos)  ||
            (token.find("POLLING_TECH_MASK") != std::string::npos)      ||
            (token.find("NXP_RF_CONF_BLK") != std::string::npos)        ||
            (token.find("NXP_CN_TRANSIT_BLK_NUM_CHECK_ENABLE") != std::string::npos) ||
            (token.find("NXP_FWD_FUNCTIONALITY_ENABLE") != std::string::npos))

    {
        stat = true;
    }
    return stat;
}
/*******************************************************************************
**
** Function:    CNfcConfig::moveFromList()
**
** Description: move the setting object from list to array
**
** Returns:     none
**
*******************************************************************************/
void CNfcConfig::moveFromList()
{
    if (m_list.size() == 0)
        return;

    for (list<const CNfcParam*>::iterator it = m_list.begin(), itEnd = m_list.end(); it != itEnd; ++it)
        push_back(*it);
    m_list.clear();
}

/*******************************************************************************
**
** Function:    CNfcConfig::moveToList()
**
** Description: move the setting object from array to list
**
** Returns:     none
**
*******************************************************************************/
void CNfcConfig::moveToList()
{
    if (m_list.size() != 0)
        m_list.clear();

    for (iterator it = begin(), itEnd = end(); it != itEnd; ++it)
        m_list.push_back(*it);
    clear();
}

/*******************************************************************************
**
** Function:    CNfcConfig::checkTimestamp(const char* fileName,const char* fileNameTime)
**
** Description: check if config file has modified
**
** Returns:     0 if not modified, 1 otherwise.
**
*******************************************************************************/
int CNfcConfig::checkTimestamp(const char* fileName,const char* fileNameTime)
{
    FILE*   fd;
    struct stat st;
    unsigned long value = 0,timeStamp = 0;
    int ret = 0;
    if(strcmp(config_timestamp_path,fileNameTime) == 0 )
    {
        timeStamp=m_timeStamp;
    }
    else if(strcmp(rf_config_timestamp_path,fileNameTime) == 0)
    {
        timeStamp=m_timeStampRF;
    }
    else if(strcmp(tr_config_timestamp_path,fileNameTime) == 0)
    {
        timeStamp=m_timeStampTransit;
    }
    else
        ALOGD("Invalid file \n");

    if(stat(fileNameTime, &st) != 0)
    {
        ALOGD("%s file not exist.\n", __func__);
        if ((fd = fopen(fileNameTime, "w+")) != NULL)
        {
            fwrite(&timeStamp, sizeof(unsigned long), 1, fd);
            fclose(fd);
        }
        return 1;
    }
    else
    {
        fd = fopen(fileNameTime, "r+");
        if(fd == NULL)
        {
            ALOGE("%s Cannot open file %s\n", __func__, fileNameTime);
            return 1;
        }
        fread(&value, sizeof(unsigned long), 1, fd);
        ret = (value != timeStamp)?1:0;
        if(ret)
        {
            ALOGD("Config File Modified Update timestamp");
            fseek(fd, 0, SEEK_SET);
            fwrite(&timeStamp, sizeof(unsigned long), 1, fd);
        }
        fclose(fd);
    }
    return ret;
}
/*******************************************************************************
**
** Function:    CNfcConfig::updateTimestamp()
**
** Description: update if config file has modified
**
** Returns:     0 if not modified, 1 otherwise.
**
*******************************************************************************/
int CNfcConfig::updateTimestamp()
{
    FILE*   fd;
    struct stat st;
    unsigned long value = 0;
    int ret = 0;

    if(stat(config_timestamp_path, &st) != 0)
    {
        ALOGD("%s file %s not exist, creat it.\n", __func__, config_timestamp_path);
        if ((fd = fopen(config_timestamp_path, "w+")) != NULL)
        {
            fwrite(&m_timeStamp, sizeof(unsigned long), 1, fd);
            fclose(fd);
        }
        return 1;
    }
    else
    {
        fd = fopen(config_timestamp_path, "r+");
        if(fd == NULL)
        {
            ALOGE("%s Cannot open file %s\n", __func__, config_timestamp_path);
            return 1;
        }

        fread(&value, sizeof(unsigned long), 1, fd);
        ret = (value != m_timeStamp);
        if(ret)
        {
            fseek(fd, 0, SEEK_SET);
            fwrite(&m_timeStamp, sizeof(unsigned long), 1, fd);
        }
        fclose(fd);
    }
    return ret;
}

/*******************************************************************************
**
** Function:    CNfcParam::CNfcParam()
**
** Description: class constructor
**
** Returns:     none
**
*******************************************************************************/
CNfcParam::CNfcParam() :
    m_numValue(0)
{
}

/*******************************************************************************
**
** Function:    CNfcParam::~CNfcParam()
**
** Description: class destructor
**
** Returns:     none
**
*******************************************************************************/
CNfcParam::~CNfcParam()
{
}

/*******************************************************************************
**
** Function:    CNfcParam::CNfcParam()
**
** Description: class copy constructor
**
** Returns:     none
**
*******************************************************************************/
CNfcParam::CNfcParam(const char* name,  const string& value) :
    string(name),
    m_str_value(value),
    m_numValue(0)
{
}

/*******************************************************************************
**
** Function:    CNfcParam::CNfcParam()
**
** Description: class copy constructor
**
** Returns:     none
**
*******************************************************************************/
CNfcParam::CNfcParam(const char* name,  unsigned long value) :
    string(name),
    m_numValue(value)
{
}

/*******************************************************************************
**
** Function:    readOptionalConfig()
**
** Description: read Config settings from an optional conf file
**
** Returns:     none
**
*******************************************************************************/
void readOptionalConfig(const char* extra)
{
    string strPath;
    strPath.assign(transport_config_path);
    if (alternative_config_path[0] != '\0')
        strPath.assign(alternative_config_path);

    strPath += extra_config_base;
    strPath += extra;
    strPath += extra_config_ext;
    CNfcConfig::GetInstance().readConfig(strPath.c_str(), false);
}

} // namespace nxp
/*******************************************************************************
**
** Function:    GetStrValue
**
** Description: API function for getting a string value of a setting
**
** Returns:     length if found, FALSE[0] otherwise.
**
*******************************************************************************/
extern "C" int GetNxpStrValue(const char* name, char* pValue, unsigned long l)
{
    size_t len = l;
    nxp::CNfcConfig& rConfig = nxp::CNfcConfig::GetInstance();

    bool b = rConfig.getValue(name, pValue, len);
    return b ? len : 0;
}

/*******************************************************************************
**
** Function:    GetByteArrayValue()
**
** Description: Read byte array value from the config file.
**
** Parameters:
**              name    - name of the config param to read.
**              pValue  - pointer to input buffer.
**              bufflen - input buffer length.
**              len     - out parameter to return the number of bytes read from config file,
**                        return -1 in case bufflen is not enough.
**
** Returns:     TRUE[1] if config param name is found in the config file, else FALSE[0]
**
*******************************************************************************/
extern "C" int GetNxpByteArrayValue(const char* name, char* pValue, unsigned long bufflen, long *len)
{
    nxp::CNfcConfig& rConfig = nxp::CNfcConfig::GetInstance();

    return rConfig.getValue(name, pValue, bufflen, len);
}

/*******************************************************************************
**
** Function:    GetNumValue
**
** Description: API function for getting a numerical value of a setting
**
** Returns:     true, if successful
**
*******************************************************************************/
extern "C" int GetNxpNumValue(const char* name, void* pValue, unsigned long len)
{
    if (!pValue)
        return false;

    nxp::CNfcConfig& rConfig = nxp::CNfcConfig::GetInstance();
    const nxp::CNfcParam* pParam = rConfig.find(name);

    if (pParam == NULL)
        return false;
    unsigned long v = pParam->numValue();
    if (v == 0 && pParam->str_len() > 0 && pParam->str_len() < 4)
    {
        const unsigned char* p = (const unsigned char*)pParam->str_value();
        for (unsigned int i = 0 ; i < pParam->str_len(); ++i)
        {
            v *= 256;
            v += *p++;
        }
    }
    switch (len)
    {
    case sizeof(unsigned long):
        *(static_cast<unsigned long*>(pValue)) = (unsigned long)v;
        break;
    case sizeof(unsigned short):
        *(static_cast<unsigned short*>(pValue)) = (unsigned short)v;
        break;
    case sizeof(unsigned char):
        *(static_cast<unsigned char*> (pValue)) = (unsigned char)v;
        break;
    default:
        return false;
    }
    return true;
}

/*******************************************************************************
**
** Function:    resetConfig
**
** Description: reset settings array
**
** Returns:     none
**
*******************************************************************************/
extern "C" void resetNxpConfig()

{
    nxp::CNfcConfig& rConfig = nxp::CNfcConfig::GetInstance();

    rConfig.clean();
}

/*******************************************************************************
**
** Function:    isNxpConfigModified()
**
** Description: check if config file has modified
**
** Returns:     0 if not modified, 1 otherwise.
**
*******************************************************************************/
extern "C" int isNxpConfigModified()
{
    nxp::CNfcConfig& rConfig = nxp::CNfcConfig::GetInstance();
    return rConfig.checkTimestamp(default_nxp_config_path,config_timestamp_path);
}
/*******************************************************************************
**
** Function:    isNxpRFConfigModified()
**
** Description: check if config file has modified
**
** Returns:     0 if not modified, 1 otherwise.
**
*******************************************************************************/
extern "C" int isNxpRFConfigModified()
{
    int retRF = 0,rettransit=0,ret=0;
    nxp::CNfcConfig& rConfig = nxp::CNfcConfig::GetInstance();
    retRF=rConfig.checkTimestamp(nxp_rf_config_path,rf_config_timestamp_path);
    rettransit=rConfig.checkTimestamp(transit_config_path,tr_config_timestamp_path);
    ret=retRF | rettransit;
    ALOGD("ret RF or Transit value %d",ret);
    return ret;
}


/*******************************************************************************
**
** Function:    updateNxpConfigTimestamp()
**
** Description: update if config file has modified
**
** Returns:     0 if not modified, 1 otherwise.
**
*******************************************************************************/
extern "C" int updateNxpConfigTimestamp()
{
    nxp::CNfcConfig& rConfig = nxp::CNfcConfig::GetInstance();
    return rConfig.updateTimestamp();
}
