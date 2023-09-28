#include <linux/i2c-dev.h>
//#include <i2c/smbus.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

int main(){
    // Settings
    const unsigned char tmp102_addr = 0x48; // I2C address of TMP102
    const unsigned char read_addr = 0x00;   // Address to read tmp from

    int file;
    int adapter_nr = 2;
    char filename[20];
    int16_t temp_int;
    float temp_c;

    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);

    // Open file to read/write
    file = open(filename, O_RDWR);
    if (file<0){
        /* Error opening I2C */
        printf("Failed to open bus. \n");
        exit(1);
    }

    // Change to I2C address of TMP102
    if(ioctl(file, I2C_SLAVE, tmp102_addr) < 0){
        printf("Failed to acquire bus access to device. \n");
        exit(1);
    }

    // Start read by writing to temp reg
    char buf[5];
    buf[0] = 0x00;
    if(write(file, buf, 1) != 1){
        printf("Could not write to I2C device. \n");
        exit(1);
    }

    // Read temp from reg
    if(read(file, buf, 2) != 2){
        printf("Could not read from I2C device. \n");
        exit(1);
    }

    // Combine received bytes to single 16-bit value
    temp_int = (buf[0] << 4) | (buf[1] >> 4); // temperature in count

    // Check for negative temp
    if(temp_int > 0x7FF){
        temp_int |= 0xF000;
    }

    temp_c = temp_int * 0.0625; // Convert from count to degrees C

    // Print result
    printf("Temperature: %.2f C\n", temp_c);

    return 0;
}

