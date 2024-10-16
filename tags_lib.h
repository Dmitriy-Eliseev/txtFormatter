/* tags_lib.h
 *
 * Copyright (C) 2024 Dmitriy Eliseev
 * This file is part of txtFormatter.
 *
 * txtFormatter is licensed under the GNU General Public License, version 3.
 * See the LICENSE file or <https://www.gnu.org/licenses/gpl-3.0.en.html>
 * for details.
 */
#ifndef TAGS_LIB_H
#define TAGS_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <dirent.h>
#include "tinyexpr.h"
#include "tags.h"
#include "tag_handler.h"

/* errors */
void           exit_on_error         (char* msg,      void* ptr);
void           is_memory_allocated   (void* mem_ptr);
void           is_directory_opened   (void* dir_ptr);
void           print_file_error      (char* filename);
void           print_tag_error       (char* tag);

/* files */
uint16_t       get_files_count       (char* dirname,  char* file_extension);
char**         get_files_in_dir      (char* dirname,  char* file_extension);
char*          get_file_content      (char* filename);
void           write_to_file         (char* filename, char* str);
char*          change_file_extension (char* filename, char* extension);

/* strings */
uint16_t       get_elements_count    (char  sym,  char* str);
char**         split                 (char  sym,  char* str);
char*          get_str_from_sym      (char  sym,  uint16_t count);
void           change_symbols        (char  from, char to, char* str);
uint8_t        is_number             (char* str,  uint8_t mode);

uint16_t       get_number_len        (uint16_t number);
char*          rm_spaces_from_str    (char*    str);
char*          rm_spaces_start_end   (char*    str);

/* text formatting */
#define        DEFAULT_DOC_WIDTH     80
extern uint8_t DOC_WIDTH;
void           set_doc_width         (uint8_t width);

/* arrays */
uint16_t       get_arr_size          (char** str_arr);
uint8_t        in_str_array          (char** arr, char* value);
uint32_t       get_max_len           (char** str_arr, uint32_t arr_size);
uint16_t       get_max               (const uint16_t* arr, uint16_t size);
uint16_t       get_min               (const uint16_t* arr, uint16_t size);
int32_t        get_index             (const uint16_t* arr, uint16_t size,
                                      uint16_t value);
uint16_t       get_min_index         (const uint16_t* arr, uint16_t size);

/* basic functions for some tags */
char*          get_aligned_text      (char*  str, char** attrs);
char*          header                (char*  str, uint8_t header_type, 
                                      char** attrs);

/* tables */
uint16_t       get_rows_count        (char*   tbl_str);
uint16_t*      get_cells_count       (char*   tbl_str);
char***        get_table_data        (char*   tbl_str);
uint16_t*      get_cells_len         (char**  row, uint16_t cells_count);
uint16_t*      get_rows_len          (char*** table_data, uint16_t rows_count,
                                      const uint16_t* cells_in_row);
char*          get_table_border      (const char* row1, const char* row2);
char*          add_table_border      (char*   table_str);
void           calc_in_table         (char*** table_data, uint16_t rows_count,
                                      const uint16_t* cells_in_row);
uint16_t**     get_column_width      (char*** table_data, uint16_t rows_count,
                                      uint16_t* cells_in_row);
void           align_to_columns      (char*** table_data, uint16_t rows_count,
                                      uint16_t* cells_in_row, uint8_t na);
uint16_t       get_max_row_len       (char*** table_data, uint16_t rows_count,
                                      uint16_t* cells_in_row);

/* histograms */
double         get_max_value         (char** values, uint16_t values_count);
void           get_histogram_data    (char*  str, char** names, char** values);


#endif /* TAGS_LIB_H */
