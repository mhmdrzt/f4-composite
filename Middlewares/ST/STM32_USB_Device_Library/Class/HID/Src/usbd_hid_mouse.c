/* Src/usbd_hid_mouse.c */
#include "usbd_hid_mouse.h"
#include "usbd_composite.h"
#include "usbd_def.h"
#include "usbd_ioreq.h"
#include "usbd_desc.h"


__ALIGN_BEGIN uint8_t HID_Mouse_ReportDesc[] __ALIGN_END = {
  0x05, 0x01,       // Usage Page (Generic Desktop)
  0x09, 0x02,       // Usage (Mouse)
  0xA1, 0x01,       // Collection (Application)
  0x09, 0x01,       //   Usage (Pointer)
  0xA1, 0x00,       //   Collection (Physical)
  0x05, 0x09,       //     Usage Page (Buttons)
  0x19, 0x01,       //     Usage Minimum (1)
  0x29, 0x03,       //     Usage Maximum (3)
  0x15, 0x00,       //     Logical Minimum (0)
  0x25, 0x01,       //     Logical Maximum (1)
  0x95, 0x03,       //     Report Count (3)
  0x75, 0x01,       //     Report Size (1)
  0x81, 0x02,       //     Input (Data, Variable, Absolute) - 3 button bits
  0x95, 0x01,       //     Report Count (1)
  0x75, 0x05,       //     Report Size (5)
  0x81, 0x03,       //     Input (Constant) - 5 bit padding
  0x05, 0x01,       //     Usage Page (Generic Desktop)
  0x09, 0x30,       //     Usage (X)
  0x09, 0x31,       //     Usage (Y)
  0x15, 0x81,       //     Logical Minimum (-127)
  0x25, 0x7F,       //     Logical Maximum (127)
  0x75, 0x08,       //     Report Size (8)
  0x95, 0x02,       //     Report Count (2)
  0x81, 0x06,       //     Input (Data, Variable, Relative)
  0xC0,             //   End Collection
  0xC0              // End Collection
};



extern USBD_HandleTypeDef hUsbDeviceFS;  // Ensure this global is accessible

uint8_t USBD_HID_MOUSE_Init(USBD_HandleTypeDef *pdev)
{
	USBD_COMPOSITE_HandleTypeDef *composite = (USBD_COMPOSITE_HandleTypeDef *)pdev->pClassData;
	/* Set interval */
	pdev->ep_in[0x81 & 0xFU].bInterval = 0x5;
	/* Open endpoint 0x81 as an interrupt IN endpoint with packet size 4 */
	USBD_LL_OpenEP(pdev, 0x81, USBD_EP_TYPE_INTR, 4);
	pdev->ep_in[0x81 & 0xFU].is_used = 1U;
	/* Set interface state to IDLE */
	memset(&composite->mouse, 0, sizeof(composite->mouse));
	return USBD_OK;
}


static uint8_t HID_MOUSE_Protocol = 1; // 1 = Report Protocol (default), 0 = Boot Protocol
uint8_t USBD_HID_MOUSE_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
    /* Handle class-specific requests like GET_REPORT, SET_REPORT, etc.
       For now, simply return USBD_OK to acknowledge them */
	USBD_StatusTypeDef ret = USBD_OK;
  return ret;
}
uint32_t last_tick_mouse = 0;
uint8_t USBD_HID_MOUSE_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report, uint16_t len)
{
    USBD_COMPOSITE_HandleTypeDef *composite = (USBD_COMPOSITE_HandleTypeDef *)pdev->pClassData;
    USBD_HID_MOUSE_HandleTypeDef *hhid = &composite->mouse;

    if (pdev->dev_state == USBD_STATE_CONFIGURED)
    {
        if (hhid->state == HID_MOUSE_IDLE|| (HAL_GetTick() - last_tick_mouse > 50))
        {
            hhid->state = HID_MOUSE_BUSY;
            return USBD_LL_Transmit(pdev, 0x81, report, len);
        }
    }
last_tick_mouse = HAL_GetTick();
    return USBD_BUSY;
}
uint8_t USBD_HID_MOUSE_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	USBD_COMPOSITE_HandleTypeDef *composite = (USBD_COMPOSITE_HandleTypeDef *)pdev->pClassData;
  composite->mouse.state = HID_MOUSE_IDLE;
  return USBD_OK;
}

uint8_t* USBD_HID_MOUSE_GetReportDescriptor(uint16_t* length)
{
    *length = HID_MOUSE_REPORT_DESC_SIZE;
    return (uint8_t*)HID_Mouse_ReportDesc;
}