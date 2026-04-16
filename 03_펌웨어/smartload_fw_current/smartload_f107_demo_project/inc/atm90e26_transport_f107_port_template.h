#ifndef ATM90E26_TRANSPORT_F107_PORT_TEMPLATE_H
#define ATM90E26_TRANSPORT_F107_PORT_TEMPLATE_H

/*
 * STM32F107 + ATM90E26 porting input template
 *
 * 목적:
 * - 회로도 수신 후 실제 구현 전에 필요한 입력값을 한 장으로 정리
 * - SPI/UART 방식, 핀맵, HAL handle, delay 함수를 빠르게 확정
 *
 * 사용 방법:
 * 1. 회로도 확인 후 아래 항목을 채움
 * 2. 이 파일을 기준으로 실제 `atm90e26_transport_f107_port.h` 작성
 * 3. transport 구현에 연결
 */

/* -------------------------------------------------------------------------- */
/* 1. 통신 방식 확정                                                          */
/* -------------------------------------------------------------------------- */

/* 둘 중 하나 선택
 * - 1: SPI
 * - 2: UART
 */
#define ATM90E26_F107_BUS_TYPE_SPI   1
#define ATM90E26_F107_BUS_TYPE_UART  2

/* TODO: 회로도 확인 후 실제 값으로 변경 */
#define ATM90E26_F107_BUS_TYPE       ATM90E26_F107_BUS_TYPE_SPI

/* -------------------------------------------------------------------------- */
/* 2. 공통 보드 함수                                                          */
/* -------------------------------------------------------------------------- */

/* TODO:
 * - HAL 기반이면 HAL_Delay 사용
 * - 기존 LCU 보드 tick 함수 사용 시 그 함수명으로 변경
 */
/* example: #define ATM90E26_F107_DELAY_MS(ms) HAL_Delay(ms) */

/* -------------------------------------------------------------------------- */
/* 3. SPI 연결 정보                                                           */
/* -------------------------------------------------------------------------- */

/*
 * 회로도에서 아래 정보 확인 필요:
 * - ATM90E26 SCLK  -> STM32F107 어떤 핀인지
 * - ATM90E26 MISO  -> STM32F107 어떤 핀인지
 * - ATM90E26 MOSI  -> STM32F107 어떤 핀인지
 * - ATM90E26 CS    -> STM32F107 어떤 GPIO인지
 * - ATM90E26 RESET -> STM32F107 어떤 GPIO인지
 * - 필요 시 IRQ / ZX / CF1 / CF2 / WarnOut 연결 여부
 */

/* example:
 * extern SPI_HandleTypeDef hspi1;
 * #define ATM90E26_F107_SPI_HANDLE     (&hspi1)
 * #define ATM90E26_F107_CS_GPIO_Port   GPIOA
 * #define ATM90E26_F107_CS_Pin         GPIO_PIN_4
 * #define ATM90E26_F107_RST_GPIO_Port  GPIOB
 * #define ATM90E26_F107_RST_Pin        GPIO_PIN_0
 */

/* -------------------------------------------------------------------------- */
/* 4. UART 연결 정보                                                          */
/* -------------------------------------------------------------------------- */

/*
 * 회로도에서 아래 정보 확인 필요:
 * - ATM90E26 RX -> STM32F107 TX 어느 핀인지
 * - ATM90E26 TX -> STM32F107 RX 어느 핀인지
 * - half/full duplex 여부
 * - DE/RE 핀 사용 여부
 * - baudrate / parity / frame format
 */

/* example:
 * extern UART_HandleTypeDef huart2;
 * #define ATM90E26_F107_UART_HANDLE    (&huart2)
 * #define ATM90E26_F107_UART_BAUD      9600
 */

/* -------------------------------------------------------------------------- */
/* 5. Bring-up 1차 목표                                                       */
/* -------------------------------------------------------------------------- */

/*
 * 회로도 확인 후 처음부터 모든 기능을 붙이지 말고 아래 순서로 확인:
 * 1. Soft reset write
 * 2. CalStart / AdjStart write
 * 3. SysStatus read
 * 4. URMS read
 * 5. IRMS read
 * 6. PMEAN read
 * 7. FREQ read
 */

#endif
