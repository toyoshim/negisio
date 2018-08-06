#ifndef __usbconfig_h_included__
#define __usbconfig_h_included__

/* ---------------------------- Hardware Config ---------------------------- */
#define USB_CFG_IOPORTNAME D
#define USB_CFG_DMINUS_BIT 3
#define USB_CFG_DPLUS_BIT  2
#define USB_CFG_CLOCK_KHZ  (F_CPU/1000)
#define USB_CFG_CHECK_CRC  0

/* --------------------------- Functional Range ---------------------------- */
#define USB_CFG_HAVE_INTRIN_ENDPOINT 1
#define USB_CFG_INTR_POLL_INTERVAL   10
#define USB_CFG_IS_SELF_POWERED      0
#define USB_CFG_MAX_BUS_POWER        250
#define USB_USE_FAST_CRC             1

/* -------------------------- Device Description --------------------------- */
#define USB_CFG_VENDOR_ID                    0xbc, 0x20
#define USB_CFG_DEVICE_ID                    0x00, 0x55
#define USB_CFG_DEVICE_VERSION               0x00, 0x01
#define USB_CFG_VENDOR_NAME                  'J', 'J'
#define USB_CFG_VENDOR_NAME_LEN              2
#define USB_CFG_DEVICE_CLASS                 0
#define USB_CFG_DEVICE_SUBCLASS              0
#define USB_CFG_INTERFACE_CLASS              3
#define USB_CFG_INTERFACE_SUBCLASS           0
#define USB_CFG_INTERFACE_PROTOCOL           0
#define USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH 97

/* ------------------- Fine Control over USB Descriptors ------------------- */
#define USB_CFG_DESCR_PROPS_DEVICE                  0
#define USB_CFG_DESCR_PROPS_CONFIGURATION           0
#define USB_CFG_DESCR_PROPS_STRINGS                 0
#define USB_CFG_DESCR_PROPS_STRING_0                0
#define USB_CFG_DESCR_PROPS_STRING_VENDOR           0
#define USB_CFG_DESCR_PROPS_STRING_PRODUCT          0
#define USB_CFG_DESCR_PROPS_STRING_SERIAL_NUMBER    0
#define USB_CFG_DESCR_PROPS_HID                     0
#define USB_CFG_DESCR_PROPS_HID_REPORT              0
#define USB_CFG_DESCR_PROPS_UNKNOWN                 0

#endif /* __usbconfig_h_included__ */
