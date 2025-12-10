# MAX6675_RK

*MAX6675 SPI thermocouple library for Particle devices*

From the datasheet:

**Cold-Junction-Compensated K-Thermocouple-to-Digital Converter (0°C to +1024°C)**

The MAX6675 performs cold-junction compensation and digitizes the signal from a type-K thermocouple. The data is output in a 12-bit resolution, SPI-compatible, read-only
format.

This converter resolves temperatures to 0.25°C, allows readings as high as +1024°C, and exhibits thermocouple accuracy of 8 LSBs for temperatures ranging from 0°C to +700°C.

The MAX6675 is available in a small, 8-pin SO package.

- Direct Digital Conversion of Type -K Thermocouple Output
- Cold-Junction Compensation
- Simple SPI-Compatible Serial Interface
- 12-Bit, 0.25°C Resolution
- Open Thermocouple Detection

Additionally:

- Unidirectional 3-wire SPI interface (SCK, CS, and SO only)
- 3.0 to 5.5V recommended supply voltage
- Serial clock frequency maximum: 4.3 MHz
- SPI Mode 3 (CPOL=1, CPHA=1) (data sampled on the second edge, idle high)

![SPI timing](images/spi-timing.png)

## API

