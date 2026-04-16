# STM32F107 소스 읽기 가이드

회로도 오기 전까지는 이 문서 순서대로 따라가면 전체 흐름이 가장 빨리 잡힙니다.

## 1. 가장 먼저 볼 파일

### [main.c](D:/work/15_지선차단기/03_펌웨어/smartload_fw_current/smartload_f107_demo_project/src/main.c)

프로그램 진입점입니다.

여기서 보는 핵심 순서:

1. `board_f107_lcu_init(&board)`
2. `smartload_target_f107_init(&target)`
3. `smartload_target_f107_start(&target)`
4. `while(1)` 안에서 `smartload_target_f107_process(...)`

즉, `main.c`는 "프로그램 구조"만 잡고 실제 계량칩 로직은 아래 레이어로 넘깁니다.

---

## 2. 보드 타깃 레이어

### [smartload_target_f107.c](D:/work/15_지선차단기/03_펌웨어/smartload_fw_current/smartload_f107_demo_project/src/smartload_target_f107.c)

이 파일의 역할은:

- 공통 ATM90E26 앱 로직
- STM32F107 보드 전용 transport

를 연결하는 것입니다.

핵심 함수:

- `smartload_target_f107_init()`
  - transport 함수 포인터를 공통 app에 연결
- `smartload_target_f107_start()`
  - 계량칩 startup 시도
- `smartload_target_f107_process()`
  - polling 실행

이 파일은 "실제 계량칩 알고리즘"보다는 "연결 레이어"라고 보면 됩니다.

---

## 3. 공통 앱 로직

### [lcu_demo_app.c](D:/work/15_지선차단기/03_펌웨어/smartload_fw_current/smartload_f107_demo_project/common_src/lcu_demo_app.c)

이 파일은 bring-up 단계에서 실제로 가장 중요한 파일 중 하나입니다.

여기서 하는 일:

- 계량칩 startup 호출
- polling 주기 관리
- 상태값 읽기
- 측정값 읽기
- snapshot 저장
- 간단한 보호 로직 적용

핵심 함수:

- `lcu_demo_app_init()`
  - 공통 app 상태 초기화
- `lcu_demo_app_start_meter()`
  - demo mode startup
- `lcu_demo_app_poll()`
  - 주기적으로 값 읽고 relay/trip 로직 적용

bring-up 때는 `lcu_demo_app_poll()` 안 흐름을 가장 잘 이해하면 됩니다.

---

## 4. 공통 ATM90E26 드라이버

### [atm90e26.c](D:/work/15_지선차단기/03_펌웨어/smartload_fw_current/smartload_f107_demo_project/common_src/atm90e26.c)

이 파일은 계량칩 레지스터를 읽고 실제 값으로 바꾸는 공통 드라이버입니다.

여기서 하는 일:

- soft reset
- demo mode startup
- SysStatus read
- URMS / IRMS / PMEAN / QMEAN / SMEAN / POWERF / FREQ / PANGLE read
- raw 레지스터 값을 engineering unit으로 변환

핵심 함수:

- `atm90e26_init()`
- `atm90e26_soft_reset()`
- `atm90e26_startup_demo_mode()`
- `atm90e26_read_sys_status()`
- `atm90e26_read_all_measurements()`

이 파일은 MCU에 독립적입니다.
즉, SPI/UART/HAL을 모르고 함수 포인터로만 동작합니다.

---

## 5. 실제 하드웨어 미구현 지점

### [atm90e26_transport_f107_lcu.c](D:/work/15_지선차단기/03_펌웨어/smartload_fw_current/smartload_f107_demo_project/src/atm90e26_transport_f107_lcu.c)

현재 이 파일이 비어 있어서 실제 계량칩과는 아직 통신하지 못합니다.

여기서 나중에 구현할 것:

- `atm90e26_transport_f107_init()`
- `atm90e26_transport_f107_read_reg()`
- `atm90e26_transport_f107_write_reg()`
- `atm90e26_transport_f107_delay_ms()`

즉, 회로도가 오면 제일 먼저 손댈 파일입니다.

---

## 6. 읽는 순서 추천

가장 추천하는 읽기 순서:

1. `main.c`
2. `smartload_target_f107.c`
3. `lcu_demo_app.c`
4. `atm90e26.c`
5. `atm90e26_transport_f107_lcu.c`

이 순서대로 보면:

- 프로그램 진입
- 보드 연결
- 앱 로직
- 계량칩 드라이버
- 실제 하드웨어 접점

순으로 이해할 수 있습니다.

---

## 7. 1차 bring-up에서 실제 확인 목표

회로도 오면 처음부터 전부 보지 말고 아래 5개만 확인하면 됩니다.

1. `SoftReset write`
2. `CalStart write`
3. `AdjStart write`
4. `SysStatus read`
5. `URMS / IRMS / PMEAN / FREQ read`

이게 되면 transport가 붙었다고 볼 수 있습니다.

---

## 8. 다음에 볼 문서

- [F107_ATM90E26_BRINGUP_CHECKLIST.md](D:/work/15_지선차단기/03_펌웨어/smartload_fw_current/target_f107/F107_ATM90E26_BRINGUP_CHECKLIST.md)
- [F107_SCHEMATIC_INPUT_SHEET.md](D:/work/15_지선차단기/03_펌웨어/smartload_fw_current/target_f107/F107_SCHEMATIC_INPUT_SHEET.md)

위 두 문서는 회로도 받았을 때 바로 구현으로 넘어가기 위한 준비 문서입니다.
