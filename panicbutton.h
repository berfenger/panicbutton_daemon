/** 
 * @file panicbutton.h
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

#ifndef _PANICBUTTON_H
#define	_PANICBUTTON_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <usb.h>

    #define PB_VENDOR 0x1130
    #define PB_MODEL 0x0202

    #define N_INTERFACES 2
    #define INTERFACE_NO 0
    #define CONFIG_NO 1

    #define REQTYPE 0xA1
    #define REQ 0x1
    #define VAL 0x300
    #define GET_SIZE 8
    #define TIMEOUT 500


    typedef struct {
        usb_dev_handle *udev;
    } panicbutton_t;

    int pb_init_device(panicbutton_t *pb);

    int pb_next_event(panicbutton_t *pb);

    int pb_destroy_device(panicbutton_t *pb);


#ifdef	__cplusplus
}
#endif

#endif	/* _PANICBUTTON_H */

