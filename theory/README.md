# Theory

이 디렉터리는 구현 설명이 아니라 문제의 수학적 명세와 알고리즘의 정당성을 기록한다.

## 공통 구조

각 알고리즘의 이론 문서는 다음 순서를 따른다.

1. 문제와 정의 — 변수, feasible set, 목적함수.
2. 정리와 공식 — 핵심 등식, 부등식, recurrence, 민감도 공식.
3. 증명 — 정확성에 필요한 가정과 논리적 근거.
4. 알고리즘 — 수학적 의사코드와 각 단계의 의미.
5. 알고리즘 분석 — 시간복잡도, 메모리, 수렴성, 정확성 및 오차항.
6. 수치 구현과의 경계 — exact-oracle 결과와 실제 floating-point/black-box 계산을 구분한다.

## 기본 이론

| 문서 | 내용 |
| --- | --- |
| [problem.md](problem.md) | 원 문제와 piecewise-affine majorant 정의 |
| [existence.md](existence.md) | 최적해 존재성 |
| [fixed-breakpoint.md](fixed-breakpoint.md) | 고정 breakpoint의 convex semi-infinite LP |
| [dynamic-programming.md](dynamic-programming.md) | 연속 vertex-height Bellman formulation |
| [algorithm.md](algorithm.md) | breakpoint-grid DP의 정확성, 수렴성, 복잡도, 오차항 |

## Free-breakpoint solver 이론

| 문서 | 내용 |
| --- | --- |
| [direct-height.md](direct-height.md) | 고정 breakpoint cutting-plane inner solver |
| [breakpoint-search.md](breakpoint-search.md) | exhaustive subdivision의 global convergence |
| [coordinate-search.md](coordinate-search.md) | coordinate descent의 monotonicity와 limit-point 성질 |
| [envelope-sqp.md](envelope-sqp.md) | envelope sensitivity, L-BFGS-style outer method, curvature asymptotics |

## 보장 범위

- algorithm.md의 breakpoint-grid 정리는 정확한 continuous-height transition oracle을 가정한다.
- breakpoint-search.md의 global convergence도 정확한 fixed-breakpoint oracle을 가정한다.
- 실제 C++ solver의 support search, LP, integration, finite-difference, stopping tolerance는 별도의 수치 오차를 만든다.
- 따라서 수치 실행 결과를 수학적 global-optimality certificate로 표현하지 않는다.
- curvature density는 초기화/가속을 위한 asymptotic heuristic이며 finite-n optimality theorem이 아니다.

## GitHub 수식 표기

인라인 수식은 `$...$`, 블록 수식은 `$$...$$`를 사용한다. 모든 수식은 GitHub가 해석할 수 있는 표준 LaTeX 문법으로 작성한다.
