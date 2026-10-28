#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

namespace ib {

using i64 = long long;
const int INF32 = 0x3f3f3f3f;
constexpr int NTT_MOD = 998244353;
constexpr int NTT_ROOT = 3;

int nttPow(int a, int e) {
    i64 r = 1;
    while (e) {
        if (e & 1) r = r * a % NTT_MOD;
        a = (int)((i64)a * a % NTT_MOD);
        e >>= 1;
    }
    return (int)r;
}

void ntt(vector<int> &a, bool invert) {
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        int wlen = nttPow(NTT_ROOT, (NTT_MOD - 1) / len);
        if (invert) wlen = nttPow(wlen, NTT_MOD - 2);
        for (int i = 0; i < n; i += len) {
            i64 w = 1;
            for (int j = 0; j < len / 2; j++) {
                int u = a[i + j];
                int v = (int)(w * a[i + j + len / 2] % NTT_MOD);
                int sum = u + v;
                if (sum >= NTT_MOD) sum -= NTT_MOD;
                int diff = u - v;
                if (diff < 0) diff += NTT_MOD;
                a[i + j] = sum;
                a[i + j + len / 2] = diff;
                w = w * wlen % NTT_MOD;
            }
        }
    }
    if (invert) {
        int invN = nttPow(n, NTT_MOD - 2);
        for (int &x : a) x = (int)((i64)x * invN % NTT_MOD);
    }
}

int nttSize(int need) {
    int size = 1;
    while (size < need) size <<= 1;
    return size;
}

vector<vector<int>> convolution2D(const vector<vector<int>> &a,
                                  const vector<vector<int>> &b) {
    int ar = (int)a.size(), ac = ar ? (int)a[0].size() : 0;
    int br = (int)b.size(), bc = br ? (int)b[0].size() : 0;
    int outR = ar + br - 1, outC = ac + bc - 1;
    int width = nttSize(max(1, outC));
    int size = nttSize(max(1, outR * width));
    vector<int> x(size), y(size);
    for (int i = 0; i < ar; i++)
        for (int j = 0; j < ac; j++) x[i * width + j] = a[i][j];
    for (int i = 0; i < br; i++)
        for (int j = 0; j < bc; j++) y[i * width + j] = b[i][j];
    ntt(x, false);
    ntt(y, false);
    for (int i = 0; i < size; i++)
        x[i] = (int)((i64)x[i] * y[i] % NTT_MOD);
    ntt(x, true);
    vector<vector<int>> result(outR, vector<int>(outC));
    for (int i = 0; i < outR; i++)
        for (int j = 0; j < outC; j++)
            result[i][j] = x[i * width + j];
    return result;
}

vector<vector<int>> nextOccurrences(const string &s, const string &alphabet) {
    int n = (int)s.size();
    vector<vector<int>> next(alphabet.size(), vector<int>(n + 1, n));
    for (int i = n - 1; i >= 0; i--) {
        for (int d = 0; d < (int)alphabet.size(); d++)
            next[d][i] = next[d][i + 1];
        int d = (int)alphabet.find(s[i]);
        if (d != (int)string::npos) next[d][i] = i;
    }
    return next;
}

void solve() {
    int R, C, N;
    if (!(cin >> R >> C >> N)) return;
    vector<string> grid(R);
    for (string &row : grid) cin >> row;

    int sr = -1, sc = -1, tr = -1, tc = -1;
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            if (grid[r][c] == 'S') sr = r, sc = c;
            if (grid[r][c] == 'T') tr = r, tc = c;
        }
    }

    string commands;
    cin >> commands;
    const string alphabet = "UDLR";
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    vector<pair<int, int>> offsets;
    offsets.reserve(commands.size() + 1);
    int pr = 0, pc = 0;
    int minR = 0, maxR = 0, minC = 0, maxC = 0;
    offsets.push_back({0, 0});
    for (char ch : commands) {
        int d = (int)alphabet.find(ch);
        pr += dr[d];
        pc += dc[d];
        offsets.push_back({pr, pc});
        minR = min(minR, pr);
        maxR = max(maxR, pr);
        minC = min(minC, pc);
        maxC = max(maxC, pc);
    }

    int total = R * C;
    vector<char> whole(total, false);
    int patternH = maxR - minR + 1;
    int patternW = maxC - minC + 1;
    if (patternH <= R && patternW <= C) {
        vector<vector<int>> walls(R, vector<int>(C));
        for (int r = 0; r < R; r++)
            for (int c = 0; c < C; c++)
                walls[r][c] = grid[r][c] == '#';

        vector<vector<int>> reversedPattern(
            patternH, vector<int>(patternW));
        for (auto [r, c] : offsets)
            reversedPattern[maxR - r][maxC - c] = 1;

        auto hit = convolution2D(walls, reversedPattern);
        for (int r = 0; r < R; r++) {
            if (r + minR < 0 || r + maxR >= R) continue;
            for (int c = 0; c < C; c++) {
                if (c + minC < 0 || c + maxC >= C) continue;
                whole[r * C + c] = hit[r + maxR][c + maxC] == 0;
            }
        }
    }

    auto next = nextOccurrences(commands, alphabet);
    vector<int> gold(total, INF32);
    vector<int> used(total, (int)commands.size() + 1);
    using State = tuple<int, int, int>;
    priority_queue<State, vector<State>, greater<State>> pq;
    int start = sr * C + sc;
    gold[start] = 0;
    used[start] = N;
    pq.push({0, N, start});

    auto relax = [&](int idx, int newGold, int newUsed) {
        if (newGold > gold[idx]) return;
        if (newGold == gold[idx] && newUsed >= used[idx]) return;
        gold[idx] = newGold;
        used[idx] = newUsed;
        pq.push({newGold, newUsed, idx});
    };

    while (!pq.empty()) {
        auto [g, pos, idx] = pq.top();
        pq.pop();
        if (g != gold[idx] || pos != used[idx]) continue;
        int r = idx / C, c = idx % C;

        for (int d = 0; d < 4; d++) {
            int nextPosition = next[d][pos];
            int addGold = 0;
            if (nextPosition == N) {
                nextPosition = next[d][0];
                addGold = 1;
            }
            if (nextPosition == N) continue;
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
            if (grid[nr][nc] == '#') continue;
            relax(nr * C + nc, g + addGold, nextPosition + 1);
        }

        if (whole[idx]) {
            int nr = r + pr, nc = c + pc;
            if (nr >= 0 && nr < R && nc >= 0 && nc < C &&
                grid[nr][nc] != '#')
                relax(nr * C + nc, g, N);
        }
    }

    int target = tr * C + tc;
    int answer = gold[target];
    cout << (answer == INF32 ? -1 : answer) << '\n';
}

} // namespace ib

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ib::solve();
    return 0;
}
