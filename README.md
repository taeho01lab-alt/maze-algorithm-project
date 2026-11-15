# 미로 생성 알고리즘 비교 프로젝트

2026 컴퓨터알고리즘설계 팀 프로젝트입니다. 서로 다른 세 분류의 Randomized DFS, Randomized Kruskal, Sidewinder를 C++17로 구현합니다.

## 현재 상태

**세 생성 알고리즘과 미로 출력·저장이 구현되어 있습니다.** 공통 미로 표현, CLI, 빌드와 자동 테스트도 포함합니다. 생성 알고리즘 테스트는 별도 그래프 탐색으로 연결성·순환 여부를 확인합니다. **제품용 완전 미로 검증 API, 지표 계산, 반복 실험 모듈은 아직 미구현**이며 해당 담당자가 연결해야 합니다.

| 선택값 | 알고리즘 | 과제상 분류 | 소스 |
| --- | --- | --- | --- |
| `A` 또는 `dfs` | Randomized DFS | Graph-Search Based | `src/algorithm_a.cpp` |
| `B` 또는 `kruskal` | Randomized Kruskal | Spanning Tree-based | `src/algorithm_b.cpp` |
| `C` 또는 `sidewinder` | Sidewinder | Row/Cell Rule-based | `src/algorithm_c.cpp` |

동작 원리·완전 미로가 되는 이유·복잡도·참고문헌은 [알고리즘 설명](docs/generators.md)에 정리했습니다.

## 역할분배

| 담당 | 업무 | 주요 파일·결과물 |
| --- | --- | --- |
| 1번 — 생성 코드 | 세 생성 알고리즘·공통 입출력·통합 | `src/algorithm_a.cpp`, `src/algorithm_b.cpp`, `src/algorithm_c.cpp`, `src/main.cpp` |
| 2번 — 생성 결과 | 여러 크기·시드의 샘플 생성·미로 검증·결과 정리 | `src/validation.cpp`, `samples/` |
| 3번 — 비교 결과 | Maze Analyzer·측정·반복 실험·비교표·CSV | `src/analysis.cpp`, `src/experiments.cpp`, `results/` |
| 4번 — 최종 보고서 | 영어 보고서 취합·편집·PDF·실행 시연 | 보고서, 실행 결과 자료, 약 10분 시연 |

각자 맡은 부분의 참고문헌을 정리합니다. 결과 해석·결론·영어 검토·최종 보고서 확인은 네 명이 함께 진행합니다. 시연의 세부 질문은 해당 구현자가 답할 수 있도록 준비합니다. 그래프는 필수 작업으로 배정하지 않습니다.

공통 미로와 빌드 틀은 이미 준비되어 있습니다. 생성 코드 담당자는 알고리즘 설명·복잡도·출처를 보고서 담당자에게 전달합니다. 각 개발자는 자기 코드 오류를 수정합니다. 본 실험의 조건과 결과 해석은 함께 결정합니다.

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
./build/maze_app kruskal 20 20 42 samples/maze_kruskal_20x20_seed42.txt
./build/maze_app sidewinder 30 30 42 samples/maze_sidewinder_30x30_seed42.txt
```

Windows Visual Studio 빌드:

```powershell
.\build\Release\maze_app.exe --help
.\build\Release\maze_app.exe A 10 10 42
.\build\Release\maze_app.exe dfs 10 10 42 samples/maze_dfs_10x10_seed42.txt
```

`samples/`에는 세 알고리즘으로 실제 생성한 10×10·20×20·30×30 미로(시드 42)를 포함합니다. 출력 경로의 상위 폴더는 미리 만들어야 합니다. `results/schema.csv`는 열 이름만 있는 양식이며 아직 비교 측정값은 없습니다.

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

## 보고서와 실행 시연

보고서는 [Notion 공동 작성 페이지](https://app.notion.com/p/412a2865869181e08ed4ed129114105a)에서 영어로 공동 작성하고 최종 PDF로 제출합니다. 분량을 늘리기보다 구현·실험 근거를 명확히 적습니다.

- 미로 생성뿐 아니라 결과를 비교·분석하는 소프트웨어도 구현합니다.
- 소스 코드, 여러 크기의 텍스트 미로, 실제 실험 결과를 준비합니다.
- 막다른 길 수, S→E 경로 길이, 평균 직선 통로 길이, 생성 시간을 비교표로 보여주고 결과 해석과 참고문헌을 작성합니다. 측정 정의는 [공통 계약](docs/contract.md)을 따르고 미정인 항목은 구현 전에 합의합니다.
- PPT 없이 약 10분 동안 프로그램을 실행하고 동작과 비교 결과를 설명한 뒤 간단한 질의응답을 진행합니다.
- 4번이 실행·전체 설명을 맡고, 각 담당자는 자신의 알고리즘 또는 분석 방법에 관한 질문에 대비합니다.

본 실험의 크기·시드·반복 횟수, 평균 직선 통로 길이의 세부 정의는 아직 팀에서 정해야 합니다. 샘플 미로의 크기·시드는 최종 실험 조건을 확정한 것이 아닙니다. 실제 측정 전에는 결과표에 임의 수치를 넣지 않습니다.

