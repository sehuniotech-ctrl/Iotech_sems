# STM32F107 + ATM90E26 Bring-up Checklist

## 1. 현재 코드 상태

현재 `smartload_fw_current`는 아래처럼 나뉘어 있습니다.

- 공통 드라이버
  - `src/atm90e26.c`
  - `src/lcu_demo_app.c`
- F107 보드 진입점
  - `target_f107/src/main.c`
  - `target_f107/src/smartload_target_f107.c`
- F107 보드 실제 하드웨어 연결부
  - `target_f107/src/atm90e26_transport_f107_lcu.c`
  - `target_f107/src/board_f107_lcu.c`

## 2. 이미 준비된 내용

- ATM90E26 soft reset
- demo mode startup
- SysStatus read
- 전압 / 전류 / 유효전력 / 무효전력 / 피상전력 / 역률 / 주파수 / 위상각 read 구조
- active/reactive energy 누적 구조
- 1초 주기 polling
- 과전류 / 과전압 / 대기전력 기준 데모 로직

## 3. 아직 구현이 필요한 내용

`target_f107/src/atm90e26_transport_f107_lcu.c`

- `atm90e26_transport_f107_init()`
- `atm90e26_transport_f107_read_reg()`
- `atm90e26_transport_f107_write_reg()`
- `atm90e26_transport_f107_delay_ms()`

즉, STM32F107 보드와 ATM90E26를 실제로 이어주는 transport 레이어가 아직 비어 있습니다.

## 4. 사용자 확인 필요 사항

아래 정보가 확인되어야 F107 transport 구현을 진행할 수 있습니다.

1. ATM90E26 연결 방식
- SPI인지
- UART인지

2. STM32F107 사용 핀
- SCLK
- MISO
- MOSI
- CS
- RESET
- IRQ / ZX / CF1 / CF2 / WarnOut 사용 여부

3. 보드 초기화 방식
- 기존 F107 펌웨어 프로젝트 유무
- CubeMX 프로젝트 유무
- HAL 사용 여부
- delay 함수 사용 방식 (`HAL_Delay` 또는 기존 tick)

4. 디버그 / 로그 확인 방법
- ST-LINK 연결 가능 여부
- UART 로그 포트 존재 여부

## 5. 현재 바로 가능한 검증

실보드 없이도 아래는 확인 가능합니다.

- `target_f107/run_host_demo_reference.py`
  - startup -> poll -> trip 흐름 확인

예상 로그:

- soft reset
- demo mode startup success
- 전압/전류/전력 출력
- 과전류 구간에서 relay off / trip=2

## 6. 실보드 bring-up 순서

1. ST-LINK 연결 확인
2. F107 보드 전원 확인
3. ATM90E26 연결 방식 확인
4. SPI/UART 핀맵 확인
5. `atm90e26_transport_f107_lcu.c` 구현
6. `main.c` 기반 빌드 및 다운로드
7. startup success 확인
8. SysStatus / URMS / IRMS 우선 read 확인
9. 나머지 measurement 확장 확인

## 7. 최소 1차 목표

처음부터 모든 값을 다 읽으려 하지 않고 아래 5개만 우선 확인합니다.

- SysStatus
- URMS
- IRMS
- PMEAN
- FREQ

이 5개가 읽히면 transport가 맞게 붙었다고 볼 수 있습니다.
