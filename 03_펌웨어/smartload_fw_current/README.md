# Smart Load Firmware Current

현재 폴더는 `ATM90E26-YU` 계량칩 데모와 최종 제품 이식을 같이 준비하는 기준 코드입니다.

## 1. 구성 원칙

- `main.c`
  - 프로그램 진입점만 담당
- `board_*`
  - MCU/보드 초기화, tick, GPIO, HAL 연결 담당
- `atm90e26.*`
  - 계량칩 공통 드라이버
- `lcu_demo_app.*`
  - 데모용 측정/보호/대기전력 판단 흐름
- `target_f107/*`
  - 현재 LCU 보드 데모 타깃
- `target_l053/*`
  - 최종 STM32L053 제품 보드 타깃

## 2. 공통 코드

- `inc/atm90e26_regs.h`
- `inc/atm90e26_transport.h`
- `inc/atm90e26.h`
- `src/atm90e26.c`
- `inc/lcu_demo_app.h`
- `src/lcu_demo_app.c`

목표:

- ATM90E26 reset / startup
- register read / write
- 전압 / 전류 / 유효전력 / 무효전력 / 피상전력 / 역률 / 주파수 read
- 이후 RS-485 polling, 릴레이 제어 로직으로 확장

## 3. STM32F107 / LCU 데모 타깃

- `target_f107/inc/board_f107_lcu.h`
- `target_f107/src/board_f107_lcu.c`
- `target_f107/inc/atm90e26_transport_f107_lcu.h`
- `target_f107/src/atm90e26_transport_f107_lcu.c`
- `target_f107/inc/smartload_target_f107.h`
- `target_f107/src/smartload_target_f107.c`
- `target_f107/src/main.c`

흐름:

1. `main.c`
2. `board_f107_lcu_init()`
3. `smartload_target_f107_init()`
4. `smartload_target_f107_start()`
5. `while(1)`에서 `smartload_target_f107_process()`

보드 없이 흐름만 확인하려면:

- `target_f107/src/main_host_demo.c`
- `target_f107/build_host_demo_gcc.bat`
- `target_f107/run_host_demo_reference.py`

이 파일은 가짜 register read/write를 사용해서 `startup -> poll -> 보호/대기전력 판단` 순서를 PC에서 확인하는 용도다.

- `build_host_demo_gcc.bat`
  - PC에 `gcc`가 설치돼 있으면 바로 빌드 후 실행
- `run_host_demo_reference.py`
  - 현재 PC에 컴파일러가 없어도 같은 흐름의 예상 로그를 확인

## 4. STM32L053 최종 보드 타깃

- `target_l053/inc/smartload_target_l053.h`
- `target_l053/src/smartload_target_l053.c`
- `target_l053/inc/atm90e26_transport_l053_hal.h`
- `target_l053/src/atm90e26_transport_l053_hal.c`

목표:

- 최종 `STM32L053R8T6 + ATM90E26-YU` 보드가 오면
- CubeMX 생성 코드와 HAL transport만 붙여서
- 공통 ATM90E26 드라이버를 그대로 이식

## 5. 현재 TODO

- LCU 보드에서 ATM90E26 연결 방식 확인
  - SPI인지 UART인지
- F107 transport 구현
  - register read / write
  - delay
- 1채널 계측값 read 성공 확인
