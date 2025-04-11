/* usbd_hid_mouse.h */
#ifndef __USBD_HID_MOUSE_H
#define __USBD_HID_MOUSE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "usbd_def.h"

uint8_t USBD_HID_MOUSE_Init(USBD_HandleTypeDef *pdev);
uint8_t USBD_HID_MOUSE_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_HID_MOUSE_H */
