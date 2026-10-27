# IEEE Xtreme 100K Board

一个面向 IEEE Xtreme 的单文件 C++17 算法板子。`board.cpp` 保持在
100,000 字节以内，测试不依赖 Boost 或其他第三方库。

## 当前状态

- `board.cpp`: 63,772 / 100,000 bytes.
- 单元测试: 11 组，覆盖大整数、数论、图、树、数据结构、字符串、
  几何、DP、滑动窗口、表达式解析和日期。
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

## 题库

- [题库方法与来源](docs/corpus/README.md)
- [机器可读题库索引](docs/corpus/problem-tags.tsv)
- [正确性回放清单](docs/corpus/replay-checklist.md)

题库频次来自当前已整理出的公开题解样本，不等同于官方完整题集统计。
所有推断性标签都在数据表中保留了来源和备注，新增题目时必须同步更新
频次和回放状态。
