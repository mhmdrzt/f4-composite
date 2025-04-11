/* Src/usbd_hid_mouse.c */
#include "usbd_hid_mouse.h"
#include "usbd_def.h"

extern USBD_HandleTypeDef hUsbDeviceFS;  // Ensure this global is accessible

uint8_t USBD_HID_MOUSE_Init(USBD_HandleTypeDef *pdev)
{
    /* Open endpoint 0x81 as an interrupt IN endpoint with packet size 4 */
    USBD_LL_OpenEP(pdev, 0x81, USBD_EP_TYPE_INTR, 4);
    /* If required, allocate the HID report buffer or do additional init here */
    return USBD_OK;
}

uint8_t USBD_HID_MOUSE_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
    /* Handle class-specific requests like GET_REPORT, SET_REPORT, etc.
       For now, simply return USBD_OK to acknowledge them */
    return USBD_OK;
}
