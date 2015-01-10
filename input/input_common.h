/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2015 - Daniel De Matteis
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

#ifndef INPUT_COMMON_H__
#define INPUT_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "input_context.h"
#include <file/config_file.h>
#include <stdint.h>
#include "../driver.h"

typedef uint64_t retro_input_t ;

/* Input config. */
struct input_bind_map
{
   bool valid;

   /* Meta binds get input as prefix, not input_playerN".
    * 0 = libretro related.
    * 1 = Common hotkey.
    * 2 = Uncommon/obscure hotkey.
    */
   unsigned meta;

   const char *base;
   const char *desc;
   unsigned retro_key;
};

extern const struct input_bind_map input_config_bind_map[];

bool input_translate_coord_viewport(int mouse_x, int mouse_y,
      int16_t *res_x, int16_t *res_y, int16_t *res_screen_x,
      int16_t *res_screen_y);

/* auto_bind can be NULL. */
void input_get_bind_string(char *buf, const struct retro_keybind *bind,
      const struct retro_keybind *auto_bind, size_t size);

void input_translate_rk_to_str(enum retro_key key, char *buf, size_t size);
enum retro_key input_translate_str_to_rk(const char *str);

const char *input_config_get_prefix(unsigned user, bool meta);

/* Returns RARCH_BIND_LIST_END on not found. */
unsigned input_translate_str_to_bind_id(const char *str);

void input_config_parse_key(config_file_t *conf,
      const char *prefix, const char *btn,
      struct retro_keybind *bind);

void input_config_parse_joy_button(config_file_t *conf, const char *prefix,
      const char *btn, struct retro_keybind *bind);

void input_config_parse_joy_axis(config_file_t *conf, const char *prefix,
      const char *axis, struct retro_keybind *bind);

void input_push_analog_dpad(struct retro_keybind *binds, unsigned mode);

void input_pop_analog_dpad(struct retro_keybind *binds);

#ifdef __cplusplus
}
#endif

#endif

