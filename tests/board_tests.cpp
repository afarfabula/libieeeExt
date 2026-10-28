#define main ieee_board_unused_main
#include "../board.cpp"
#undef main

#include <cassert>
#include <iostream>

using namespace ib;

#define CHECK(cond) do { \
    if (!(cond)) { \
        std::cerr << "CHECK failed at line " << __LINE__ << ": " #cond "\n"; \
        std::abort(); \
    } \
} while (0)

static void testBigInt() {
    BigInt a("12345678901234567890");
    BigInt b("9876543210987654321");
    CHECK((a + b).toString() == "22222222112222222211");
    CHECK((a - b).toString() == "2469135690246913569");
    CHECK((a * b).toString() ==
          "121932631137021795223746380111126352690");
    CHECK((a / b).toString() == "1");
    CHECK((a % b).toString() == "2469135690246913569");

    BigInt x("999999999999999999999");
    BigInt y("123456789");
    CHECK((x / y).toString() == "8100000073710");
    CHECK((x % y).toString() == "82809");
    CHECK((BigInt(-7) / BigInt(3)).toString() == "-2");
    CHECK((BigInt(-7) % BigInt(3)).toString() == "-1");
    CHECK((BigInt(1) - BigInt(2)).toString() == "-1");
    CHECK(BigInt("00000123").toString() == "123");
}

static void testBigIntDifferential() {
    mt19937_64 rng(20261027);
    for (int tc = 0; tc < 1000; tc++) {
        i64 x = (i64)(rng() % 2000000001ULL) - 1000000000LL;
        i64 y = (i64)(rng() % 2000000001ULL) - 1000000000LL;
        BigInt a(x), b(y);
        CHECK(a.toString() == to_string(x));
        CHECK(b.toString() == to_string(y));
        CHECK((a + b).toString() == to_string(x + y));
        CHECK((a - b).toString() == to_string(x - y));
        CHECK((a * b).toString() == to_string(x * y));
        if (y != 0) {
            CHECK((a / b).toString() == to_string(x / y));
            CHECK((a % b).toString() == to_string(x % y));
        }
    }

    // Fixed large-number oracles were checked independently with Python's int.
    struct Case {
        string a, b, sum, difference, product, quotient, remainder;
    };
    const vector<Case> cases{
        {"999999999999999999999999999999999999999",
         "-123456789012345678901234567890",
         "999999999876543210987654321098765432109",
         "1000000000123456789012345678901234567889",
         "-123456789012345678901234567889999999999876543210987654321098765432110",
         "-8100000072",
         "111111192011111119202111111919"},
        {"-340282366920938463463374607431768211456",
         "18446744073709551616",
         "-340282366920938463444927863358058659840",
         "-340282366920938463481821351505477763072",
         "-6277101735386680763835789423207666416102355444464034512896",
         "-18446744073709551616",
         "0"},
        {"12345678901234567890123456789012345678901234567890",
         "98765432109876543210987654321",
         "12345678901234567890222222221122222222112222222211",
         "12345678901234567890024691356902469135690246913569",
         "1219326311370217952261850327337448559633744855963362292333223746380111126352690",
         "124999998860937500014",
         "23533950614699073961469907396"},
    };
    for (const auto &tc : cases) {
        BigInt a(tc.a), b(tc.b);
        CHECK((a + b).toString() == tc.sum);
        CHECK((a - b).toString() == tc.difference);
        CHECK((a * b).toString() == tc.product);
        CHECK((a / b).toString() == tc.quotient);
        CHECK((a % b).toString() == tc.remainder);
    }
}

static void testNumberTheory() {
    CHECK(powMod(2, 100, 1000000007ULL) == 976371285ULL);
    CHECK(!isPrime(1));
    CHECK(isPrime(2));
    CHECK(isPrime(1000000007ULL));
    CHECK(!isPrime(1000000016000000063ULL));

    auto factors = primeFactors(1000000016000000063ULL);
    CHECK((factors == vector<u64>{1000000007ULL, 1000000009ULL}));
    CHECK((sievePrimes(20) == vector<int>{2, 3, 5, 7, 11, 13, 17, 19}));
    CHECK((divisors(28) == vector<u64>{1, 2, 4, 7, 14, 28}));

    auto [x, mod] = crtCoprime({2, 3, 2}, {3, 5, 7});
    CHECK(x == 23 && mod == 105);
    CHECK(modInverse(3, 7) == 5);

    Comb c(20, 1000000007);
    CHECK(c.choose(10, 3) == 120);
    CHECK(c.permute(5, 2) == 20);
    CHECK(c.catalan(5) == 42);
}

static void testConvolutionAndAutomata() {
    vector<vector<int>> a{{1, 2}, {3, 4}};
    vector<vector<int>> b{{5, 6}, {7, 8}};
    CHECK((convolution2D(a, b) ==
           vector<vector<int>>{{5, 16, 12},
                               {22, 60, 40},
                               {21, 52, 32}}));

    auto next = nextOccurrences("RULDR", "UDLR");
    CHECK(next[3][0] == 0);  // R
    CHECK(next[0][0] == 1);  // U
    CHECK(next[2][0] == 2);  // L
    CHECK(next[1][0] == 3);  // D
    CHECK(next[3][1] == 4);  // the second R
    CHECK(next[0][2] == 5);  // no U remains
}

static void testStructures() {
    Frac f = Frac(1, 3) + Frac(1, 6);
    CHECK(f == Frac(1, 2));

    Matrix<i64> fib(2, 2);
    fib.a = {{1, 1}, {1, 0}};
    auto p = fib.power(10);
    CHECK(p.a[0][1] == 55);

    Matrix<u64> modFib(2, 2);
    modFib.a = {{1, 1}, {1, 0}};
    auto mp = matrixPowMod(modFib, 1000000000ULL, 1000000007ULL);
    CHECK(mp.a[0][1] == 21ULL);

    DSU dsu(6);
    CHECK(dsu.unite(0, 1));
    CHECK(dsu.unite(1, 2));
    CHECK(dsu.same(0, 2));
    CHECK(dsu.size(1) == 3);
    CHECK(dsu.components == 4);

    WeightedDSU wd(4);
    CHECK(wd.unite(0, 1, 5));
    CHECK(wd.unite(1, 2, 7));
    CHECK(wd.unite(0, 2, 12));
    CHECK(!wd.unite(0, 2, 99));
}

static void testGraphs() {
    WeightedAdj g(5);
    auto add = [&](int u, int v, i64 w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    };
    add(0, 1, 2);
    add(0, 2, 7);
    add(1, 2, 1);
    add(2, 3, 3);
    add(1, 4, 20);
    vector<int> parent;
    auto dist = dijkstra(g, 0, &parent);
    CHECK(dist == vector<i64>({0, 2, 3, 6, 22}));
    CHECK((restorePath(parent, 3) == vector<int>{0, 1, 2, 3}));

    vector<Edge> edges{{0, 1, 4}, {0, 2, 1}, {1, 2, 2},
                       {1, 3, 1}, {2, 3, 5}};
    auto [mst, used] = kruskal(4, edges);
    CHECK(mst == 4 && used.size() == 3);

    vector<i64> bf;
    vector<Edge> directed{{0, 1, 4}, {0, 2, 1}, {2, 1, -3}};
    CHECK(bellmanFord(3, directed, 0, bf));
    CHECK(bf == vector<i64>({0, -2, 1}));
    directed.push_back({1, 2, -1});
    CHECK(!bellmanFord(3, directed, 0, bf));

    vector<vector<i64>> fw(3, vector<i64>(3, INF64));
    for (int i = 0; i < 3; i++) fw[i][i] = 0;
    fw[0][1] = 2;
    fw[1][2] = 3;
    fw[0][2] = 9;
    floydWarshall(fw);
    CHECK(fw[0][2] == 5);

    vector<vector<int>> dag(4);
    dag[0] = {1, 2};
    dag[1] = {3};
    dag[2] = {3};
    CHECK((topologicalSort(dag) == vector<int>{0, 1, 2, 3}));
    vector<vector<int>> cyc(2);
    cyc[0] = {1};
    cyc[1] = {0};
    CHECK(topologicalSort(cyc).empty());

    vector<vector<int>> sccg(4);
    sccg[0] = {1};
    sccg[1] = {0, 2};
    sccg[2] = {3};
    sccg[3] = {2};
    TarjanSCC scc(sccg);
    auto comps = scc.run();
    CHECK(comps.size() == 2);

    vector<vector<int>> tree(6);
    auto addTree = [&](int u, int v) {
        tree[u].push_back(v);
        tree[v].push_back(u);
    };
    addTree(0, 1);
    addTree(0, 2);
    addTree(1, 3);
    addTree(1, 4);
    addTree(4, 5);
    BinaryLifting lift(tree, 0);
    CHECK(lift.lca(3, 5) == 1);
    CHECK(lift.lca(2, 3) == 0);
    CHECK(lift.kthAncestor(5, 2) == 1);
    CHECK(lift.distance(3, 5) == 3);
    CHECK(lift.kthAncestor(5, 20) == -1);

    vector<pair<int, int>> undirected{
        {0, 1}, {1, 2}, {2, 0}, {1, 3}, {3, 4}
    };
    auto low = lowLink(5, undirected);
    CHECK((low.bridges == vector<pair<int, int>>{{3, 4}, {1, 3}}));
    CHECK(low.articulation == vector<char>({0, 1, 0, 1, 0}));

    Dinic flow(4);
    flow.addEdge(0, 1, 3);
    flow.addEdge(0, 2, 2);
    flow.addEdge(1, 2, 5);
    flow.addEdge(1, 3, 2);
    flow.addEdge(2, 3, 3);
    CHECK(flow.maxFlow(0, 3) == 5);

    Kuhn match(3, 3);
    match.addEdge(0, 0);
    match.addEdge(0, 1);
    match.addEdge(1, 1);
    match.addEdge(2, 1);
    match.addEdge(2, 2);
    CHECK(match.solve() == 3);

    vector<string> grid{
        "S.#.",
        "..#.",
        "....",
    };
    auto res = gridBFS(grid, 0, 0);
    CHECK(res.dist[2][3] == 5);
    CHECK(res.path(2, 3).front() == make_pair(0, 0));
    CHECK(res.path(2, 3).back() == make_pair(2, 3));
}

static void testDataStructures() {
    Fenwick<i64> bit(6);
    for (int i = 0; i < 6; i++) bit.add(i, i + 1);
    CHECK(bit.prefix(5) == 21);
    CHECK(bit.range(2, 4) == 12);

    FenwickKth order(6);
    for (int i = 0; i < 6; i++) order.add(i, 1);
    order.add(2, -1);
    CHECK(order.kth(0) == 0);
    CHECK(order.kth(2) == 3);

    LazySum lazy(6);
    for (int i = 0; i < 6; i++) lazy.add(i, i, i + 1);
    lazy.add(1, 4, 10);
    CHECK(lazy.query(0, 5) == 61);
    CHECK(lazy.query(1, 4) == 54);

    RangeMin rm({5, 3, 8, 1, 9, 2});
    CHECK(rm.query(1, 4) == 1);
    SparseTable<i64> st({5, 3, 8, 1, 9, 2});
    CHECK(st.query(2, 5) == 1);

    mt19937 rng(1234567);
    vector<i64> naive(31);
    LazySum randomTree(31);
    for (int op = 0; op < 1000; op++) {
        int l = (int)(rng() % 31);
        int r = l + (int)(rng() % (31 - l));
        i64 v = (i64)(rng() % 41) - 20;
        randomTree.add(l, r, v);
        for (int i = l; i <= r; i++) naive[i] += v;
        int ql = (int)(rng() % 31);
        int qr = ql + (int)(rng() % (31 - ql));
        i64 want = accumulate(naive.begin() + ql,
                              naive.begin() + qr + 1, 0LL);
        CHECK(randomTree.query(ql, qr) == want);
    }
}

static void testStrings() {
    CHECK((prefixFunction("ababaca") ==
           vector<int>{0, 0, 1, 2, 3, 0, 1}));
    CHECK((kmpMatches("aaaa", "aa") == vector<int>{0, 1, 2}));
    CHECK((zFunction("aabxaabx") ==
           vector<int>{0, 1, 0, 0, 4, 1, 0, 0}));
    CHECK(manacherLength("forgeeksskeegfor") == 10);

    DoubleHash h("abcdef");
    CHECK(h.get(1, 4) == h.get(1, 4));
    CHECK(h.get(1, 4) != h.get(2, 5));

    Trie trie;
    trie.insert("hello");
    trie.insert("world");
    CHECK(trie.contains("hello"));
    CHECK(!trie.contains("hell"));
    CHECK((suffixArray("banana") == vector<int>{5, 3, 1, 0, 4, 2}));
}

static void testGeometry() {
    Point a(0, 0), b(4, 4), c(0, 4), d(4, 0);
    CHECK(segmentsIntersect(a, b, c, d));
    auto p = lineIntersection(a, b, c, d);
    CHECK(fabsl(p.x - 2) < 1e-12L && fabsl(p.y - 2) < 1e-12L);

    vector<Point> poly{{0, 0}, {4, 0}, {4, 4}, {0, 4}};
    CHECK(polygonArea(poly) == 16);
    CHECK(pointInPolygon(Point(2, 2), poly) == 1);
    CHECK(pointInPolygon(Point(0, 2), poly) == 0);
    CHECK(pointInPolygon(Point(5, 2), poly) == -1);

    vector<PointI> pts{{0, 0}, {2, 0}, {1, 1}, {2, 2}, {0, 2}, {1, 1}};
    auto hull = convexHull(pts);
    CHECK(hull.size() == 4);
}

static void testDPAndBits() {
    CHECK(lisLength({10, 9, 2, 5, 3, 7, 101, 18}) == 4);
    CHECK(lcsLength("AGGTAB", "GXTXAYB") == 4);
    CHECK(knapsack01({1, 3, 4, 5}, {1, 4, 5, 7}, 7) == 9);
    CHECK(minCoins({1, 3, 4}, 6) == 2);
    CHECK(countCoinSequences({1, 2}, 4, 1000000007) == 5);
    CHECK(countSubsetSum({1, 2, 3}, 3, 1000000007) == 2);

    vector<i64> sequence{-2, 1, -3, 4, -1, 2, 1, -5, 4};
    auto best = kadaneWithIndices(sequence);
    CHECK(best.best == 6 && best.left == 3 && best.right == 6);
    CHECK(kadane({-8, -3, -5}) == -3);
    CHECK(maxSubrectangle({{1, 2, -1}, {-2, 3, 4}, {-1, 2, 1}}) == 11);
    CHECK(maxBalancedGroups(4, 3) == 2);
    CHECK(maxBalancedGroups(0, 10000000000LL) == 0);

    vector<i64> sorted{1, 3, 3, 5, 8};
    CHECK(firstTrue(0, 4, [&](i64 i) { return sorted[i] >= 3; }) == 1);
    CHECK(lastTrue(0, 4, [&](i64 i) { return sorted[i] < 5; }) == 2);
    long double root = binarySearchReal(
        0, 2, [](long double x) { return x * x <= 2; });
    CHECK(fabsl(root * root - 2) < 1e-12L);

    vector<i64> zeta(8);
    zeta[0] = 1;
    zeta[1] = 2;
    zeta[2] = 3;
    subsetZetaSum(zeta);
    CHECK(zeta[3] == 6);
    CHECK(zeta[7] == 6);

    XorBasis basis;
    CHECK(basis.insert(1));
    CHECK(basis.insert(2));
    CHECK(!basis.insert(3));
    CHECK(basis.rank() == 2);
    CHECK(basis.maximum() == 3);
    CHECK(basis.contains(3));
}

static void testSlidingWindow() {
    vector<i64> a{2, 1, 3, 5, 4};
    CHECK((windowSums(a, 3) == vector<i64>{6, 9, 12}));
    auto [mins, maxs] = windowExtrema(a, 3);
    CHECK((mins == vector<i64>{1, 1, 3}));
    CHECK((maxs == vector<i64>{3, 5, 5}));
    CHECK(countSubarraysAtMostSum({1, 2, 3}, 3) == 4);
    CHECK(countSubarraysAtMostSum({0, 0, 0}, 0) == 6);
}

static void testHighPriorityRandom() {
    mt19937_64 rng(2026102701);

    for (int tc = 0; tc < 300; tc++) {
        int n = 1 + (int)(rng() % 30);
        vector<i64> a(n);
        for (i64 &x : a) x = (i64)(rng() % 41) - 20;
        i64 want = -INF64;
        for (int l = 0; l < n; l++) {
            i64 sum = 0;
            for (int r = l; r < n; r++) {
                sum += a[r];
                ckmax(want, sum);
            }
        }
        CHECK(kadane(a) == want);
    }

    for (int tc = 0; tc < 300; tc++) {
        int n = 1 + (int)(rng() % 25);
        vector<i64> a(n);
        for (i64 &x : a) x = (i64)(rng() % 10);
        i64 limit = (i64)(rng() % 30);
        i64 want = 0;
        for (int l = 0; l < n; l++) {
            i64 sum = 0;
            for (int r = l; r < n; r++) {
                sum += a[r];
                if (sum <= limit) want++;
            }
        }
        CHECK(countSubarraysAtMostSum(a, limit) == want);
    }

    for (int tc = 0; tc < 100; tc++) {
        int n = 2 + (int)(rng() % 12);
        vector<vector<int>> tree(n);
        for (int v = 1; v < n; v++) {
            int p = (int)(rng() % v);
            tree[p].push_back(v);
            tree[v].push_back(p);
        }
        BinaryLifting lift(tree, 0);
        for (int q = 0; q < 20; q++) {
            int a = (int)(rng() % n);
            int b = (int)(rng() % n);
            vector<char> ancestor(n, false);
            for (int x = a; x != -1; x = lift.up[0][x]) {
                ancestor[x] = true;
                if (x == 0) break;
            }
            int want = b;
            while (!ancestor[want]) want = lift.up[0][want];
            CHECK(lift.lca(a, b) == want);
        }
    }

    for (int tc = 0; tc < 100; tc++) {
        int n = 2 + (int)(rng() % 6);
        set<pair<int, int>> unique;
        for (int v = 1; v < n; v++) {
            int p = (int)(rng() % v);
            unique.insert({min(p, v), max(p, v)});
        }
        int extra = (int)(rng() % 7);
        while (extra--) {
            int a = (int)(rng() % n);
            int b = (int)(rng() % n);
            if (a == b || unique.count({min(a, b), max(a, b)})) continue;
            unique.insert({min(a, b), max(a, b)});
        }
        vector<pair<int, int>> edges(unique.begin(), unique.end());
        auto componentsWithout = [&](int removedVertex, int removedEdge) {
            DSU dsu(n);
            for (int v = 0; v < n; v++)
                if (v != removedVertex) dsu.unite(v, v);
            for (int i = 0; i < (int)edges.size(); i++) {
                if (i == removedEdge) continue;
                auto [a, b] = edges[i];
                if (a != removedVertex && b != removedVertex)
                    dsu.unite(a, b);
            }
            int count = 0;
            for (int v = 0; v < n; v++)
                if (v != removedVertex && dsu.find(v) == v) count++;
            return count;
        };

        auto got = lowLink(n, edges);
        set<pair<int, int>> gotBridges;
        for (auto [a, b] : got.bridges)
            gotBridges.insert({min(a, b), max(a, b)});
        set<pair<int, int>> wantBridges;
        for (int i = 0; i < (int)edges.size(); i++)
            if (componentsWithout(-1, i) > 1)
                wantBridges.insert(edges[i]);
        CHECK(gotBridges == wantBridges);

        for (int v = 0; v < n; v++)
            CHECK((bool)got.articulation[v] ==
                  (componentsWithout(v, -1) > 1));
    }
}

static void testParsingAndDates() {
    CHECK(trim("  abc \n") == "abc");
    CHECK((split("a,\"b,c\", d") ==
           vector<string>{"a", "b,c", "d"}));
    CHECK((words("  a  bb\tc ") == vector<string>{"a", "bb", "c"}));

    ExpressionParser parser("2+3*4^2/(1+1)-5");
    CHECK(fabsl(parser.eval() - 21) < 1e-12L);

    i64 epoch = daysFromCivil(1970, 1, 1);
    CHECK(epoch == 0);
    i64 z = daysFromCivil(2000, 1, 1);
    CHECK(z == 10957);
    auto [y, m, d] = civilFromDays(z);
    CHECK(y == 2000 && m == 1 && d == 1);
    CHECK(leapYear(2000));
    CHECK(!leapYear(1900));
    CHECK(daysInMonth(2024, 2) == 29);

    CHECK(convertBase("101010101010101010101010", 2, 10) == "11184810");
    CHECK(convertBase("11184810", 10, 2) == "101010101010101010101010");
}

int main() {
    testBigInt();
    testBigIntDifferential();
    testNumberTheory();
    testConvolutionAndAutomata();
    testStructures();
    testGraphs();
    testDataStructures();
    testStrings();
    testGeometry();
    testDPAndBits();
    testSlidingWindow();
    testHighPriorityRandom();
    testParsingAndDates();
    std::cout << "all board tests passed\n";
    return 0;
}
