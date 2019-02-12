//
// example.c
// Example using breakfast.h to read and write serial data.
//

#include <stdio.h>
#include "breakfast.h"

int main(int argument_count, char ** arguments)
{
    setbuf(stderr, NULL);

    if (argument_count <= 1)
    {
        fputs("You need to specify which serial port to open!", stderr);
        exit(1);
    }

    // Open a handle to the serial device using a baud rate of 9600.
    bf_handle handle = bf_open(arguments[1], 9600);
    if (handle == BF_BAD_HANDLE)
    {
        fprintf(stderr, "Could not open serial device '%s'.\n", arguments[1]);
        exit(1);
    }

    fprintf(stderr, "Opened serial device '%s'.\n", arguments[1]);

    //
    // Writing data.
    //

    // Write some data to the serial device.
    // Here we send the string "Hello".
    bf_write(handle, "Hello", sizeof("Hello"));

    fprintf(stderr, "Wrote 'Hello'\n");

    // Send an array of bytes.
    unsigned char data[64];
    for (int i = 0; i < 64; ++i)
    {
        data[i] = i;
    }
    bf_write(handle, data, 64);

    fprintf(stderr, "Wrote 64 bytes.\n");


    // Send one number.
    int number = 12345;
    bf_write(handle, &number, sizeof(number));

    fprintf(stderr, "Wrote number.\n");

    //
    // Reading data.
    //

    // This will be filled with data when we read from the serial device.
    char buffer[512];
    // Set all bytes to zero to avoid junk.
    memset(buffer, 0, 512);

    fprintf(stderr, "Reading data...\n");

    // Read data from the serial device.
    int bytes_read = bf_read(handle, buffer, 512);
    if (bytes_read == -1)
    {
        fprintf(stderr, "Error reading from serial device.\n");
        exit(1);
    }

    fprintf(stderr, "Read %d bytes:\n", bytes_read);

    if (bytes_read > 0)
    {
        // We successfully read some data, so let's print it out.
        // Make sure that the data ends in a zero as this is what puts expects.
        buffer[511] = '\0';
        puts(buffer);
    }

    fprintf(stderr, "Closing serial device.\n");

    // We're done, so close the handle.
    bf_close(handle);
}
