# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [v1.0.0] - 2026-09-24

### Added

- FLRP-BURST image transfer with initiator and slave roles on the LoRa Plus™ Evaluation Kit (XIAO nRF54L15 + Wio-LR2021).
- README and build instructions aligned with USP for Zephyr v1.2.1 and USP v1.2.1.
- Support two modes for sending packets, one is the periodic automatic trigger(default mode), the other is to send a packet each time the user button is pressed.

### Changed

- Replace the FLRC BURST implementation with FLRP-BURST (LoRa® WOR + FLRC burst).
- Update OLED status text for the FLRP workflow.
- Validate on Zephyr RTOS v4.4.0 and Zephyr SDK v1.0.1 (previously Zephyr 4.2 and SDK 0.17.0).
- Changed the number of the transmitted frames, from 20 to 42.

### Removed

- FLRC BURST image transfer (superseded by FLRP-BURST).

## [v0.5.0] - 2026-02-13

### Added

- This preview release is not intended for production use. A stable release will be available soon based on.
