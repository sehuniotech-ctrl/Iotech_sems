# PC Demo

보드 없이 아래 흐름을 시뮬레이션하는 데모입니다.

- GUI -> 계량칩 시뮬레이터
- 계량칩 시뮬레이터 -> MCU 시뮬레이터
- MCU 시뮬레이터 -> UART/485 형태 문자열 전송
- Hercules에서 수신 확인

## 파일

- `smartload_meter_sim.py`
  - ATM90E26 레지스터를 흉내내는 TCP 서버
- `smartload_gui.py`
  - 전압/전류/주파수/역률을 보내는 GUI
- `smartload_mcu_sim.py`
  - 계량칩 시뮬레이터의 계측 결과를 읽어서 UART 프레임을 만들어 COM 포트로 송신
- `start_demo.bat`
  - meter / gui / mcu 시뮬레이터를 한 번에 실행
- `check_com_ports.py`
  - 현재 PC의 COM 포트 목록 확인

## 실행 순서

### 한 번에 실행

```powershell
D:\work\15_지선차단기\03_펌웨어\smartload_fw_current\pc_demo\start_demo.bat
```

기본 MCU 송신 포트는 `COM6`입니다. 필요하면 `start_demo.bat` 안의 `MCU_PORT` 값을 바꾸면 됩니다.

### 개별 실행

1. 계량칩 시뮬레이터 실행
```powershell
python D:\work\15_지선차단기\03_펌웨어\smartload_fw_current\pc_demo\smartload_meter_sim.py
```

2. GUI 실행
```powershell
python D:\work\15_지선차단기\03_펌웨어\smartload_fw_current\pc_demo\smartload_gui.py
```

3. MCU 시뮬레이터 실행
```powershell
python D:\work\15_지선차단기\03_펌웨어\smartload_fw_current\pc_demo\smartload_mcu_sim.py --port COM6
```

## Hercules 사용

- Hercules에서 가상 COM 반대편 포트를 엽니다.
- 예: Python이 `COM6`으로 송신하면 Hercules는 짝 포트 `COM5`를 엽니다.
- 가상 COM 포트 쌍은 별도 설치가 필요할 수 있습니다.

현재 COM 포트 확인:

```powershell
python D:\work\15_지선차단기\03_펌웨어\smartload_fw_current\pc_demo\check_com_ports.py
```

## 참고

- 이 PC 데모는 "보이는 계측값 검증"이 목적이라 MCU 시뮬레이터가 계량칩 시뮬레이터의 상태값을 그대로 프레임화합니다.
- 실제 보드 단계에서는 ATM90E26 raw register read 방식으로 전환합니다.
