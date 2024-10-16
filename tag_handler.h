/* tag_handler.h
 *
 * Copyright (C) 2024 Dmitriy Eliseev
 * This file is part of txtFormatter.
 *
 * txtFormatter is licensed under the GNU General Public License, version 3.
 * See the LICENSE file or <https://www.gnu.org/licenses/gpl-3.0.en.html>
 * for details.
 */
#ifndef TAG_HANDLER_H
#define TAG_HANDLER_H

#include "tags_lib.h"

extern char      tag_list[][20];
extern const int tag_count;

int8_t  have_attributes      (char* tag);
char**  get_tag_attributes   (char* tag);

char*   get_tag_name         (char* tag);

int8_t  is_valid_tag         (char* tag);
int8_t  is_single_tag        (char* tag);

char*   get_open_tag         (char* tag);
char*   get_close_tag        (char* tag);

char*   get_tag              (const char* str);
char*   get_tag_content      (char* str, char* tag);
char*   get_text_before_tag  (char* str, char* tag);
char*   get_text_after_tag   (char* str, char* tag);

char*   execute_tag          (char* tag, char* tag_content);
char*   execute_nested_tags  (char* str);
char*   execute_all_tags     (char* str);

#endif /* TAG_HANDLER_H */
