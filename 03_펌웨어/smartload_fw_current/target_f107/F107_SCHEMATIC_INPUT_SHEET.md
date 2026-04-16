# F107 회로도 수신 후 바로 채울 입력값

이 문서는 회로도만 받으면 바로 transport 구현으로 넘어갈 수 있도록
확인할 항목만 따로 뽑아놓은 입력 시트입니다.

## 1. ATM90E26 연결 방식

- [ ] SPI
- [ ] UART

## 2. STM32F107 <-> ATM90E26 핀 연결

### SPI일 때

- [ ] SCLK:
- [ ] MISO:
- [ ] MOSI:
- [ ] CS:
- [ ] RESET:
- [ ] IRQ:
- [ ] ZX:
- [ ] CF1:
- [ ] CF2:
- [ ] WarnOut:

### UART일 때

- [ ] TX:
- [ ] RX:
- [ ] RESET:
- [ ] DE/RE:
- [ ] baudrate:
- [ ] parity:
- [ ] stop bits:

## 3. 전원 / 레벨

- [ ] ATM90E26 VDD:
- [ ] STM32F107 IO level:
- [ ] reset active level:

## 4. 펌웨어 프로젝트 정보

- [ ] 기존 STM32F107 프로젝트 존재 여부:
- [ ] CubeMX 프로젝트 존재 여부:
- [ ] HAL 사용 여부:
- [ ] 사용 IDE:
- [ ] ST-LINK 다운로드 가능 여부:

## 5. delay / tick / log

- [ ] delay 함수:
- [ ] system tick 함수:
- [ ] UART 로그 포트:

## 6. 1차 구현 목표

회로도 확인 후 처음에는 아래만 붙이면 됩니다.

- [ ] SoftReset write
- [ ] CalStart write
- [ ] AdjStart write
- [ ] SysStatus read
- [ ] URMS read
- [ ] IRMS read
- [ ] PMEAN read
- [ ] FREQ read
