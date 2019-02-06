//
// example.c
// Example using breakfast.h to read and write serial data.
//

#include <stdio.h>
#include "breakfast.c"

int main(int argument_count, char ** arguments)
{
    // Open a handle to the serial device,
    // using a baud rate of 9600.
    bf_handle handle = bf_open(arguments[1], 9600);

    //
    // Reading data.
    //

    // Create a empty buffer.
    // This will be filled with data when we read from the serial device.
    char buffer[512];
    memset(buffer, 0, 512);

    // Read data from the serial device.
    int bytes_read = bf_read(handle, buffer, 512);

    // bf_read will return -1 if the read failed.
    // Otherwise, this value will hold the number of bytes read.

    if (bytes_read > 0)
    {
        // We successfully read some data, so print it out.
        // Make sure that the data ends in a zero,
        // as this is what puts() expects.
        buffer[511] = '\0';
        puts(buffer);
    }

    //
    // Writing data.
    //

    // Write some data to the serial device.
    //Here we send the string hello.
    bf_write(handle, "Hello", sizeof("Hello"));

    // Send an array of bytes.
    unsigned char data[64];
    for (int i = 0; i < 64; ++i)
    {
        data[i] = i;
    }
    bf_write(handle, number, 64);

    // Send one number.
    int number = 12345;
    bf_write(handle, &number, sizeof(number));

    // We're done, so close the handle.
    bf_close(handle);
}
