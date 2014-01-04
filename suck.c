/* $Copyright: $
 * Copyright (c) 1994 by Steve Baker (ice@mama.indstate.edu)
 * All Rights reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * suck: Clear your screen with suckage.  Takes "quad" as an argument to clear
 *       the screen in quadrants.
 * Usage: suck [quad]
 *
 * Make: gcc -O -o suck suck.c -s
 */
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/time.h>

typedef enum bool {TRUE = 1, FALSE = 0} bool;

int main(int argc, char **argv)
{
    struct winsize win;
    int i, j;
    bool quad = FALSE;

    if (argc > 1 && !strncasecmp("quad", argv[1], 5)) quad = TRUE;

    /* get the size of the window */
    ioctl(0, TIOCGWINSZ, &win);
    if (win.ws_row < 4)
    {
        fprintf(stderr, "suck: Window size unreasonably small.\n");
        return 1;
    }

    /* assumes you are using all the lines in the window */
    for (i = 0; i < win.ws_row / 2; i++)
    {
        if (!quad) usleep(20000); /* slow it down so we can watch it */
        printf("\033[1;%dr\033[0H\033M", win.ws_row / 2);
        printf("\033[%d;%dr\033[%dH\n", win.ws_row / 2 + 1, win.ws_row, win.ws_row);

        /* crappy qurter-screen sucking action */
        if (quad)
        {
            for (j = i; j < win.ws_row - i; j++)
            {
                printf("\033[%dH\033[2@", j + 1);
                printf("\033[%dG\033[4P", (win.ws_col / 2) - 2);
            }
        }
    }
    printf("\033[1;%dr\033[H", win.ws_row);
    return 0;
}