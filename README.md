# Breakfast
Serial communication for UNIX and Windows.
by Benedict Henshaw, 2019

## API
```C
// Returns BF_BAD_HANDLE on failure.
bf_handle bf_open(char * serial_port_name, int baud)

// returns -1 on failure.
int bf_read(bf_handle handle, void * buffer, int byte_count)

// returns -1 on failure.
int bf_write(bf_handle handle, void * data, int byte_count)

void bf_close(bf_handle handle)
```

## Tips
The function `bf_open()` takes a string that refers to a serial port.\
On Windows it looks like: `COM1`, `COM24`, etc.\
On UNIX it looks like: `/dev/ttyUSB0`, `/dev/cu.usbmodem123`, etc.

The following baud values are available in both UNIX and Windows:

```
110     300     600
1200    2400    4800
9600    19200   38400
```

Some of these values might not be supported by the hardware. There are also values supported only on Windows, or only on UNIX.

| UNIX  | Windows |
|-------|---------|
| 0     | .       |
| 50    | .       |
| 75    | .       |
| 110   | 110     |
| 134   | .       |
| 150   | .       |
| 200   | .       |
| 300   | 300     |
| 600   | 600     |
| 1200  | 1200    |
| 1800  | .       |
| 2400  | 2400    |
| 4800  | 4800    |
| 9600  | 9600    |
| .     | 14400   |
| 19200 | 19200   |
| 38400 | 38400   |
| .     | 57600   |
| .     | 115200  |
| .     | 128000  |
| .     | 256000  |

See [Windows Supported Baud Rates](https://docs.microsoft.com/en-us/windows/desktop/api/winbase/ns-winbase-_dcb#members)\
See [UNIX/POSIX Supported Baud Rates](https://linux.die.net/man/3/termios) (near bottom of page).