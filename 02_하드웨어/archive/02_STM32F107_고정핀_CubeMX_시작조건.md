# STM32F107 고정 핀 및 CubeMX 시작 조건

> 현재 문서는 초기 검토 이력입니다. 현행 핀맵 기준 문서는 `08_STM32G071CBT6_BL0942_권장핀맵_초안.md`를 사용합니다.

작성일: 2026-04-02  
대상 MCU: `STM32F107VCT6`

## 1. 목적

이 문서는 STM32CubeMX로 개발을 시작하기 전에, 회로 설계에서 사실상 고정되거나 우선 예약해야 하는 핀을 정리한 문서다.

## 2. 반드시 반영해야 하는 고정 핀

### 2.1 전원/리셋

- `VDD`, `VSS`: 모두 연결 필수
- `VDDA`, `VSSA`: ADC 사용 여부와 관계없이 권장 연결, ADC 사용 시 특히 필수
- `NRST`: 외부 리셋 회로 연결 권장
- `BOOT0`: 부트 모드 결정용 풀다운 또는 점퍼 구조 필요
- `VBAT`: RTC/백업영역 미사용이면 VDD에 적절히 처리, 사용 시 별도 검토

## 3. 클럭 관련 고정 핀

### 3.1 HSE 사용 시

- `PD0 / OSC_IN`
- `PD1 / OSC_OUT`

이번 프로젝트에서는 HSE 사용을 권장한다.

이유:

- 시스템 클럭 안정성
- UART/RS-485 통신 안정성
- 향후 USB 사용 가능성 확보

참고:

- ST 데이터시트에는 `USB OTG FS는 외부 8 MHz / 14.7456 MHz / 25 MHz HSE가 있어야 동작`한다고 명시되어 있다.

### 3.2 LSE 사용 시

- `PC14 / OSC32_IN`
- `PC15 / OSC32_OUT`

RTC 정확도가 중요하면 예약한다.  
이번 1차 제품에서 RTC가 필수 아니면 일단 옵션으로 두어도 된다.

## 4. 디버그 관련 우선 예약 핀

### 4.1 SWD 권장

최소 예약:

- `PA13 / SWDIO`
- `PA14 / SWCLK`
- `NRST`

### 4.2 JTAG까지 유지할 경우

추가 점유:

- `PA15`
- `PB3`
- `PB4`

권장안:

- 개발 편의를 위해 초기에 `SWD`는 반드시 살린다.
- 핀을 아끼려면 `JTAG 비활성 + SWD 유지`로 간다.

## 5. 이번 프로젝트에서 사실상 예약해야 하는 기능 핀

아래 핀들은 MCU 고정 핀은 아니지만, 회로 설계 단계에서 사실상 예약이 필요하다.

### 5.1 BL0942 통신

BL0942는 `UART 또는 SPI` 사용 가능하다.

권장안:

- 1차는 `UART` 사용

이유:

- 구현 단순
- 디버깅 쉬움
- BL0942가 UART 반이중 프로토콜을 제공

MCU 측 예약:

- UART RX 1핀
- UART TX 1핀

추가 메모:

- BL0942는 UART 사용 시 `SEL=0`으로 설정
- `SCLK_BPS` 핀으로 기본 baud 설정 가능

### 5.2 RS-485 통신

예약 필요:

- UART TX 1핀
- UART RX 1핀
- DE/RE 제어 GPIO 1핀

### 5.3 릴레이/차단기 제어

예약 필요:

- 릴레이 ON/OFF GPIO 1핀
- 필요 시 피드백 입력 GPIO 1핀

### 5.4 상태 표시

예약 권장:

- RUN LED GPIO 1핀
- TRIP/FAULT LED GPIO 1핀

## 6. CubeMX 시작 시 권장 설정

### 6.1 기본

- Debug: `Serial Wire`
- RCC: `HSE Crystal/Ceramic Resonator`
- SYSCLK: HSE + PLL 기반

### 6.2 우선 활성화 주변장치

- GPIO
- USART 1개: BL0942용
- USART 1개: RS-485용
- NVIC
- 필요 시 RTC

### 6.3 초기 핀 전략

1차는 다음 핀만 먼저 고정한다.

- 전원/그라운드
- NRST
- BOOT0
- SWDIO/SWCLK
- HSE
- BL0942 UART
- RS-485 UART
- DE/RE GPIO
- 릴레이 제어 GPIO
- LED GPIO

## 7. BL0942 쪽에서 고정되는 핀

BL0942도 아래 핀은 회로에서 사실상 고정된다.

### 7.1 필수 핀

- `VDD`
- `GND`
- `VP`, `VN`: 전압 입력
- `IP`, `IN`: 전류 입력

### 7.2 통신 모드 결정 핀

- `SEL`: UART/SPI 선택
- `SCLK_BPS`: SPI clock 또는 UART baud 설정
- `RX/SDI`
- `TX/SDO`

권장안:

- UART 사용
- `SEL = 0`

## 8. HW팀에 반드시 전달해야 하는 내용

- HSE 사용 전제인지
- LSE 사용 여부
- SWD 유지
- BL0942는 UART로 연결
- RS-485는 별도 UART 사용
- 릴레이 드라이버 및 상태 피드백 여부
- BOOT0, NRST 회로 노출 여부

## 9. 데이터시트 링크

- STM32F107VC 공식 데이터시트 PDF: https://www.st.com/resource/en/datasheet/stm32f107vc.pdf
- BL0942 공식 데이터시트 PDF: https://www.belling.com.cn/media/file_object/bel_product/BL0942/datasheet/BL0942_V1.1_en.pdf
