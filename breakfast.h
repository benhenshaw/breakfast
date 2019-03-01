/*
    breakfast.h
    Serial communication for UNIX and Windows.
    by Benedict Henshaw, 2019-02-06
    PUBLIC DOMAIN - See license at the end of this file.

    The function `bf_open()` takes a string that refers to a serial port.
    On Windows it looks like: `COM1`, `COM24`, etc.
    On UNIX    it looks like: `/dev/ttyUSB0`, `/dev/cu.usbmodem123`, etc.

    Commonly supported baud values:
    110     300     600
    1200    2400    4800
    9600    19200   38400
*/

#ifdef _WIN32
    #include <windows.h>
    typedef HANDLE bf_handle;
    #define BF_BAD_HANDLE (INVALID_HANDLE_VALUE)
#else
    #include <fcntl.h>
    #include <termios.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <string.h>
    #include <errno.h>
    typedef int bf_handle;
    #define BF_BAD_HANDLE (-1)
#endif

// Returns BF_BAD_HANDLE on failure.
bf_handle bf_open(char * serial_port_name, int baud);

// Returns number of bytes read, returns -1 on failure.
int bf_read(bf_handle handle, void * buffer, int byte_count);

// Returns number of bytes written, returns -1 on failure.
int bf_write(bf_handle handle, void * data, int byte_count);

void bf_close(bf_handle handle);

//
// Implementation.
//

bf_handle bf_open(char * serial_port_name, int baud)
{
    if (!serial_port_name) return BF_BAD_HANDLE;

#ifdef _WIN32
    bf_handle handle = CreateFile(serial_port_name,
        GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING,
        FILE_FLAG_WRITE_THROUGH | FILE_FLAG_NO_BUFFERING,
        NULL);
    if (handle == INVALID_HANDLE_VALUE) return BF_BAD_HANDLE;
    Sleep(500);
    DCB settings = {0};
    settings.DCBlength = sizeof(settings);
    BOOL success = GetCommState(handle, &settings);
    if (!success)
    {
        bf_close(handle);
        return BF_BAD_HANDLE;
    }
    settings.BaudRate      = baud;
    settings.ByteSize      = 8;
    settings.fAbortOnError = 0;
    settings.fBinary       = 1;
    settings.fDtrControl   = DTR_CONTROL_ENABLE;
    settings.fNull         = 0;
    settings.Parity        = NOPARITY;
    settings.StopBits      = ONESTOPBIT;
    success = SetCommState(handle, &settings);
    if (!success)
    {
        bf_close(handle);
        return BF_BAD_HANDLE;
    }
    PurgeComm(handle, PURGE_RXCLEAR | PURGE_TXCLEAR);
    return handle;
#else
    bf_handle handle = open(serial_port_name, O_RDWR | O_NOCTTY);
    if (handle == -1) return BF_BAD_HANDLE;
    usleep(500000);
    struct termios settings;
    int result = tcgetattr(handle, &settings);
    if (result == -1)
    {
        bf_close(handle);
        return BF_BAD_HANDLE;
    }
    cfsetispeed(&settings, baud);
    cfsetospeed(&settings, baud);
    settings.c_cflag     &= ~PARENB;
    settings.c_cflag     &= ~CSTOPB;
    settings.c_cflag     &= ~CSIZE;
    settings.c_cflag     |= CS8;
    settings.c_lflag     |= ~ICANON;
    settings.c_cc[VMIN]   = 0;
    settings.c_cc[VTIME]  = 0;
    result = tcsetattr(handle, TCSANOW, &settings);
    if (result == -1)
    {
        bf_close(handle);
        return BF_BAD_HANDLE;
    }
    tcflush(handle, TCIOFLUSH);
    return handle;
#endif
}

int bf_read(bf_handle handle, void * buffer, int byte_count)
{
#ifdef _WIN32
    DWORD bytes_read = 0;
    BOOL success = ReadFile(handle, buffer, byte_count, &bytes_read, NULL);
    if (!success) return -1;
    return bytes_read;
#else
    return read(handle, buffer, byte_count);
#endif
}

int bf_write(bf_handle handle, void * data, int byte_count)
{
#ifdef _WIN32
    DWORD bytes_written = 0;
    BOOL success = WriteFile(handle, data, byte_count, &bytes_written, NULL);
    if (!success) return -1;
    return bytes_written;
#else
    return write(handle, data, byte_count);
#endif
}

void bf_close(bf_handle handle)
{
#ifdef _WIN32
    CloseHandle(handle);
#else
    close(handle);
#endif
}

/*
    This is free and unencumbered software released into the public domain.

    Anyone is free to copy, modify, publish, use, compile, sell, or
    distribute this software, either in source code form or as a compiled
    binary, for any purpose, commercial or non-commercial, and by any
    means.

    In jurisdictions that recognize copyright laws, the author or authors
    of this software dedicate any and all copyright interest in the
    software to the public domain. We make this dedication for the benefit
    of the public at large and to the detriment of our heirs and
    successors. We intend this dedication to be an overt act of
    relinquishment in perpetuity of all present and future rights to this
    software under copyright law.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
*/
