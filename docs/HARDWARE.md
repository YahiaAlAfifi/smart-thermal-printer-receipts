# Hardware and safety notes

## Power domains

- The microcontroller operates at 5 V.
- A thermal printer typically requires a separate, adequately rated supply.
- Motors and inductive loads require an appropriate driver stage and protection.
- All control and power domains must share a defined reference ground when required by the selected driver/interface.

## Inputs

The design reserves interfaces for paper presence, end-of-paper, alignment, cover condition, thermal status, reset, and quadrature encoder signals. Confirm whether each sensor is active-high or active-low in physical hardware and adjust pull-ups/pull-downs accordingly.

## Commissioning checklist

1. Test the motor driver without paper loaded.
2. Verify encoder direction and pulse count.
3. Confirm the measured feed distance before enabling receipt printing.
4. Validate sensor logic in the serial monitor.
5. Test thermal-printer communication with its documented command set.
