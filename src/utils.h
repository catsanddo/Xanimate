#ifndef UTILS_H
#define UTILS_H

void Daemonize(void);
void LoadConfig(int *fps, int *frames);
void tick(int fps);          // Sleeps for a delay based on a target framerate

#endif
