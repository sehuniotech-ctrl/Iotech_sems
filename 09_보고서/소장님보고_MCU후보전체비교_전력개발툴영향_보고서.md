# MCU 후보 전체 비교 및 전력/개발툴 영향 보고서

작성일: 2026-04-03  
작성 목적: 기존 검토 MCU와 저전력 MCU를 포함한 전체 후보를 비교하고, 전력/개발툴/개발 난이도 관점에서 선정 방향을 정리

## 1. 보고 목적

기존에 검토했던 MCU 후보와 새롭게 검토한 저전력 MCU 후보를 한 표에서 비교하여,  
전력 특성, 개발툴, 무료 여부, CubeMX 사용 가능 여부, 개발 난이도까지 포함한 종합 판단 자료를 만들고자 합니다.

이번 비교는 아래 관점으로 정리했습니다.

- 기능 적합성
- 전력 특성
- `STM32F107 = 100` 기준 상대지표
- 개발툴
- 개발툴 무료 여부
- CubeMX 사용 가능 여부
- 개발 난이도

## 2. 비교 기준

전력 비교는 `Run mode current (μA/MHz)` 기준으로 상대 비교했습니다.

기준 MCU:

- `STM32F107`
- 전력 지표 약 `393 μA/MHz`
- 이 값을 상대지표 `100`으로 설정

계산식:

- `상대지표 = (비교 MCU의 μA/MHz / 393) × 100`

해석:

- 숫자가 낮을수록 더 저전력
- 실제 시스템 소비전력은 외부 회로, 클럭, 통신 사용량, 슬립 전략에 따라 달라질 수 있음

## 3. 비교 대상 MCU

### 3.1 기존 검토 MCU

- `STM32F107VCT6`
- `STM32G071CBT6`
- `STM32F103C8T6`
- `STM32C071CBT6`
- `STM32G0B1CCT6`
- `STM32F303CCT6`

### 3.2 저전력 중심 추가 MCU

- `STM32U031C8T6`
- `STM32U073C8T6`
- `STM32L071CBT6`
- `STM32L412CBT6`
- `Renesas RA2L1`
- `NXP K32L2B31`
- `TI MSPM0G3507`
- `Silicon Labs EFM32TG11`

## 4. 전체 후보 비교표

| MCU | 제조사 | 전력 지표 | STM32F107=100 기준 | 개발툴 | 무료 여부 | CubeMX | 개발 난이도 | 종합 해석 |
|---|---|---:|---:|---|---|---|---|---|
| `STM32F107VCT6` | ST | `393 μA/MHz` | `100` | CubeMX + CubeIDE | 무료 | 가능 | 기준선 | 현재 비교 기준 |
| `STM32G071CBT6` | ST | `약 110 μA/MHz (예상)` | `28.0` | CubeMX + CubeIDE | 무료 | 가능 | 낮음 | 통신/보드 크기/개발 편의 균형 좋음 |
| `STM32F103C8T6` | ST | `약 400 μA/MHz (예상)` | `101.8` | CubeMX + CubeIDE | 무료 | 가능 | 낮음 | 저가형 실무 대안 |
| `STM32C071CBT6` | ST | `약 120 μA/MHz (예상)` | `30.5` | CubeMX + CubeIDE | 무료 | 가능 | 낮음 | 최소 구성형 후보 |
| `STM32G0B1CCT6` | ST | `약 130 μA/MHz (예상)` | `33.1` | CubeMX + CubeIDE | 무료 | 가능 | 낮음 | 확장형 후보 |
| `STM32F303CCT6` | ST | `약 220 μA/MHz (예상)` | `56.0` | CubeMX + CubeIDE | 무료 | 가능 | 낮음 | 성능형, 이번엔 과한 편 |
| `STM32U031C8T6` | ST | `52 μA/MHz` | `13.2` | CubeMX + CubeIDE | 무료 | 가능 | 낮음 | 저전력 최우선 후보 |
| `STM32U073C8T6` | ST | `52 μA/MHz` | `13.2` | CubeMX + CubeIDE | 무료 | 가능 | 낮음 | 저전력 + 실무 균형 최고 |
| `STM32L071CBT6` | ST | `93 μA/MHz` | `23.7` | CubeMX + CubeIDE | 무료 | 가능 | 낮음 | 안정적인 저전력 대안 |
| `STM32L412CBT6` | ST | `79 μA/MHz` | `20.1` | CubeMX + CubeIDE | 무료 | 가능 | 낮음 | 저전력 + 성능 여유 |
| `Renesas RA2L1` | Renesas | `64 μA/MHz` | `16.3` | e² studio + FSP | 무료 | 불가 | 보통 | 비-ST 중 가장 현실적 |
| `NXP K32L2B31` | NXP | `약 70 μA/MHz (예상)` | `17.8` | MCUXpresso IDE | 무료 | 불가 | 보통 | 비-ST 무난한 실무형 |
| `TI MSPM0G3507` | TI | `101 μA/MHz` | `25.7` | CCS + SysConfig | 무료 | 불가 | 보통~약간 높음 | 통신 수는 좋음 |
| `Silicon Labs EFM32TG11` | Silicon Labs | `37 μA/MHz` 또는 `21 μA/MHz` | `9.4` 또는 `5.3` | Simplicity Studio | 무료 | 불가 | 높음 | 전력은 강하나 적응비용 큼 |

## 5. 표 해석

### 5.1 기존 후보군 해석

#### STM32F107VCT6

- 기준선 MCU
- 기능은 충분하지만 전력 측면에서는 매우 불리
- 100핀이라 보드가 크다

#### STM32G071CBT6

- 저전력 전용 MCU는 아니지만, 실무적으로 가장 균형이 좋았던 후보
- USART 4개, 48핀, CubeMX 사용 가능
- 데이터시트에서 `Low-power run mode` 수치가 확인되며, 이를 기준으로 보면 대략 `110 μA/MHz` 수준으로 추정 가능
- U계열보다 불리하지만 개발 편의성과 구조 균형은 우수

#### STM32F103C8T6

- 저가형 실무 대안
- 개발은 쉬움
- 전력 관점에서는 강점이 아님
- F1/F107 계열 특성을 감안하면 `STM32F107`과 거의 비슷한 수준으로 보는 것이 안전

#### STM32C071CBT6

- 경량형 대안
- USART 2개라 최소 조건 충족
- 여유가 적음
- C0 계열 특성상 `G071`보다 약간 불리한 수준으로 추정

#### STM32G0B1CCT6

- 기능 여유는 좋음
- 이번 용도에는 다소 과함
- G0 계열이지만 메모리/주변장치가 늘어난 만큼 `G071`보다 약간 높은 수준으로 추정

#### STM32F303CCT6

- 성능은 좋지만 BL0942를 별도 계량칩으로 쓰는 구조에서는 장점이 크지 않음
- M4 + 아날로그 기능이 많아 저전력 관점에서는 중간 이하로 추정

### 5.2 저전력 후보 해석

#### STM32U031C8T6 / STM32U073C8T6

- 전력은 `STM32F107 대비 약 13% 수준`
- CubeMX를 계속 사용할 수 있다는 점이 가장 큰 장점
- `U073`은 `U031`보다 여유가 더 있어 실무적

#### STM32L071CBT6 / STM32L412CBT6

- 전력은 `STM32F107 대비 약 20~24% 수준`
- 저전력 계열이면서 개발 방식이 ST 그대로 유지됨

#### Renesas RA2L1

- 전력은 `STM32F107 대비 약 16% 수준`
- 비-ST 중에서는 가장 균형이 좋음
- 다만 CubeMX는 사용 불가

#### NXP K32L2B31

- 저전력 계열로 알려져 있고 UART/SPI 수가 충분
- 비-ST 실무형 대안으로 괜찮음
- exact run current 수치는 이번 조사에서 동일 기준으로 직접 확보하지 못했으나, 계열 특성상 `약 70 μA/MHz` 수준으로 추정

#### TI MSPM0G3507

- 전력은 `STM32F107 대비 약 26% 수준`
- 통신 수가 좋고 툴도 무료
- CubeMX 대비 개발 흐름은 달라짐

#### EFM32TG11

- 전력 수치상으로는 가장 강한 편
- 다만 툴과 생태계 적응 비용이 높고 일부 lifecycle 확인이 필요함

## 6. 개발툴 비교

| 제조사 | 대표 후보 | 개발툴 | 무료 여부 | 특징 |
|---|---|---|---|---|
| ST | STM32F107, G071, U073 등 | `STM32CubeMX + STM32CubeIDE` | 무료 | 현재 흐름 유지 가능 |
| Renesas | RA2L1 | `e² studio + FSP` | 무료 | 설정기와 HAL 구조가 잘 정리됨 |
| NXP | K32L2B31 | `MCUXpresso IDE` | 무료 | SDK 중심, 실무형 |
| TI | MSPM0G3507 | `Code Composer Studio + SysConfig` | 무료 | TI SDK/DriverLib 구조 |
| Silicon Labs | EFM32TG11 | `Simplicity Studio` | 무료 | 저전력 강점, 툴 적응 필요 |

## 7. CubeMX 대비 바뀌는 점

### 7.1 ST 계열 유지 시

- CubeMX 사용 가능
- 핀 설정, 클럭 설정, UART/SPI 설정 방식 유지
- 기존 STM32 개발 흐름 유지
- AI 활용도 가장 쉬움

### 7.2 비-ST 계열 변경 시

- CubeMX 사용 불가
- 각 벤더 전용 툴로 프로젝트 생성
- 핀맵/클럭/드라이버 구조가 달라짐
- 예제와 SDK를 해당 벤더 기준으로 다시 익혀야 함

## 8. 개발 난이도 비교

| 후보군 | 개발 난이도 | 이유 |
|---|---|---|
| ST 기존 계열 (`F107`, `G071`, `F103`, `C071`, `G0B1`, `F303`) | 낮음 | CubeMX 유지 가능 |
| ST 저전력 계열 (`U031`, `U073`, `L071`, `L412`) | 낮음 | CubeMX 유지 + 전력만 개선 |
| Renesas RA2L1 | 보통 | FSP/e² studio 적응 필요 |
| NXP K32L2B31 | 보통 | SDK 중심 개발 적응 필요 |
| TI MSPM0G3507 | 보통~약간 높음 | SysConfig/DriverLib 적응 필요 |
| Silicon Labs EFM32TG11 | 높음 | 툴체인/생태계 적응 부담 |

## 9. AI 사용 관점

### 9.1 ST 계열

- AI 활용이 가장 쉬움
- CubeMX 기반 코드 구조와 HAL 예제가 많음
- STM32 관련 학습 데이터가 풍부함

### 9.2 비-ST 계열

- AI 활용은 가능하지만 `벤더 SDK 기준`으로 요청해야 함
- `CubeMX처럼 해줘`라고 요청하면 오히려 혼선 가능

실무 권장 방식:

- ST: `CubeMX + HAL 기준`
- Renesas: `FSP 기준`
- NXP: `MCUXpresso SDK 기준`
- TI: `SysConfig + DriverLib 기준`

## 10. 결론

### 10.1 기존 후보 포함 전체 관점

- `STM32F107`은 기준선으로는 의미가 있으나, 전력 측면에서 가장 불리
- `STM32G071`은 여전히 실무 균형이 좋은 후보
- `STM32F103`은 저가형 대안

### 10.2 저전력까지 포함한 전체 관점

- 전력과 개발 편의성을 같이 보면 `STM32U073`이 가장 유리
- 비-ST까지 허용하면 `Renesas RA2L1`이 가장 현실적인 비-ST 대안

### 10.3 최종 추천

#### 1안

- `STM32U073`

이유:

- 전력 `STM32F107 대비 약 13.2`
- CubeMX 유지 가능
- 개발 난이도 증가 거의 없음

#### 2안

- `STM32G071`

이유:

- 전력 최적화보다 실무 개발 안정성과 통신 여유가 중요하면 여전히 유효
- 기존 검토 흐름과 연결이 좋음

#### 3안

- `Renesas RA2L1`

이유:

- 비-ST 중 가장 현실적인 저전력 대안
- 전력 `STM32F107 대비 약 16.3`
- 다만 개발환경 전환 필요

## 11. 한줄 요약

- `전력 + 개발편의성` 기준 최우선: `STM32U073`
- `실무 안정성` 기준 유지 후보: `STM32G071`
- `비-ST 저전력 대안`: `Renesas RA2L1`

## 12. 참고 링크

- STM32F107VC 데이터시트: https://www.st.com/resource/en/datasheet/stm32f107vc.pdf
- STM32G071CB 데이터시트: https://www.st.com/resource/en/datasheet/stm32g071cb.pdf
- STM32U073 데이터시트: https://www.st.com/resource/en/datasheet/stm32u073c8.pdf
- STM32U031 데이터시트: https://www.st.com/resource/en/datasheet/stm32u031c6.pdf
- STM32L071CB 데이터시트: https://www.st.com/resource/en/datasheet/stm32l071cb.pdf
- STM32L412CB 데이터시트: https://www.st.com/resource/en/datasheet/stm32l412cb.pdf
- Renesas RA2L1 제품 페이지: https://www.renesas.com/products/microcontrollers-microprocessors/ra-cortex-m-mcus/ra2l1-48mhz-arm-cortex-m23-ultra-low-power-general-purpose-microcontroller
- NXP K32 L2 제품 페이지: https://www.nxp.com/products/K32-L2
- TI MSPM0G3507 제품 페이지: https://www.ti.com/product/MSPM0G3507
- TI MSPM0G3507 데이터시트: https://www.ti.com/lit/gpn/mspm0g3507
- Silicon Labs EFM32TG11 데이터시트: https://www.silabs.com/documents/public/data-sheets/efm32tg11-datasheet.pdf
- STM32CubeMX: https://www.st.com/en/development-tools/stm32cubemx.html
- MCUXpresso IDE: https://www.nxp.com/pages/:MCUXpresso-IDE
- Renesas e² studio: https://www.renesas.com/en/software-tool/e2studio-information-ra-family
- TI Code Composer Studio: https://www.ti.com/tool/CCSTUDIO
- Silicon Labs Simplicity Studio: https://www.silabs.com/software-and-tools/simplicity-studio

## 13. 예상치 사용 기준

이번 보고서에서 `예상`으로 표기한 전력값은 아래 기준으로 작성했습니다.

- `STM32G071CBT6`: ST 데이터시트의 `Low-power run mode` 표를 근거로 환산
- `STM32F103C8T6`: F1/F107 계열 특성과 동급 72MHz Cortex-M3 계열 소비전력을 기준으로 추정
- `STM32C071CBT6`: C0 계열의 경량 특성과 G0/U0 계열 사이 위치로 추정
- `STM32G0B1CCT6`: G0 계열 중 기능 확장형이라는 점을 반영해 G071보다 약간 높게 추정
- `STM32F303CCT6`: M4 + 혼합신호 계열이라는 점을 반영해 중간 수준으로 추정
- `NXP K32L2B31`: NXP K32 L 저전력 계열 특성과 동급 Cortex-M0+ MCU 범위를 기준으로 추정

즉, `예상` 값은 의사결정용 상대 비교를 위한 값이며, 최종 선정 전에는 exact part 데이터시트 기준으로 재확인이 필요합니다.
