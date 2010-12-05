/** 
 * @file main.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "panicbutton.h"

/*
 * 
 */

void usage(char *arg1) {

    printf("usage: %s command_path\n", arg1, arg1);

    exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {

    panicbutton_t pb;
    int pid;

    if (argc != 2 && argc > 0) usage(argv[0]);
    else return (EXIT_FAILURE);

    if (!pb_init_device(&pb)) {
        return 1;
    }

    while(1) {
        pb_next_event(&pb);
        printf("PANIC!\n");
        pid = fork();
        if (pid==0) {
            system(argv[1]);
            return (EXIT_SUCCESS);
        }
    }

    pb_destroy_device(&pb);


    return (EXIT_SUCCESS);
}

