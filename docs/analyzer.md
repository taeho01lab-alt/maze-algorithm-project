# Maze Analyzer와 생성 시간

## 인터페이스

`include/analysis.h`의 MazeMetrics는 deadEnds, longestPathLength, averageCorridorLength를 제공합니다. `include/measurement.h`의 MeasuredMaze는 Maze, MazeMetrics, generationMs를 함께 제공합니다.

```cpp
#include "measurement.h"

const auto result = generateAndAnalyze("dfs", 10, 10, 42);
// result.maze
// result.metrics.deadEnds
// result.metrics.longestPathLength
// result.metrics.averageCorridorLength
// result.generationMs
```

## 막다른 길

열린 이웃이 하나인 칸을 셉니다. 시작·끝 표시와 관계없이 같은 기준을 적용합니다. 1×1은 이웃이 없으므로 막다른 길 수가 0입니다.

## 최장 경로

과제 원문은 Longest Path Length를 Start(S)에서 End(E)까지의 경로 길이로 정의합니다. 완전 미로는 두 칸 사이의 단순 경로가 하나이므로 S에서 BFS를 한 뒤 E의 거리를 사용합니다. S는 왼쪽 위, E는 오른쪽 아래 칸이며 단위는 이동 횟수입니다. 미로 전체에서 가장 먼 임의의 두 칸 사이 거리인 트리의 지름과 구분합니다.

## 평균 직선 복도

꺾임·갈림길·막다른 길을 경계로 나눈 최대 직선 구간의 평균 이동 횟수입니다. 열린 이웃이 두 개이고 둘 모두 같은 행 또는 같은 열에 있는 칸만 구간 내부로 이어집니다. S/E 표시는 구간을 추가로 나누지 않습니다.

구간 경계 칸들의 차수 합을 2로 나누면 복도 수입니다. 모든 통로 간선이 하나의 구간에만 속하므로 전체 통로 수를 복도 수로 나누어 평균을 구합니다. 구간이 없는 1×1은 평균 0입니다.

| 예제 | 막다른 길 | 최장 경로 | 평균 복도 |
| --- | --- | --- | --- |
| 1×1 | 0 | 0 | 0 |
| 1×5 또는 5×1 | 2 | 4 | 4 |
| 2×2의 (0,0)→(1,0)→(1,1)→(0,1) | 2 | 2 | 1 |

## 생성 시간

`src/measurement.cpp`의 generateAndAnalyze는 생성 함수 호출 전후의 steady_clock 시간을 밀리초로 계산합니다. 시드와 작업 자료구조 초기화도 생성 함수 안에서 이루어지므로 포함됩니다. 이후 analyzeMaze를 호출하여 검증과 구조 지표 계산을 합니다. 따라서 검증·분석·출력·저장 시간은 생성 시간에 들어가지 않습니다.

## 검증과 복잡도

`src/maze_graph.h`에서 칸을 정점, 열린 통로를 간선으로 바꿉니다. 배열 모양·벽·외곽을 확인하고, 연결 요소와 통로 수로 연결성과 순환 여부를 검사합니다. Analyzer는 연결되고 순환이 없는 완전 미로만 받습니다.

칸 수 N, 통로 수 N-1인 완전 미로에서 검증과 구조 분석은 시간 O(N), 추가 공간 O(N)입니다. 반복문과 큐를 사용합니다.

`tests/test_analysis.cpp`는 수작업 예제와 잘못된 미로를 검사하고, 작은 미로 576개의 S→E 길이를 논리적 칸을 직접 탐색하는 별도 DFS 결과와 대조합니다. 생성 시간은 유한한 0 이상의 값인지 확인합니다.

## 실행

```powershell
.\bin\maze_app.exe --analyze dfs 10 10 42
.\bin\maze_app.exe --analyze kruskal 20 20 42 maze.txt
```

파일 경로를 생략하면 미로와 네 측정값을 화면에 표시합니다. 경로를 지정하면 미로 그림은 파일로 저장하고 네 측정값은 화면에 표시합니다. Perfect maze는 통과 여부 안내입니다. 작은 미로의 생성 시간은 실행 환경에 따라 달라집니다.
