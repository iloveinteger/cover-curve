# Implementation

이 디렉터리는 theory에서 정의한 문제를 현재 C++/Web 코드가 어떻게 계산하는지를 설명한다.

## 문서 원칙

각 문서는 실제 interface, 자료구조와 상태 표현, numerical procedure, stopping condition과 tolerance, complexity와 병목, theory와 다른 numerical approximation의 범위를 명시한다.

수학적 정리와 global-convergence proof는 theory/를 기준으로 한다.

## Core numerical methods

| 문서 | 구현 |
| --- | --- |
| [adaptive-grid-dp.md](adaptive-grid-dp.md) | continuous-height DP baseline |
| [numerical-methods.md](numerical-methods.md) | integration, transition evaluation, tolerances |
| [support-maximization.md](support-maximization.md) | black-box support maximization |
| [interpolation.md](interpolation.md) | sampled-data interpolation |

## Outer solvers

| 문서 | 구현 |
| --- | --- |
| [breakpoint-search.md](breakpoint-search.md) | deterministic breakpoint-space subdivision |
| [coordinate-search.md](coordinate-search.md) | cyclic coordinate optimization |
| [envelope-sqp.md](envelope-sqp.md) | direct-height + envelope sensitivity + safeguarded L-BFGS-style search |
| [curvature-adaptive.md](curvature-adaptive.md) | curvature-density initialization |

## Reference primitive

[slope-minimization.md](slope-minimization.md)는 독립적인 one-segment 문제를 다룬다. 이는 진단/검증용이며 shared-height DP transition으로 사용하지 않는다.

## Theory와 implementation의 경계

Theory는 exact continuous heights, exact transition supremum, exact integration, exact continuous minimization을 다룬다. Implementation은 floating-point arithmetic, numerical support/separation, finite LP iterations, finite one-dimensional searches, finite outer budgets를 사용한다.

따라서 실제 실행값은 이론적 최적값의 수치 근사이며, 문서에서는 두 층을 혼동하지 않는다.

## GitHub 수식 표기

인라인 수식은 $...$, 블록 수식은 $$...$$를 사용한다. 수식은 fenced code block 안에 넣지 않는다.
