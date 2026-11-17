# System architecture

## Purpose

The prototype automates receipt printing by controlling paper movement before transmitting a receipt to a thermal printer. Encoder feedback is used to determine paper travel, while dedicated input points support operational-status monitoring.

## Functional blocks

### Controller

An ATmega328P-based Arduino board coordinates all I/O, computes paper distance from encoder pulses, decides when the target feed length has been reached, and formats receipt text for serial transmission.

### Paper-feed subsystem

The paper-feed motor advances the paper. An incremental encoder returns two quadrature signals. The firmware attaches an interrupt to encoder channel A and reads channel B to track pulse direction. It derives travelled distance using:

```text
distance per pulse = (pi x wheel diameter) / pulses per revolution
```

With a 2.5 cm wheel diameter and 240 pulses per revolution, the configured 10 cm feed length corresponds to the calculated target pulse count.

### Sensor interfaces

The design reserves signals for paper presence, paper end, thermal state, alignment, cover state, and a reset button. The firmware configures most digital status inputs with internal pull-ups. In a production device, each signal must be validated against its physical sensor logic and incorporated into an explicit fault state before feeding paper or printing.

### Thermal-printer interface

The printer is connected through a software UART on digital pins D11 and D12 at 9600 baud. The demonstration sends a fixed, itemized receipt containing a header, items, total, and a placeholder for QR-code content. A physical printer should use the command set and power specification supplied by its manufacturer.

## Runtime sequence

1. The system initializes serial communication, I/O pins, and encoder interrupt handling.
2. The motor feeds paper while encoder pulses accumulate.
3. Every 50 ms, the firmware converts counted pulses into paper movement and reports the current position over the debug serial port.
4. Once the target distance is reached, the firmware stops the motor and detaches the encoder interrupt.
5. The receipt is transmitted to the thermal printer over the software UART.
6. A reset action clears tracking variables, restores the interrupt, and starts a new feed cycle.

## Power and commissioning

The controller uses a 5 V domain, while the thermal printer and motor may require separate higher-current supplies. Before hardware testing, verify the driver circuitry, common reference ground, protection for inductive loads, sensor levels, and printer power budget. Test each subsystem independently before executing the full workflow.
