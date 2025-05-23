/* Src/usbd_custom_hid.c */
#include "usbd_custom_hid.h"
#include "usbd_composite.h"
#include "usbd_def.h"

__ALIGN_BEGIN uint8_t Custom_HID_ReportDesc[] __ALIGN_END = {
  0x06, 0x00, 0xFF,  // Usage Page (Vendor Defined 0xFF00)
  0x09, 0x01,        // Usage (Vendor Usage 1)
  0xA1, 0x01,        // Collection (Application)
	0x85, 0x02,       //   << REPORT ID 2
    0x15, 0x00,      //   Logical Minimum (0)
    0x26, 0xFF, 0x00,//   Logical Maximum (255)
    0x75, 0x08,      //   Report Size (8)
    0x95, 0x08,      //   Report Count (8)
    0x09, 0x01,      //   Usage (Vendor Usage 1) 
    0x81, 0x00,      //   Input (Data, Array)
    0x95, 0x08,      //   Report Count (8)
    0x09, 0x01,      //   Usage (Vendor Usage 1)
    0x91, 0x00,      //   Output (Data, Array)
  0xC0               // End Collection
};

static uint8_t CustomHIDRxBuffer[9];

uint8_t* USBD_CustomHID_GetReportDescriptor(uint16_t* length)
{
	*length = CUSTOM_HID_REPORT_DESC_SIZE;
	return (uint8_t*)Custom_HID_ReportDesc;
}
uint8_t USBD_CustomHID_Init(USBD_HandleTypeDef *pdev)
{
	
	USBD_COMPOSITE_HandleTypeDef *composite = (USBD_COMPOSITE_HandleTypeDef *)pdev->pClassData;
	/* Set interval */
	pdev->ep_in[0x82 & 0xFU].bInterval = 0x5;
  pdev->ep_out[0x02 & 0xFU].bInterval = 0x5;
	/* Open IN endpoint 0x82 and OUT endpoint 0x02 for the custom HID */
	USBD_LL_OpenEP(pdev, 0x82, USBD_EP_TYPE_INTR, 9);   // IN endpoint
	pdev->ep_in[0x82 & 0xFU].is_used = 1U;
	USBD_LL_OpenEP(pdev, 0x02, USBD_EP_TYPE_INTR, 9);   // OUT endpoint
	pdev->ep_out[0x02 & 0xFU].is_used = 1U;
	memset(&composite->custom, 0, sizeof(composite->custom));
	USBD_LL_PrepareReceive(pdev, 0x02, CustomHIDRxBuffer, sizeof(CustomHIDRxBuffer));

	return USBD_OK;
}

extern uint8_t custom_rx_buff[9];
extern uint8_t new_packet;
uint8_t USBD_CustomHID_DataOut(USBD_HandleTypeDef *pdev)
{
	USBD_LL_PrepareReceive(pdev, 0x02, CustomHIDRxBuffer, sizeof(CustomHIDRxBuffer));

	memcpy(custom_rx_buff, CustomHIDRxBuffer, 9);
	new_packet = 1;
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
  return USBD_OK;
}
uint8_t USBD_CustomHID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	  USBD_COMPOSITE_HandleTypeDef *composite = (USBD_COMPOSITE_HandleTypeDef *)pdev->pClassData;
    composite->custom.state = CUSTOM_HID_IDLE;
    return USBD_OK;
}
uint32_t try_cnt_custom = 0;
uint32_t last_tick_custom = 0;
uint8_t USBD_CUSTOM_HID_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report, uint16_t len)
{
	USBD_COMPOSITE_HandleTypeDef *composite = (USBD_COMPOSITE_HandleTypeDef *)pdev->pClassData;
	USBD_CUSTOM_HID_HandleTypeDef *hhid = &composite->custom;
		
	if (pdev->dev_state == USBD_STATE_CONFIGURED)
	{
		if (hhid->state == CUSTOM_HID_IDLE || (HAL_GetTick() - last_tick_custom > 50))
		{
			hhid->state = CUSTOM_HID_BUSY;
			return USBD_LL_Transmit(pdev, 0x82, report, len);
		}
	}
	last_tick_custom = HAL_GetTick();
	return USBD_BUSY;
}

uint8_t USBD_CustomHID_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
	/* TODO: Handle custom HID class-specific requests */
	return USBD_OK;
}
