/* Src/usbd_composite.c */
#include "usbd_composite.h"
#include "usbd_def.h"
#include "usbd_hid_mouse.h"
#include "usbd_custom_hid.h"

/* Forward declarations of composite class callbacks */
static uint8_t Composite_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t Composite_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t Composite_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t Composite_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t Composite_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t Composite_EP0_RxReady(USBD_HandleTypeDef *pdev);

/* Composite Class callbacks structure */
USBD_ClassTypeDef USBD_Composite =
{
  Composite_Init,
  Composite_DeInit,
  Composite_Setup,
  NULL,              /* EP0_TxSent */
  Composite_EP0_RxReady,
  Composite_DataIn,
  Composite_DataOut,
  NULL,
  NULL,
  NULL,
};

/* Composite_Init: Initialize both HID interfaces */
static uint8_t Composite_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
    uint8_t ret_mouse, ret_custom;
    ret_mouse = USBD_HID_MOUSE_Init(pdev);
    ret_custom = USBD_CustomHID_Init(pdev);
    
    if ((ret_mouse == USBD_OK) && (ret_custom == USBD_OK))
    {
        return USBD_OK;
    }
    else
    {
        return USBD_FAIL;
    }
}

/* Composite_DeInit: Deinitialize both HID interfaces */
static uint8_t Composite_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
    // If needed, add deinitialization calls for each interface.
    return USBD_OK;
}

/* Composite_Setup: Dispatch class-specific requests based on request type and interface (wIndex) */
static uint8_t Composite_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
    uint8_t ret = USBD_OK;
    
    /* Check the request type */
    if ((req->bmRequest & USB_REQ_TYPE_MASK) == USB_REQ_TYPE_CLASS)
    {
        /* Dispatch based on the interface number in the wIndex field.
           Assumption: Interface 0 is for the mouse HID,
                       Interface 1 is for the custom HID.
        */
        if(req->wIndex == 0)
        {
            ret = USBD_HID_MOUSE_Setup(pdev, req);
        }
        else if(req->wIndex == 1)
        {
            ret = USBD_CustomHID_Setup(pdev, req);
        }
        else
        {
            ret = USBD_FAIL;  // Unknown interface number
        }
    }
    else if ((req->bmRequest & USB_REQ_TYPE_MASK) == USB_REQ_TYPE_STANDARD)
    {
        /* For standard requests, typically the USB core will handle GET_DESCRIPTOR, etc.
           If you need to handle any standard request specifically for your composite device,
           add the logic here. For now, we return USBD_OK for default handling.
        */
        ret = USBD_OK;
    }
    else
    {
        /* For other types (vendor-specific etc.), you could add handling if needed */
        ret = USBD_OK;
    }
    
    return ret;
}

/* Composite_DataIn: Handle data IN events */
static uint8_t Composite_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    /* Dispatch based on endpoint number if necessary.
       For example, if endpoint 0x81 is allocated to the mouse and 0x82 for custom HID IN.
       Uncomment and implement callbacks as needed:
       
       if(epnum == 0x81)
       {
          return USBD_HID_MOUSE_DataIn(pdev, epnum);
       }
       else if(epnum == 0x82)
       {
          return USBD_CustomHID_DataIn(pdev, epnum);
       }
    */
    return USBD_OK;
}

/* Composite_DataOut: Handle data OUT events */
static uint8_t Composite_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
    /* Similarly, dispatch based on endpoint number.
       For example, if endpoint 0x02 is allocated for custom HID OUT:
       
       if(epnum == 0x02)
       {
          return USBD_CustomHID_DataOut(pdev, epnum);
       }
    */
    return USBD_OK;
}

/* Composite_EP0_RxReady: Endpoint0 Rx Ready callback */
static uint8_t Composite_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
    /* Process any control transfer reception here if needed */
    return USBD_OK;
}
