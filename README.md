STM32 Digital Stethoscope & Waveform Visualizer


Overview:
This project demonstrates a complete data acquisition (DAQ) pipeline. It captures analog signals from an electret microphone, processes them through a digital filter, and renders a live waveform on an SSD1306 OLED display using optimized DMA transfers with I2C.


Technical Features:

1. High-Speed Data Acquisition (ADC + DMA)
Sampling Rate: 4kHz sampling frequency to capture the human heart rate frequency range (20Hz - 150Hz).

Zero CPU Overhead: Used DMA (Direct Memory Access) in circular mode to transfer ADC samples to memory, allowing the CPU to focus entirely on signal processing and UI rendering.


2. Digital Signal Processing (DSP):
Filtering: Implemented a software-based Exponential Moving Average (EMA) filter to suppress high-frequency environmental noise and isolate S1/S2 heart sounds.

Hysteresis & Peak Detection: Developed a robust thresholding algorithm with software hysteresis to calculate BPM (Beats Per Minute) from low-amplitude ($<20mV$) signals.


3. Embedded UI & Visualization:
OLED Driver: Developed a custom low-level driver for the SSD1306 (I2C) display.

Zoom In/Out Feature: Implemented a real-time "Zoom" feature (x2, x4, x8) using External Interrupts (EXTI) to visualize low-amplitude signals without hardware pre-amplification.



System Architecture:

MCU: STM32F407VGT6 (Cortex-M4)

Display: 128x64 I2C OLED

Sensor: Electret Microphone with a passive RC biasing circuit.

Communication: I2C (Standard Mode), ADC (Single Channel), DMA1/DMA2.



Hardware Interface:
Since we are not using an active Op-Amp, a passive RC Biasing Circuit is required to shift the microphone's AC signal into the ADC's measurable range ($0V - 3.3V$).

VCC: Connect the Electret Microphone to 3.3V through a 2.2kΩ - 10kΩ resistor (Pull-up).

Coupling Capacitor: Use a 0.1µF to 1µF Ceramic/Electrolytic Capacitor between the microphone output and the STM32 ADC pin (PA0). This removes the DC offset from the power supply.

DC Bias: Use a voltage divider (two 22kΩ resistors) at the ADC pin to center the signal at 1.65V ($VCC / 2$). This allows the STM32 to "see" both the positive and negative peaks of the sound wave.


Challenges & Lessons Learned:
SNR Management: Analyzed the Signal-to-Noise Ratio (SNR) limitations of passive sensing. Without an active Op-Amp stage, software-side stabilization (Hysteresis) was critical to prevent false-positive peak detection.

Timing Optimization: Balanced the I2C bus speed with the ADC sampling rate to ensure a jitter-free waveform display.

For the video: 
https://drive.google.com/file/d/1i4VS3BvGQo7P7t75VjLHtIJPYrasT4gu/view?usp=sharing

![3](https://github.com/user-attachments/assets/fb385d19-c688-4039-a64f-30da6ffc1ea4)
![1](https://github.com/user-attachments/assets/1cf5c3da-62af-49a4-a140-83979e268c24)
