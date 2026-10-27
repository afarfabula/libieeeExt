#define main ieee_board_unused_main
#include "../board.cpp"
#undef main

#include <cassert>
#include <iostream>

namespace replay {

using namespace ib;

// IEEEXtreme 10.0, Dog Walking.
// Sample: https://github.com/ieeesbspain/IEEEXtreme/tree/master/challenges/ieeextreme10/P01%20-%20Dog%20Walking
i64 dogWalking(vector<i64> dogs, int groups) {
    sort(dogs.begin(), dogs.end());
    i64 answer = dogs.back() - dogs.front();
    vector<i64> gaps;
    for (int i = 1; i < (int)dogs.size(); i++)
        gaps.push_back(dogs[i] - dogs[i - 1]);
    sort(gaps.rbegin(), gaps.rend());
    for (int i = 0; i < groups - 1; i++) answer -= gaps[i];
    return answer;
}

// IEEEXtreme 10.0, Full Adder.
// Sample: https://github.com/ieeesbspain/IEEEXtreme/tree/master/challenges/ieeextreme10/P16%20-%20Full%20Adder
string addInCustomBase(const string &lhs, const string &rhs,
                       const string &symbols) {
    int base = (int)symbols.size();
    vector<int> digit(256, -1);
    for (int i = 0; i < base; i++)
        digit[(unsigned char)symbols[i]] = i;

    string answer;
    int i = (int)lhs.size() - 1;
    int j = (int)rhs.size() - 1;
    int carry = 0;
    while (i >= 0 || j >= 0 || carry) {
        int value = carry;
        if (i >= 0) value += digit[(unsigned char)lhs[i--]];
        if (j >= 0) value += digit[(unsigned char)rhs[j--]];
        answer.push_back(symbols[value % base]);
        carry = value / base;
    }
    while (answer.size() > 1 && answer.back() == symbols[0])
        answer.pop_back();
    reverse(answer.begin(), answer.end());
    return answer;
}

// IEEEXtreme 10.0, N-Palindromes.
// Sample: https://github.com/ieeesbspain/IEEEXtreme/tree/master/challenges/ieeextreme10/P21%20-%20N-Palindromes
i64 nPalindromes(const string &s, int wanted) {
    constexpr i64 MOD = 1000000007;
    vector<i64> dp(wanted + 1);
    dp[0] = 1;
    auto multiply = [&](const vector<pair<int, i64>> &options) {
        vector<i64> next(wanted + 1);
        for (int used = 0; used <= wanted; used++) {
            if (!dp[used]) continue;
            for (auto [changed, ways] : options) {
                if (used + changed > wanted) continue;
                next[used + changed] =
                    (next[used + changed] + dp[used] * ways) % MOD;
            }
        }
        dp.swap(next);
    };

    int pairCount = (int)s.size() / 2;
    for (int i = 0; i < pairCount; i++) {
        if (s[i] == s[s.size() - 1 - i]) {
            multiply({{0, 1}, {2, 25}});
        } else {
            multiply({{1, 2}, {2, 24}});
        }
    }
    if (s.size() % 2) multiply({{0, 1}, {1, 25}});
    return dp[wanted];
}

} // namespace replay

int main() {
    assert(replay::dogWalking({3, 5, 1, 1}, 2) == 2);
    assert(replay::dogWalking({30, 40, 20, 41, 50}, 4) == 1);

    assert(replay::addInCustomBase("752", "76045", "0123456789") == "76797");
    assert(replay::addInCustomBase("H", "8J4J", "wj8Ma04HJg") == "8JH0");

    assert(replay::nPalindromes("alice", 2) == 4);
    assert(replay::nPalindromes("racecar", 1) == 25);
    assert(replay::nPalindromes("alice", 3) == 196);

    std::cout << "all historical problem replays passed\n";
    return 0;
}
