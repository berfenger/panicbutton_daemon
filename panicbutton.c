/** 
 * @file panicbutton.c
 * @author Arturo Casal
 * @date 2010
 * @version v0.1
 *
 * @brief PanicButton Daemon
 *
 * @note Copyright (C) Arturo Casal
 *
 * @par Licences
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <stdio.h>
#include <unistd.h>
#include "panicbutton.h"

#define TRUE 1
#define FALSE 0

int pb_init_device(panicbutton_t *pb) {
    int i, interface, tmp;
    struct usb_bus *bus;
    struct usb_device *dev;
    char found = FALSE;
    char tmpchar[50];

    /* Initialize libusb */
    usb_init();

    /* Find all USB busses on system */
    usb_find_busses();

    /* Find all devices on all USB devices */
    usb_find_devices();
    
    /* usb_busses is a global variable. */
    for (bus = usb_busses; bus; bus = bus->next) {
        for (dev = bus->devices; dev; dev = dev->next) {
            int ret, i;
            char string[256];
            usb_dev_handle *udev;

            if (dev->descriptor.idVendor == PB_VENDOR && dev->descriptor.idProduct==PB_MODEL) {

                printf("Panic Button found.\n", bus->dirname, dev->filename,
                        dev->descriptor.idVendor, dev->descriptor.idProduct);

                /* Opens a USB device */
                udev = usb_open(dev);
                if (udev) {
                    pb->udev = udev;
                    found = TRUE;
                    break;
                } else {
                    fprintf(stderr, "cannot open panic button.\n");
                }
            }
        }
    }

    if (!found) {
        fprintf(stderr, "cannot find panic button.\n");
        return FALSE;
    }

    // Driver detach
    #if defined(LIBUSB_HAS_GET_DRIVER_NP)
    #if defined(LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP)
    for(i=0;i<N_INTERFACES;i++) {
        if(usb_get_driver_np(pb->udev, i, tmpchar, 49) == 0) {
            if(usb_detach_kernel_driver_np(pb->udev, i) < 0) {
                fprintf(stderr, "cannot detach kernel driver '%d'.\n", i);
                return FALSE;
            }

        }
    }
    #endif
    #endif

    // Config
    if(usb_set_configuration(pb->udev, CONFIG_NO) < 0) {
        fprintf(stderr, "Error setting configuration no. %d:.\n", CONFIG_NO);
        return FALSE;
    }

    if(usb_claim_interface(pb->udev, INTERFACE_NO) < 0) {
        fprintf(stderr, "Error claiming interface no. %d.\n", INTERFACE_NO);
        return FALSE;
    }




    return TRUE;
}

int pb_next_event(panicbutton_t *pb) {

    int count, i;
    char data[GET_SIZE];

    while(1) {
        
        count = usb_control_msg(pb->udev, REQTYPE, REQ, VAL, 0, data, GET_SIZE, TIMEOUT);

        if(count < 0) {
                fprintf(stderr, "error reading device. \n");
                return -1;
        } else if (count != 8) {
                fprintf(stderr, "error reading device: unknown answer!\n");
                return -1;
        } else {
                /*if(ord(substr($data, 0, 1)) == 0x1) {
                        return 1;
                }*/
            /*printf("Data: 0x");
            for(i=0;i<count;i++) {
                printf("%x ", data[i]);
            }
            printf("\n");*/
            if (data[0]>=0x1) { return (int)data[0];}
            //found = TRUE;
        }
        usleep(100000);
    }

}

int pb_destroy_device(panicbutton_t *pb) {
    usb_close(pb->udev);
}
