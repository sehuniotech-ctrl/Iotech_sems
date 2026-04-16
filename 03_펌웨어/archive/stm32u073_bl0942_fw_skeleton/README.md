# STM32U073C8T6 BL0942 Firmware Skeleton

이 폴더는 `STM32U073C8T6 + BL0942 + RS-485` 기준의 펌웨어 골격이다.

## 목적

- CubeMX 프로젝트 생성 전/후에 공통 구조를 먼저 잡기 위함
- BL0942 계측, 릴레이 제어, 보호 차단, 대기전력 복구 흐름을 분리하기 위함

## 구성

- `inc/app_types.h`
- `inc/app_config.h`
- `inc/bl0942.h`
- `inc/relay_control.h`
- `inc/rs485_link.h`
- `inc/protection.h`
- `inc/standby_recovery.h`
- `inc/device_app.h`
- `src/bl0942.c`
- `src/relay_control.c`
- `src/rs485_link.c`
- `src/protection.c`
- `src/standby_recovery.c`
- `src/device_app.c`

## CubeMX 연동 방법

1. CubeMX로 `STM32U073C8T6` 프로젝트 생성
2. `Core/Inc`, `Core/Src` 또는 별도 `App` 폴더에 이 파일들을 복사
3. `MX_USARTx_UART_Init()`와 GPIO 핀 정의를 실제 회로에 맞게 연결
4. `main.c`의 `while (1)` 안에서 `device_app_process()` 호출
5. UART Rx callback 또는 DMA 수신 완료 callback에서 각 링크 입력 함수 연결

## 현재 가정

- BL0942는 UART로 수신한다.
- RS-485는 UART + DE/RE GPIO 제어 구조다.
- 대기전력 차단 후에도 MCU와 통신은 유지된다.
