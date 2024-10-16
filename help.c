/* help.c
 *
 * Copyright (C) 2024 Dmitriy Eliseev
 * This file is part of txtFormatter.
 *
 * txtFormatter is licensed under the GNU General Public License, version 3.
 * See the LICENSE file or <https://www.gnu.org/licenses/gpl-3.0.en.html>
 * for details.
 */
#include "help.h"
#include "tag_handler.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

/***************************************************************************
* functions for working with console
***************************************************************************/
void clear_input_buffer(void) {
    int c;
    while ( (c = getchar()) != '\n' && c != EOF );
}

void clear_console(void)
{
    #ifdef _WIN32 /* For Windows */
        system("cls");
    #else /* For Unix */
        printf("\033[H\033[J");
    #endif
}

uint8_t get_terminal_width(void)
{
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
            return csbi.srWindow.Right - csbi.srWindow.Left + 1;
        else
            return 80; /* Default terminal width */
        
    #else
        struct winsize w;
        if ( ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 )
            return w.ws_col;
        else
            return 80; /* Default terminal width */

    #endif
}

uint8_t get_terminal_height(void)
{
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
            return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        else
            return 24; /* Default terminal height */
    
    #else
        struct winsize w;
        if ( ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 )
            return w.ws_row;
        else
            return 24; /* Default terminal height */
    
    #endif
}
/***************************************************************************/

char TAG_TYPE[][8]     = { "single", "paired" };
char attr_values[][20] = { "NONE", "any symbol", "number",
                           "nb",   "nc",  "na",  "/path/to/text/file" };
const uint8_t ATTR_ASSIGN_LEN = 90;

struct tags_help
{
    char**   names;
    uint8_t* types;
    char**   assignment;
    char**** attributes;
};

uint8_t tag_types[]   = { 1, 1, 1, 1, 1, 0, 1, 1, 1, 0,
                          1, 1, 1, 1, 0, 0, 0, 0, 0, 0 };

uint8_t attrs_count[] = { 0, 0, 2, 0, 2, 1, 2, 4, 2, 2,
                          2, 2, 2, 2, 1, 1, 0, 0, 0, 0 };

struct tags_help tags_hlp;
void init_hlp(void)
{
    tags_hlp.names      = &tag_list[0];
    tags_hlp.types      = &tag_types;
    tags_hlp.assignment = calloc(tag_count, sizeof(char*));
    tags_hlp.attributes = calloc(tag_count, sizeof(char***));

    init_assignment();
    init_attrs();
}

void init_assignment(void)
{
    tags_hlp.assignment[0]  = calloc(30, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[0]);
    strcpy(tags_hlp.assignment[0], "right-alignment of the text");

    tags_hlp.assignment[1]  = calloc(31, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[1]);
    strcpy(tags_hlp.assignment[1], "text alignment in the center");

    tags_hlp.assignment[2]  = calloc(12, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[2]);
    strcpy(tags_hlp.assignment[2], "paragraph");

    tags_hlp.assignment[3]  = calloc(14, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[3]);
    strcpy(tags_hlp.assignment[3], "framed text");

    tags_hlp.assignment[4]  = calloc(28, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[4]);
    strcpy(tags_hlp.assignment[4], "numbered or bulleted list");

    tags_hlp.assignment[5]  = calloc(45, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[5]);
    strcpy(tags_hlp.assignment[5], 
        "insert the specified number of empty lines");

    tags_hlp.assignment[6]  = calloc(42, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[6]);
    strcpy(tags_hlp.assignment[6], "a histogram based on the specified data");

    tags_hlp.assignment[7]  = calloc(109, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[7]);
    strcpy(tags_hlp.assignment[7], 
        "a table with borders, calculations of expressions, and right " 
        "alignment of\n  numbers (can be disabled)");

    tags_hlp.assignment[8]  = calloc(51, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[8]);
    strcpy(tags_hlp.assignment[8], 
        "calculate the specified mathematical expressions");

    tags_hlp.assignment[9]  = calloc(17, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[9]);
    strcpy(tags_hlp.assignment[9], "text separator");

    tags_hlp.assignment[10] = calloc(84, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[10]);
    strcpy(tags_hlp.assignment[10], 
        "header type 1 (centered text between 2 "
        "separators drawn using the symbol \"=\")");

    tags_hlp.assignment[11] = calloc(74, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[11]);
    strcpy(tags_hlp.assignment[11], 
        "header type 2 (centered text in separator drawn "
        "using the symbol \"=\")");

    tags_hlp.assignment[12] = calloc(70, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[12]);
    strcpy(tags_hlp.assignment[12], 
        "header type 3 (centered text in separator drawn "
        "using symbol \"-\")");

    tags_hlp.assignment[13] = calloc(34, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[13]);
    strcpy(tags_hlp.assignment[13], "header type 4 (underlined text)");

    tags_hlp.assignment[14] = calloc(53, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[14]);
    strcpy(tags_hlp.assignment[14], 
        "insert the contents of a text file into a document");

    tags_hlp.assignment[15] = calloc(35, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[15]);
    strcpy(tags_hlp.assignment[15], "change the width of the document");

    tags_hlp.assignment[16] = calloc(49, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[16]);
    strcpy(tags_hlp.assignment[16], 
        "set the default document width (80 characters)");

    tags_hlp.assignment[17] = calloc(22, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[17]);
    strcpy(tags_hlp.assignment[17], "insert current date");

    tags_hlp.assignment[18] = calloc(22, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[18]);
    strcpy(tags_hlp.assignment[18], "insert current time");

    tags_hlp.assignment[19] = calloc(31, sizeof(char));
    is_memory_allocated(tags_hlp.assignment[19]);
    strcpy(tags_hlp.assignment[19], "insert current date and time");
}

void init_attrs(void)
{
    /* Memory allocation */
    uint8_t i, j;
    for ( i=0; i<tag_count; i++ ) {
        if ( attrs_count[i] != 0 ) {
            tags_hlp.attributes[i]  = calloc(attrs_count[i], sizeof(char**));
            is_memory_allocated(tags_hlp.attributes[i]);
            for ( j=0; j<attrs_count[i]; j++ ) {
                tags_hlp.attributes[i][j] = calloc(2, sizeof(char*));
                is_memory_allocated(tags_hlp.attributes[i][j]);
                if ( attrs_count[i] == 1 )
                    tags_hlp.attributes[i][j][1] = NULL;
            }
        } else {
            tags_hlp.attributes[i]  = NULL;
        }
    }

    /* Setting attribute values and their assignments */
    /* p */
    tags_hlp.attributes[2][0][0]  = &attr_values[0];
    tags_hlp.attributes[2][0][1]  = calloc(12, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[2][0][1]);
    strcpy(tags_hlp.attributes[2][0][1], "paragraph");
    tags_hlp.attributes[2][1][0]  = &attr_values[1];
    tags_hlp.attributes[2][1][1]  = calloc(33, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[2][1][1]);
    strcpy(tags_hlp.attributes[2][1][1], "paragraph with right alignment");

    /* list */
    tags_hlp.attributes[4][0][0]  = &attr_values[0];
    tags_hlp.attributes[4][0][1]  = calloc(17, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[4][0][1]);
    strcpy(tags_hlp.attributes[4][0][1], "numbered list");
    tags_hlp.attributes[4][1][0]  = &attr_values[1];
    tags_hlp.attributes[4][1][1]  = calloc(17, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[4][1][1]);
    strcpy(tags_hlp.attributes[4][1][1], "bulleted list");

    /* lines */
    tags_hlp.attributes[5][0][0]  = &attr_values[2];

    /* histogram */
    tags_hlp.attributes[6][0][0]  = &attr_values[0];
    tags_hlp.attributes[6][0][1]  = calloc(44, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[6][0][1]);
    strcpy(tags_hlp.attributes[6][0][1],
        "a histogram drawn using the symbol \"#\"");
    tags_hlp.attributes[6][1][0]  = &attr_values[1];
    tags_hlp.attributes[6][1][1]  = calloc(48, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[6][1][1]);
    strcpy(tags_hlp.attributes[6][1][1],
        "a histogram drawn using the specified symbol");

    /* table */
    tags_hlp.attributes[7][0][0]  = &attr_values[0];
    tags_hlp.attributes[7][0][1]  = calloc(90, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[7][0][1]);
    strcpy(tags_hlp.attributes[7][0][1], 
        "a table with borders, calculations of expressions, "
        "and right alignment\n         of numbers");
    tags_hlp.attributes[7][1][0]  = &attr_values[3];
    tags_hlp.attributes[7][1][1]  = calloc(12, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[7][1][1]);
    strcpy(tags_hlp.attributes[7][1][1], "  no border");
    tags_hlp.attributes[7][2][0]  = &attr_values[4];
    tags_hlp.attributes[7][2][1]  = calloc(19, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[7][2][1]);
    strcpy(tags_hlp.attributes[7][2][1], "  no calculations");
    tags_hlp.attributes[7][3][0]  = &attr_values[5];
    tags_hlp.attributes[7][3][1]  = calloc(36, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[7][3][1]);
    strcpy(tags_hlp.attributes[7][3][1], "  don`t align numbers to the right");

    /* calc */
    tags_hlp.attributes[8][0][0]  = &attr_values[0];
    tags_hlp.attributes[8][0][1]  = calloc(28, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[8][0][1]);
    strcpy(tags_hlp.attributes[8][0][1], "calculation results only");
    tags_hlp.attributes[8][1][0]  = &attr_values[1];
    tags_hlp.attributes[8][1][1]  = calloc(51, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[8][1][1]);
    strcpy(tags_hlp.attributes[8][1][1],
        "expression and result (<expression> = <result>)");
    
    /* sep */
    tags_hlp.attributes[9][0][0]  = &attr_values[0];
    tags_hlp.attributes[9][0][1]  = calloc(44, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[9][0][1]);
    strcpy(tags_hlp.attributes[9][0][1],
        "      a separator drawn using the symbol \"-\"");
    tags_hlp.attributes[9][1][0]  = &attr_values[1];
    tags_hlp.attributes[9][1][1]  = calloc(48, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[9][1][1]);
    strcpy(tags_hlp.attributes[9][1][1],
        "a separator drawn using the specified symbol");

    /* h1 */
    tags_hlp.attributes[10][0][0] = &attr_values[0];
    tags_hlp.attributes[10][0][1] = calloc(46, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[10][0][1]);
    strcpy(tags_hlp.attributes[10][0][1],
        "      the h1 header drawn using the symbol \"=\"");
    tags_hlp.attributes[10][1][0] = &attr_values[1];
    tags_hlp.attributes[10][1][1] = calloc(50, sizeof(char));
     is_memory_allocated(tags_hlp.attributes[10][1][1]);
    strcpy(tags_hlp.attributes[10][1][1],
        "the h1 header drawn using the specified symbol");

    /* h2 */
    tags_hlp.attributes[11][0][0] = &attr_values[0];
    tags_hlp.attributes[11][0][1] = calloc(46, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[11][0][1]);
    strcpy(tags_hlp.attributes[11][0][1],
        "      the h2 header drawn using the symbol \"=\"");
    tags_hlp.attributes[11][1][0] = &attr_values[1];
    tags_hlp.attributes[11][1][1] = calloc(50, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[11][1][1]);
    strcpy(tags_hlp.attributes[11][1][1],
        "the h2 header drawn using the specified symbol");
    
    /* h3 */
    tags_hlp.attributes[12][0][0] = &attr_values[0];
    tags_hlp.attributes[12][0][1] = calloc(46, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[12][0][1]);
    strcpy(tags_hlp.attributes[12][0][1],
        "      the h3 header drawn using the symbol \"-\"");
    tags_hlp.attributes[12][1][0] = &attr_values[1];
    tags_hlp.attributes[12][1][1] = calloc(50, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[12][1][1]);
    strcpy(tags_hlp.attributes[12][1][1],
        "the h3 header drawn using the specified symbol");

    /* h4 */
    tags_hlp.attributes[13][0][0] = &attr_values[0];
    tags_hlp.attributes[13][0][1] = calloc(46, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[13][0][1]);
    strcpy(tags_hlp.attributes[13][0][1],
        "      the h4 header drawn using the symbol \"-\"");
    tags_hlp.attributes[13][1][0] = &attr_values[1];
    tags_hlp.attributes[13][1][1] = calloc(50, sizeof(char));
    is_memory_allocated(tags_hlp.attributes[13][1][1]);
    strcpy(tags_hlp.attributes[13][1][1],
        "the h4 header drawn using the specified symbol");

    /* insert */
    tags_hlp.attributes[14][0][0] = &attr_values[6];

    /* doc_width */
    tags_hlp.attributes[15][0][0] = &attr_values[2];
}


void free_hlp(void)
{
    uint8_t i, j, k;

    for ( i=0; i<tag_count; i++ ) {
        free(tags_hlp.assignment[i]);
        if ( tags_hlp.attributes[i] == NULL ) 
            continue;

        for ( j=0; j<attrs_count[i]; j++ ) {
            if ( tags_hlp.attributes[i][j][1] != NULL ) 
                free(tags_hlp.attributes[i][j][1]);

            free(tags_hlp.attributes[i][j]);
        }

        free(tags_hlp.attributes[i]);
    }

     free(tags_hlp.assignment);
     free(tags_hlp.attributes);
}

void help (void)
{
    init_hlp();
    while ( 1 ) {
        clear_console();
        uint8_t i;
        for ( i=0; i<(get_terminal_width()-34)/2; i++ ) 
            putchar(' ');
        
        puts("txtFormatter v1.0 interactive help");
        
        for ( i=0; i<get_terminal_width(); i++ ) 
            putchar('-');

        puts("\nSelect the tag for which you want to get help:");
        int selected = -100;

        for ( i=0; i<tag_count; i++ ) {
            if ( i < 9 ) 
                printf(" ");

            printf("  %d. %s\n", i+1, tag_list[i]);
        }

      
        /* tag selection menu */
        while ( 1 ) {
            printf(":> ");
            char* str_selected = calloc(100, sizeof(char));
            is_memory_allocated(str_selected);
            scanf("%[^\n]", &str_selected[0]);
            
            if ( str_selected[0] == '\n' || str_selected[0] == '\0' ) {
                free(str_selected);
                clear_input_buffer();
                continue;
            }

            if ( str_selected[0] == 'q' || str_selected[0] == 'Q' ) {
                free(str_selected);
                free_hlp();
                exit(0);
            }
            
            if ( is_number(str_selected, 0) == 1 ) {
                selected = atoi(str_selected);
                if ( selected >= 1 && selected <= tag_count ) {
                    free(str_selected);
                    break;
                }
            }

            printf ("Invalid value. Please enter a number from 1 to %d\n",
                tag_count);
            clear_input_buffer();
            free(str_selected);
            selected = -100;
        }

        selected--;
        clear_console();
        printf("<%s> ", tag_list[selected]);
        if ( tag_types[selected] == 1 ) 
            printf("... </%s>", tag_list[selected]);
        
        puts  ("\n");
        printf("  %s\n", tags_hlp.assignment[selected]);
        printf("\n\ntype: %s\n\n", TAG_TYPE[tag_types[selected]]);
        printf("\nattributes: ");

        if ( (tags_hlp.attributes[selected]!=NULL)&&(attrs_count[selected]>1) ){
            puts("\n");
            for ( i=0; i<attrs_count[selected]; i++ ) {
                printf("  %s - %s\n\n", tags_hlp.attributes[selected][i][0], 
                    tags_hlp.attributes[selected][i][1]);
            }
        } else {
            if ( tags_hlp.attributes[selected] == NULL )
                printf("%s\n", attr_values[0]);
            else
                printf("%s\n", tags_hlp.attributes[selected][0][0]);
        }

        printf("\n\nq - quit ; "
               "any other symbol - view help for other tags\n:> ");
        char s;
        clear_input_buffer();
        scanf("%c", &s);
        if ( s == 'q' ) 
            break;
        
        clear_input_buffer();
    }

    free_hlp();
    exit(0);
}
