#include <stdio.h>
#include <stdlib.h>
#include "config.h"

int main(void)
{
    FILE* foo = popen("ls", "r");

    return 0;
}
