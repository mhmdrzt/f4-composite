/* usbd_hid_mouse.h */
#ifndef __USBD_HID_MOUSE_H
#define __USBD_HID_MOUSE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "usbd_def.h"

typedef struct {
  uint8_t state;
} USBD_HID_MOUSE_HandleTypeDef;
#define HID_MOUSE_IDLE 0
#define HID_MOUSE_BUSY 1

extern uint8_t HID_Mouse_ReportDesc[];
#define HID_MOUSE_REPORT_DESC_SIZE   50//(sizeof(HID_Mouse_ReportDesc))

uint8_t USBD_HID_MOUSE_Init(USBD_HandleTypeDef *pdev);
uint8_t USBD_HID_MOUSE_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
uint8_t* USBD_HID_MOUSE_GetReportDescriptor(uint16_t* length);
uint8_t USBD_HID_MOUSE_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_HID_MOUSE_H */
