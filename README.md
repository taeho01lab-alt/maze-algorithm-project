# 미로 생성 알고리즘 비교 프로젝트

2026 컴퓨터알고리즘설계 팀 프로젝트의 C++17 공통 틀입니다. 알고리즘은 팀에서 서로 다른 분류의 세 가지를 선정하고 A·B·C에 연결합니다.

## 현재 상태

공통 미로 표현, 양쪽 벽 열기·닫기, 텍스트 출력, CLI, 빌드와 공통 규격 테스트가 준비되어 있습니다. **생성 알고리즘, 완전 미로 검증, 지표 계산, 반복 실험은 아직 미구현**입니다. 미구현 함수를 호출하면 오류를 반환하며 가짜 결과를 출력하지 않습니다.

## 담당 파일

| 담당 | 작업 파일 |
| --- | --- |
| 1번 | `src/algorithm_a.cpp`, 공통 자료구조·출력 유지보수 |
| 2번 | `src/algorithm_b.cpp`, `src/validation.cpp` |
| 3번 | `src/algorithm_c.cpp`, `src/experiments.cpp`의 반복 실행·CSV |
| 4번 | `src/analysis.cpp`, 생성 시간 측정·결과 집계·그래프 |

함수 선언은 `include/`에 있습니다. 공통 규격을 바꿀 때는 먼저 팀과 합의합니다. 알고리즘은 내부 자료구조를 자유롭게 사용하고 공통 `Maze`로 반환하면 됩니다.

## 빌드와 실행

CMake 3.16 이상과 C++17 컴파일러가 필요합니다. Windows에서는 C++ 빌드 도구가 설치된 Visual Studio 또는 GCC/Clang을 사용합니다.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Linux/macOS 또는 단일 구성 빌드:

```sh
./build/maze_app --help
./build/maze_app A 10 10 42
./build/maze_app A 10 10 42 samples/maze_A_10x10_seed42.txt
```

Windows Visual Studio 빌드:

```powershell
.\build\Release\maze_app.exe --help
.\build\Release\maze_app.exe A 10 10 42
```

현재 A·B·C 실행 시 미구현 오류가 나오는 것이 정상입니다. `samples/`에는 실제 생성 결과를, `results/`에는 실제 CSV를 저장합니다. `results/schema.csv`는 열 이름만 있는 양식입니다.

## 공통 규격

- `Maze = std::vector<std::vector<Cell>>`, 접근은 `maze[row][col]`입니다.
- `width`열 × `height`행이며 둘 다 양수입니다.
- `true`는 벽, `false`는 통로입니다. 바깥 테두리는 닫아 둡니다.
- 벽 변경은 `openPassage` / `closePassage`로 양쪽 칸에 함께 적용합니다.
- S는 `(0,0)`, E는 `(height-1,width-1)`입니다. 1×1에서는 S만 표시하고 경로 길이는 0입니다.
- 같은 알고리즘·크기·시드는 같은 결과를 생성해야 합니다.

자세한 기준은 [공통 계약](docs/contract.md)을 참고하세요.

## 협업

각자 작업 브랜치를 만들고 PR로 `main`에 합칩니다. 예: `feature/algorithm-a`, `feature/algorithm-b`, `feature/algorithm-c`, `feature/analysis`. 자동 빌드와 테스트 통과를 확인합니다. 팀원 초대와 브랜치 보호는 저장소 관리자가 팀 계정에 맞춰 설정합니다.

보고서는 [Notion 공동 작성 페이지](https://app.notion.com/p/412a2865869181e08ed4ed129114105a)에서 작성하고 최종 PDF로 제출합니다. 소스 코드, 여러 크기의 텍스트 미로, 비교 결과 및 발표 자료도 별도로 준비합니다.

