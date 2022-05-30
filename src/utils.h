#ifndef UTILS_H
#define UTILS_H

void Daemonize(void);
int CountFrames(const char *path);
void tick(int fps);          // Sleeps for a delay based on a target framerate
int atoi(const char *num);
int clampi(int val, int min, int max);

#endif
