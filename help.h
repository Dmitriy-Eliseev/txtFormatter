/* help.h
 *
 * Copyright (C) 2024 Dmitriy Eliseev
 * This file is part of txtFormatter.
 *
 * txtFormatter is licensed under the GNU General Public License, version 3.
 * See the LICENSE file or <https://www.gnu.org/licenses/gpl-3.0.en.html>
 * for details.
 */
#ifndef HELP_H
#define HELP_H

#include "tags_lib.h"

void  clear_input_buffer (void);
void  clear_console      (void);

void  init_hlp           (void);
void  init_assignment    (void);
void  init_attrs         (void);
void  free_hlp           (void);

void help                (void);

#endif /* HELP_H */
