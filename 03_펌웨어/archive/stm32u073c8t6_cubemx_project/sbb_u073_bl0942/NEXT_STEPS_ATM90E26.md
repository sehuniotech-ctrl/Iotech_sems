# ATM90E26 기준 다음 작업

## 현재 반영 완료

- `device_app`를 `ATM90E26` 기준으로 변경
- 계측 구조를 `전압 / L전류 / N전류 / 유효전력 / 무효전력 / 피상전력 / 역률 / 주파수 / 위상각` 구조로 변경
- 보호 판단에 `과전류`, `과전압` 비교 함수 추가
- `ATM90E26` driver placeholder 추가
- `CubeMX_추가설정_체크리스트.md`를 ATM90E26 기준으로 변경

## 다음으로 반드시 할 일

1. CubeMX에서 ATM90E26 인터페이스 선택
   - `UART` 또는 `SPI`
2. 해당 peripheral 활성화 후 코드 재생성
3. 실제 핀 반영
   - `USEL`
   - `Reset`
   - 필요 시 `IRQ`, `WarnOut`, `ZX`, `CF1`, `CF2`
4. `atm90e26_transport`에 실제 HAL read/write 함수 연결
5. ATM90E26 레지스터 map 기준 초기화 시퀀스 작성
6. 실측값 scale/calibration 값 반영
7. RS-485 polling 응답 프레임에 실제 계측값 매핑

## 현재 가정

- 인터페이스 미확정 상태라 driver는 transport abstraction 형태로 작성됨
- `atm90e26.c`의 register address와 scale은 placeholder 상태임
- 실제 bring-up 전에는 반드시 HW 인터페이스와 레지스터 맵을 확정해야 함
