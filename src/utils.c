#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include "utils.h"

extern int running;

static int new_fps;
static int new_frames;

// Easier to sleep in milliseconds
static int msleep(int millis)
{
    return usleep(millis * 1000);
}

static void ParseConfig(int *fps, int *frames)
{
    char *home = getenv("HOME");
    char config_path[PATH_MAX];
    sprintf(config_path, "%s/.Xanimaterc", home);
    FILE *file = fopen(config_path, "r");

    if (file) {
        char frame_path[PATH_MAX];
        fscanf(file, "%d", fps);
        fscanf(file, "%255s", frame_path);
        fclose(file);
        
        chdir(frame_path);
        file = popen("ls | wc -l", "r");
        fscanf(file, "%d", frames);
        fclose(file);
    } else {
        exit(1);
    }
}

void sig_handler(int signal)
{
    switch (signal) {
        case SIGHUP: {
                         ParseConfig(&new_fps, &new_frames);
                     } break;
        case SIGTERM: {
                          running = 0;
                      } break;
    }
}

void Daemonize(void)
{
    pid_t pid;

    pid = fork();
    if (pid > 0)
        exit(0);
    else if (pid < 0)
        exit(1);

    if (setsid() < 0)
        exit(1);

    struct sigaction act = {0};
    act.sa_flags = SA_NODEFER;
    act.sa_handler = sig_handler;
    sigaction(SIGHUP, &act, 0);
    sigaction(SIGTERM, &act, 0);

    pid = fork();
    if (pid > 0)
        exit(0);
    else if (pid < 0)
        exit(1);

    for (int fd = sysconf(_SC_OPEN_MAX); fd >= 0; --fd) {
        close(fd);
    }
}

void LoadConfig(int *fps, int *frames)
{
    if (!new_fps && !new_frames) {
        ParseConfig(&new_fps, &new_frames);
    }
    *fps = new_fps;
    *frames = new_frames;
}

void tick(int fps)
{
    // milliseconds to sleep per frame
    msleep(1.0 / fps * 1000);
}
