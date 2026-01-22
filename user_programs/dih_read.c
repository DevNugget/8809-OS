#include "syscall.h"
#include "string.h"

#define IMG_BUFFER_SIZE (1024*1024)

void _start() {
    char filename[256];
    readline(filename, sizeof(filename));

    char buf[IMG_BUFFER_SIZE];  // make sure it's big enough for the image

    int size = read_file(filename, buf, sizeof(buf));
    if (size <= 0) {
        syscall_write("Failed to read image\n");
        syscall_exit();
    }

    unsigned int width = *(unsigned int*)&buf[0];
    unsigned int height = *(unsigned int*)&buf[4];
    unsigned int* pixels = (unsigned int*)&buf[8];

    syscall_drawimage(0, 0, width, height, pixels);

    syscall_exit();
}
