# Bootloaders-Firmware-Update

## Objective
Demonstrate the use of bootloaders for firmware update using different methods:

P1)  IAP - perform In Application Programming (IAP) via UART Protocol with a host application.

P2) SD Card - read and flash a binary file stored on an SD card accessed via SPI.

P3) FOTA + generic server - download a binary file over WIFI using HTTP and forward it over UART to the microcontroller.

P4) FOTA + AWS (S3 bucket) - download a binary file over WIFI using HTTPS (TLS) and forward it over UART to the microcontroller.

### Skills Learned

- Bootloader architecture and memory mapping  
- SPI and UART communication protocols   
- SD card interfacing and FAT file handling  
- Wi-Fi and network stack integration (HTTP/HTTPS)  
- Firmware-over-the-air (FOTA) workflow implementation  

### Tools Used

- STM32CubeIDE & STM32CubeProgrammer
- PyCharm

### Languages

- C
- Python
