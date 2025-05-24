# SAGE OS - Minimal Edition

This is a minimal implementation of SAGE OS for Raspberry Pi. It provides a basic "Hello World" kernel that initializes the UART and echoes characters back to the console.

## Building

To build the minimal kernel:

```bash
cd minimal
make
```

This will generate `kernel8.img` which can be copied to an SD card along with the appropriate `config.txt` file.

## Running

1. Format an SD card with FAT32
2. Copy the following files to the SD card:
   - `kernel8.img`
   - `config.txt` (for Raspberry Pi 3/4) or `config_rpi5.txt` (renamed to `config.txt` for Raspberry Pi 5)
   - Raspberry Pi firmware files (bootloader.bin, start.elf, fixup.dat) from the [Raspberry Pi firmware repository](https://github.com/raspberrypi/firmware/tree/master/boot)
3. Insert the SD card into your Raspberry Pi
4. Connect a serial console (115200 baud rate)
5. Power on the Raspberry Pi

## Features

- Bare-metal kernel for Raspberry Pi
- UART initialization and communication
- Simple character echo functionality
- Supports Raspberry Pi 3, 4, and 5 (with appropriate config)

## Next Steps

This minimal implementation can be extended with:

1. Memory management
2. Process scheduling
3. File system support
4. Device drivers
5. Shell interface
6. AI capabilities

## License

This code is provided under the same license as the main SAGE OS project.