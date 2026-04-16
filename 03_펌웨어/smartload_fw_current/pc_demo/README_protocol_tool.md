# Smart Load Protocol Test Tool

지선차단기-SL / DCU 전용 프로토콜을 PC에서 바로 송수신 테스트하기 위한 간단한 도구입니다.

## 파일

- `smartload_protocol.py`
  - 프레임 생성 / CRC16 / TLV 파싱 공용 모듈
- `smartload_protocol_cli.py`
  - 실제 COM 포트로 요청 전송 및 응답 확인
- `smartload_protocol_sim.py`
  - 가상 SL 시뮬레이터

## 준비

```bash
pip install pyserial
```

## COM 포트 확인

```bash
python smartload_protocol_cli.py ports
```

## 조회 요청 예시

전압 / 전류 / 릴레이 상태 조회:

```bash
python smartload_protocol_cli.py read --port COM6 --baud 9600 --sl-id 0x11 --seq 0x21 --items 0x0002,0x0003,0x0005
```

## 릴레이 제어 예시

릴레이 OFF:

```bash
python smartload_protocol_cli.py control --port COM6 --baud 9600 --sl-id 0x11 --seq 0x22 --state OFF
```

릴레이 ON:

```bash
python smartload_protocol_cli.py control --port COM6 --baud 9600 --sl-id 0x11 --seq 0x23 --state ON
```

## Raw Frame 송신

```bash
python smartload_protocol_cli.py raw --port COM6 --baud 9600 --hex "7E 01 01 21 11 00 06 00 02 00 03 00 05 98 77 7E"
```

## 수신 모니터링

```bash
python smartload_protocol_cli.py monitor --port COM6 --baud 9600
```

## 가상 SL 시뮬레이터

가상 COM 포트 쌍이 있을 때 한쪽에는 시뮬레이터, 다른 쪽에는 CLI를 붙여 테스트할 수 있습니다.

```bash
python smartload_protocol_sim.py --port COM7 --baud 9600 --sl-id 0x11 --voltage 220.0 --current 50.0 --energy 100000 --relay ON
```

## 현재 프로토콜 기준

- Frame:
  - `STX | VER | MSG_TYPE | SEQ | SL_ID | LEN | PAYLOAD | CRC16 | ETX`
- `STX = 0x7E`
- `ETX = 0x7E`
- `VER = 0x01`
- `SL_ID = 1 byte`
- 전압 / 전류 / 전력량은 `uint32`

## 파싱 기준

- Voltage: `raw / 10`
- Current: `raw / 100`
- Energy_Acc: `raw Wh`
- Alarm_Code: 16bit bit field
