# STM32H7 DOS Remote Desktop - Keil + CubeMX Complete Project

## نسخهٔ واقعی برای Keil MDK و STM32CubeMX

### سخت‌افزار
- **MCU**: STM32H743ZI / STM32H750
- **Compiler**: Keil MDK-ARM v5.x
- **RTOS**: FreeRTOS
- **Display**: TFT 320x240 RGB565 (40-pin parallel)
- **Input**: USB HID Keyboard (USB OTG HS)
- **Serial**: UART1 (PA9=TX, PA10=RX)

### معماری پروژه
```
DOS PC (9600 baud RS-232)
    |
    | UART Serial Protocol
    |
 STM32H7 (Keil + FreeRTOS)
    |
    +--- USB Host (OTG HS) ---> HID Keyboard
    |
    +--- FSMC 16-bit Parallel --> TFT 320x240 RGB565
    |
    +--- Tasks:
         - Serial RX/TX Handler
         - USB HID Keyboard Scanner
         - TFT Text Renderer (80x25 -> 320x240)
```

### فایل‌های اصلی

#### 1. **STM32H7_PCAnywhere.ioc**
فایل CubeMX برای تنظیم:
- SystemClock: HSE 25MHz -> PLL -> 400MHz
- UART1: 9600 baud (PA9, PA10)
- USB OTG HS: Host Mode + HID Support
- FSMC Bank 1: 16-bit parallel (PD0-PD15, PE0-PE1)
- DMA1/DMA2: for UART RX/TX
- FreeRTOS: Kernel + Tasks
- SysTick: for RTOS tick

#### 2. **Core/Src/main.c**
اصلی‌ترین فایل:
- HAL_Init() و SystemClock_Config()
- QueueHandle_t برای screen و keyboard
- 3 FreeRTOS Task
- Error handling

#### 3. **Drivers/**
- **UART**: UART1 interrupt RX، polling TX
- **TFT**: FSMC write، frame buffer rendering
- **USB**: HID Keyboard callback
- **Protocol**: Packet sync، checksum، screen/keyboard

#### 4. **Tasks/**
- **serial_task.c**: UART recv screen buffer
- **keyboard_task.c**: USB HID key polling
- **display_task.c**: TFT rendering with FreeRTOS

### نحوهٔ استفاده در Keil

1. **Open CubeMX**:
   ```
   File > Open > STM32H7_PCAnywhere.ioc
   ```

2. **Generate Code**:
   ```
   Project > Generate Code
   ```

3. **Open in Keil**:
   ```
   Keil MDK > File > Open > STM32H7_PCAnywhere.uvprojx
   ```

4. **Build**:
   ```
   Project > Build
   ```

5. **Flash**:
   ```
   Flash > Download
   ```

### نقاط مهم برای تطبیق

#### پین‌گذاری:
```
UART1:
  PA9  (TX)
  PA10 (RX)

FSMC (TFT Parallel):
  PD0-PD15 (Data D0-D15)
  PD4 (RD)
  PD5 (WR)
  PE1 (DCX)
  
USB OTG HS:
  PB12 (VBUS)
  PB13 (DM)
  PB14 (DP)
```

#### Clock Configuration (CubeMX):
```
HSE: 25 MHz
PLL:
  /M = 5
  x N = 160
  /P = 2
Result: 400 MHz
```

#### FSMC Configuration (CubeMX):
```
Bank 1, NOR/SRAM:
  Address Setup: 15 cycles
  Data Setup: 60 cycles
  Hold: 15 cycles
  Bus Turnaround: 0
```

#### USB Configuration (CubeMX):
```
OTG HS:
  Mode: Host Only
  VBUS Sensing: Enabled
  PHY: Internal (ULPI disabled)
  Low Power: Disabled
```

### Protocol Details

**Packet Structure**:
```
[SYNC1: 0xAA] [SYNC2: 0x55] [TYPE] [SEQ] [LEN_L] [LEN_H] [DATA...] [CHECKSUM]

Total: 2 + 1 + 1 + 2 + 256 + 1 = 263 bytes max
```

**Message Types**:
- `0x01`: MSG_HANDSHAKE
- `0x02`: MSG_ACK
- `0x07`: MSG_SCREEN (4KB frame in 16 packets)
- `0x08`: MSG_KEYBOARD (1 byte key code)
- `0x09`: MSG_DISCONNECT
- `0x0A`: MSG_PING

### Screen Rendering Logic

```
DOS Screen: 80 cols x 25 rows = 2000 cells
Each cell: 2 bytes (ASCII + attribute)
Total: 4000 bytes

TFT: 320x240 pixels
Split: 320 / 80 = 4 pixels per char width
       240 / 25 = 9.6 ≈ 10 pixels per char height

Font: 8x16 bitmap (scaled down to 4x10 per cell)
Colors: 16 DOS colors -> RGB565
```

### FreeRTOS Tasks

```
Task 1: SerialTask (Priority 3)
  - RX: UART interrupt -> screen_queue
  - Protocol packet handler
  - Max 4KB screen buffer

Task 2: KeyboardTask (Priority 4)
  - USB HID polling
  - Key code conversion
  - TX: Protocol keyboard packet

Task 3: DisplayTask (Priority 2)
  - Wait on screen_queue
  - TFT rendering
  - 30 FPS target (33ms delay)
```

### ملاحظات اجرایی

1. **تنظیم Baud Rate**:
   - DOS side: 9600 baud (پیش‌فرض)
   - STM32 side: UART1 init 9600

2. **USB Host Power**:
   - VBUS از GPIO یا USB چیپ
   - کیبورد باید < 100mA

3. **TFT Timing**:
   - FSMC must be fast enough
   - DMA2D optional برای تسریع

4. **Memory**:
   - DTCM: stack + heap
   - D2 SRAM: framebuffer (optional)
   - D3 SRAM: protocol buffers

### Debugging

```
Keil Debug:
  SWD/JTAG interface
  Real-time watch: screen_queue, keyboard_queue
  Breakpoints در serial_task.c, display_task.c
```

### نتیجه‌گیری

این پروژه یک نمونهٔ واقعی و کاملاً قابل‌پیاده‌سازی برای STM32H7 است که:
- بر اساس Keil MDK و STM32CubeMX
- دارای فایل `.ioc` برای CubeMX
- شامل تمام driver‌های لازم
- با FreeRTOS task-based design
- دارای protocol handler برای DOS

برای شروع:
1. STM32H7_PCAnywhere.ioc را در CubeMX باز کن
2. Code generate کن
3. Keil پروژه را اضافه کن
4. Build و Flash کن

---

**تنبیه**: این پروژه آموزشی است و باید قبل از استفاده در محیط تولید تست شود.
