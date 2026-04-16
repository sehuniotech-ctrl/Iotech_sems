# STM32U073C8T6 CubeMX 추가설정 체크리스트

현재 `sbb_u073_bl0942.ioc`는 MCU만 생성된 상태이며, 계량칩이 `ATM90E26-YU`로 변경되었기 때문에 아래 항목을 CubeMX에서 추가 설정해야 한다.

## 1. 필수 설정

- `SYS -> Debug = Serial Wire`
- `USART2`
  - 용도: `RS-485`
  - 권장 핀: `PA2 = TX`, `PA3 = RX`
- `GPIO Output`
  - `PA1 = RS-485 DE/RE`
  - `PB10 = Relay Drive`
  - `PC6 = RUN LED`
  - `PC7 = TRIP/FAULT LED`
- `GPIO Input`
  - `PB11 = Relay Feedback`

## 2. ATM90E26 인터페이스 선택

### 2.1 UART 방식 사용 시

- `USART1`
  - 용도: `ATM90E26`
  - 권장 핀: `PA9 = TX`, `PA10 = RX`
- 추가 GPIO
  - `USEL` 고정
  - `Reset` 제어 핀 선택 가능

### 2.2 SPI 방식 사용 시

- `SPI1` 권장
  - `PA5 = SCK`
  - `PA6 = MISO`
  - `PA7 = MOSI`
- 추가 GPIO
  - `CS`
  - `USEL`
  - `Reset`

### 2.3 확장 입력 핀

- `IRQ`
- `WarnOut`
- `ZX`
- `CF1`
- `CF2`

## 3. 권장 설정

- `Project Manager`
  - `Keep User Code when re-generating = Enable`
  - peripheral별 `.c/.h` 분리 생성 옵션 사용 권장
- `Clock Configuration`
  - 초기 bring-up은 내부 `HSI` 유지
- `NVIC`
  - 초기에는 기본값 유지 가능
- `DMA`
  - 초기 bring-up 단계에서는 미적용 가능

## 4. 이후 연결 포인트

- `App/Inc`, `App/Src` 폴더의 파일을 프로젝트 include/source path에 반영
- UART 수신 callback에서 아래 함수 연결
  - `device_app_on_rs485_rx_byte()`
- ATM90E26 transport HAL 연결
  - `atm90e26_transport_read_reg()`
  - `atm90e26_transport_write_reg()`
- 실제 GPIO 제어는 아래 파일에서 연결
  - `App/Src/relay_control.c`
- 실제 ATM90E26 register read는 아래 파일에서 연결
  - `Core/Src/atm90e26.c`

## 5. 현재 반영된 내용

- `main.c`에 `device_app_init()` 추가
- `while(1)`에 `device_app_process()` 추가
- 앱 골격 파일을 프로젝트에 복사 완료

## 6. 주의

- 현재 상태에서는 UART/SPI/GPIO init 함수가 생성되지 않았으므로 실제 동작 전에는 반드시 `.ioc` 설정을 추가하고 코드 재생성해야 한다.
