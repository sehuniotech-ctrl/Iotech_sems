# STM32G071CBT6 CubeMX 펌웨어 착수 준비

작성일: 2026-04-03

## 1. 기준

- MCU: `STM32G071CBT6`
- 계량칩: `BL0942`
- 통신: `RS-485`

## 2. 착수 순서

1. 회로도 확인
2. 최종 핀맵 정리
3. CubeMX IOC 생성
4. 클럭 설정
5. USART2/USART3/GPIO 초기화
6. BL0942 드라이버 작성
7. RS-485 드라이버 작성
8. 계측값 변환
9. 과전류 차단 로직
10. 이벤트 전송

## 3. CubeMX 우선 설정

- Debug: SWD
- RCC: HSE 사용 권장
- USART2: BL0942
- USART3: RS-485
- GPIO: Relay, LED, DE/RE

## 4. 1차 펌웨어 목표

- BL0942 값 정상 수신
- RS-485 전송
- 과전류 차단
- 상태 표시
