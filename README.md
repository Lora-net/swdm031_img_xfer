# FLRP image transfer demo

This demo transfers a monochrome image between two devices using **FLRP-BURST** (the file-transfer feature of **FLRP**, Fast LoRa communication Protocol) on [USP for Zephyr](https://github.com/Lora-net/usp_zephyr).

**FLRP-BURST = LoRa® + FLRC.** A short LoRa WOR exchange synchronizes the link in time and frequency; the payload is then sent as a high-speed FLRC burst (up to 2.6 Mbps raw bit rate).

> For FLRP-BURST principles, default configuration, and limitations, see the [FLRP overview](https://github.com/Lora-net/usp_zephyr/blob/v1.2.1/doc/FLRP.md) in USP for Zephyr.

## Overview

This application is a west manifest repository. It demonstrates a point-to-point image transfer:

- the **initiator** sends a 128×64 monochrome image (42 frames × 1024 bytes = 42 KB),
- the **slave** runs periodic LoRa® listening, receives the FLRC burst, and renders the image on the onboard OLED.

Target hardware (see the [LoRa Plus™ Evaluation Kit](https://github.com/Lora-net/usp_zephyr/blob/v1.2.1/doc/LORA_PLUS_EVK.md) documentation):

- **MCU:** [Seeed XIAO nRF54L15](https://www.seeedstudio.com/XIAO-nRF54L15-p-6493.html)
- **Radio:** Wio-LR2021 on the [LoRa Plus Expansion Board](https://www.semtech.com/products/wireless-rf/lora-plus/lr2021) (**FLRP requires FLRC**; use LR2021, not LR2022 or LR2012)
- **Display:** SSD1306 OLED (128×64) on the LoRa Plus Expansion Board shield

This demo passed Semtech validation with **Zephyr RTOS v4.4.0** and **Zephyr SDK v1.0.1** (pulled in when you run `west update` on this manifest).

## Software stack

Dependencies are defined in [west.yml](west.yml):


| Component      | Version / revision |
| -------------- | ------------------ |
| USP for Zephyr | v1.2.1             |
| USP            | v1.2.1 (imported)  |
| Zephyr RTOS    | v4.4.0 (imported)  |


## Roles

Two compile-time roles are supported (see [Kconfig](Kconfig)):


| Role                    | Kconfig              | Behavior                                                                                |
| ----------------------- | -------------------- | --------------------------------------------------------------------------------------- |
| **Initiator** (default) | `CONFIG_INITIATOR=y` | Sends the WOR, starts the FLRP-BURST transfer, and transmits the image data             |
| **Slave**               | `CONFIG_SLAVE=y`     | Runs periodic LoRa listening (active-sleep), receives the burst, and displays the image |


In this demo the initiator is always the **transmitter** and the slave is always the **receiver**. Each board must be built with the matching role. Flash one initiator binary and one slave binary on two separate boards.

## Configuration

> For **FLRP-BURST protocol configuration** (default values, PA ramp time, burst interframe, WOR presets, and where to configure FLRP), see [FLRP-BURST — Guidelines → Configuration](https://github.com/Lora-net/usp_zephyr/blob/v1.2.1/doc/FLRP_guidelines.md#configuration).

### Application macros

Application-specific macros are defined in [src/common/app_flrp_img_xfer.h](src/common/app_flrp_img_xfer.h).


| Macro                                 | Default | Description                                                                                         |
| ------------------------------------- | ------- | --------------------------------------------------------------------------------------------------- |
| `FLRP_IMG_XFER_CRYPTO_ENABLED`        | `true`  | Enable per-packet AES-CMAC integrity (MIC); the image payload is **not** encrypted                  |
| `FLRP_IMG_XFER_IS_LOW_FREQUENCY`      | `true`  | Use sub-GHz frequency plan (`SMTC_FLRP_FREQ_SUB_G`); set `false` for 2.4 GHz (`SMTC_FLRP_FREQ_2G4`) |
| `FLRP_IMG_XFER_INITIATOR_PERIODIC_MS` | `2000`  | In **periodic** mode: idle time after a completed round before the next auto transfer (ms)          |
| `FLRP_IMG_XFER_BUTTON_LONG_PRESS_MS`  | `2000`  | Hold the user button this long on the initiator to switch periodic ↔ button mode (ms)               |
| `FLRP_IMG_XFER_BUTTON_DEBOUNCE_MS`    | `50`    | Minimum press duration to count as a short press (ms)                                                 |
| `FLRP_IMG_XFER_CRYSTAL_ERROR`         | `10`    | Crystal error in ppm (drift margin, ≤ ~60 ppm)                                                      |
| `IMAGE_NUMS`                          | `42`    | Number of image frames                                                                              |
| `SINGLE_IMAGE_SIZE`                   | `1024`  | Bytes per frame (128×64 monochrome)                                                                 |
| `TOTAL_IMAGES_SIZE`                   | `43008` | Total payload size (42 × 1024 bytes)                                                                |


**TX power, FLRC data rate, frequency plan, channels, WOR settings, and burst interframe** come from the FLRP protocol defaults in [flrp_configuration.h](https://github.com/Lora-net/usp/blob/v1.2.1/protocols/smtc_flrc_protocol/flrp_configuration.h) (USP module). In a west workspace, the file is located at: `modules/lib/usp/protocols/smtc_flrc_protocol/flrp_configuration.h`.

This demo overrides several FLRP compile-time defaults in [CMakeLists.txt](CMakeLists.txt) (applied to both initiator and slave when `CONFIG_USP_FLRP` is enabled):


| Parameter                              | USP default (`flrp_configuration.h`) | This demo (`CMakeLists.txt`) |
| -------------------------------------- | -------------------------------------- | ---------------------------- |
| `DATA_FLRC_CR`                         | CR 1/2                                 | CR 3/4                       |
| `DATA_FLRC_MIN_INTERFRAME_DURATION_US` | 700 µs                                 | 200 µs                       |
| `WOR_865MHz_RF_FREQ_IN_HZ`             | 865100000 Hz                           | 915120000 Hz                 |
| `TX_OUTPUT_POWER_DBM`                  | 14 dBm                                 | 22 dBm                       |
| `WOR_LORA_LONG_PREAMBLE_MS`            | 1000 (preset A)                        | 100 (preset B)               |
| `RESTART_WOR_RX_DELAY_MS`              | 950 (preset A)                         | 80 (preset B)                |

`WOR_LORA_LONG_PREAMBLE_MS` and `RESTART_WOR_RX_DELAY_MS` form one WOR preset and must stay paired. This demo uses **preset B** (100 ms long preamble, 80 ms slave RX gap). Rebuild and flash both the initiator and the slave after changing either value.

This demo uses `SMTC_FLRP_BIDIRECTIONAL` with `SMTC_FLRP_LINK_ADAPTATION_DISABLED`. With adaptive link disabled, the initiator and slave must use the **same** payload size, uniform packet size, and burst interframe on both sides (see [FLRP-BURST — Guidelines → Via the FLRP API](https://github.com/Lora-net/usp_zephyr/blob/v1.2.1/doc/FLRP_guidelines.md#via-the-flrp-api-application-code)). The initiator and slave must also share the **same** radio configuration.

### Compile-time protocol overrides (`EXTRA_CFLAGS`)

Protocol defaults from `flrp_configuration.h` can be overridden at build time with `EXTRA_CFLAGS`. **Both initiator and slave must be built with the same overrides.**

Common overrides (see [FLRP-BURST — Guidelines → Compile-time defaults](https://github.com/Lora-net/usp_zephyr/blob/v1.2.1/doc/FLRP_guidelines.md#compile-time-defaults--flrp_configurationh)):


| Parameter                      | Define                                  | Default                                                    |
| ------------------------------ | --------------------------------------- | ---------------------------------------------------------- |
| TX power                       | `TX_OUTPUT_POWER_DBM`                   | 22 dBm (override by [CMakeLists.txt](CMakeLists.txt))      |
| FLRC coding rate               | `DATA_FLRC_CR`                          | CR 3/4 (override by [CMakeLists.txt](CMakeLists.txt))      |
| Burst interframe               | `DATA_FLRC_MIN_INTERFRAME_DURATION_US`  | 200 µs (override by [CMakeLists.txt](CMakeLists.txt))      |
| WOR / FLRC frequency (865 MHz) | `WOR_865MHz_RF_FREQ_IN_HZ`              | 915.12 MHz (override by [CMakeLists.txt](CMakeLists.txt))  |
| FLRC raw bit rate              | `DATA_FLRC_RAW_BIT_RATE`                | 2.6 Mbps                                                   |
| WOR / FLRC frequency (2.4 GHz) | `WOR_2GHz4_RF_FREQ_IN_HZ`               | 2.412 GHz                                                  |
| Burst target PER [0–100]       | `DATA_FLRC_BURST_TARGET_PER_PERCENTAGE` | 0 %                                                        |


Example with several overrides:

```bash
west build --pristine --board xiao_nrf54l15/nrf54l15/cpuapp \
  --shield semtech_loraplus_expansion_board \
  --shield semtech_wio_lr2021 \
  -- -DEXTRA_CFLAGS="-DWOR_865MHz_RF_FREQ_IN_HZ=868120000 -DDATA_FLRC_RAW_BIT_RATE=RAL_FLRC_RAW_BIT_RATE_2_600_MBPS -DTX_OUTPUT_POWER_DBM=14 -DDATA_FLRC_CR=RAL_FLRC_CR_1_2 -DDATA_FLRC_BURST_TARGET_PER_PERCENTAGE=5"
```

> When you change `DATA_FLRC_MIN_INTERFRAME_DURATION_US`, also tune the PA ramp time in the devicetree overlay (`pa-ramp-time`). See [Interframe and PA ramp time tuning](https://github.com/Lora-net/usp_zephyr/blob/v1.2.1/doc/FLRP_guidelines.md#interframe-and-pa-ramp-time-tuning).

Runtime overrides are also possible via `smtc_flrp_set_new_radio_config()` and `smtc_flrp_set_new_advanced_flrc_radio_config()` in [src/common/app_flrp_img_xfer.c](src/common/app_flrp_img_xfer.c) (commented-out examples are provided).

## Getting started

### Install Zephyr

The following steps were tested on Linux and Windows.

Install Zephyr following the [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html), then initialize this workspace:

```bash
mkdir zephyr_workspace
cd zephyr_workspace
git clone https://github.com/Lora-net/swdm031_img_xfer.git
west init -l swdm031_img_xfer
west update
```

### Build

Build from the application directory with the LoRa Plus EVK shields:

```bash
cd zephyr_workspace/swdm031_img_xfer
west build --pristine --board xiao_nrf54l15/nrf54l15/cpuapp \
  --shield semtech_loraplus_expansion_board \
  --shield semtech_wio_lr2021
```

Build as **slave**:

```bash
west build --pristine --board xiao_nrf54l15/nrf54l15/cpuapp \
  --shield semtech_loraplus_expansion_board \
  --shield semtech_wio_lr2021 \
  -- -DCONFIG_SLAVE=y
```

> **Wio-LR2021 CN variant:** use `--shield semtech_wio_lr2021_cn` instead of `semtech_wio_lr2021`, and adjust the PA table as described in the [USP for Zephyr porting guide](https://github.com/Lora-net/usp_zephyr/blob/v1.2.1/doc/usp_zephyr_porting_guide.md) and [Supported boards & shields](https://github.com/Lora-net/usp_zephyr/blob/v1.2.1/README.md#supported-boards--shields).

### Flash

The XIAO nRF54L15 uses a CMSIS-DAP interface (no onboard J-Link). Semtech recommends **pyOCD** (≥ 0.38.0) for flashing and debugging. It is usually installed with the Zephyr Python environment. On Debian-based systems you may need udev rules. Follow the [pyOCD udev instructions](https://github.com/pyocd/pyOCD/tree/main/udev).

Flash from the demo directory:

```bash
west flash --runner pyocd
```

Or with native pyOCD:

```bash
pyocd flash -t nrf54l build/zephyr/zephyr.hex
```

When two boards are connected, target a specific device by unique ID:

```bash
pyocd list
pyocd flash -t nrf54l build/zephyr/zephyr.hex -u UNIQUE_ID
```

If the board reports a lock error:

```bash
pyocd erase -c -t nrf54l
```

## Runtime behavior

### Initiator

1. Shows a boot animation, then FLRP radio configuration on the OLED (role, frequency, TX power, FLRC bit rate).
2. Runs in one of two modes (default **periodic**). **Long-press the user button for 2 s** to switch modes; the OLED idle lines show the active mode and hint.
3. **Periodic mode:** after each completed round (TX + slave statistics), waits **2 s** idle, then starts the next transfer automatically (`FLRP_IMG_XFER_INITIATOR_PERIODIC_MS` in `main.c`).
4. **Button mode:** each **short press** (release before 2 s, after debounce) starts one transfer; no automatic repeats.
5. Sends **42 frames × 1024 bytes = 42 KB** of image data from `image_chick_looking_buf`.
6. After a successful TX, fetches the slave RX statistics report and displays PER / RSSI on the OLED. The same PER line also shows **All ET** (see below).

**Note: All ET(All Elapse Time)** is the full image-transfer transaction time, shown only on the initiator (`All ET: xxxms`). The timer starts at the call to `smtc_flrp_initiate_transmission()` and stops when the transmit callback reports completion, which is the data burst ACK. So **All ET** covers **WOR, WOR ACK, the adaptive-link phase when that phase runs, and the FLRC data burst through the data burst ACK**. It does not include the later exchange that fetches the slave RX statistics. This demo disables link adaptation, so the adaptive-link phase does not run and is not part of the measured time.

### Slave

1. Starts **periodic listening** at initialization (`smtc_flrp_start_periodic_listening`).
2. On successful reception, renders the received image on the OLED and shows link statistics:
  - packets OK / check error / NOK
  - mean RSSI
  - byte loss percentage (PER)
3. Prepares a small acknowledgment payload (RX statistics) for the bidirectional FLRP exchange.

The user button requires the `smtc-user-button` devicetree alias (configured in [boards/xiao_nrf54l15_nrf54l15_cpuapp.overlay](boards/xiao_nrf54l15_nrf54l15_cpuapp.overlay)).

## Device EUI and key configuration

Pairing is configured in [src/common/app_flrp_img_xfer.c](src/common/app_flrp_img_xfer.c):


| Role      | Local DevEUI              | Target DevEUI             |
| --------- | ------------------------- | ------------------------- |
| Initiator | `FF:FE:FD:FC:FB:FA:F9:F8` | `00:FE:FD:FC:FB:FA:F9:F8` |
| Slave     | `00:FE:FD:FC:FB:FA:F9:F8` | `FF:FE:FD:FC:FB:FA:F9:F8` |


Default FLRP key (16 bytes): `000102030405060708090A0B0C0D0E0F`

Update these values for custom deployments.

## Related documentation

- [FLRP overview](https://github.com/Lora-net/usp_zephyr/blob/v1.2.1/doc/FLRP.md) (USP for Zephyr)
- [FLRP-BURST — Principles](https://github.com/Lora-net/usp_zephyr/blob/v1.2.1/doc/FLRP_principles.md)
- [FLRP-BURST — Guidelines](https://github.com/Lora-net/usp_zephyr/blob/v1.2.1/doc/FLRP_guidelines.md)
- [LoRa Plus Evaluation Kit](https://github.com/Lora-net/usp_zephyr/blob/v1.2.1/doc/LORA_PLUS_EVK.md)
- [FLRP API sample](https://github.com/Lora-net/usp_zephyr/blob/v1.2.1/samples/usp/rac/flrp_api/README.md) — minimal FLRP workflow reference
- [USP for Zephyr](https://github.com/Lora-net/usp_zephyr/blob/v1.2.1/README.md)
- [USP (baremetal)](https://github.com/Lora-net/usp/blob/v1.2.1/README.md) — FLRP API reference
- [Changelog](CHANGELOG.md)

## Notes

- Regulatory compliance (duty cycle, dwell time, channel plan, EIRP, etc.) is the responsibility of the application. See [FLRP-BURST — Guidelines → Regulatory compliance](https://github.com/Lora-net/usp_zephyr/blob/v1.2.1/doc/FLRP_guidelines.md#regulatory-compliance).
- When using `EXTRA_CFLAGS` to change protocol defaults, rebuild with `--pristine` (or clean the build directory) so the new macros are applied.
- Both boards must share the same frequency plan, radio parameters, EUIs, and MIC key.
- Only `SMTC_FLRP_BIDIRECTIONAL` is validated in USP v1.2.1; other communication modes are available but not fully validated (see FLRP-BURST — Guidelines).

