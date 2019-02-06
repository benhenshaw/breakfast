// serial.c

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef int serial_device;

serial_device open_serial_device(char * device_name, int baud)
{
    int device = open(device_name, O_RDWR);
    if (device == -1) return -1;

    struct termios settings;
    tcgetattr(device, &settings);

    cfsetispeed(&settings, B9600);
    cfsetospeed(&settings, B9600);
    settings.c_cflag &= ~PARENB;
    settings.c_cflag &= ~CSTOPB;
    settings.c_cflag &= ~CSIZE;
    settings.c_cflag |= CS8 | CLOCAL;
    settings.c_lflag = ICANON;
    settings.c_oflag &= ~OPOST;

    tcsetattr(device, TCSANOW, &settings);
    tcflush(device, TCOFLUSH);

    return device;
}

int read_serial_device(serial_device device, void * buffer, int byte_count)
{
    return read(device, buffer, byte_count);
}

void close_serial_device(serial_device device)
{
    close(device);
}

int main(int argument_count, char ** arguments)
{
    int device = open_serial_device(arguments[1], 9600);

    while (1)
    {
        char buffer[512];
        int bytes_read = read_serial_device(device, buffer, 512);
        if (bytes_read > 0) fwrite(buffer, 1, 512, stdout);
        usleep(1000);
    }

    close_serial_device(device);
}
