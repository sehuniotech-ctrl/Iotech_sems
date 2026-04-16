# 저전력 MCU 후보 조사

작성일: 2026-04-03  
기준: 단상 지선차단기 / 계량칩 `BL0942` / 개발환경 `STM32CubeMX`

## 1. 조사 목적

기존에 검토하던 `STM32G071CBT6` 대신, 더 저전력 지향 MCU로 변경 가능한지 검토한다.

이번 조사 기준은 아래와 같다.

- `UART 2개 이상`
- `RS-485 통신 가능`
- `SPI 통신 가능`
- `STM32CubeMX 사용 가능`
- 48핀 전후 패키지 선호
- 공급성과 가격이 지나치게 나쁘지 않을 것

참고:

- `RS-485 통신 가능`은 MCU 내부에 RS-485 전용 PHY가 있다는 뜻이 아니라,
- `UART + 외부 RS-485 트랜시버` 조합으로 구현 가능한지를 의미한다.

## 2. 비교 기준 MCU

현재 기준선:

- `STM32G071CBT6`

이 MCU는 저전력 전용 계열은 아니지만,

- 48핀
- USART 4개
- SPI 2개
- CubeMX 지원
- 공급성 양호

라서 비교 기준으로 적합하다.

## 3. 후보군

이번 조사에서 실무적으로 의미 있는 후보는 아래 4개로 본다.

- `STM32U031C8T6`
- `STM32U073C8T6`
- `STM32L071CBT6`
- `STM32L412CBT6`

## 4. 후보 비교표

| MCU | 계열 | 전력 특성 | UART/USART | SPI | 코어/속도 | 메모리 | 패키지 | 가격 참고 | Longevity | 판단 |
|---|---|---|---:|---:|---|---|---|---|---|---|
| `STM32G071CBT6` | G0 | 저전력 전용 아님 | 4 USART + 1 LPUART | 2 | M0+ / 64MHz | 128KB / 36KB | LQFP-48 | LCSC `1k=$1.2843` | 10년 계열 확인 | 기준선 |
| `STM32U031C8T6` | U0 | 매우 유리 | 4 USART + 2 LPUART | 2 | M0+ / 56MHz | 64KB / 12KB | LQFP-48 | LCSC `1k=$1.3581` | `10년`, 시작일 `2026-01-01` 직접 확인 | 저전력 최우선 후보 |
| `STM32U073C8T6` | U0 | 매우 유리 | 다수 USART/LPUART | 2 | M0+ / 56MHz | 최대 256KB / 40KB 계열 | LQFP-48 | 가격 정보 제한적 | `10년`, 시작일 `2026-01-01` 직접 확인 | 성능/저전력 균형 후보 |
| `STM32L071CBT6` | L0 | 유리 | 4 USART + 1 LPUART | 2 | M0+ / 32MHz | 128KB / 20KB | LQFP-48 | LCSC `1k=$1.1965` | `10년`, 시작일 `2025-01-01` 직접 확인 | 안정적 저전력 후보 |
| `STM32L412CBT6` | L4 | 유리 | 3 USART + 1 LPUART | 2 | M4F / 80MHz | 128KB / 40KB | LQFP-48 | LCSC 검색 기준 유통 존재 | exact 가격 확인 필요 | 고성능 저전력 후보 |

## 5. 후보별 상세 검토

### 5.1 STM32U031C8T6

장점:

- ST 공식 페이지 기준 `Run mode 52 uA/MHz`
- `4 USART + 2 LPUART`
- `2 SPI`
- 48핀
- CubeMX 지원
- exact part 기준 `10년 longevity`, 시작일 `2026-01-01`

주의:

- SRAM이 `12KB`로 작다
- 기능이 계속 늘어날 경우 메모리 여유가 부족할 수 있다

이번 프로젝트 적합성:

- `BL0942 + RS-485 + 서비스 UART` 구성 가능
- 저전력 최우선이면 가장 강한 후보

### 5.2 STM32U073C8T6

장점:

- ST 공식 페이지 기준 `Run mode 52 uA/MHz`
- U0 계열이라 매우 저전력
- 메모리와 주변장치가 U031보다 더 여유 있는 편
- exact part 기준 `10년 longevity`, 시작일 `2026-01-01`

주의:

- 유통 가격 정보가 이번 조사에서는 충분히 확보되지 않음
- 실구매 가격은 발주 전 재확인 필요

이번 프로젝트 적합성:

- 저전력과 기능 여유를 같이 보려면 좋은 후보

### 5.3 STM32L071CBT6

장점:

- ST 공식 페이지 기준 `Down to 93 uA/MHz in Run mode`
- `4 USART + 1 LPUART`
- `2 SPI`
- 48핀
- exact part 기준 `10년 longevity`, 시작일 `2025-01-01`
- LCSC 기준 가격도 무난

주의:

- 최대 주파수 `32MHz`
- U0 계열보다 최신 느낌은 덜함

이번 프로젝트 적합성:

- 통신 수는 충분하고 구조도 단순하다
- 저전력 + 안정성 + 가격의 균형이 좋은 편

### 5.4 STM32L412CBT6

장점:

- ST 공식 페이지 기준 `79 uA/MHz run mode (LDO)` / `28 uA/MHz (SMPS mode @3.3V)`
- Cortex-M4F / 80MHz
- `3 USART + 1 LPUART`
- `2 SPI`
- 메모리 여유 좋음

주의:

- 이번 용도에는 성능이 다소 과할 수 있다
- 가격은 U0/L0 계열보다 높을 가능성이 크다

이번 프로젝트 적합성:

- 여유는 많지만 저전력 목적만 보면 과한 편

## 6. 우리 용도 기준 판단

### 6.1 꼭 필요한 조건 체크

| 조건 | U031 | U073 | L071 | L412 |
|---|---|---|---|---|
| UART 2개 이상 | 가능 | 가능 | 가능 | 가능 |
| RS-485 연결 가능 | 가능 | 가능 | 가능 | 가능 |
| SPI 가능 | 가능 | 가능 | 가능 | 가능 |
| CubeMX 가능 | 가능 | 가능 | 가능 | 가능 |
| 48핀 패키지 | 가능 | 가능 | 가능 | 가능 |
| 저전력 장점 뚜렷 | 매우 큼 | 매우 큼 | 큼 | 있음 |

## 7. 추천 순위

### 7.1 저전력 최우선이면

1. `STM32U031C8T6`
2. `STM32U073C8T6`
3. `STM32L071CBT6`

### 7.2 저전력 + 메모리 여유까지 같이 보면

1. `STM32U073C8T6`
2. `STM32L071CBT6`
3. `STM32U031C8T6`

### 7.3 가장 현실적인 추천

현재 프로젝트 기준 개인 추천:

1. `STM32U073C8T6`
2. `STM32L071CBT6`
3. `STM32U031C8T6`

이유:

- `U073`은 저전력 특성이 좋으면서 메모리와 기능 여유도 더 낫다
- `L071`은 가격과 공급성이 비교적 안정적이고, 통신 수가 충분하다
- `U031`은 아주 매력적이지만 SRAM 12KB가 아쉬울 수 있다

## 8. 지금 당장 의사결정용 한줄 정리

- `전력 소모를 가장 우선`: `STM32U031C8T6`
- `저전력 + 실무 균형`: `STM32U073C8T6`
- `안정적이고 무난한 저전력 대안`: `STM32L071CBT6`

## 9. 참고 링크

- STM32G071CB 제품 페이지: https://www.st.com/en/microcontrollers-microprocessors/stm32g071cb.html
- STM32G071CB 데이터시트: https://www.st.com/resource/en/datasheet/stm32g071cb.pdf
- STM32U031C8 제품 페이지: https://www.st.com/en/microcontrollers-microprocessors/stm32u031c8.html
- STM32U031C8 데이터시트: https://www.st.com/resource/en/datasheet/stm32u031c6.pdf
- STM32U073C8 제품 페이지: https://www.st.com/en/microcontrollers-microprocessors/stm32u073c8.html
- STM32U073C8 데이터시트: https://www.st.com/resource/en/datasheet/stm32u073c8.pdf
- STM32L071CB 제품 페이지: https://www.st.com/en/microcontrollers-microprocessors/stm32l071cb.html
- STM32L071CB 데이터시트: https://www.st.com/resource/en/datasheet/stm32l071cb.pdf
- STM32L412CB 제품 페이지: https://www.st.com/en/microcontrollers-microprocessors/stm32l412cb.html
- STM32L412CB 데이터시트: https://www.st.com/resource/en/datasheet/stm32l412cb.pdf
- ST Product Longevity: https://www.st.com/content/st_com/en/support/resources/product-longevity.html

## 10. 메모

- `RS-485`는 MCU 단독 기능이 아니라 `UART + 외부 RS-485 트랜시버`로 구현한다.
- 가격은 시점에 따라 변동 가능하므로 발주 전 재확인이 필요하다.
- 저전력만 보고 고르면 메모리 여유가 부족할 수 있으므로, 펌웨어 구조와 로그 저장 요구까지 같이 봐야 한다.
