# STM32F107 + ATM90E26 SPI 기준 정리

## 1. 기준

현재 STM32F107 LCU 보드용 bring-up은 아래 기준으로 준비한다.

- MCU: `STM32F107`
- Metering IC: `ATM90E26`
- 통신 방식: `SPI`

즉, 회로도 오기 전까지는 `UART`가 아니라 `SPI` 기준으로 transport를 준비한다.

## 2. 왜 SPI 기준으로 보는가

ATM90E26 핀 정의에서 아래 SPI 신호가 이미 정리되어 있다.

- `SDI/URX`
- `SDO/UTX`
- `SCLK`
- `CS`

최종 제품 핀 정리 방향도 ATM90E26를 SPI로 붙이는 구조에 가깝다.

따라서 F107 LCU bring-up도 우선은 SPI 기준으로 준비하는 것이 가장 합리적이다.

## 3. SPI 기준으로 꼭 필요한 핀

F107에서 ATM90E26를 읽으려면 최소 아래 핀 정보가 필요하다.

- `SCLK`
- `MISO`
- `MOSI`
- `CS`
- `RESET`

추가 확인 가능 핀:

- `IRQ`
- `ZX`
- `CF1`
- `CF2`
- `WarnOut`

1차 bring-up에서는 보조 핀보다 아래 5개가 우선이다.

- `SCLK`
- `MISO`
- `MOSI`
- `CS`
- `RESET`

## 4. 현재 코드 구조

### 엔트리

- `target_f107/src/main.c`

흐름:

1. `board_f107_lcu_init()`
2. `smartload_target_f107_init()`
3. `smartload_target_f107_start()`
4. `while(1)`에서 `smartload_target_f107_process()`

### 보드 연결 레이어

- `target_f107/src/smartload_target_f107.c`

역할:

- 공통 app 로직과 F107 transport 연결

### 공통 app

- `src/lcu_demo_app.c`

역할:

- startup
- polling
- snapshot 저장
- 간단한 보호 로직

### 공통 ATM90E26 드라이버

- `src/atm90e26.c`

역할:

- soft reset
- demo mode startup
- SysStatus / URMS / IRMS / PMEAN / FREQ 등 read
- raw 값을 engineering value로 변환

### 실제 미구현 지점

- `target_f107/src/atm90e26_transport_f107_lcu.c`

여기가 실제 SPI transaction을 넣어야 하는 파일이다.

## 5. 실제로 구현해야 하는 함수

### `atm90e26_transport_f107_init()`

여기서 정리할 것:

- SPI 인스턴스 바인딩
- CS GPIO 설정
- RESET GPIO 설정
- 필요 시 초기 reset sequence

### `atm90e26_transport_f107_write_reg(uint16_t reg, uint16_t value)`

여기서 해야 할 것:

- CS Low
- ATM90E26 write frame 전송
- CS High

1차 목표 write:

- `SOFT_RESET`
- `CAL_START`
- `ADJ_START`

### `atm90e26_transport_f107_read_reg(uint16_t reg, uint16_t *value)`

여기서 해야 할 것:

- CS Low
- ATM90E26 read frame 전송
- 16bit 응답 수신
- CS High
- `*value` 저장

1차 목표 read:

- `SYS_STATUS`
- `URMS`
- `IRMS`
- `PMEAN`
- `FREQ`

### `atm90e26_transport_f107_delay_ms(uint32_t delay_ms)`

여기서 해야 할 것:

- `HAL_Delay()` 또는 기존 board delay 연결

## 6. bring-up 순서

회로도 오면 아래 순서로 진행한다.

1. SPI 핀맵 확인
2. `CS`, `RESET` 핀 확인
3. SPI handle 결정
4. `atm90e26_transport_f107_init()` 구현
5. `write_reg()` 구현
6. `read_reg()` 구현
7. `delay_ms()` 구현
8. `smartload_target_f107_start()` 성공 확인
9. `lcu_demo_app_poll()`에서 측정값 갱신 확인

## 7. 1차 성공 조건

아래가 되면 SPI bring-up은 성공으로 본다.

1. `SOFT_RESET` write 성공
2. `CAL_START` write 성공
3. `ADJ_START` write 성공
4. `SYS_STATUS` read 성공
5. `URMS` 값 읽힘
6. `IRMS` 값 읽힘

그 다음에

- `PMEAN`
- `FREQ`
- `QMEAN`
- `POWERF`
- `Energy`

순으로 확장한다.

## 8. 지금 할 수 있는 것과 없는 것

### 지금 가능한 것

- SPI 기준 구조 정리
- transport 함수 역할 정리
- 구현 순서 고정
- 회로도 입력 시트 준비

### 회로도 있어야 가능한 것

- 실제 SPI 핀 확정
- 실제 CS/RESET GPIO 확정
- HAL handle 확정
- 최종 write/read frame 실제 적용

## 9. 결론

현재 STM32F107 bring-up은 `ATM90E26 SPI` 기준으로 준비하면 된다.

회로도 오기 전까지는:

- `SPI 기준 transport`를 전제로 코드 이해
- `read_reg/write_reg/init/delay` 역할 이해
- `main -> target -> app -> driver -> transport` 흐름 이해

까지 해두면 된다.
