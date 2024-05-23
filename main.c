#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF 512

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s [FILE]\n", argv[0]);
        return 0;
    }

    char *path = getenv("PWD");
    path = !path? getenv("CD"): path;

    char *s = (char*) malloc(strlen(argv[1]) + strlen(path) + 1);
    strcat(s, path);
    strcat(s, "/");
    strcat(s, argv[1]);

    FILE* f = fopen(s, "rb+");

    if(!f)
    {
        fprintf(stderr, "Cannot open file.\n");
        return 2;
    }

    while(1)
    {
        unsigned char buffer[BUF] = {0, };
        size_t sig = fread(buffer, 1, BUF, f);

        for(int i = 0; i < sig; ++ i) {
            if(buffer[i] != 0 && buffer[i] != 255) buffer[i] ^= 255;
        }

        fseek(f, -(int)sig, SEEK_CUR);
        if (sig == BUF) fwrite(buffer, 1, BUF, f);
        else
        {
            fwrite(buffer, 1, sig, f);
            break;
        }
    }

    fclose(f);
    free(s);
    return 0;
}
