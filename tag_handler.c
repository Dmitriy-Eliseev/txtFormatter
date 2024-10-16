/* tag_handler.c
 *
 * Copyright (C) 2024 Dmitriy Eliseev
 * This file is part of txtFormatter.
 *
 * txtFormatter is licensed under the GNU General Public License, version 3.
 * See the LICENSE file or <https://www.gnu.org/licenses/gpl-3.0.en.html>
 * for details.
 */
#include "tag_handler.h"

char    tag_list[][20]                  =  { "right", "center", "p", "frame",
                                             "list", "lines", "histogram",
                                             "table", "calc", "sep", "h1", "h2",
                                             "h3", "h4", "insert", "doc_width",
                                             "default_width", "date", "time", 
                                             "datetime"  };
const int tag_count = sizeof(tag_list) / sizeof(tag_list[0]);
char* (*tag_functions[])(char*, char**) = { right, center, p, get_framed_text,
                                            get_list, get_lines, get_histogram,
                                            get_table, calc, separator, h1, h2,
                                            h3, h4, insert, doc_width,
                                            def_width, get_date, get_time,
                                            get_datetime };

char single_tags[][20] = { "date", "time", "datetime", "doc_width",
                           "default_width", "sep", "lines", "insert" };
const int single_tags_count = sizeof(single_tags) / sizeof(single_tags[0]);


int8_t have_attributes(char* tag)
{
    uint16_t elements_count= get_elements_count(' ', tag);
    if ( elements_count == 1 )
        return 0;
    
    return elements_count;
}

char** get_tag_attributes(char* tag)
{
    char** attrs = NULL;
    if ( have_attributes(tag) != 0 ) {
        attrs = (char**)calloc(have_attributes(tag),  sizeof(char*));
        is_memory_allocated(attrs);
        char** tag_attr = split(' ', tag);
        free(tag_attr[0]);
        for ( uint16_t i=1; i<have_attributes(tag); i++ ) {
            attrs[i-1] = calloc(strlen(tag_attr[i]) + 1, sizeof(char));
            is_memory_allocated(attrs[i-1]);
            strcpy(attrs[i-1], tag_attr[i]);
            free(tag_attr[i]);
        }

        free(tag_attr);
        attrs[have_attributes(tag) - 1] = NULL;
    }

    return attrs;
}

char* get_tag_name(char* tag)
{
    int8_t attr = have_attributes(tag);
    if ( attr != 0 ) {
        char** tmp = split(' ', tag);
        char* res = strdup(tmp[0]);
        for ( uint16_t i=0; i<get_elements_count(' ', tag); i++ )
            free(tmp[i]);
        
        free(tmp);
        return res;
    }

    return tag;
}

int8_t is_valid_tag(char* tag)
{
    char* tag_name = get_tag_name(tag);
    for ( uint8_t i=0; i<tag_count; i++ ) {
        if ( strcmp(tag_list[i], tag_name) == 0 ) {
            if ( tag != tag_name ) 
                free(tag_name);
            
            return i;
        }
    }

    if ( tag != tag_name )
        free(tag_name);
    
    return -1;
}

int8_t is_single_tag(char* tag)
{
    char* tag_name = get_tag_name(tag);
    for ( uint8_t i=0; i<single_tags_count; i++ ) {
        if ( strcmp(single_tags[i], tag_name) == 0 ) {
            if ( tag != tag_name )
                free(tag_name);

            return 1;
        }
    }

    if ( tag != tag_name )
        free(tag_name);
    
    return 0;
}

char* get_open_tag(char* tag)
{
    char* open_tag = (char*)calloc(strlen(tag) + 3,  sizeof(char));
    is_memory_allocated(open_tag);
    sprintf(open_tag, "<%s>", tag);
    return open_tag;
}

char* get_close_tag(char* tag)
{
    char* t_tag = strdup(tag);
    t_tag = rm_spaces_start_end(t_tag);
    if ( is_single_tag(t_tag) == 0 ) {
        char* close_tag = NULL;
        char* tag_name = get_tag_name(t_tag);
        close_tag = (char*) calloc(strlen(tag_name) + 4,  sizeof(char));
        is_memory_allocated(close_tag);
        sprintf(close_tag, "</%s>", tag_name);
        free(tag_name);
        if ( t_tag != tag_name )
            free(t_tag);
        
        return close_tag;
    } else {
        free(t_tag);
        return get_open_tag(tag);
    }
}

char* get_tag(const char* str)
{
    if ( str == NULL ) 
        return NULL;

    char* tag = NULL;
    char* start = strchr(str, '<') + 1;
    char* end = strchr(str, '>');
    if ( start == NULL || end == NULL ) 
        return NULL;

    if ( end > start ) {
        if ( *start != '/' ) {
            tag = (char*)calloc(end - start + 1,  sizeof(char));
            is_memory_allocated(tag);
            memcpy(tag, start, end - start);
            tag[end - start] = '\0';
        }
    }

    return tag;
}

char* get_tag_content(char* str, char* tag)
{
    char* tag_content = NULL;
    if ( is_single_tag(tag) != 0 ) {
        tag_content = (char *) calloc(3,  sizeof(char));
        is_memory_allocated(tag_content);
        strcpy(tag_content, " ");
        return tag_content;
    }

    char* open_tag = get_open_tag(tag);
    char* close_tag = get_close_tag(tag);
    char *start = strstr(str, open_tag) +
                  sizeof(char) * strlen(open_tag);
    char *end = strstr(str, close_tag);
    free(open_tag);
    free(close_tag);
    if ( end == NULL || end - start == 0 ) {
        tag_content = (char *)  calloc(3,  sizeof(char));
        is_memory_allocated(tag_content);
        if ( end != NULL && end - start == 0 )
            strcpy(tag_content, "\v");

        if ( end == NULL )
            strcpy(tag_content, "\r");

        return tag_content;
    }

    tag_content = (char *) calloc(end - start + 1, sizeof(char));
    is_memory_allocated(tag_content);
    memcpy(tag_content, start, end - start);
    tag_content[end - start] = '\0';

    if ( tag_content[0] == '\n' ) {
        char *tmp = calloc(strlen(tag_content), sizeof(char));
        is_memory_allocated(tmp);
        strcpy(tmp, &tag_content[1]);
        free(tag_content);
        tag_content = tmp;
    }

    if ( tag_content[strlen(tag_content) - 1] == '\n' )
        tag_content[strlen(tag_content) - 1] = '\0';

    return tag_content;
}

char* get_text_before_tag(char* str, char* tag)
{
    char* open_tag = get_open_tag(tag);
    char* start_tag = strstr(str, open_tag);
    char* text_before_tag =(char*)calloc(start_tag - &str[0] + 1, sizeof(char));
    is_memory_allocated(text_before_tag);
    memcpy(text_before_tag, &str[0], start_tag - &str[0]);
    text_before_tag[start_tag - &str[0]] = '\0';
    free(open_tag);
    return text_before_tag;
}

char* get_text_after_tag(char* str, char* tag)
{
    char* close_tag = get_close_tag(tag);
    char* end_tag = strstr(str, close_tag);
    if ( end_tag == NULL ) {
        if ( is_valid_tag(tag) != -1 )
            printf("  Error: no closing tag found for \"%s\". Ignoring\n", tag);
        else
            print_tag_error(tag);
        
        free(close_tag);
        close_tag = get_open_tag(tag);
        end_tag = strstr(str, close_tag);
    } else if ( is_valid_tag(tag) == -1 ) 
        print_tag_error(tag);

    end_tag = end_tag + sizeof(char) * strlen(close_tag);
    free(close_tag);
    char* text_after_tag = (char*)calloc(strlen(str) - (end_tag - &str[0]) + 1,
                                         sizeof(char));
    is_memory_allocated(text_after_tag);
    strcpy(text_after_tag, end_tag);
    return text_after_tag;
}



char* execute_tag(char* tag, char* tag_content)
{
    char* t_tag = strdup(tag);
    t_tag = rm_spaces_start_end(t_tag);
    int8_t tag_i = is_valid_tag(t_tag);
    
    if ( tag_i != -1 && strcmp(tag_content, "\r") != 0 ) {
        char** attr = get_tag_attributes(t_tag);
        char* tag_result = (*tag_functions[tag_i])(tag_content, attr);
        for ( uint16_t i=0; i<have_attributes(tag)-1; i++ ) {
            if ( attr[i] != NULL )
                free(attr[i]);
            else
                break;
        }

        free(t_tag);
        free(attr);
        return tag_result;
    }

    free(t_tag);
    return tag_content;
}

char* execute_nested_tags(char* str)
{
    char* tag = get_tag(str);
    if ( tag != NULL ) {
        char* t_tag = strdup(tag);
        t_tag = rm_spaces_start_end(t_tag);

        char* tag_content = get_tag_content(str, tag);
        char* res = execute_nested_tags(tag_content);

        char* text_before_tag = get_text_before_tag(str, tag);
        char* text_after_tag = get_text_after_tag(str, t_tag);

        char* result = NULL;
        char* tag_result = execute_tag(tag, res);
        uint32_t len = strlen(text_before_tag) + strlen(tag_result) + \
                       strlen(text_after_tag) + 1;
        result = (char*)calloc(len,  sizeof(char));
        is_memory_allocated(result);
        sprintf(result, "%s%s%s", text_before_tag, tag_result, text_after_tag);
        free(text_before_tag);
        free(tag_result);
        free(text_after_tag);
        free(t_tag);
        if ( tag_content != tag_result )
            free(tag_content);

        if ( res != tag_content )
            free(res);
        
        free(tag);
        return result;
    }

    free(tag);
    return str;
}

char* execute_all_tags(char* str)
{
    char* result = strdup(str);
    is_memory_allocated(result);
    char* tmp = NULL;
    char* tag = get_tag(result);
    while ( tag != NULL ) {
        tmp = strdup(result);
        free(result);
        result = execute_nested_tags(tmp);
        free(tmp);
        free(tag);
        tag = get_tag(result);
    }

    return result;
}