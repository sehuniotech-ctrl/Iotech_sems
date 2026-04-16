# 저전력 MCU 후보 조사

작성일: 2026-04-03  
기준: 단상 지선차단기 / 계량칩 `BL0942` / 조건 `UART 2개 이상`, `RS-485 가능`, `SPI 가능`

## 1. 조사 목적

STM32 계열에 한정하지 않고, 저전력 MCU 후보를 다시 조사한다.

이번 조사 기준:

- 저전력 특성이 있을 것
- UART/USART 2개 이상일 것
- SPI 통신이 가능할 것
- 외부 RS-485 트랜시버와 연결 가능한 UART 구조일 것
- 단상 지선차단기 1차 제품에 적용 가능한 수준일 것

참고:

- `RS-485 가능`은 MCU 내부에 RS-485 PHY가 있다는 의미가 아니라,
- `UART + 외부 RS-485 트랜시버` 조합으로 구현 가능하다는 의미다.

## 2. 전제

현재 우리 구조는 `BL0942 + MCU + RS-485`다.

따라서 MCU는 아래 역할을 수행하면 된다.

- BL0942 통신
- RS-485 통신
- 상태/이벤트 처리
- 차단 제어
- 기본 연산

즉, 초고성능 MCU는 필요 없고, `저전력 + 통신 수 + 개발 편의성`이 핵심이다.

## 3. 후보군

실무적으로 의미 있는 후보는 아래와 같다.

### STM 계열

- `STM32U031C8T6`
- `STM32U073C8T6`
- `STM32L071CBT6`

### 비-STM 계열

- `NXP K32L2B31`
- `Renesas RA2L1`
- `TI MSPM0G3507`
- `Silicon Labs EFM32TG11`

## 4. 후보 비교표

| MCU | 제조사 | 저전력 특성 | UART/USART | SPI | 코어/속도 | 개발환경 | 우리 용도 적합성 | 비고 |
|---|---|---|---:|---:|---|---|---|---|
| `STM32U031C8T6` | ST | 매우 좋음 | 4 USART + 2 LPUART | 2 | Cortex-M0+ / 56MHz | CubeMX | 높음 | SRAM 12KB |
| `STM32U073C8T6` | ST | 매우 좋음 | 다수 USART/LPUART | 2 | Cortex-M0+ / 56MHz | CubeMX | 매우 높음 | 저전력/여유 균형 |
| `STM32L071CBT6` | ST | 좋음 | 4 USART + 1 LPUART | 2 | Cortex-M0+ / 32MHz | CubeMX | 높음 | 안정적 |
| `K32L2B31` | NXP | 좋음 | 최대 3 LPUART | 최대 3 SPI | Cortex-M0+ 계열 | MCUXpresso | 높음 | NXP 저전력 대표 계열 |
| `RA2L1` | Renesas | 매우 좋음 | SCI 다수 | SCI 기반 SPI + SPI | Cortex-M23 / 48MHz | e2 studio / FSP | 높음 | 1.6V~5.5V, longevity 우수 |
| `MSPM0G3507` | TI | 좋음 | 4 UART | 2 | Cortex-M0+ / 80MHz | Code Composer / SysConfig | 높음 | 통신 수 매우 좋음 |
| `EFM32TG11` | Silicon Labs | 매우 좋음 | USART 다수 | USART 기반 SPI | Cortex-M4 / 48MHz | Simplicity Studio | 보통 이상 | 일부 파트 NRND 주의 |

## 5. 후보별 상세 검토

### 5.1 STM32U031C8T6

장점:

- ST 공식 데이터시트 기준 `Run mode 52 uA/MHz`
- `4 USART + 2 LPUART`
- `2 SPI`
- CubeMX 사용 가능

주의:

- SRAM `12KB`는 조금 빠듯할 수 있다
- 로그/버퍼/향후 기능 확장 시 여유가 적을 수 있다

판단:

- 저전력 최우선이면 매우 매력적
- 메모리 여유는 아쉬움

### 5.2 STM32U073C8T6

장점:

- U0 계열 저전력 특성 우수
- U031보다 기능/메모리 여유가 더 낫다
- CubeMX 사용 가능

주의:

- 가격과 유통 정보는 발주 전 재확인 필요

판단:

- STM 계열 저전력 MCU 중 가장 균형이 좋다

### 5.3 STM32L071CBT6

장점:

- 저전력 계열로 안정적
- USART 수 충분
- SPI 가능
- CubeMX 사용 가능

주의:

- 동작 속도는 최신 계열보다 낮다

판단:

- 무난하고 안정적인 저전력 대안

### 5.4 NXP K32L2B31

공식 페이지 기준 특징:

- K32 L2 계열은 `Ultra Low-Power MCU`
- `up to three LPUART`
- `up to three SPI`

장점:

- 통신 수가 충분하다
- NXP 저전력 계열이라 산업용 적용성이 좋다
- MCUXpresso 생태계가 안정적이다

주의:

- STM32CubeMX는 사용할 수 없다
- 팀이 ST 위주에 익숙하면 전환 비용이 있다

판단:

- 비-ST 계열 중 상당히 현실적인 후보

### 5.5 Renesas RA2L1

공식 페이지 기준 특징:

- `48MHz Arm Cortex-M23 ultra-low power`
- `1.6V ~ 5.5V`
- `SCI (UART, Simple SPI, Simple I2C)`
- Product Longevity `2037` 표기

장점:

- 초저전력 계열
- 산업용 관점에서 수명 보장이 명확하다
- 전압 범위가 넓다

주의:

- CubeMX 대신 Renesas FSP/e2 studio 사용
- 팀 적응 필요

판단:

- 저전력성과 longevity 관점에서 매우 좋은 비-ST 후보

### 5.6 TI MSPM0G3507

공식 페이지 기준 특징:

- `4 UART`
- `2 SPI`
- `Run: 101 uA/MHz`
- `Standby: 1.5 uA`

장점:

- 통신 수가 많다
- 메모리도 무난하다
- TI 생태계와 자료가 좋다

주의:

- 전력 수치만 보면 STM U0/L0보다 아주 유리하다고 보긴 어렵다
- CubeMX는 사용 불가

판단:

- 통신 위주 + 저전력 실무형 후보

### 5.7 Silicon Labs EFM32TG11

공식 페이지 기준 특징:

- ultra-low-power 계열
- `UART / USART / SPI` 지원

장점:

- 저전력 특성이 좋다
- EFM32는 저전력 MCU로 유명하다

주의:

- 일부 파트는 `NRND` 표기가 보인다
- 파트 선정 시 lifecycle 재확인 필요
- 툴체인 전환 필요

판단:

- 성능은 괜찮지만 공급성 확인이 더 필요하다

## 6. 우리 프로젝트 기준 추천

### 6.1 STM까지 포함한 전체 추천

1. `STM32U073C8T6`
2. `Renesas RA2L1`
3. `STM32L071CBT6`
4. `NXP K32L2B31`
5. `TI MSPM0G3507`

### 6.2 비-ST만 따로 보면

1. `Renesas RA2L1`
2. `NXP K32L2B31`
3. `TI MSPM0G3507`
4. `EFM32TG11`

## 7. 추천 이유

### 7.1 가장 추천

- `STM32U073C8T6`

이유:

- 저전력 특성 우수
- UART/SPI 여유 충분
- ST 생태계 유지 가능
- 기존 STM 흐름을 크게 바꾸지 않아도 됨

### 7.2 비-ST에서 가장 추천

- `Renesas RA2L1`

이유:

- 저전력 특성이 좋다
- longevity가 비교적 명확하다
- 산업용 적용 관점에서 안정적이다

### 7.3 가장 무난한 비-ST

- `NXP K32L2B31`

이유:

- 저전력 MCU로 널리 알려진 계열
- UART/SPI 수가 충분
- 구조적으로 우리 용도에 잘 맞는다

## 8. 중요한 판단 포인트

STM이 아니어도 괜찮다면, 기술적인 문제보다 개발환경 변화가 더 큰 이슈다.

즉 실제 선택 기준은 아래 2개다.

- 정말 저전력이 최우선인가
- 아니면 팀의 기존 개발환경과 개발속도가 더 중요한가

정리하면:

- `개발속도 + 저전력`: STM32U073C8T6
- `비-ST 허용 + 저전력/수명`: RA2L1
- `비-ST 허용 + 무난한 실무형`: K32L2B31

## 9. 참고 링크

- STM32U031C8 데이터시트: https://www.st.com/resource/en/datasheet/stm32u031c6.pdf
- STM32U073C8 데이터시트: https://www.st.com/resource/en/datasheet/stm32u073c8.pdf
- STM32L071CB 데이터시트: https://www.st.com/resource/en/datasheet/stm32l071cb.pdf
- NXP K32 L2 제품 페이지: https://www.nxp.com/products/K32-L2
- Renesas RA2L1 제품 페이지: https://www.renesas.com/products/microcontrollers-microprocessors/ra-cortex-m-mcus/ra2l1-48mhz-arm-cortex-m23-ultra-low-power-general-purpose-microcontroller
- TI MSPM0G3507 제품 페이지: https://www.ti.com/product/MSPM0G3507
- TI MSPM0G3507 데이터시트: https://www.ti.com/lit/ds/symlink/mspm0g3507.pdf
- Silicon Labs EFM32TG11 데이터시트: https://www.silabs.com/documents/public/data-sheets/efm32tg11-datasheet.pdf

## 10. 메모

- 비-ST 계열로 가면 `STM32CubeMX`는 사용할 수 없다.
- 대신 각 벤더별 개발환경을 써야 한다.
  - NXP: MCUXpresso
  - Renesas: e2 studio / FSP
  - TI: CCS / SysConfig
  - Silicon Labs: Simplicity Studio
