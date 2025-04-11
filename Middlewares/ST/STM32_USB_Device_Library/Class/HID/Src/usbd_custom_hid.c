/* Src/usbd_custom_hid.c */
#include "usbd_custom_hid.h"
#include "usbd_def.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

uint8_t USBD_CustomHID_Init(USBD_HandleTypeDef *pdev)
{
    /* Open IN endpoint 0x82 and OUT endpoint 0x02 for the custom HID */
    USBD_LL_OpenEP(pdev, 0x82, USBD_EP_TYPE_INTR, 8);   // IN endpoint
    USBD_LL_OpenEP(pdev, 0x02, USBD_EP_TYPE_INTR, 8);   // OUT endpoint
    return USBD_OK;
}

uint8_t USBD_CustomHID_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
    /* Handle custom HID class-specific requests (e.g. GET_REPORT, SET_REPORT, etc.) */
    return USBD_OK;
}
