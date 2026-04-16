# STM32U073C8T6 CubeMX Project Start

이 폴더는 `STM32U073C8T6 + BL0942` 기준 CubeMX 프로젝트 작업 폴더다.

## 현재 단계

- CubeMX 실행 파일 확인 완료
- 경로: `D:\work\1_SW\2_tool\STM32CubeMX.exe`
- 프로젝트 생성 대상 폴더 준비 완료

## CubeMX에서 먼저 잡을 항목

- MCU: `STM32U073C8T6`
- Debug: `Serial Wire`
- USART1: `BL0942`
- USART2: `RS-485`
- GPIO:
  - Relay control output
  - RS-485 DE/RE output
  - Status LED output
- Timebase:
  - SysTick 또는 TIM 기반 1ms tick

## 권장 생성 순서

1. CubeMX에서 `STM32U073C8T6` 선택
2. 위 인터페이스 기준으로 핀 할당
3. 프로젝트 폴더를 현재 폴더로 지정
4. 코드 생성
5. 생성 후 `stm32u073_bl0942_fw_skeleton` 내용을 프로젝트에 반영

## 다음 단계

- `.ioc` 초안 작성
- 실제 핀맵 확정
- HAL callback과 앱 골격 연결
