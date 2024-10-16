/* tags_lib.c
 *
 * Copyright (C) 2024 Dmitriy Eliseev
 * This file is part of txtFormatter.
 *
 * txtFormatter is licensed under the GNU General Public License, version 3.
 * See the LICENSE file or <https://www.gnu.org/licenses/gpl-3.0.en.html>
 * for details.
 */
#include "tags_lib.h"

uint8_t DOC_WIDTH = DEFAULT_DOC_WIDTH;
/***************************************************************************
* functions for working with errors
***************************************************************************/
void exit_on_error(char* msg, void* ptr)
{
    if ( ptr == NULL ) {
        fprintf(stderr, "%s", msg);
        exit(EXIT_FAILURE);
    }
}

void is_memory_allocated(void* mem_ptr)
{
    exit_on_error("Memory allocation error\n", mem_ptr);
}

void is_directory_opened(void* dir_ptr)
{
    exit_on_error("Error opening directory\n", dir_ptr);
}

void print_file_error(char* filename)
{
    printf("  Error opening file \"%s\"\n", filename);
}

void print_tag_error(char* tag)
{
    char* tag_name = get_tag_name(tag);
    printf("  Error: invalid tag \"%s\". Ignoring\n", tag_name);
    if ( tag_name != tag )
        free(tag_name);
}


/***************************************************************************
* functions for working with files
***************************************************************************/
uint16_t get_files_count(char* dirname, char* file_extension)
{
    DIR *dir = opendir(dirname);
    is_directory_opened(dir);

    struct dirent *file;
    uint16_t i = 0;
    while ( (file = readdir(dir)) != NULL ) {
        /* find start of file extension */
        char* extension = strrchr(file->d_name, '.');
        if ( extension != NULL && strcmp(extension, file_extension) == 0 )
            i++;
    }

    closedir(dir);
    return i;
}

char** get_files_in_dir(char* dirname, char* file_extension)
{
    uint16_t files_count = get_files_count(dirname, file_extension);
    struct dirent *file;
    DIR *dir = opendir(dirname);
    is_directory_opened(dir);

    char** file_names = calloc(files_count, sizeof(char*));
    is_memory_allocated(file_names);
    uint16_t i = 0;
    while ( (file = readdir(dir)) != NULL ) {
        /* find start of file extension */
        char* extension = strrchr(file->d_name, '.');
        if ( extension != NULL && strcmp(extension, file_extension) == 0 ) {
            file_names[i] = calloc(256,  sizeof(char));
            is_memory_allocated(file_names[i]);
            strcpy(file_names[i],file->d_name);
            i++;
        }
    }

    closedir(dir);
    return  file_names;
}

char* get_file_content(char* filename)
{
    FILE *file = fopen(filename, "r");
    if ( file == NULL ) {
        print_file_error(filename);
        return NULL;
    }

    /* getting file size */
    fseek(file, 0, SEEK_END);
    uint32_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    /* getting text from file */
    char* str = (char*)calloc(file_size + 1,  sizeof(char));
    is_memory_allocated(str);
    size_t result = fread(str, 1, file_size, file);
    str[result] = '\0';

    fclose(file);
    return str;
}

void write_to_file(char* filename, char* str)
{
    FILE *file;
    file = fopen(filename, "w");
    if ( file == NULL )
        print_file_error(filename);
    
    fprintf(file, "%s", str);
    fclose(file);
}

char* change_file_extension(char* filename, char* extension)
{
    char* result = calloc(strlen(filename) + strlen(extension)+1, sizeof(char));
    is_memory_allocated(result);
    strcpy(result, filename);
    /* find start of file extension */
    char* extension_start = strrchr(result, '.');
    if ( extension_start != NULL )
        strcpy(extension_start, extension);
    
    return result;
}


/***************************************************************************
* functions for working with strings
***************************************************************************/
uint16_t get_elements_count(char sym, char* str)
{
    uint16_t count = 0;
    char* delims = calloc(3,  sizeof(char));
    is_memory_allocated(delims);
    sprintf(delims, "%c", sym);
    char* temp_str = strdup(str);
    char* token = strtok(temp_str, delims);
    while ( token != NULL ) {
        count++;
        token = strtok(NULL, delims);
    }

    free(delims);
    free(temp_str);
    return count;
}

char** split(char sym, char* str)
{
    char* delims = calloc(3, sizeof(char));
    is_memory_allocated(delims);
    sprintf(delims, "%c", sym);
    uint16_t count = get_elements_count(sym, str);
    char** elements = (char**)calloc(count, sizeof(char*));
    is_memory_allocated(elements);
    char* temp_str = strdup(str);
    count = 0;
    char* token = strtok(temp_str, delims);
    while ( token != NULL ) {
        elements[count] = strdup(token);
        is_memory_allocated(elements[count]);
        count++;
        token = strtok(NULL, delims);
    }

    free(delims);
    free(temp_str);
    return elements;
}

char* get_str_from_sym(char sym, uint16_t count)
{
    char* str = calloc(count + 1, sizeof(char));
    is_memory_allocated(str);
    char* symbol = calloc(2, sizeof(char));
    is_memory_allocated(symbol);
    sprintf(symbol, "%c", sym);
    for ( uint16_t i=0; i<count; i++ )
        strcat(str, symbol);
    
    free(symbol);
    return str;
}

void change_symbols(char from, char to, char* str)
{
    for ( uint64_t i=0; i<strlen(str); i++ ) {
        if ( str[i] == from )
            str[i] = to;
    }
}

uint8_t is_number(char* str, uint8_t mode)
{
    if ( str[0] == '\n' || str[0] == '\0' )
        return 0;
    /* mode:
       0 - only digits
       1 - digits + '-', '.', ' '
       2 - digits + '-', '.', ',', ' ' */
    if ( mode > 2 )
        mode = 2;
    
    uint8_t result = 0;
    uint16_t len = strlen(str);
    for ( uint16_t i=0; i<len; i++ ) {
        if ( isdigit(str[i])||(str[i] == '-' || str[i] == '.' || str[i] == ' ')
            &&(mode == 1 || mode == 2)||(str[i] == ',')&&(mode == 2) )
            result = 1;
        else
            return 0;
    }

    return result;
}

uint16_t get_number_len(uint16_t number)
{
    char* num_str = calloc(20, sizeof(char));
    is_memory_allocated(num_str);
    sprintf(num_str, "%d", number);
    uint16_t len = strlen(num_str);
    free(num_str);
    return len;
}

char* rm_spaces_from_str(char* str)
{
    char sym = ' ';
    char* result = calloc(strlen(str) + 2, sizeof(char));
    is_memory_allocated(result);
    uint64_t res_i = 0;
    for ( uint64_t i=0; i<strlen(str); i++ ) {
        if ( str[i] != sym ) {
            result[res_i] = str[i];
            res_i++;
        }
    }

    result[res_i + 1] = '\0';
    free(str);
    return result;
}

char* rm_spaces_start_end(char* str)
{
    uint64_t start = 0;
    uint64_t end = 0;
    for ( uint64_t i=0; i<strlen(str); i++ ) {
        if ( str[i] != ' ' ) {
            start = i;
            break;
        }
    }

    for ( uint64_t i=strlen(str)-1; i>=0; i-- ) {
        if ( str[i] != ' ' ) {
            end = i;
            break;
        }
    }

    end += 1;
    char* result = calloc(end - start + 2, sizeof(char));
    is_memory_allocated(result);
    memcpy(result, &str[start], end-start);
    result[end-start] = '\0';
    free(str);
    return result;
}


/***************************************************************************
* functions for Text Formatting
***************************************************************************/
void set_doc_width(uint8_t width)
{
    if ( width < 10 ) {
        printf("  Error: Document width cannot be less than 10 characters\n");
        DOC_WIDTH = 10;
    } else if ( width > 200 ) {
        printf("  Error: Document width cannot be more than 250 characters\n");
        DOC_WIDTH = 250;
    } else {
        DOC_WIDTH = width;
    }
}


/***************************************************************************
* functions for working with arrays
***************************************************************************/
uint16_t get_arr_size(char** str_arr)
{
    uint16_t count = 0;
    while ( str_arr[count] != NULL )
        count++;
    
    return count;
}

uint8_t in_str_array(char** arr, char* value)
{
    if ( arr == NULL )
        return 0;

    uint16_t arr_size = get_arr_size(arr);
    for ( uint16_t i=0; i<arr_size; i++ ) {
        if ( strcmp(arr[i], value) == 0 )
            return 1;
    }

    return 0;
}

uint32_t get_max_len(char** str_arr, uint32_t arr_size)
{
    uint32_t max_len = strlen(str_arr[0]);
    for ( uint32_t i=0; i<arr_size; i++ ) {
        if ( max_len < strlen(str_arr[i]) )
            max_len = strlen(str_arr[i]);
    }

    return max_len;
}

uint16_t get_max(const uint16_t* arr, uint16_t size)
{
    uint16_t max = arr[0];
    for ( uint16_t i=1; i<size; i++ ) {
        if ( max < arr[i] )
            max = arr[i];
    }

    return max;
}

uint16_t get_min(const uint16_t* arr, uint16_t size)
{
    uint16_t min = arr[0];
    for ( uint16_t i=1; i<size; i++ ) {
        if ( min > arr[i] )
            min = arr[i];
    }

    return min;
}

int32_t get_index(const uint16_t* arr, uint16_t size, uint16_t value)
{
    int32_t index = -1;
    for ( uint16_t i=0; i<size; i++ ) {
        if ( value == arr[i] )
            index = i;
    }

    return index;
}

uint16_t get_min_index(const uint16_t* arr, uint16_t size)
{
    uint16_t min = get_min(arr, size);
    uint16_t index = get_index(arr, size, min);
    return index;
}


/***************************************************************************
* Basic functions for some tags
***************************************************************************/
char* get_aligned_text(char* str, char** attrs)
{
    char** lines = split('\n', str);
    uint32_t lines_count = get_elements_count('\n', str);
    uint32_t max_line = get_max_len(lines, lines_count);
    uint32_t len = (max_line > DOC_WIDTH) ? max_line : DOC_WIDTH;
    len = (len + 1) * lines_count;
    char* aligned = calloc(len, sizeof(char));
    is_memory_allocated(aligned);
    for ( uint32_t i=0; i<lines_count; i++ ) {
        uint16_t spaces = (strlen(lines[i]) > DOC_WIDTH) ? 0 \
            : DOC_WIDTH - strlen(lines[i]);
        if ( attrs == NULL )
            spaces /= 2;
        
        char* al = get_str_from_sym(' ', spaces);
        strcat(aligned, al);
        free(al);
        strcat(aligned, lines[i]);
        spaces = DOC_WIDTH - strlen(lines[i]) - spaces;
        al = get_str_from_sym(' ', spaces);
        strcat(aligned, al);
        free(lines[i]);
        free(al);
        if ( i < lines_count - 1 )
            strcat(aligned, "\n");
    }

    free(lines);
    return aligned;
}

char* header(char* str, uint8_t header_type, char** attrs)
{
    char sep_sym = '\0';
    if ( attrs != NULL ) {
        if ( attrs[0] != NULL )
            sep_sym = attrs[0][0];
    }

    if ( header_type == 1 && sep_sym == '\0' )
        sep_sym = '=';
    
    uint8_t doc_width_bak = DOC_WIDTH;
    if ( DOC_WIDTH < strlen(str) )
        set_doc_width(strlen(str) + 4);
    
    char* header = NULL;
    uint16_t len = DOC_WIDTH + 1;
    if ( header_type == 1 ) {
        char* centered_str = center(str, NULL);
        len = DOC_WIDTH * 2 + strlen(centered_str) + 3;
        header = calloc(len, sizeof(char));
        is_memory_allocated(header);
        char* tmp = get_str_from_sym(sep_sym, DOC_WIDTH);
        sprintf(header, "%s\n%s\n%s", tmp, centered_str, tmp);
        free(tmp);
        free(centered_str);
    } else {
        header = calloc(len, sizeof(char));
        is_memory_allocated(header);
        if ( sep_sym == '\0' )
            sep_sym = (header_type == 2) ? '=' : '-';
        
        char* tmp = get_str_from_sym(sep_sym, (DOC_WIDTH-strlen(str) - 1) / 2);
        sprintf(header, "%s %s ", tmp, str);
        free(tmp);
        tmp = get_str_from_sym(sep_sym, DOC_WIDTH - strlen(header));
        strcat(header, tmp);
        free(tmp);
    }

    set_doc_width(doc_width_bak);
    return header;
}

/***************************************************************************
* functions for working with tables
***************************************************************************/
uint16_t get_rows_count(char* tbl_str)
{
    return get_elements_count('\n', tbl_str);
}

uint16_t* get_cells_count(char* tbl_str)
{
    uint16_t  rows_count   = get_rows_count(tbl_str);
    uint16_t* cells_in_row = calloc(rows_count, sizeof(uint16_t));
    is_memory_allocated(cells_in_row);
    char** rows = split('\n', tbl_str);
    for ( uint16_t i=0; i<rows_count; i++ ) {
        cells_in_row[i] = get_elements_count('|', rows[i]);
        free(rows[i]);
    }

    free(rows);
    return cells_in_row;
}

char*** get_table_data(char* tbl_str)
{
    uint16_t  rows_count   = get_rows_count(tbl_str);
    char**    rows         = split('\n', tbl_str);
    char***   table_data   = (char***)calloc(rows_count, sizeof(char**));
    is_memory_allocated(table_data);
    for ( uint16_t i=0; i<rows_count; i++ ) {
        table_data[i] = split('|', rows[i]);
        free(rows[i]);
    }

    free(rows);
    return table_data;
}

uint16_t* get_cells_len(char** row, uint16_t cells_count)
{
    uint16_t* cells_len = calloc(cells_count, sizeof(uint16_t));
    is_memory_allocated(cells_len);
    for ( uint16_t i=0; i<cells_count; i++ )
        cells_len[i] = strlen(row[i]);

    return cells_len;
}

uint16_t* get_rows_len(char*** table_data, uint16_t rows_count,
                       const uint16_t* cells_in_row)
{
    uint16_t* rows_len = calloc(rows_count, sizeof(uint16_t));
    is_memory_allocated(rows_len);
    for ( uint16_t i=0; i<rows_count; i++ ) {
        uint16_t row_len = 0;
        for ( uint16_t j=0; j<cells_in_row[i]; j++ )
            row_len += strlen(table_data[i][j]);

        rows_len[i] = row_len + cells_in_row[i] - 1;
    }

    return rows_len;
}

char* get_table_border(const char* row1, const char* row2)
{
    uint16_t row_len = strlen(row1);
    char* border = calloc(row_len + 1, sizeof(char));
    is_memory_allocated(border);
    for ( uint16_t i=0; i<row_len; i++ ) {
        if ( row1[i] == '|' || row2[i] == '|' )
            strcat(border, "+");
        else
            strcat(border, "-");
    }

    return border;
}

char* add_table_border(char* table_str)
{
    char**   rows = split('\n', table_str);
    uint16_t rows_count = get_rows_count(table_str);
    uint16_t row_len = strlen(rows[0]);
    uint32_t len = (rows_count * 2 + 1) * (row_len + 1);
    char*    table = calloc(len, sizeof(char));
    is_memory_allocated(table);
    char* space_str = get_str_from_sym(' ', row_len);
    for ( uint16_t i=0; i<rows_count; i++ ) {
        char* row1;
        char* row2;
        if ( i == 0 ) {
            row1 = space_str;
            row2 = rows[i];
        } else if ( i == rows_count - 1 ) {
            row1 = rows[i];
            row2 = space_str;
        } else {
            row1 = rows[i - 1];
            row2 = rows[i];
        }

        char* border = get_table_border(row1, row2);
        strcat(table, border);
        strcat(table, "\n");
        strcat(table, rows[i]);
        if ( i != rows_count - 1 ) {
            free(border);
            strcat(table, "\n");
        } else {
            strcat(table, "\n");
            strcat(table, border);
            free(border);
        }
    }
    /* Cleaning */
    for ( uint16_t i=0; i<rows_count; i++ )
        free(rows[i]);

    free(rows);
    free(space_str);

    return table;
}

void calc_in_table(char*** table_data, uint16_t rows_count,
                   const uint16_t* cells_in_row)
{
    char* calc_res = NULL;
    for ( uint16_t i=0; i<rows_count; i++ ) {
        for ( uint16_t j=0; j<cells_in_row[i]; j++ ) {
            char* tmp = calloc(strlen(table_data[i][j]) + 1, sizeof(char));
            strcpy(tmp, table_data[i][j]);
            change_symbols(',', '.', tmp);

            calc_res = calc(tmp, NULL);
            if ( strcmp(calc_res, "error") != 0 ) {
                free(table_data[i][j]);
                table_data[i][j] = strdup(calc_res);
            }

            free(calc_res);
            free(tmp);
        }
    }
}

uint16_t** get_column_width(char*** table_data, uint16_t rows_count,
                            uint16_t* cells_in_row)
{
    uint16_t max_cells = get_max(cells_in_row, rows_count);
    /* Memory allocation */
    uint16_t** column_width = calloc(max_cells, sizeof(uint16_t*));
    is_memory_allocated(column_width);
    for ( uint16_t i=0; i<max_cells; i++ ) {
        column_width[i] = calloc(i + 1, sizeof(uint16_t));
        is_memory_allocated(column_width[i]);
    }

    for ( uint16_t i=0; i<rows_count; i++ ) {
        for ( uint16_t j=0; j<cells_in_row[i]; j++ ) {
            if ( strlen(table_data[i][j]) > column_width[cells_in_row[i]-1][j] )
                column_width[cells_in_row[i] - 1][j] = strlen(table_data[i][j]);
        }
    }

    return column_width;
}

void align_to_columns(char*** table_data, uint16_t rows_count,
                      uint16_t* cells_in_row, uint8_t na)
{
    uint16_t** column_width = get_column_width(table_data, rows_count,
                                               cells_in_row);
    char* align = NULL;
    char* tmp = NULL;
    for ( uint16_t i=0; i<rows_count; i++ ) {
        for ( uint16_t j=0; j<cells_in_row[i]; j++ ) {
            uint16_t al_len = column_width[cells_in_row[i] - 1][j] \
                - strlen(table_data[i][j]);
            if ( al_len > 0 ) {
                align = get_str_from_sym(' ', al_len);
                tmp = strdup(table_data[i][j]);
                free(table_data[i][j]);
                table_data[i][j] = calloc(al_len + strlen(tmp) + 1, sizeof(char));
                if ( is_number(tmp, 2) && na == 0 )
                    sprintf(table_data[i][j], "%s%s", align, tmp);
                else
                    sprintf(table_data[i][j], "%s%s", tmp, align);
                
                free(tmp);
                free(align);
            }
        }
    }
    /* Cleaning */
    uint16_t max_cells = get_max(cells_in_row, rows_count);
    for ( uint16_t i=0; i<max_cells; i++ )
        free(column_width[i]);
    
    free(column_width);
}

uint16_t get_max_row_len(char*** table_data, uint16_t rows_count,
                         uint16_t* cells_in_row)
{
    uint16_t max_row_len = 0;
    uint16_t max_cells = get_max(cells_in_row, rows_count);
    uint16_t** column_width = get_column_width(table_data, rows_count,
                                               cells_in_row);
    for ( uint16_t i=0; i<max_cells; i++ ) {
        uint16_t row_len = 0;
        for ( uint16_t j=0; j<=i; j++ )
            row_len += column_width[i][j];
        
        row_len += i;
        if ( row_len > max_row_len )
            max_row_len = row_len;
    }
    /* Cleaning */
    for ( uint16_t i=0; i<max_cells; i++ )
        free(column_width[i]);
    
    free(column_width);
    return max_row_len;
}


/***************************************************************************
* functions for working with Histograms
***************************************************************************/
double get_max_value(char** values, uint16_t values_count)
{
    double max_value = 0;
    for ( uint16_t i=0; i<values_count; i++ ) {
        if ( is_number(values[i], 1) ) {
            double vl = strtod(values[i], NULL);
            vl = (vl < 0) ? vl * (double)-1 : vl;/* abs */
            max_value = (max_value < vl) ? vl : max_value;
        }
    }
    return max_value;
}

void get_histogram_data(char* str, char** names, char** values)
{
    char** lines = split('\n', str);
    uint16_t lines_count = get_elements_count('\n', str);
    for ( uint16_t i=0; i<lines_count; i++ ) {
        char* name = NULL;
        char* value = NULL;
        uint16_t t_count = get_elements_count('|', lines[i]);
        if ( t_count >= 2 ) {
            char** t = split('|', lines[i]);
            name = strdup(t[0]);
            is_memory_allocated(name);

            value = strdup(t[1]);
            is_memory_allocated(value);

            if ( strcmp(value, " ") != 0 )
                value = rm_spaces_from_str(value);
            
            change_symbols(',', '.', value);
            if ( is_number(value, 1) == 0 ) {
                free(value);
                value = calloc(6, sizeof(char));
                is_memory_allocated(value);
                strcpy(value, "error");
            }
            
            for ( uint16_t j=0; j<t_count; j++ )
                free(t[j]);
            
            free(t);
        } else if ( t_count == 1 ) {
            name = calloc(2, sizeof(char));
            is_memory_allocated(name);
            strcpy(name, " ");

            change_symbols(',', '.', lines[i]);

            if ( is_number(lines[i], 1) ) {
                value = calloc(strlen(lines[i]) + 1, sizeof(char));
                is_memory_allocated(value);
                strcpy(value, lines[i]);
            } else {
                value = calloc(6, sizeof(char));
                is_memory_allocated(value);
                strcpy(value, "error");
            }
        }

        if ( strcmp(name, " ") != 0 )
            name = rm_spaces_start_end(name);
        
        value = rm_spaces_from_str(value);
        names[i] = calloc(strlen(name) + 1, sizeof(char));
        is_memory_allocated(names[i]);
        values[i] = calloc(strlen(value) + 1, sizeof(char));
        is_memory_allocated(values[i]);
        strcpy(names[i], name);
        strcpy(values[i], value);
        free(name);
        free(value);
        free(lines[i]);
    }
    free(lines);
}
