/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2013-2014 - Jason Fetters
 *  Copyright (C) 2011-2016 - Daniel De Matteis
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <boolean.h>
#include "joypad_connection.h"

struct hidpad_nesusb_data
{
   struct pad_connection* connection;
   uint8_t data[64];
   uint32_t slot;
   uint64_t buttons;
};

static void* hidpad_nesusb_init(void *data, uint32_t slot, send_control_t ptr)
{
   struct pad_connection* connection = (struct pad_connection*)data;
   struct hidpad_nesusb_data* device    = (struct hidpad_nesusb_data*)
      calloc(1, sizeof(struct hidpad_nesusb_data));

   if (!device)
      return NULL;

   if (!connection)
   {
      free(device);
      return NULL;
   }

   device->connection   = connection;
   device->slot         = slot;

   return device;
}

static void hidpad_nesusb_deinit(void *data)
{
   struct hidpad_nesusb_data *device = (struct hidpad_nesusb_data*)data;

   if (device)
      free(device);
}

static uint64_t hidpad_nesusb_get_buttons(void *data)
{
   struct hidpad_nesusb_data *device = (struct hidpad_nesusb_data*)data;
   if (!device)
      return 0;
   return device->buttons;
}

static int16_t hidpad_nesusb_get_axis(void *data, unsigned axis)
{
   int val;
   struct hidpad_nesusb_data *device = (struct hidpad_nesusb_data*)data;

   if (!device || axis >= 2)
      return 0;

   val = device->data[4 + axis];
   val = (val << 8) - 0x8000;

   return (abs(val) > 0x1000) ? val : 0;
}

static void hidpad_nesusb_packet_handler(void *data, uint8_t *packet, uint16_t size)
{
   uint32_t i, pressed_keys;
   static const uint32_t button_mapping[17] =
   {
      NO_BTN,
      NO_BTN,
      NO_BTN,
      NO_BTN,
      RETRO_DEVICE_ID_JOYPAD_SELECT,
      RETRO_DEVICE_ID_JOYPAD_START,
      NO_BTN,
      NO_BTN,
      NO_BTN,
      NO_BTN,
      NO_BTN,
      NO_BTN,
      RETRO_DEVICE_ID_JOYPAD_B,
      RETRO_DEVICE_ID_JOYPAD_A,
      NO_BTN,
      NO_BTN,
      16, /* HOME BUTTON when pressing SELECT+START */
   };
   struct hidpad_nesusb_data *device = (struct hidpad_nesusb_data*)data;

   if (!device)
      return;

   memcpy(device->data, packet, size);

   device->buttons = 0;

   pressed_keys  = device->data[7] | (device->data[6] << 8) |
                (((device->data[7] & 0x30) == 0x30) ? (1 << 16) : 0);  /* SELECT+START=HOME */

   for (i = 0; i < 17; i ++)
      if (button_mapping[i] != NO_BTN)
         device->buttons |= (pressed_keys & (1 << i)) ? (UINT64_C(1) << button_mapping[i]) : 0;
}

static void hidpad_nesusb_set_rumble(void *data,
      enum retro_rumble_effect effect, uint16_t strength)
{
	(void)data;
	(void)effect;
   (void)strength;
}

const char * hidpad_nesusb_get_name(void *data)
{
	(void)data;
	/* For now we return a single static name */
	return "Generic NES USB Controller";
}

pad_connection_interface_t pad_connection_nesusb = {
   hidpad_nesusb_init,
   hidpad_nesusb_deinit,
   hidpad_nesusb_packet_handler,
   hidpad_nesusb_set_rumble,
   hidpad_nesusb_get_buttons,
   hidpad_nesusb_get_axis,
   hidpad_nesusb_get_name,
};
