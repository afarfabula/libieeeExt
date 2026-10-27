# IEEEXtreme Problem Corpus

本目录记录题源、算法标签、频次依据和正确性回放状态。机器可读数据在
`problem-tags.tsv`，本文件解释数据口径。

## 范围与限制

第一版索引包含 92 条公开题目记录，覆盖 IEEEXtreme 10.0-19.0。来源
不是官方完整题库：其中 90 条来自公开题解仓库 `HliasOuzounis/
IEEE-Xtreme-Problems`，另补入公开题面仓库中的 Full Adder 和
N-Palindromes。后者带有可执行样例回放。

因此：

- “频次”表示**当前审计样本中出现次数**，不是历年全部正式题目中的
  精确出现率。
- 标签来自题名、公开题解文件提示和已阅读的题面。备注中标为
  `Needs closer problem classification` 的条目仍待题面复核。
- 一个题目可以有多个标签，所以标签频次之和大于题目数。
- 目录不复制公开题解代码，只保存链接式引用、题目名和可执行样例断言。
- 8.0/9.0 的公开题面已找到，但暂未完成逐题标签复核，因此不纳入第一版
  频次统计；16.0-19.0 的公开题解也不是官方完整题集。

## 来源快照

| 用途 | 仓库 | 快照 commit | License metadata |
|---|---|---|---|
| 10.0-19.0 题解与得分提示 | [HliasOuzounis/IEEE-Xtreme-Problems](https://github.com/HliasOuzounis/IEEE-Xtreme-Problems) | `9d08f53c626d` | 未声明 |
| 8/9/10.0 题面与样例 | [ieeesbspain/IEEEXtreme](https://github.com/ieeesbspain/IEEEXtreme) | `8c8b01122365` | 未声明 |
| 11.0 题面 | [ieee-ensias/IEEEXtreme-11.0](https://github.com/ieee-ensias/IEEEXtreme-11.0) | `e76d07dc0f42` | 未声明 |
| 12.0 题面 | [ieee-ensias/IEEEXtreme-12.0](https://github.com/ieee-ensias/IEEEXtreme-12.0) | `8f45b6d87919` | 未声明 |
| 13.0 题面 | [ieee-ensias/IEEEXtreme-13.0](https://github.com/ieee-ensias/IEEEXtreme-13.0) | `f24e607eba10` | 未声明 |
| 14.0 题面 | [ieee-ensias/IEEEXtreme-14.0](https://github.com/ieee-ensias/IEEEXtreme-14.0) | `54123395451d` | 未声明 |
| 15.0 题面 | [ieee-ensias/IEEEXtreme-15.0](https://github.com/ieee-ensias/IEEEXtreme-15.0) | `56e9d5a53527` | 未声明 |
| 15.0-19.0 交叉核对 | [Pavith19 solutions](https://github.com/Pavith19?tab=repositories&q=IEEEXtreme) | `ebc69e1ef50b` … `0e8aec9c700e` | MIT |

`solution_ref` 前缀：

- `HL`: HliasOuzounis 的仓库目录。
- `ES`: ieeesbspain 的题面、样例或题解目录。
- `EN`, `PA`: 题面或题解交叉核对来源，主要用于确认题名和题意。

## 标签频次

| 标签 | 次数 | 出现届数 |
|---|---:|---|
| simulation | 15 | 10-12,14,17,19 |
| dp | 14 | 10-15,17-19 |
| strings | 13 | 10,13,14,16-18 |
| graph | 11 | 11,13,14,17-19 |
| geometry | 10 | 11,12,14,17-19 |
| greedy | 8 | 10,13-16,18 |
| custom-logic | 6 | 11-13 |
| number-theory | 7 | 11,13,18,19 |
| combinatorics | 5 | 10,11,18,19 |
| grid-search | 5 | 11,14,16,17,19 |
| bfs-dfs | 5 | 11,16,17,19 |
| modular-arithmetic | 5 | 11,17-19 |
| sorting | 4 | 10,11,13,15 |
| tree | 4 | 10,12,13,16 |
| hashing | 4 | 11,12,14,17 |
| math | 5 | 12,13,15,18 |
| binary-search | 4 | 13,16,18,19 |
| parsing | 4 | 16-18 |

完整标签统计可由 `python3 tools/check_corpus.py --limit 100` 重新计算。

## 对板子的直接结论

样本中占据前列的是模拟、DP、字符串、图、几何和贪心，而不是单纯的高级
图论或复杂数据结构。第一轮高优核心能力已经补齐并通过模块测试：

- 树上 binary lifting、LCA、kth ancestor 和路径距离。
- 单调整数/实数二分答案框架。
- Kadane、最佳子数组位置和最大和子矩形。
- 桥和割点。
- 固定窗口和、窗口最值、非负数组双指针计数。
- 模矩阵快速幂。

这组补充把索引中的 `TODO-CORE` 从 13 条降为 0。剩余 60 条是题目级
适配工作，需要针对具体输入输出和题意组合这些基础件。

## 状态口径

| 状态 | 含义 |
|---|---|
| `PASS-E2E` | 公开官方样例已通过题目适配器回放；不等于在官方 judge 上提交通过 |
| `PASS-MODULE` | 题目依赖的主要基础算法已有单元测试 |
| `TODO-ADAPTER` | 核心能力基本存在，但尚未完成该题的输入解析、组合和输出适配 |
| `TODO-CORE` | 当前板子缺少该题所需的关键核心能力，或题意尚未分类清楚 |

所有 `PASS-E2E` 条目必须同时出现在
`tests/problem_replay_tests.cpp` 中，`tools/check_corpus.py` 会强制校验。
