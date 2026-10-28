#include "board.cpp"

void solve() {
    int R, C, N;
    if (!ib::in.read(R) || !ib::in.read(C) || !ib::in.read(N)) return;
    vector<string> grid(R);
    for (string &row : grid) ib::in.read(row);

    int sr = -1, sc = -1, tr = -1, tc = -1;
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            if (grid[r][c] == 'S') sr = r, sc = c;
            if (grid[r][c] == 'T') tr = r, tc = c;
        }
    }

    string commands;
    ib::in.read(commands);
    if (sr < 0 || tr < 0) {
        ib::out.line(-1);
        return;
    }

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
        ib::ckmin(minR, pr);
        ib::ckmax(maxR, pr);
        ib::ckmin(minC, pc);
        ib::ckmax(maxC, pc);
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

        auto hit = ib::convolution2D(walls, reversedPattern);
        for (int r = 0; r < R; r++) {
            if (r + minR < 0 || r + maxR >= R) continue;
            for (int c = 0; c < C; c++) {
                if (c + minC < 0 || c + maxC >= C) continue;
                whole[r * C + c] = hit[r + maxR][c + maxC] == 0;
            }
        }
    }

    auto next = ib::nextOccurrences(commands, alphabet);
    vector<int> gold(total, ib::INF32);
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
                relax(nr * C + nc, g, pos);
        }
    }

    int answer = gold[tr * C + tc];
    ib::out.line(answer == ib::INF32 ? -1 : answer);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}

