/**
 * @file    usbd_desc.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for USB descriptor interface layer
 * @note    Based on the STM32 usbd_desc_template.c
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "usbd_core.h"
#include "usbd_conf.h"
#include "usb-otg/usbd_desc.h"

/***************************** Macros Definitions ****************************/

#define DEVICE_ID1                   (UID_BASE)         /**< Address of the first 32 bits of the unique device ID */
#define DEVICE_ID2                   (UID_BASE + 0x4UL) /**< Address of the second 32 bits of the unique device ID */
#define DEVICE_ID3                   (UID_BASE + 0x8UL) /**< Address of the third 32 bits of the unique device ID */

#define USBD_VID                     0xf055                  /**< USB Vendor ID (VID) (here open source ID) */
#define USBD_MANUFACTURER_STRING     "TOLOSAT"               /**< Manufacturer name string for USB descriptor */
#define USBD_PID_FS                  0x4e37                  /**< USB Product ID (PID) for the full-speed device */
#define USBD_PRODUCT_STRING_FS       "TAPAS Flight Software" /**< Product name string shown to the host */
#define USBD_CONFIGURATION_STRING_FS "MSC Config"            /**< USB configuration name string */
#define USBD_INTERFACE_STRING_FS     "MSC Interface"         /**< USB interface name string */
#define USBD_LANGID_STRING           1033                    /**< Language ID for USB string descriptors (1033 = English - United States) */

#define USB_SIZ_BOS_DESC             0x0C /**< Size of the BOS (Binary Object Store) descriptor */

#define USB_SIZ_STRING_SERIAL        0x1A /**< Size of the USB serial number string descriptor */

/*************************** Functions Declarations **************************/

static void Get_SerialNum(void);
static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len);

uint8_t *USBD_FS_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);

/*************************** Variables Definitions ***************************/

/**
 * @var     g_usbd_descriptor_if
 * @brief   USB Device descriptor interface
 *
 * This interface contains all function allowing the libUSB to handle provide the descriptor
 */
USBD_DescriptorsTypeDef g_usbd_descriptor_if = { USBD_FS_DeviceDescriptor,          USBD_FS_LangIDStrDescriptor,
                                                 USBD_FS_ManufacturerStrDescriptor, USBD_FS_ProductStrDescriptor,
                                                 USBD_FS_SerialStrDescriptor,       USBD_FS_ConfigStrDescriptor,
                                                 USBD_FS_InterfaceStrDescriptor,    NULL };

/**
 * @brief USB standard device descriptor
 */
__ALIGN_BEGIN uint8_t USBD_FS_DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END = {
    0x12,                 /*bLength */
    USB_DESC_TYPE_DEVICE, /*bDescriptorType*/
    0x00,                 /*bcdUSB */
    0x02,
    0x00,                /*bDeviceClass*/
    0x00,                /*bDeviceSubClass*/
    0x00,                /*bDeviceProtocol*/
    USB_MAX_EP0_SIZE,    /*bMaxPacketSize*/
    LOBYTE(USBD_VID),    /*idVendor*/
    HIBYTE(USBD_VID),    /*idVendor*/
    LOBYTE(USBD_PID_FS), /*idProduct*/
    HIBYTE(USBD_PID_FS), /*idProduct*/
    0x00,                /*bcdDevice rel. 2.00*/
    0x02,
    USBD_IDX_MFC_STR,          /*Index of manufacturer  string*/
    USBD_IDX_PRODUCT_STR,      /*Index of product string*/
    USBD_IDX_SERIAL_STR,       /*Index of serial number string*/
    USBD_MAX_NUM_CONFIGURATION /*bNumConfigurations*/
};

#if defined(USBD_LPM_ENABLED) && (USBD_LPM_ENABLED == 1U)
/**
 * @brief BOS descriptor
 */
__ALIGN_BEGIN uint8_t USBD_FS_BOSDesc[USB_SIZ_BOS_DESC] __ALIGN_END = { 0x5, USB_DESC_TYPE_BOS, 0xC, 0x0, 0x1,   /* 1 device capability*/
                                                                                                                 /* device capability*/
                                                                        0x7, USB_DEVICE_CAPABITY_TYPE, 0x2, 0x2, /* LPM capability bit set*/
                                                                        0x0, 0x0, 0x0 };
#endif /* (USBD_LPM_ENABLED == 1) */

/**
 * @brief USB lang identifier descriptor.
 */
__ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END = { USB_LEN_LANGID_STR_DESC, USB_DESC_TYPE_STRING,
                                                                               LOBYTE(USBD_LANGID_STRING), HIBYTE(USBD_LANGID_STRING) };

/**
 * @brief Internal string descriptor.
 */
/*  */
__ALIGN_BEGIN uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

/**
 * @brief String Serial
 */
__ALIGN_BEGIN uint8_t USBD_StringSerial[USB_SIZ_STRING_SERIAL] __ALIGN_END = {
    USB_SIZ_STRING_SERIAL,
    USB_DESC_TYPE_STRING,
};

/*************************** Functions Definitions ***************************/

/**
 * @brief  Return the device descriptor
 * @param  speed : Current device speed
 * @param  length : Pointer to data length variable
 * @retval Pointer to descriptor buffer
 */
uint8_t *USBD_FS_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    *length = sizeof(USBD_FS_DeviceDesc);
    return USBD_FS_DeviceDesc;
}

/**
 * @brief  Return the LangID string descriptor
 * @param  speed : Current device speed
 * @param  length : Pointer to data length variable
 * @retval Pointer to descriptor buffer
 */
uint8_t *USBD_FS_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    *length = sizeof(USBD_LangIDDesc);
    return USBD_LangIDDesc;
}

/**
 * @brief  Return the product string descriptor
 * @param  speed : Current device speed
 * @param  length : Pointer to data length variable
 * @retval Pointer to descriptor buffer
 */
uint8_t *USBD_FS_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    if (speed == 0)
    {
        USBD_GetString((uint8_t *)USBD_PRODUCT_STRING_FS, USBD_StrDesc, length); // cppcheck-suppress misra-c2012-11.8; const qualifier has not been
                                                                                 // provided by the libUSB
    }
    else
    {
        USBD_GetString((uint8_t *)USBD_PRODUCT_STRING_FS, USBD_StrDesc, length); // cppcheck-suppress misra-c2012-11.8; const qualifier has not been
                                                                                 // provided by the libUSB
    }
    return USBD_StrDesc;
}

/**
 * @brief  Return the manufacturer string descriptor
 * @param  speed : Current device speed
 * @param  length : Pointer to data length variable
 * @retval Pointer to descriptor buffer
 */
uint8_t *USBD_FS_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    USBD_GetString((uint8_t *)USBD_MANUFACTURER_STRING, USBD_StrDesc, length); // cppcheck-suppress misra-c2012-11.8; const qualifier has not been
                                                                               // provided by the libUSB
    return USBD_StrDesc;
}

/**
 * @brief  Return the serial number string descriptor
 * @param  speed : Current device speed
 * @param  length : Pointer to data length variable
 * @retval Pointer to descriptor buffer
 */
uint8_t *USBD_FS_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    *length = USB_SIZ_STRING_SERIAL;

    /* Update the serial number string descriptor with the data from the unique
     * ID */
    Get_SerialNum();

    return (uint8_t *)USBD_StringSerial;
}

/**
 * @brief  Return the configuration string descriptor
 * @param  speed : Current device speed
 * @param  length : Pointer to data length variable
 * @retval Pointer to descriptor buffer
 */
uint8_t *USBD_FS_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    if (speed == USBD_SPEED_HIGH)
    {
        USBD_GetString((uint8_t *)USBD_CONFIGURATION_STRING_FS, USBD_StrDesc, length); // cppcheck-suppress misra-c2012-11.8; const qualifier has not
                                                                                       // been provided by the libUSB
    }
    else
    {
        USBD_GetString((uint8_t *)USBD_CONFIGURATION_STRING_FS, USBD_StrDesc, length); // cppcheck-suppress misra-c2012-11.8; const qualifier has not
                                                                                       // been provided by the libUSB
    }
    return USBD_StrDesc;
}

/**
 * @brief  Return the interface string descriptor
 * @param  speed : Current device speed
 * @param  length : Pointer to data length variable
 * @retval Pointer to descriptor buffer
 */
uint8_t *USBD_FS_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
    if (speed == 0)
    {
        USBD_GetString((uint8_t *)USBD_INTERFACE_STRING_FS, USBD_StrDesc, length); // cppcheck-suppress misra-c2012-11.8; const qualifier has not been
                                                                                   // provided by the libUSB
    }
    else
    {
        USBD_GetString((uint8_t *)USBD_INTERFACE_STRING_FS, USBD_StrDesc, length); // cppcheck-suppress misra-c2012-11.8; const qualifier has not been
                                                                                   // provided by the libUSB
    }
    return USBD_StrDesc;
}

/**
 * @brief  Create the serial number string descriptor
 * @param  None
 * @retval None
 */
static void Get_SerialNum(void)
{
    uint32_t deviceserial0;
    uint32_t deviceserial1;
    uint32_t deviceserial2;

    // Read the 96-bit unique device ID from system memory
    deviceserial0 = *(uint32_t *)DEVICE_ID1;
    deviceserial1 = *(uint32_t *)DEVICE_ID2;
    deviceserial2 = *(uint32_t *)DEVICE_ID3;

    // Mix part of the unique ID for more variability (optional)
    deviceserial0 += deviceserial2;

    // Convert different parts of the unique device ID to Unicode and insert them into the USB serial string,
    // combining both significant and less significant bits to ensure a sufficiently unique serial number
    IntToUnicode(deviceserial0, &USBD_StringSerial[2], 8);
    IntToUnicode(deviceserial1, &USBD_StringSerial[18], 4);
}

/**
 * @brief  Convert Hex 32Bits value into char
 * @param  value: value to convert
 * @param  pbuf: pointer to the buffer
 * @param  len: buffer length
 * @retval None
 */
static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len)
{
    uint32_t remaining_value = value;

    for (uint8_t idx = 0u; idx < len; idx++)
    {
        if (((remaining_value >> 28)) < 0x0Au)
        {
            pbuf[2u * idx] = (remaining_value >> 28) + (uint8_t)'0';
        }
        else
        {
            pbuf[2u * idx] = (remaining_value >> 28) + (uint8_t)'A' - 10u;
        }

        remaining_value = remaining_value << 4;

        pbuf[(2u * idx) + 1u] = 0u;
    }
}
