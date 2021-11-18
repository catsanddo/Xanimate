#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include "utils.h"

int running = 1;

int main(void)
{
    int fps;
    int frames;
    int cur_frame = 0;

    Daemonize();

    openlog("Xanimate", LOG_PID, LOG_DAEMON);

    LoadConfig(&fps, &frames);

    while (running) {
        LoadConfig(&fps, &frames);

        char frame[15];
        sprintf(frame, "frame%d.png", cur_frame + 1);

        /* Necessary because exec replaces process */
        pid_t pid = fork();
        if (pid == 0)
            execlp("feh", "feh", "--no-fehbg", "--bg-max", frame, (char *) 0);

        /* Increment and wrap around cur_frame */
        cur_frame = ++cur_frame >= frames ? 0 : cur_frame;
        tick(fps);
    }

    closelog();
    return 0;
}
