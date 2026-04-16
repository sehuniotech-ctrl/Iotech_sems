# STM32U073C8T6 ATM90E26 Firmware Skeleton

이 폴더는 `STM32U073C8T6 + ATM90E26-YU + RS-485` 기준의 펌웨어 골격이다.

## 특징

- `UART/SPI` 둘 다 수용할 수 있도록 transport abstraction 사용
- Smart Load 기준 계측/보호/복구/통신 구조 포함
- HAL 구체 구현 전 단계의 아키텍처 정리용

## 핵심 파일

- `atm90e26.h/.c`
- `atm90e26_transport.h`
- `device_app.h/.c`
- `relay_control.h/.c`
- `protection.h/.c`
- `rs485_link.h/.c`

## 다음 단계

1. CubeMX peripheral 확정
2. `atm90e26_transport`에 HAL 연동
3. 레지스터 map 기반 실측값 파싱
