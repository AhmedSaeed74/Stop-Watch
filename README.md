# Stop-Watch

## Description

Welcome to the Stop Watch System, a versatile timekeeping application built using the ATmega32 microcontroller. This system employs Timer1 in CTC mode to count time and utilizes multiplexed 7-segment displays to showcase the elapsed time. The Stop Watch provides you with precise timing capabilities and a user-friendly interface.

## Features

- **Timer-Based Counting:** The system uses Timer1 in ATmega16 with CTC mode to count time, ensuring accurate and reliable time measurement.

- **Multiplexed 7-Segments:** Six common anode 7-segment displays are multiplexed using a 7447 decoder and NPN BJT transistors. This approach minimizes hardware complexity while providing a clear display.

- **Reset Functionality:** The system includes a reset feature triggered by an external interrupt (INT0) with a falling edge, enabling you to reset the Stop Watch timer when needed.

- **Pause and Resume:** An external interrupt (INT1) with a rising edge acts as a pause/resume button, allowing you to control the timing functions conveniently.

- **Automatic Start:** The Stop Watch begins counting as soon as power is supplied to the microcontroller, ensuring immediate functionality.

## Usage

1. **Configuration:** Configure the ATmega32 microcontroller to operate at a frequency of 1MHz.

2. **Hardware Setup:** Connect the multiplexed 7-segment displays, ensuring proper connections with the 7447 decoder and NPN BJT transistors. Use PORTA for enabling/disabling segments and PORTC for connecting the decoder.

3. **Power-Up:** Supply power to the microcontroller to initiate the Stop Watch system.

4. **Start, Reset, and Pause:** Use external interrupt buttons to start, reset, and pause/resume the Stop Watch timer.

5. **Display:** Observe the elapsed time on the multiplexed 7-segment displays.

