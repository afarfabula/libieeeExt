# Correctness Replay Checklist

## 自动门禁

- [x] `board.cpp` 小于 100,000 bytes。
- [x] `board.cpp` 使用 C++17 严格警告选项编译通过。
- [x] 12 组模块测试通过，包含高优算法补充和二维卷积。
- [x] 3 道历史题官方样例通过题目适配器回放。
- [x] ASan/UBSan 测试通过。
- [x] 92 条题库记录通过字段、枚举、唯一 ID 和回放状态校验。
- [ ] 在官方 HackerRank judge 上重新提交全部已回放题目。
- [ ] 为 60 条 `TODO-ADAPTER` 题目补输入输出适配。
- [x] 为 13 条 `TODO-CORE` 题目补核心能力。

## 已通过官方样例

- [x] `10.0 / Dog Walking` — 贪心、排序。
- [x] `10.0 / Full Adder` — 自定义进制、模拟。
- [x] `10.0 / N-Palindromes` — 字符串、组合计数、DP。

## 已通过模块验证

- [x] `11.0 / BeetleBag` — 0/1 背包。
- [x] `10.0 / Pirates` — binary lifting。
- [x] `11.0 / Collecting Gold` — 网格 BFS、路径恢复。
- [x] `11.0 / Fibonacci` — 模矩阵快速幂。
- [x] `11.0 / Jarawi and the Interview` — STL 哈希表。
- [x] `11.0 / Math Challenge` — CRT、组合数。
- [x] `11.0 / Maximum Sum` — Kadane。
- [x] `11.0 / Odd Cycle Check` — DSU。
- [x] `11.0 / Running Up Stairs` — 线性递推。
- [x] `11.0 / The Fast and the Curious` — 桥。
- [x] `12.0 / Bear Sums` — STL 容器。
- [x] `12.0 / Product Rectangle` — 最大和子矩形。
- [x] `12.0 / Tree Fun` — binary lifting。
- [x] `13.0 / Bearcity Renting` — 桥、DSU。
- [x] `13.0 / Googolplex` — 大整数。
- [x] `13.0 / Infinite String` — 二分答案。
- [x] `13.0 / Xtreme Rappers` — 贪心计数。
- [x] `14.0 / Coupon Codes` — STL 哈希表。
- [x] `14.0 / Magic Stones I` — 拓扑排序。
- [x] `16.0 / ANDman` — binary lifting。
- [x] `16.0 / Scheduler` — 二分答案。
- [x] `17.0 / Rumors` — 拓扑排序。
- [x] `18.0 / Another Sliding Window Problem` — 滑动窗口。
- [x] `18.0 / King's Order` — 堆 + 拓扑排序。
- [x] `18.0 / Laser Defence` — 二分答案、几何。
- [x] `18.0 / Power of three` — 模幂。
- [x] `19.0 / New Casino Game` — 模运算。
- [x] `19.0 / Stable Power Network` — DSU、MST。
- [x] `19.0 / The Twin Occurrence Quest` — 二分。

## 高优核心补充

- [x] binary lifting、LCA、kth ancestor、树路径距离。
- [x] 单调整数区间 `firstTrue` / `lastTrue` 和实数二分。
- [x] Kadane、最佳子数组位置、最大和子矩形。
- [x] 桥和割点。
- [x] 固定窗口和、窗口最小/最大值、双指针子数组计数。
- [x] 模矩阵快速幂。
- [x] NTT、二维卷积和字符串 next-occurrence 自动机。
- [x] CS Academy `Robot in a Labyrinth` 三组官方样例通过 workspace 模板回放。

## 后续适配顺序

1. 先给高频 `simulation` 和 `dp` 题补输入输出适配器。
2. 再推进字符串、图和几何题的题目级回放。
3. 每次适配后把状态从 `TODO-ADAPTER` 改为 `PASS-E2E`，并把官方样例
   断言加入 `problem_replay_tests.cpp`。
4. 只有题目确实需要新的通用能力时，才继续扩展 `board.cpp`。

完整清单以 `problem-tags.tsv` 为准；当前 `TODO-CORE` 为 0。
