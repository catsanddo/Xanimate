#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "utils.h"

int running = 1;

static void usage(void)
{
    printf("Usage: xanimate [OPTION] [FILE]\n");
    printf("Animated wallpapers in X\n\n");
    printf("Optional arguments:\n");
    printf("  -D       Run in the background as a daemon\n");
    printf("  -f FPS   Set target fps\n");
    printf("  -h       Display this help\n");
}

int main(int argc, char **argv)
{
    int fps = 10;
    int frames;
    int cur_frame = 0;
    char *frame_path = 0;

    int is_daemon = getopt(argc, argv, ":D");
    optind = 1;
    int help = getopt(argc, argv, ":h");
    optind = 1;
    int fps_opt = getopt(argc, argv, ":f:");

    if (fps_opt == 'f') {
        fps = clampi(atoi(optarg), 1, 20);
    }
    if (help == 'h') {
        usage();
        exit(EXIT_SUCCESS);
    }

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            continue;
        }
        frame_path = argv[i];
    }

    if (!frame_path) {
        usage();
        exit(EXIT_FAILURE);
    }

    frames = CountFrames(frame_path);
    if (frames < 0) {
        printf("%s does not exist!\n", frame_path);
        exit(EXIT_FAILURE);
    } else if (frames == 0) {
        printf("Frame path is empty.\n");
        exit(EXIT_FAILURE);
    }

    if (is_daemon == 'D') {
        Daemonize();
    }

    while (running) {
        char frame[15];
        sprintf(frame, "frame%d.png", cur_frame + 1);

        /* Necessary because exec replaces process */
        pid_t pid = fork();
        if (pid == 0) {
            execlp("feh", "feh", "--no-fehbg", "--bg-max", frame, (char *) 0);
            // Only runs if execlp has failed
            exit(EXIT_FAILURE);
        } else {
            waitpid(pid, 0, 0);
        }

        /* Increment and wrap around cur_frame */
        cur_frame = ++cur_frame >= frames ? 0 : cur_frame;
        tick(fps);
    }

    return 0;
}
