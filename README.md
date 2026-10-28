# IEEE Xtreme 100K Board

一个面向 IEEE Xtreme 的单文件 C++17 算法板子。`board.cpp` 保持在
100,000 字节以内，测试不依赖 Boost 或其他第三方库。

## 当前状态

- `board.cpp`: 70,482 / 100,000 bytes.
- 单元测试: 12 组，覆盖大整数、数论、NTT/二维卷积、图、树、数据结构、
  字符串、几何、DP、滑动窗口、表达式解析和日期。
- 历史题样例回放: 3 道，分别为 Dog Walking、Full Adder 和
  N-Palindromes。
- 题库索引: 92 条公开题目记录，覆盖 IEEEXtreme 10.0-19.0。
- 回放状态: 3 条历史样例通过，29 条模块级通过，60 条待题目适配；
  已知的核心能力缺口已清零。

## 使用

```bash
# 正常测试
tests/run_tests.sh

# ASan + UBSan
SANITIZE=1 tests/run_tests.sh

# 校验题库数据并打印频次
python3 tools/check_corpus.py --limit 50
```

测试脚本同时检查源码体积、编译 `board.cpp`、运行单元测试和历史题
样例回放。提交具体题目时，在 `board.cpp` 的 `ib::solve()` 中编写输入、
算法组合和输出。

## CS Academy Workspace

`workspace/board.cpp` 是从通过测试的单文件板子自动生成的库文件，
`workspace/TemplateMain.cpp` 是展开库源码并压缩到 32 KB 以内的空白模板，
`workspace/SubmissionMain.cpp` 是压缩后的完整本题提交文件。CS Academy
的 Library File 不自动加入编译路径，因此提交入口必须自包含；不要让
模板携带上一题的 `solve()`：

CS Academy 的任务源码上限实测为 32,768 bytes。主模板因此省略 BigInt 和
日期解析两个可选大模块；完整实现仍在 `board.cpp`，需要时生成题目级最小
切片，或从 Library File 取回对应模块。

```bash
python3 tools/build_workspace_board.py
tests/run_workspace_tests.sh
```

当前 workspace 样例是 CS Academy `Robot in a Labyrinth`，使用 NTT
二维卷积判断整串命令能否安全执行，并用 Dijkstra + next-occurrence
自动机求最少金币数。

## 题库

- [题库方法与来源](docs/corpus/README.md)
- [机器可读题库索引](docs/corpus/problem-tags.tsv)
- [正确性回放清单](docs/corpus/replay-checklist.md)

题库频次来自当前已整理出的公开题解样本，不等同于官方完整题集统计。
所有推断性标签都在数据表中保留了来源和备注，新增题目时必须同步更新
频次和回放状态。
