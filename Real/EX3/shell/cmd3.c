#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int duration;
    if (argc < 2)
    {
        duration = 1;
    }
    else
    {
        sscanf(argv[1], "%d", &duration);
    }
    printf("A sleep program\n");
    if (duration < 0 || duration > 1000)
    {
        printf("duration invalid!\n");
        return -1;
    }
    printf("sleep %ds\n", duration);
    sleep(duration);
    printf("sleep finished\n");
    return 0;
}