/* txtfmt.c
 *
 * txtFormatter - A simple text formatting utility.
 * Copyright (C) 2024 Dmitriy Eliseev
 * <code.eliseev2003.dmitriy@yandex.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
#include "tags.h"
#include "help.h"

void print_logo()
{
    puts("\
\n    __       __  ______                           __  __           \
\n   / /__  __/ /_/ ____/___  _________ ___  ____ _/ /_/ /____  _____\
\n  / __/ |/_/ __/ /_  / __ \\/ ___/ __ `__ \\/ __ `/ __/ __/ _ \\/ ___/\
\n / /__>  </ /_/ __/ / /_/ / /  / / / / / / /_/ / /_/ /_/  __/ /    \
\n \\__/_/|_|\\__/_/    \\____/_/  /_/ /_/ /_/\\__,_/\\__/\\__/\\___/_/\n");
}

int main(int argc, char* argv[])
{
    if ( argc > 1 )
        help();
        
    print_logo();
    puts("txtFormatter text formatting utility v1.0\n"
         "Copyright (C) 2024 Dmitriy Eliseev\n");
    char source_file_extension[] = ".txtm";
    char result_file_extension[] = ".txt";
    char** files = get_files_in_dir(".", source_file_extension);
    uint16_t files_count = get_files_count(".", source_file_extension);

    if ( files_count == 0 ) {
        puts("Error: .txtm files not found");
        exit(EXIT_SUCCESS);
    }

    uint16_t i;
    
    for ( i=0; i<files_count; i++) {
        set_doc_width(DEFAULT_DOC_WIDTH);
        printf("processing file: %s\n", files[i]);
        char* file_content = get_file_content(files[i]);

        char* result = execute_all_tags(file_content);

        change_symbols('\f', '<', result);
        change_symbols('\a', '>', result);
        change_symbols('\r', ' ', result);
        change_symbols('\v', ' ', result);

        char* result_file = change_file_extension(files[i],
            result_file_extension);
        
        write_to_file(result_file, result);
        puts("  done");

        free(result_file);
        free(file_content);
        free(result);
        free(files[i]);
    }

    free(files);
    return 0;
}
