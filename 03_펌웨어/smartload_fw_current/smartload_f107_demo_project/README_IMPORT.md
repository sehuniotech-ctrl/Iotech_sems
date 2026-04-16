# smartload_f107_demo_project

STM32CubeIDE에서 Ctrl+클릭 / F3 심볼 이동이 잘 되도록 만든 인덱싱 전용 프로젝트입니다.

## 포함 내용

- `src`
  - F107 타깃 소스 복사본
- `inc`
  - F107 타깃 헤더 복사본
- `common_src`
  - 공통 ATM90E26 / app 소스 복사본
- `common_inc`
  - 공통 헤더 복사본

## Import 방법

1. STM32CubeIDE 실행
2. `File -> Import -> Existing Projects into Workspace`
3. 아래 폴더 선택

```text
D:\work\15_지선차단기\03_펌웨어\smartload_fw_current\smartload_f107_demo_project
```

4. import 후 아래 실행
   - `Project -> C/C++ Index -> Rebuild`
   - `Project -> Clean`

## 주의

- 이 폴더는 우선 "코드 읽기 / 인덱싱 / 구조 이해"용입니다.
- 실제 빌드/다운로드는 기존 STM32F107 보드 프로젝트와 별도로 맞추는 것이 좋습니다.
