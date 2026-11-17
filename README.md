# 미로 생성 및 Maze Analyzer - C++ 담당 코드

이 폴더는 Randomized DFS, Randomized Kruskal, Sidewinder와 Maze Analyzer를 구현한 C++17 프로젝트입니다.

## 제공하는 기능

- 세 알고리즘 중 하나로 미로 생성
- 막다른 길 수, 최장 경로, 평균 직선 복도 길이, 생성 시간 측정
- 분석 전에 완전 미로와 벽 구조 검증
- 단일 미로 화면 출력 또는 텍스트 파일 저장
- 소스 빌드와 정확성 검사

## GitHub에서 내려받아 실행하기

Windows에서는 먼저 CMake와 Visual Studio 또는 Build Tools의 **C++를 사용한 데스크톱 개발** 구성 요소를 설치합니다. VS Code만 설치한 것으로는 C++ 컴파일러가 설치되지 않습니다.

GitHub의 `Code → Download ZIP`으로 받은 뒤 압축을 풀거나, 다음 명령으로 내려받습니다.

```powershell
git clone https://github.com/taeho01lab-alt/maze-algorithm-project.git
cd maze-algorithm-project
```

Windows 시작 메뉴에서 **Developer PowerShell for VS** 또는 **x64 Native Tools Command Prompt for VS**를 열고 프로젝트 폴더로 이동합니다. 이 개발자 터미널은 `cl`, `nmake` 같은 C++ 빌드 도구를 찾을 수 있게 해 줍니다. 일반 PowerShell에서 `nmake` 또는 컴파일러를 찾지 못하면 개발자 터미널에서 다시 실행합니다.

GitHub 저장소에는 소스 코드가 있습니다. 프로젝트 폴더에서 먼저 빌드하고 검사합니다.

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

PowerShell에서 빌드된 실행 파일을 선택하고 실행합니다.

```powershell
if (Test-Path .\build\Release\maze_app.exe) {
    $mazeApp = '.\build\Release\maze_app.exe'
} else {
    $mazeApp = '.\build\maze_app.exe'
}
& $mazeApp --analyze dfs 10 10 42
& $mazeApp --analyze kruskal 20 20 42
& $mazeApp --analyze sidewinder 30 30 42
```

Visual Studio 방식은 `build/Release/maze_app.exe`, NMake/Ninja 방식은 `build/maze_app.exe`입니다. Linux에서는 `./build/maze_app`을 사용합니다. 생성 방식을 바꿀 때는 기존 `build`와 다른 새 빌드 폴더를 지정합니다.

## VS Code에서 열기

`파일 → 폴더 열기`로 `include`, `src`, `CMakeLists.txt`가 함께 있는 프로젝트 폴더 전체를 엽니다. Microsoft C/C++ 확장을 설치하면 저장소의 `.vscode/settings.json`이 헤더 검색 경로와 C++17을 설정합니다. 컴파일러 선택을 요청하면 PC에 설치된 C++17 컴파일러를 선택합니다.

각 `.cpp`는 하나의 프로그램을 구성하는 부품입니다. `io.cpp` 같은 파일에 있는 **활성 파일 실행/컴파일** 대신 위 CMake 명령으로 전체 프로젝트를 빌드합니다. 헤더를 잘못 인식하던 표시가 남으면 명령 팔레트에서 `C/C++: Reset IntelliSense Database`를 실행합니다.

## 미로 크기와 출력 읽기

`10 10`은 이동할 수 있는 논리적 칸이 가로 10개·세로 10개라는 뜻입니다. 벽을 별도 문자로 표시하므로 화면에는 **21열 × 21행**으로 보입니다. 20×20칸은 41×41문자, 30×30칸은 61×61문자입니다. `#`은 벽, 공백은 통로, `S`는 왼쪽 위 시작 칸, `E`는 오른쪽 아래 도착 칸입니다. 정렬이 어긋나면 고정폭 글꼴과 줄바꿈 없는 보기로 확인합니다.

## 로컬 전달본의 실행 파일 사용하기

로컬 전달본에 bin/maze_app.exe가 있을 때 사용할 수 있습니다. GitHub에서 받은 소스는 위 방법으로 직접 빌드합니다.

프로젝트 폴더에서 PowerShell을 열고 실행합니다.

```powershell
.\bin\maze_app.exe --analyze dfs 10 10 42
.\bin\maze_app.exe --analyze kruskal 20 20 42
.\bin\maze_app.exe --analyze sidewinder 30 30 42
```

숫자는 가로 칸 수, 세로 칸 수, 난수 시드입니다. `A`, `B`, `C`는 각각 `dfs`, `kruskal`, `sidewinder`의 별칭입니다.

미로를 파일로 저장하면서 네 지표를 화면에서 확인하려면:

```powershell
.\bin\maze_app.exe --analyze dfs 10 10 42 maze.txt
```

분석 없이 미로만 출력하거나 저장하는 기본 명령:

```powershell
.\bin\maze_app.exe dfs 10 10 42
.\bin\maze_app.exe dfs 10 10 42 maze.txt
.\bin\maze_app.exe --help
```

저장할 상위 폴더는 미리 있어야 합니다. 같은 이름으로 저장하면 기존 파일을 덮어씁니다. 제공된 bin은 Windows용 실행 파일이며 소스를 수정한 후에는 다시 빌드해야 합니다.

## 파일 구성

| 위치 | 역할 |
| --- | --- |
| include/maze.h, src/maze.cpp | 칸·벽·전체 미로 표현과 공통 처리 |
| include/algorithms.h, src/registry.cpp | 생성 함수 선언과 알고리즘 이름 선택 |
| src/algorithm_a.cpp | Randomized DFS |
| src/algorithm_b.cpp | Randomized Kruskal |
| src/algorithm_c.cpp | Sidewinder |
| src/generator_random.h | 공통 난수 선택 |
| include/analysis.h, src/analysis.cpp | 막다른 길·최장 경로·평균 직선 복도 |
| src/maze_graph.h, src/validation.cpp | 그래프 표현과 완전 미로 검증 |
| include/measurement.h, src/measurement.cpp | 한 번의 생성 시간 측정과 분석 연결 |
| include/io.h, src/io.cpp | 화면 출력과 단일 미로 저장 |
| src/main.cpp | 실행 명령 처리 |
| tests/ | 공통 구조·생성·분석·CLI 정확성 검사 |
| docs/ | 이 코드의 규칙·생성 알고리즘·측정 방법 설명 |
| bin/maze_app.exe | 제공된 Windows 실행 파일 |

## 지표 정의

막다른 길은 열린 이웃이 하나인 칸 수입니다. 과제에서 정의한 최장 경로는 S에서 E까지의 이동 횟수입니다. 완전 미로에서는 두 끝점 사이의 단순 경로가 하나이므로 BFS로 구한 거리를 사용합니다. 평균 직선 복도는 꺾임·갈림길·막다른 길을 경계로 한 직선 구간 길이의 평균입니다. 시간은 생성 함수 호출만 밀리초 단위로 측정합니다. 검증·분석·출력·저장은 시간에서 제외합니다.

자세한 규칙은 [공통 규격](docs/contract.md), [생성 알고리즘](docs/generators.md), [Analyzer](docs/analyzer.md)를 참고합니다.

## 직접 빌드하고 검사하기

CMake와 C++17 컴파일러가 필요합니다. Windows에서는 Visual Studio의 C++ 빌드 도구 등을 사용할 수 있습니다.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Visual Studio 방식으로 빌드한 실행 파일은 보통 `build/Release/maze_app.exe`입니다. 다른 빌드 방식에서는 `build/maze_app.exe` 또는 `build/maze_app` 등을 사용합니다. 새로 빌드한 파일로 실행하며, 기존 bin은 자동으로 바뀌지 않습니다.
