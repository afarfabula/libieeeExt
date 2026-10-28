// IEEE Xtreme targeted C++17 board
// Version: 2026-10-28
//
// Design priorities based on a sample of IEEEXtreme solution repositories:
//   1. robust input/parsing and integer boundaries;
//   2. simulation, enumeration, prefix sums and sorting;
//   3. BFS/DFS, DSU, shortest paths and basic trees;
//   4. number theory, big integers, matrices and combinatorics;
//   5. strings, grids, 2D geometry and compact DP helpers.
//
// This file is intentionally self-contained and uses only GCC/Clang C++17.
// Put contest-specific code in solve(). All reusable code lives in namespace ib.
// The whole file is kept below 100,000 bytes by the project's size check.

#include <algorithm>
#include <array>
#include <bitset>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;

namespace ib {

using i64 = long long;
using u64 = unsigned long long;
using i128 = __int128_t;
using u128 = __uint128_t;

const i64 INF64 = (i64)4e18;
const int INF32 = 0x3f3f3f3f;
const long double EPS = 1e-12L;
const long double PI = acosl(-1.0L);

template <class T> T absval(T x) { return x < 0 ? -x : x; }
template <class T> void ckmin(T &a, const T &b) { if (b < a) a = b; }
template <class T> void ckmax(T &a, const T &b) { if (b > a) a = b; }
template <class T> int sgn(T x) { return (x > 0) - (x < 0); }

u64 splitmix64(u64 x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

// ---------------------------------------------------------------------------
// IO
// ---------------------------------------------------------------------------

class Scanner {
    static constexpr int SZ = 1 << 20;
    char buf[SZ];
    int pos = 0, len = 0;

    char raw() {
        if (pos == len) {
            len = (int)fread(buf, 1, SZ, stdin);
            pos = 0;
            if (len <= 0) return 0;
        }
        return buf[pos++];
    }

    char skip() {
        char c = raw();
        while (c && c <= ' ') c = raw();
        return c;
    }

public:
    template <class T,
              typename enable_if<is_integral<T>::value, int>::type = 0>
    bool read(T &out) {
        char c = skip();
        if (!c) return false;
        bool neg = false;
        if (c == '-' || c == '+') {
            neg = c == '-';
            c = raw();
        }
        using U = typename make_unsigned<T>::type;
        U x = 0;
        while (c >= '0' && c <= '9') {
            x = x * 10 + (U)(c - '0');
            c = raw();
        }
        out = neg ? (T)(0 - x) : (T)x;
        return true;
    }

    bool read(long double &out) {
        char c = skip();
        if (!c) return false;
        string s;
        if (c == '-' || c == '+') {
            s.push_back(c);
            c = raw();
        }
        while (c && c > ' ' && c != ',') {
            s.push_back(c);
            c = raw();
        }
        char *end = nullptr;
        out = strtold(s.c_str(), &end);
        return end != s.c_str();
    }

    bool read(double &out) {
        long double x;
        if (!read(x)) return false;
        out = (double)x;
        return true;
    }

    bool read(char &out) {
        out = skip();
        return out != 0;
    }

    bool read(string &out) {
        out.clear();
        char c = skip();
        if (!c) return false;
        while (c && c > ' ') {
            out.push_back(c);
            c = raw();
        }
        return true;
    }

    string line() {
        string s;
        char c = raw();
        while (c != '\n' && c) {
            if (c != '\r') s.push_back(c);
            c = raw();
        }
        return s;
    }

    template <class T>
    T next() {
        T x{};
        read(x);
        return x;
    }

    template <class T>
    vector<T> nextArray(int n) {
        vector<T> a(n);
        for (auto &x : a) read(x);
        return a;
    }
};

class Printer {
    static constexpr int SZ = 1 << 20;
    char buf[SZ];
    int pos = 0;

    void flushIfFull() {
        if (pos == SZ) flush();
    }

public:
    ~Printer() { flush(); }

    void flush() {
        if (pos) fwrite(buf, 1, pos, stdout);
        pos = 0;
    }

    void put(char c) {
        flushIfFull();
        buf[pos++] = c;
    }

    void write(const string &s) {
        for (char c : s) put(c);
    }

    void write(const char *s) {
        while (*s) put(*s++);
    }

    template <class T,
              typename enable_if<is_integral<T>::value, int>::type = 0>
    void write(T x) {
        if (x == 0) {
            put('0');
            return;
        }
        if (is_signed<T>::value && x < 0) {
            put('-');
            using U = typename make_unsigned<T>::type;
            U y = (U)(-(x + 1)) + 1;
            write(y);
            return;
        }
        using U = typename make_unsigned<T>::type;
        U y = (U)x;
        char t[32];
        int n = 0;
        while (y) {
            t[n++] = char('0' + y % 10);
            y /= 10;
        }
        while (n) put(t[--n]);
    }

    void write(long double x, int digits = 10) {
        char t[128];
        snprintf(t, sizeof(t), "%.*Lf", digits, x);
        write(t);
    }

    void write(double x, int digits = 10) {
        write((long double)x, digits);
    }

    template <class T>
    void line(const T &x) {
        write(x);
        put('\n');
    }

    void space() { put(' '); }
};

inline Scanner in;
inline Printer out;

// ---------------------------------------------------------------------------
// Arbitrary-precision integer
// ---------------------------------------------------------------------------

class BigInt {
    static constexpr u64 BASE = 1000000000ULL;
    static constexpr int WIDTH = 9;

public:
    using Limb = uint32_t;
    vector<Limb> v;
    int sign;

    BigInt() : sign(0) {}
    BigInt(long long x) { assign(x); }

    explicit BigInt(const string &s) { parse(s); }

    void normalize() {
        while (!v.empty() && v.back() == 0) v.pop_back();
        if (v.empty()) sign = 0;
        else if (!sign) sign = 1;
    }

    void assign(long long x) {
        v.clear();
        sign = sgn(x);
        if (!x) return;
        if (x < 0) {
            unsigned long long y = (unsigned long long)(-(x + 1)) + 1;
            while (y) {
                v.push_back((Limb)(y % BASE));
                y /= BASE;
            }
        } else {
            unsigned long long y = (unsigned long long)x;
            while (y) {
                v.push_back((Limb)(y % BASE));
                y /= BASE;
            }
        }
    }

    void parse(string s) {
        v.clear();
        sign = 0;
        int i = 0;
        while (i < (int)s.size() && s[i] <= ' ') i++;
        bool neg = false;
        if (i < (int)s.size() && (s[i] == '+' || s[i] == '-')) {
            neg = s[i] == '-';
            i++;
        }
        while (i < (int)s.size() && s[i] == '0') i++;
        if (i == (int)s.size()) return;
        int end = (int)s.size();
        while (end > i && s[end - 1] <= ' ') end--;
        for (int r = end; r > i; r -= WIDTH) {
            int l = max(i, r - WIDTH);
            Limb x = 0;
            for (int k = l; k < r; k++) {
                if (s[k] < '0' || s[k] > '9')
                    throw invalid_argument("invalid BigInt");
                x = x * 10 + (Limb)(s[k] - '0');
            }
            v.push_back(x);
        }
        sign = neg ? -1 : 1;
        normalize();
    }

    bool isZero() const { return sign == 0; }
    size_t size() const { return v.size(); }

    static int cmpAbs(const BigInt &a, const BigInt &b) {
        if (a.v.size() != b.v.size()) return a.v.size() < b.v.size() ? -1 : 1;
        for (int i = (int)a.v.size() - 1; i >= 0; i--) {
            if (a.v[i] != b.v[i]) return a.v[i] < b.v[i] ? -1 : 1;
        }
        return 0;
    }

    static BigInt fromAbs(const vector<Limb> &x) {
        BigInt z;
        z.v = x;
        z.sign = z.v.empty() ? 0 : 1;
        z.normalize();
        return z;
    }

    static vector<Limb> addAbs(const vector<Limb> &a, const vector<Limb> &b) {
        vector<Limb> r(max(a.size(), b.size()) + 1, 0);
        u64 carry = 0;
        for (size_t i = 0; i < r.size(); i++) {
            u64 s = carry;
            if (i < a.size()) s += a[i];
            if (i < b.size()) s += b[i];
            r[i] = (Limb)(s % BASE);
            carry = s / BASE;
        }
        while (!r.empty() && !r.back()) r.pop_back();
        return r;
    }

    // Requires |a| >= |b|.
    static vector<Limb> subAbs(const vector<Limb> &a, const vector<Limb> &b) {
        vector<Limb> r(a.size(), 0);
        i64 borrow = 0;
        for (size_t i = 0; i < a.size(); i++) {
            i64 x = (i64)a[i] - borrow - (i < b.size() ? b[i] : 0);
            if (x < 0) {
                x += BASE;
                borrow = 1;
            } else {
                borrow = 0;
            }
            r[i] = (Limb)x;
        }
        while (!r.empty() && !r.back()) r.pop_back();
        return r;
    }

    BigInt mulSmall(u64 x) const {
        if (!x || isZero()) return BigInt(0);
        BigInt r;
        r.sign = sign;
        r.v.assign(v.size() + 2, 0);
        u64 carry = 0;
        for (size_t i = 0; i < v.size(); i++) {
            u64 cur = (u64)v[i] * x + carry;
            r.v[i] = (Limb)(cur % BASE);
            carry = cur / BASE;
        }
        size_t i = v.size();
        while (carry) {
            r.v[i++] = (Limb)(carry % BASE);
            carry /= BASE;
        }
        r.normalize();
        return r;
    }

    pair<BigInt, int> divSmall(int x) const {
        if (!x) throw domain_error("division by zero");
        BigInt q;
        q.v.assign(v.size(), 0);
        q.sign = sign * sgn(x);
        u64 rem = 0;
        u64 d = (u64)absval((long long)x);
        for (int i = (int)v.size() - 1; i >= 0; i--) {
            u64 cur = rem * BASE + v[i];
            q.v[i] = (Limb)(cur / d);
            rem = cur % d;
        }
        q.normalize();
        return {q, (int)rem};
    }

    static vector<Limb> mulAbs(const vector<Limb> &a, const vector<Limb> &b) {
        if (a.empty() || b.empty()) return {};
        vector<Limb> r(a.size() + b.size(), 0);
        for (size_t i = 0; i < a.size(); i++) {
            u64 carry = 0;
            for (size_t j = 0; j < b.size(); j++) {
                u64 cur = (u64)a[i] * b[j] + r[i + j] + carry;
                r[i + j] = (Limb)(cur % BASE);
                carry = cur / BASE;
            }
            size_t k = i + b.size();
            while (carry) {
                u64 cur = (u64)r[k] + carry;
                r[k++] = (Limb)(cur % BASE);
                carry = cur / BASE;
            }
        }
        while (!r.empty() && !r.back()) r.pop_back();
        return r;
    }

    // Unsigned long division. Returns {quotient, remainder}.
    static pair<BigInt, BigInt> divmodAbs(const BigInt &a, const BigInt &b) {
        if (b.isZero()) throw domain_error("division by zero");
        if (cmpAbs(a, b) < 0) return {BigInt(0), fromAbs(a.v)};
        BigInt q, rem;
        q.v.assign(a.v.size(), 0);
        q.sign = 1;
        for (int i = (int)a.v.size() - 1; i >= 0; i--) {
            if (!rem.isZero()) rem.v.insert(rem.v.begin(), a.v[i]);
            else if (a.v[i]) {
                rem.v.clear();
                rem.v.push_back(a.v[i]);
                rem.sign = 1;
            }
            Limb lo = 0, hi = (Limb)(BASE - 1), best = 0;
            BigInt bestProduct(0);
            while (lo <= hi) {
                Limb mid = lo + (Limb)(((u64)hi - lo) / 2);
                BigInt product = b.mulSmall(mid);
                int c = cmpAbs(product, rem);
                if (c <= 0) {
                    best = mid;
                    bestProduct = std::move(product);
                    if (mid == hi) break;
                    lo = mid + 1;
                } else {
                    if (!mid) break;
                    hi = mid - 1;
                }
            }
            q.v[i] = best;
            rem = fromAbs(subAbs(rem.v, bestProduct.v));
        }
        q.normalize();
        rem.normalize();
        return {q, rem};
    }

    int compare(const BigInt &b) const {
        if (sign != b.sign) return sign < b.sign ? -1 : 1;
        if (!sign) return 0;
        int c = cmpAbs(*this, b);
        return sign > 0 ? c : -c;
    }

    bool operator<(const BigInt &b) const { return compare(b) < 0; }
    bool operator>(const BigInt &b) const { return compare(b) > 0; }
    bool operator<=(const BigInt &b) const { return compare(b) <= 0; }
    bool operator>=(const BigInt &b) const { return compare(b) >= 0; }
    bool operator==(const BigInt &b) const { return compare(b) == 0; }
    bool operator!=(const BigInt &b) const { return compare(b) != 0; }

    BigInt operator-() const {
        BigInt r = *this;
        r.sign = -r.sign;
        return r;
    }

    BigInt operator+(const BigInt &b) const {
        if (isZero()) return b;
        if (b.isZero()) return *this;
        if (sign == b.sign) return fromAbs(addAbs(v, b.v)) * sign;
        int c = cmpAbs(*this, b);
        if (!c) return BigInt(0);
        if (c > 0) return fromAbs(subAbs(v, b.v)) * sign;
        return fromAbs(subAbs(b.v, v)) * b.sign;
    }

    BigInt operator-(const BigInt &b) const { return *this + (-b); }

    BigInt operator*(const BigInt &b) const {
        BigInt r = fromAbs(mulAbs(v, b.v));
        if (!r.isZero()) r.sign = sign * b.sign;
        return r;
    }

    BigInt operator*(long long x) const { return *this * BigInt(x); }
    BigInt operator+(long long x) const { return *this + BigInt(x); }
    BigInt operator-(long long x) const { return *this - BigInt(x); }

    BigInt &operator+=(const BigInt &b) { return *this = *this + b; }
    BigInt &operator-=(const BigInt &b) { return *this = *this - b; }
    BigInt &operator*=(const BigInt &b) { return *this = *this * b; }

    BigInt operator/(const BigInt &b) const {
        auto p = divmodAbs(BigInt::fromAbs(v), BigInt::fromAbs(b.v));
        BigInt q = p.first;
        if (!q.isZero()) q.sign = sign * b.sign;
        return q;
    }

    BigInt operator%(const BigInt &b) const {
        auto p = divmodAbs(BigInt::fromAbs(v), BigInt::fromAbs(b.v));
        BigInt r = p.second;
        if (!r.isZero()) r.sign = sign;
        return r;
    }

    BigInt &operator/=(const BigInt &b) { return *this = *this / b; }
    BigInt &operator%=(const BigInt &b) { return *this = *this % b; }

    string toString() const {
        if (isZero()) return "0";
        string s = sign < 0 ? "-" : "";
        s += to_string(v.back());
        for (int i = (int)v.size() - 2; i >= 0; i--) {
            string t = to_string(v[i]);
            s.append(WIDTH - (int)t.size(), '0');
            s += t;
        }
        return s;
    }

    long double toLongDouble() const {
        long double r = 0;
        for (int i = (int)v.size() - 1; i >= 0; i--)
            r = r * BASE + v[i];
        return sign < 0 ? -r : r;
    }

    BigInt power(u64 e) const {
        BigInt a = *this, r(1);
        while (e) {
            if (e & 1) r *= a;
            e >>= 1;
            if (e) a *= a;
        }
        return r;
    }

    friend ostream &operator<<(ostream &os, const BigInt &x) {
        return os << x.toString();
    }
    friend istream &operator>>(istream &is, BigInt &x) {
        string s;
        is >> s;
        x.parse(s);
        return is;
    }
};

// ---------------------------------------------------------------------------
// Number theory and modular arithmetic
// ---------------------------------------------------------------------------

inline u64 mulMod(u64 a, u64 b, u64 mod) {
    if (!mod) return 0;
    return (u64)((u128)a * b % mod);
}

inline u64 powMod(u64 a, u64 e, u64 mod) {
    if (!mod) return 0;
    a %= mod;
    u64 r = mod == 1 ? 0 : 1;
    while (e) {
        if (e & 1) r = mulMod(r, a, mod);
        a = mulMod(a, a, mod);
        e >>= 1;
    }
    return r;
}

// ---------------------------------------------------------------------------
// NTT and 2D convolution, modulus 998244353
// ---------------------------------------------------------------------------

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
    // A row stride of at least outC prevents column carries in the flat index.
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

inline u64 gcdU(u64 a, u64 b) {
    while (b) {
        u64 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

// Returns g = ax + by.
i64 extGcd(i64 a, i64 b, i64 &x, i64 &y) {
    if (!b) {
        x = a < 0 ? -1 : 1;
        y = 0;
        return absval(a);
    }
    i64 x1, y1;
    i64 g = extGcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// Extended gcd in 128 bits, useful when products can exceed int64.
i128 extGcd128(i128 a, i128 b, i128 &x, i128 &y) {
    if (!b) {
        x = a < 0 ? -1 : 1;
        y = 0;
        return a < 0 ? -a : a;
    }
    i128 x1, y1;
    i128 g = extGcd128(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// Inverse modulo a prime, or modulo a modulus coprime to a.
inline i64 modInverse(i64 a, i64 mod) {
    i128 x, y;
    i128 g = extGcd128(a % mod, mod, x, y);
    if (g != 1) throw domain_error("inverse does not exist");
    x %= mod;
    if (x < 0) x += mod;
    return (i64)x;
}

inline bool isPrime(u64 n) {
    if (n < 2) return false;
    static const int small[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (u64 p : small) {
        if (n % p == 0) return n == p;
    }
    u64 d = n - 1;
    int s = 0;
    while (!(d & 1)) {
        d >>= 1;
        s++;
    }
    // Deterministic for every 64-bit unsigned integer.
    static const u64 bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (u64 a : bases) {
        if (a % n == 0) continue;
        u64 x = powMod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool witness = true;
        for (int r = 1; r < s; r++) {
            x = mulMod(x, x, n);
            if (x == n - 1) {
                witness = false;
                break;
            }
        }
        if (witness) return false;
    }
    return true;
}

u64 pollardRho(u64 n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    static mt19937_64 rng(
        chrono::steady_clock::now().time_since_epoch().count());
    while (true) {
        u64 c = uniform_int_distribution<u64>(1, n - 1)(rng);
        u64 x = uniform_int_distribution<u64>(0, n - 1)(rng);
        u64 y = x, d = 1;
        auto f = [&](u64 z) { return (mulMod(z, z, n) + c) % n; };
        while (d == 1) {
            x = f(x);
            y = f(f(y));
            d = gcdU(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}

void factorRec(u64 n, vector<u64> &fac) {
    if (n == 1) return;
    if (isPrime(n)) {
        fac.push_back(n);
        return;
    }
    u64 d = pollardRho(n);
    factorRec(d, fac);
    factorRec(n / d, fac);
}

vector<u64> primeFactors(u64 n) {
    vector<u64> f;
    factorRec(n, f);
    sort(f.begin(), f.end());
    return f;
}

map<u64, int> factorMap(u64 n) {
    map<u64, int> m;
    for (u64 p : primeFactors(n)) m[p]++;
    return m;
}

vector<u64> divisors(u64 n) {
    auto f = factorMap(n);
    vector<u64> d{1};
    for (auto [p, e] : f) {
        int old = (int)d.size();
        u64 pw = 1;
        for (int k = 1; k <= e; k++) {
            pw *= p;
            for (int i = 0; i < old; i++) d.push_back(d[i] * pw);
        }
    }
    sort(d.begin(), d.end());
    return d;
}

vector<int> sievePrimes(int n) {
    vector<bool> prime(n + 1, true);
    if (n >= 0) prime[0] = false;
    if (n >= 1) prime[1] = false;
    for (int i = 2; (i64)i * i <= n; i++) {
        if (!prime[i]) continue;
        for (i64 j = (i64)i * i; j <= n; j += i)
            prime[j] = false;
    }
    vector<int> p;
    for (int i = 2; i <= n; i++)
        if (prime[i]) p.push_back(i);
    return p;
}

// Smallest prime factor for every value up to n.
vector<int> smallestPrimeFactor(int n) {
    vector<int> spf(n + 1);
    for (int i = 2; i <= n; i++) {
        if (spf[i]) continue;
        spf[i] = i;
        if ((i64)i * i <= n) {
            for (i64 j = (i64)i * i; j <= n; j += i)
                if (!spf[j]) spf[j] = i;
        }
    }
    return spf;
}

// Chinese remainder theorem for coprime moduli.
// Result x is in [0, lcm), and lcm is returned.
pair<i128, i128> crtCoprime(const vector<i64> &rem,
                            const vector<i64> &mod) {
    i128 x = 0, M = 1;
    for (size_t i = 0; i < mod.size(); i++) {
        i64 inv = modInverse((i64)(M % mod[i]), mod[i]);
        i128 t = ((i128)((rem[i] - x) % mod[i] + mod[i]) % mod[i]) * inv;
        x += t % mod[i] * M;
        M *= mod[i];
        x %= M;
    }
    return {x, M};
}

class Comb {
    int n;
    i64 mod;
    vector<i64> fact, invFact;

public:
    Comb(int maxN, i64 modulus)
        : n(max(0, maxN)), mod(modulus), fact(n + 1), invFact(n + 1) {
        fact[0] = 1;
        for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i % mod;
        invFact[n] = modInverse(fact[n], mod);
        for (int i = n; i >= 1; i--) invFact[i - 1] = invFact[i] * i % mod;
    }

    i64 choose(int a, int b) const {
        if (a < 0 || b < 0 || b > a || a > n) return 0;
        return fact[a] * invFact[b] % mod * invFact[a - b] % mod;
    }

    i64 permute(int a, int b) const {
        if (a < 0 || b < 0 || b > a || a > n) return 0;
        return fact[a] * invFact[a - b] % mod;
    }

    i64 catalan(int k) const {
        if (k < 0 || 2 * k > n) return 0;
        i64 c = choose(2 * k, k);
        return c * modInverse(k + 1, mod) % mod;
    }

    i64 factorial(int k) const {
        return 0 <= k && k <= n ? fact[k] : 0;
    }
};

// ---------------------------------------------------------------------------
// Rational number (int64 based)
// ---------------------------------------------------------------------------

struct Frac {
    i64 a = 0, b = 1;

    Frac() = default;
    Frac(i64 x) : a(x), b(1) {}
    Frac(i64 x, i64 y) : a(x), b(y) { normalize(); }

    void normalize() {
        if (b < 0) a = -a, b = -b;
        i64 g = std::gcd(absval(a), b);
        if (!g) return;
        a /= g;
        b /= g;
    }

    long double value() const { return (long double)a / b; }

    Frac operator+(const Frac &o) const {
        return Frac(a * o.b + o.a * b, b * o.b);
    }
    Frac operator-(const Frac &o) const {
        return Frac(a * o.b - o.a * b, b * o.b);
    }
    Frac operator*(const Frac &o) const {
        return Frac(a * o.a, b * o.b);
    }
    Frac operator/(const Frac &o) const {
        return Frac(a * o.b, b * o.a);
    }

    bool operator<(const Frac &o) const {
        return (i128)a * o.b < (i128)o.a * b;
    }
    bool operator==(const Frac &o) const {
        return (i128)a * o.b == (i128)o.a * b;
    }
};

// ---------------------------------------------------------------------------
// Matrix
// ---------------------------------------------------------------------------

template <class T>
struct Matrix {
    int n, m;
    vector<vector<T>> a;

    Matrix(int r = 0, int c = 0, T val = T{})
        : n(r), m(c), a(r, vector<T>(c, val)) {}

    static Matrix identity(int n) {
        Matrix r(n, n, T{});
        for (int i = 0; i < n; i++) r.a[i][i] = T(1);
        return r;
    }

    Matrix operator*(const Matrix &o) const {
        Matrix r(n, o.m, T{});
        for (int i = 0; i < n; i++)
            for (int k = 0; k < m; k++) {
                if (a[i][k] == T{}) continue;
                for (int j = 0; j < o.m; j++)
                    r.a[i][j] += a[i][k] * o.a[k][j];
            }
        return r;
    }

    Matrix power(u64 e) const {
        Matrix base = *this;
        Matrix r = identity(n);
        while (e) {
            if (e & 1) r = r * base;
            e >>= 1;
            if (e) base = base * base;
        }
        return r;
    }
};

template <class T>
Matrix<T> matrixPow(Matrix<T> a, u64 e) {
    return a.power(e);
}

Matrix<u64> matrixPowMod(Matrix<u64> a, u64 e, u64 mod) {
    int n = a.n;
    Matrix<u64> r = Matrix<u64>::identity(n);
    auto multiply = [&](const Matrix<u64> &x, const Matrix<u64> &y) {
        Matrix<u64> z(n, n);
        for (int i = 0; i < n; i++)
            for (int k = 0; k < n; k++) {
                if (!x.a[i][k]) continue;
                for (int j = 0; j < n; j++)
                    z.a[i][j] = (u64)(((u128)z.a[i][j] +
                        (u128)x.a[i][k] * y.a[k][j]) % mod);
            }
        return z;
    };
    while (e) {
        if (e & 1) r = multiply(r, a);
        e >>= 1;
        if (e) a = multiply(a, a);
    }
    return r;
}

// ---------------------------------------------------------------------------
// Disjoint sets
// ---------------------------------------------------------------------------

struct DSU {
    vector<int> p, sz;
    int components;

    explicit DSU(int n = 0) : p(n), sz(n, 1), components(n) {
        iota(p.begin(), p.end(), 0);
    }

    int find(int x) {
        while (p[x] != x) {
            p[x] = p[p[x]];
            x = p[x];
        }
        return x;
    }

    bool same(int a, int b) { return find(a) == find(b); }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
        components--;
        return true;
    }

    int size(int x) { return sz[find(x)]; }
};

// Maintains value[x] - value[parent[x]]. add(a,b,d) means value[b]-value[a]=d.
struct WeightedDSU {
    vector<int> p, sz;
    vector<i64> pot;

    explicit WeightedDSU(int n = 0) : p(n), sz(n, 1), pot(n) {
        iota(p.begin(), p.end(), 0);
    }

    pair<int, i64> find(int x) {
        if (p[x] == x) return {x, 0};
        auto [r, d] = find(p[x]);
        pot[x] += d;
        p[x] = r;
        return {p[x], pot[x]};
    }

    bool unite(int a, int b, i64 diff) {
        auto [ra, pa] = find(a);
        auto [rb, pb] = find(b);
        if (ra == rb) return pb - pa == diff;
        if (sz[ra] < sz[rb]) {
            swap(ra, rb);
            swap(pa, pb);
            diff = -diff;
        }
        p[rb] = ra;
        pot[rb] = pa + diff - pb;
        sz[ra] += sz[rb];
        return true;
    }
};

// ---------------------------------------------------------------------------
// Graphs
// ---------------------------------------------------------------------------

using WeightedAdj = vector<vector<pair<int, i64>>>;

vector<i64> dijkstra(const WeightedAdj &g, int source,
                     vector<int> *parent = nullptr) {
    int n = (int)g.size();
    vector<i64> dist(n, INF64);
    if (parent) parent->assign(n, -1);
    priority_queue<pair<i64, int>, vector<pair<i64, int>>,
                   greater<pair<i64, int>>> pq;
    dist[source] = 0;
    pq.push({0, source});
    while (!pq.empty()) {
        auto [du, u] = pq.top();
        pq.pop();
        if (du != dist[u]) continue;
        for (auto [v, w] : g[u]) {
            if (w < 0) throw invalid_argument("negative edge");
            if (dist[v] > du + w) {
                dist[v] = du + w;
                if (parent) (*parent)[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

vector<int> restorePath(const vector<int> &parent, int target) {
    if (target < 0 || (size_t)target >= parent.size()) return {};
    vector<int> path;
    while (target != -1) {
        path.push_back(target);
        target = parent[target];
    }
    reverse(path.begin(), path.end());
    return path;
}

struct Edge {
    int u, v;
    i64 w;
};

// Returns false if a negative cycle reachable from source exists.
bool bellmanFord(int n, const vector<Edge> &edges, int source,
                 vector<i64> &dist) {
    dist.assign(n, INF64);
    dist[source] = 0;
    for (int it = 0; it < n - 1; it++) {
        bool changed = false;
        for (auto e : edges) {
            if (dist[e.u] == INF64) continue;
            if (dist[e.v] > dist[e.u] + e.w) {
                dist[e.v] = dist[e.u] + e.w;
                changed = true;
            }
        }
        if (!changed) break;
    }
    for (auto e : edges) {
        if (dist[e.u] != INF64 && dist[e.v] > dist[e.u] + e.w)
            return false;
    }
    return true;
}

void floydWarshall(vector<vector<i64>> &d) {
    int n = (int)d.size();
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++) {
            if (d[i][k] == INF64) continue;
            for (int j = 0; j < n; j++) {
                if (d[k][j] == INF64) continue;
                ckmin(d[i][j], d[i][k] + d[k][j]);
            }
        }
}

// Minimum spanning forest. Returns edge indices actually used.
pair<i64, vector<int>> kruskal(int n, vector<Edge> edges) {
    vector<int> id(edges.size());
    iota(id.begin(), id.end(), 0);
    sort(id.begin(), id.end(), [&](int a, int b) {
        return edges[a].w < edges[b].w;
    });
    DSU dsu(n);
    i64 total = 0;
    vector<int> used;
    for (int i : id) {
        auto &e = edges[i];
        if (dsu.unite(e.u, e.v)) {
            total += e.w;
            used.push_back(i);
        }
    }
    return {total, used};
}

// Lexicographically smallest topological order. Empty means a cycle exists.
vector<int> topologicalSort(const vector<vector<int>> &g) {
    int n = (int)g.size();
    vector<int> deg(n), ans;
    for (int u = 0; u < n; u++)
        for (int v : g[u]) deg[v]++;
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int i = 0; i < n; i++)
        if (!deg[i]) pq.push(i);
    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();
        ans.push_back(u);
        for (int v : g[u])
            if (--deg[v] == 0) pq.push(v);
    }
    if ((int)ans.size() != n) ans.clear();
    return ans;
}

struct TarjanSCC {
    const vector<vector<int>> &g;
    vector<int> dfn, low, stk, comp, on;
    vector<char> inStack;
    vector<vector<int>> components;
    int timer = 0;

    explicit TarjanSCC(const vector<vector<int>> &graph)
        : g(graph), dfn(g.size(), -1), low(g.size()), comp(g.size(), -1),
          inStack(g.size(), false) {}

    void dfs(int u) {
        dfn[u] = low[u] = timer++;
        stk.push_back(u);
        inStack[u] = true;
        for (int v : g[u]) {
            if (dfn[v] == -1) {
                dfs(v);
                ckmin(low[u], low[v]);
            } else if (inStack[v]) {
                ckmin(low[u], dfn[v]);
            }
        }
        if (low[u] == dfn[u]) {
            int cid = (int)components.size();
            components.push_back({});
            while (true) {
                int v = stk.back();
                stk.pop_back();
                inStack[v] = false;
                comp[v] = cid;
                components.back().push_back(v);
                if (v == u) break;
            }
        }
    }

    vector<vector<int>> run() {
        for (int i = 0; i < (int)g.size(); i++)
            if (dfn[i] == -1) dfs(i);
        return components;
    }
};

struct BinaryLifting {
    int n, levels;
    vector<int> depth;
    vector<vector<int>> up;

    BinaryLifting(const vector<vector<int>> &g, int root = 0)
        : n((int)g.size()), depth(n, -1) {
        levels = 1;
        while ((1 << levels) <= max(1, n)) levels++;
        up.assign(levels, vector<int>(n, -1));
        if (n == 0 || root < 0 || root >= n) return;
        queue<int> q;
        depth[root] = 0;
        up[0][root] = root;
        q.push(root);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : g[u]) {
                if (depth[v] != -1) continue;
                depth[v] = depth[u] + 1;
                up[0][v] = u;
                q.push(v);
            }
        }
        for (int j = 1; j < levels; j++)
            for (int v = 0; v < n; v++)
                if (up[j - 1][v] != -1)
                    up[j][v] = up[j - 1][up[j - 1][v]];
    }

    int kthAncestor(int v, int k) const {
        if (v < 0 || v >= n || depth[v] == -1 || k < 0) return -1;
        if (k > depth[v]) return -1;
        for (int j = 0; j < levels; j++)
            if (k >> j & 1) {
                v = up[j][v];
                if (v == -1) return -1;
            }
        return v;
    }

    int lca(int a, int b) const {
        if (a < 0 || a >= n || b < 0 || b >= n) return -1;
        if (depth[a] == -1 || depth[b] == -1) return -1;
        if (depth[a] < depth[b]) swap(a, b);
        a = kthAncestor(a, depth[a] - depth[b]);
        if (a == b) return a;
        for (int j = levels - 1; j >= 0; j--)
            if (up[j][a] != up[j][b]) {
                a = up[j][a];
                b = up[j][b];
            }
        return up[0][a];
    }

    int distance(int a, int b) const {
        int c = lca(a, b);
        return c == -1 ? -1 : depth[a] + depth[b] - 2 * depth[c];
    }
};

struct LowLinkResult {
    vector<pair<int, int>> bridges;
    vector<char> articulation;
    vector<int> tin, low;
};

LowLinkResult lowLink(int n, vector<pair<int, int>> edges) {
    vector<vector<pair<int, int>>> g(n);
    for (int i = 0; i < (int)edges.size(); i++) {
        auto [u, v] = edges[i];
        g[u].push_back({v, i});
        g[v].push_back({u, i});
    }
    LowLinkResult result;
    result.articulation.assign(n, false);
    result.tin.assign(n, -1);
    result.low.assign(n, 0);
    int timer = 0;
    auto dfs = [&](auto &&self, int u, int parentEdge) -> void {
        result.tin[u] = result.low[u] = timer++;
        int children = 0;
        for (auto [v, edgeId] : g[u]) {
            if (edgeId == parentEdge) continue;
            if (result.tin[v] != -1) {
                ckmin(result.low[u], result.tin[v]);
                continue;
            }
            children++;
            self(self, v, edgeId);
            ckmin(result.low[u], result.low[v]);
            if (result.low[v] > result.tin[u])
                result.bridges.push_back({u, v});
            if (parentEdge != -1 && result.low[v] >= result.tin[u])
                result.articulation[u] = true;
        }
        if (parentEdge == -1 && children > 1)
            result.articulation[u] = true;
    };
    for (int i = 0; i < n; i++)
        if (result.tin[i] == -1) dfs(dfs, i, -1);
    return result;
}

struct Dinic {
    struct E {
        int to, rev;
        i64 cap;
    };
    int n;
    vector<vector<E>> g;
    vector<int> level, it;

    explicit Dinic(int count = 0) : n(count), g(count), level(count), it(count) {}

    void addEdge(int u, int v, i64 c, i64 rc = 0) {
        E a{v, (int)g[v].size(), c};
        E b{u, (int)g[u].size(), rc};
        g[u].push_back(a);
        g[v].push_back(b);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &e : g[u]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    i64 dfs(int u, int t, i64 f) {
        if (u == t) return f;
        for (int &i = it[u]; i < (int)g[u].size(); i++) {
            E &e = g[u][i];
            if (e.cap <= 0 || level[e.to] != level[u] + 1) continue;
            i64 got = dfs(e.to, t, min(f, e.cap));
            if (!got) continue;
            e.cap -= got;
            g[e.to][e.rev].cap += got;
            return got;
        }
        return 0;
    }

    i64 maxFlow(int s, int t) {
        i64 flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (i64 f = dfs(s, t, INF64)) flow += f;
        }
        return flow;
    }
};

struct Kuhn {
    int n, m;
    vector<vector<int>> g;
    vector<int> matchR, seen;
    int stamp = 0;

    Kuhn(int left, int right) : n(left), m(right), g(left),
        matchR(right, -1), seen(right, 0) {}

    void addEdge(int u, int v) { g[u].push_back(v); }

    bool dfs(int u) {
        for (int v : g[u]) {
            if (seen[v] == stamp) continue;
            seen[v] = stamp;
            if (matchR[v] == -1 || dfs(matchR[v])) {
                matchR[v] = u;
                return true;
            }
        }
        return false;
    }

    int solve() {
        int ans = 0;
        for (int u = 0; u < n; u++) {
            stamp++;
            ans += dfs(u);
        }
        return ans;
    }
};

// ---------------------------------------------------------------------------
// Grid search
// ---------------------------------------------------------------------------

struct GridBFSResult {
    vector<vector<int>> dist;
    vector<vector<pair<int, int>>> parent;
    bool reachable(int r, int c) const {
        return r >= 0 && c >= 0 && r < (int)dist.size() &&
               c < (int)dist[0].size() && dist[r][c] != -1;
    }
    vector<pair<int, int>> path(int r, int c) const {
        vector<pair<int, int>> p;
        if (!reachable(r, c)) return p;
        while (r != -1) {
            p.push_back({r, c});
            auto q = parent[r][c];
            r = q.first;
            c = q.second;
        }
        reverse(p.begin(), p.end());
        return p;
    }
};

GridBFSResult gridBFS(const vector<string> &grid, int sr, int sc,
                      char wall = '#', bool eightDirections = false) {
    int n = (int)grid.size();
    int m = n ? (int)grid[0].size() : 0;
    GridBFSResult res;
    res.dist.assign(n, vector<int>(m, -1));
    res.parent.assign(n, vector<pair<int, int>>(m, {-1, -1}));
    if (sr < 0 || sr >= n || sc < 0 || sc >= m || grid[sr][sc] == wall)
        return res;
    static const int dr4[] = {1, -1, 0, 0};
    static const int dc4[] = {0, 0, 1, -1};
    static const int dr8[] = {1, -1, 0, 0, 1, 1, -1, -1};
    static const int dc8[] = {0, 0, 1, -1, 1, -1, 1, -1};
    int dirs = eightDirections ? 8 : 4;
    queue<pair<int, int>> q;
    res.dist[sr][sc] = 0;
    q.push({sr, sc});
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        for (int k = 0; k < dirs; k++) {
            int nr = r + (eightDirections ? dr8[k] : dr4[k]);
            int nc = c + (eightDirections ? dc8[k] : dc4[k]);
            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
            if (grid[nr][nc] == wall || res.dist[nr][nc] != -1) continue;
            res.dist[nr][nc] = res.dist[r][c] + 1;
            res.parent[nr][nc] = {r, c};
            q.push({nr, nc});
        }
    }
    return res;
}

// ---------------------------------------------------------------------------
// Fenwick trees, segment tree and sparse table
// ---------------------------------------------------------------------------

template <class T>
struct Fenwick {
    int n;
    vector<T> bit;

    explicit Fenwick(int count = 0) : n(count), bit(count + 1) {}

    void add(int idx, T val) {
        for (idx++; idx <= n; idx += idx & -idx) bit[idx] += val;
    }

    // Sum on [0, idx].
    T prefix(int idx) const {
        T s{};
        for (idx++; idx > 0; idx -= idx & -idx) s += bit[idx];
        return s;
    }

    // Sum on [l, r].
    T range(int l, int r) const {
        if (l > r) return T{};
        return prefix(r) - (l ? prefix(l - 1) : T{});
    }
};

struct LazySum {
    int n;
    vector<i64> tree, lazy;

    explicit LazySum(int count = 0)
        : n(count), tree(4 * count + 4), lazy(4 * count + 4) {}

    void add(int ql, int qr, i64 v) { add(1, 0, n - 1, ql, qr, v); }
    i64 query(int ql, int qr) { return query(1, 0, n - 1, ql, qr); }

    void add(int p, int l, int r, int ql, int qr, i64 v) {
        if (ql <= l && r <= qr) {
            tree[p] += v * (r - l + 1);
            lazy[p] += v;
            return;
        }
        int mid = (l + r) >> 1;
        if (ql <= mid) add(p << 1, l, mid, ql, qr, v);
        if (qr > mid) add(p << 1 | 1, mid + 1, r, ql, qr, v);
        tree[p] = tree[p << 1] + tree[p << 1 | 1] +
                  lazy[p] * (r - l + 1);
    }

    i64 query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tree[p];
        int mid = (l + r) >> 1;
        i64 ans = lazy[p] * (min(r, qr) - max(l, ql) + 1);
        if (ql <= mid) ans += query(p << 1, l, mid, ql, qr);
        if (qr > mid) ans += query(p << 1 | 1, mid + 1, r, ql, qr);
        return ans;
    }
};

struct RangeMin {
    int n;
    i64 neutral = INF64;
    vector<i64> t;

    explicit RangeMin(vector<i64> a) {
        n = 1;
        while (n < (int)a.size()) n <<= 1;
        t.assign(2 * n, neutral);
        for (int i = 0; i < (int)a.size(); i++) t[n + i] = a[i];
        for (int i = n - 1; i; i--) t[i] = min(t[i << 1], t[i << 1 | 1]);
    }

    i64 query(int l, int r) const {
        i64 ans = neutral;
        for (l += n, r += n; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) ckmin(ans, t[l++]);
            if (!(r & 1)) ckmin(ans, t[r--]);
        }
        return ans;
    }
};

template <class T>
struct SparseTable {
    vector<int> lg;
    vector<vector<T>> st;

    explicit SparseTable(const vector<T> &a) {
        int n = (int)a.size();
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; i++) lg[i] = lg[i / 2] + 1;
        int k = n ? lg[n] + 1 : 0;
        st.assign(k, vector<T>(n));
        if (n) st[0] = a;
        for (int j = 1; j < k; j++)
            for (int i = 0; i + (1 << j) <= n; i++)
                st[j][i] = min(st[j - 1][i],
                               st[j - 1][i + (1 << (j - 1))]);
    }

    T query(int l, int r) const {
        int j = lg[r - l + 1];
        return min(st[j][l], st[j][r - (1 << j) + 1]);
    }
};

class FenwickKth {
    int n;
    vector<int> bit;

public:
    explicit FenwickKth(int count) : n(count), bit(count + 1) {}

    void add(int idx, int v) {
        for (idx++; idx <= n; idx += idx & -idx) bit[idx] += v;
    }

    int prefix(int idx) const {
        int s = 0;
        for (idx++; idx > 0; idx -= idx & -idx) s += bit[idx];
        return s;
    }

    // 0-based k-th present value, or -1.
    int kth(int k) const {
        int idx = 0;
        int step = 1;
        while ((step << 1) <= n) step <<= 1;
        for (; step; step >>= 1) {
            int next = idx + step;
            if (next <= n && bit[next] <= k) {
                idx = next;
                k -= bit[next];
            }
        }
        return idx < n ? idx : -1;
    }
};

// ---------------------------------------------------------------------------
// Strings
// ---------------------------------------------------------------------------

vector<int> prefixFunction(const string &s) {
    vector<int> pi(s.size());
    for (int i = 1; i < (int)s.size(); i++) {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}

vector<int> kmpMatches(const string &text, const string &pat) {
    vector<int> ans;
    if (pat.empty()) {
        for (int i = 0; i <= (int)text.size(); i++) ans.push_back(i);
        return ans;
    }
    string s = pat + char(1) + text;
    auto pi = prefixFunction(s);
    for (int i = (int)pat.size() + 1; i < (int)s.size(); i++)
        if (pi[i] == (int)pat.size())
            ans.push_back(i - 2 * (int)pat.size());
    return ans;
}

vector<int> zFunction(const string &s) {
    vector<int> z(s.size());
    int l = 0, r = 0;
    for (int i = 1; i < (int)s.size(); i++) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < (int)s.size() && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

// Returns maximum palindrome length.
int manacherLength(const string &s) {
    string t = "^#";
    for (char c : s) {
        t.push_back(c);
        t.push_back('#');
    }
    t.push_back('$');
    vector<int> p(t.size());
    int c = 0, r = 0, best = 0;
    for (int i = 1; i + 1 < (int)t.size(); i++) {
        if (i < r) p[i] = min(r - i, p[2 * c - i]);
        while (t[i + 1 + p[i]] == t[i - 1 - p[i]]) p[i]++;
        if (i + p[i] > r) {
            c = i;
            r = i + p[i];
        }
        best = max(best, p[i]);
    }
    return best;
}

struct DoubleHash {
    static constexpr i64 M1 = 1000000007;
    static constexpr i64 M2 = 1000000009;
    static constexpr i64 B = 911382323;
    vector<i64> h1, h2, p1, p2;

    explicit DoubleHash(const string &s) {
        int n = (int)s.size();
        h1.assign(n + 1, 0);
        h2.assign(n + 1, 0);
        p1.assign(n + 1, 1);
        p2.assign(n + 1, 1);
        for (int i = 0; i < n; i++) {
            h1[i + 1] = (h1[i] * B + (unsigned char)s[i]) % M1;
            h2[i + 1] = (h2[i] * B + (unsigned char)s[i]) % M2;
            p1[i + 1] = p1[i] * B % M1;
            p2[i + 1] = p2[i] * B % M2;
        }
    }

    // Hash of [l, r), 0-based.
    pair<i64, i64> get(int l, int r) const {
        i64 a = (h1[r] - h1[l] * p1[r - l] % M1 + M1) % M1;
        i64 b = (h2[r] - h2[l] * p2[r - l] % M2 + M2) % M2;
        return {a, b};
    }
};

struct Trie {
    struct Node {
        int next[26];
        int terminal = 0;
        Node() { fill(next, next + 26, -1); }
    };
    vector<Node> t{Node()};

    void insert(const string &s) {
        int u = 0;
        for (char ch : s) {
            int c = tolower((unsigned char)ch) - 'a';
            if (c < 0 || c >= 26) continue;
            if (t[u].next[c] == -1) {
                t[u].next[c] = (int)t.size();
                t.push_back(Node());
            }
            u = t[u].next[c];
        }
        t[u].terminal++;
    }

    bool contains(const string &s) const {
        int u = 0;
        for (char ch : s) {
            int c = tolower((unsigned char)ch) - 'a';
            if (c < 0 || c >= 26 || t[u].next[c] == -1) return false;
            u = t[u].next[c];
        }
        return t[u].terminal > 0;
    }
};

vector<int> suffixArray(const string &s) {
    int n = (int)s.size();
    vector<int> sa(n), rnk(n), tmp(n);
    iota(sa.begin(), sa.end(), 0);
    if (n <= 1) return sa;
    for (int i = 0; i < n; i++) rnk[i] = (unsigned char)s[i];
    for (int k = 1;; k <<= 1) {
        auto cmp = [&](int a, int b) {
            if (rnk[a] != rnk[b]) return rnk[a] < rnk[b];
            int ra = a + k < n ? rnk[a + k] : -1;
            int rb = b + k < n ? rnk[b + k] : -1;
            return ra < rb;
        };
        sort(sa.begin(), sa.end(), cmp);
        tmp[sa[0]] = 0;
        for (int i = 1; i < n; i++)
            tmp[sa[i]] = tmp[sa[i - 1]] + cmp(sa[i - 1], sa[i]);
        rnk = tmp;
        if (rnk[sa.back()] == n - 1) break;
        if (k > n) break;
    }
    return sa;
}

// ---------------------------------------------------------------------------
// 2D geometry
// ---------------------------------------------------------------------------

struct Point {
    long double x = 0, y = 0;
    Point() = default;
    Point(long double px, long double py) : x(px), y(py) {}
    Point operator+(const Point &o) const { return {x + o.x, y + o.y}; }
    Point operator-(const Point &o) const { return {x - o.x, y - o.y}; }
    Point operator*(long double k) const { return {x * k, y * k}; }
    Point operator/(long double k) const { return {x / k, y / k}; }
    long double dot(const Point &o) const { return x * o.x + y * o.y; }
    long double cross(const Point &o) const { return x * o.y - y * o.x; }
    long double len2() const { return dot(*this); }
    long double len() const { return sqrtl(len2()); }
};

long double distancePoint(const Point &a, const Point &b) {
    return (a - b).len();
}

// Cross product (b-a) x (c-a).
long double cross3(const Point &a, const Point &b, const Point &c) {
    return (b - a).cross(c - a);
}

bool onSegment(const Point &p, const Point &a, const Point &b) {
    if (fabsl((b - a).cross(p - a)) > EPS) return false;
    return (p - a).dot(p - b) <= EPS;
}

bool segmentsIntersect(const Point &a, const Point &b,
                       const Point &c, const Point &d) {
    long double o1 = cross3(a, b, c);
    long double o2 = cross3(a, b, d);
    long double o3 = cross3(c, d, a);
    long double o4 = cross3(c, d, b);
    if (((o1 > EPS && o2 < -EPS) || (o1 < -EPS && o2 > EPS)) &&
        ((o3 > EPS && o4 < -EPS) || (o3 < -EPS && o4 > EPS)))
        return true;
    return onSegment(c, a, b) || onSegment(d, a, b) ||
           onSegment(a, c, d) || onSegment(b, c, d);
}

Point lineIntersection(const Point &a, const Point &b,
                       const Point &c, const Point &d) {
    Point r = b - a, s = d - c;
    long double den = r.cross(s);
    if (fabsl(den) < EPS) throw domain_error("parallel lines");
    return a + r * ((c - a).cross(s) / den);
}

long double polygonArea(const vector<Point> &p) {
    long double area = 0;
    for (int i = 0; i < (int)p.size(); i++)
        area += p[i].cross(p[(i + 1) % p.size()]);
    return fabsl(area) / 2;
}

// 1 inside, 0 boundary, -1 outside.
int pointInPolygon(const Point &p, const vector<Point> &poly) {
    int n = (int)poly.size();
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (onSegment(p, poly[j], poly[i])) return 0;
        bool crosses = ((poly[i].y > p.y) != (poly[j].y > p.y));
        if (crosses) {
            long double x = (poly[j].x - poly[i].x) *
                            (p.y - poly[i].y) /
                            (poly[j].y - poly[i].y) + poly[i].x;
            if (x > p.x) inside = !inside;
        }
    }
    return inside ? 1 : -1;
}

struct PointI {
    i64 x = 0, y = 0;
    PointI() = default;
    PointI(i64 px, i64 py) : x(px), y(py) {}
    PointI operator-(const PointI &o) const { return {x - o.x, y - o.y}; }
    bool operator<(const PointI &o) const {
        return x != o.x ? x < o.x : y < o.y;
    }
    bool operator==(const PointI &o) const { return x == o.x && y == o.y; }
};

i128 crossPointI(const PointI &a, const PointI &b, const PointI &c) {
    return (i128)(b.x - a.x) * (c.y - a.y) -
           (i128)(b.y - a.y) * (c.x - a.x);
}

// Convex hull without duplicate collinear middle points.
vector<PointI> convexHull(vector<PointI> p) {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());
    if (p.size() <= 1) return p;
    vector<PointI> h;
    for (auto &x : p) {
        while (h.size() >= 2 &&
               crossPointI(h[h.size() - 2], h.back(), x) <= 0)
            h.pop_back();
        h.push_back(x);
    }
    size_t lower = h.size();
    for (int i = (int)p.size() - 2; i >= 0; i--) {
        while (h.size() > lower &&
               crossPointI(h[h.size() - 2], h.back(), p[i]) <= 0)
            h.pop_back();
        h.push_back(p[i]);
    }
    h.pop_back();
    return h;
}

// ---------------------------------------------------------------------------
// Dynamic programming and bit tricks
// ---------------------------------------------------------------------------

int lisLength(const vector<i64> &a, bool strict = true) {
    vector<i64> tails;
    for (i64 x : a) {
        auto it = strict ? lower_bound(tails.begin(), tails.end(), x)
                         : upper_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return (int)tails.size();
}

int lcsLength(const string &a, const string &b) {
    vector<int> dp(b.size() + 1);
    for (char x : a) {
        vector<int> ndp(b.size() + 1);
        for (int j = 1; j <= (int)b.size(); j++) {
            if (x == b[j - 1]) ndp[j] = dp[j - 1] + 1;
            else ndp[j] = max(dp[j], ndp[j - 1]);
        }
        dp.swap(ndp);
    }
    return dp.back();
}

struct KadaneResult {
    i64 best = -INF64;
    int left = -1, right = -1;
};

KadaneResult kadaneWithIndices(const vector<i64> &a) {
    KadaneResult result;
    i64 current = 0;
    int currentLeft = 0;
    for (int i = 0; i < (int)a.size(); i++) {
        if (current <= 0) {
            current = a[i];
            currentLeft = i;
        } else {
            current += a[i];
        }
        if (current > result.best) {
            result.best = current;
            result.left = currentLeft;
            result.right = i;
        }
    }
    return result;
}

i64 kadane(const vector<i64> &a) {
    return kadaneWithIndices(a).best;
}

i64 maxSubrectangle(const vector<vector<i64>> &a) {
    if (a.empty() || a[0].empty()) return -INF64;
    int n = (int)a.size(), m = (int)a[0].size();
    i64 best = -INF64;
    for (int top = 0; top < n; top++) {
        vector<i64> compressed(m);
        for (int bottom = top; bottom < n; bottom++) {
            for (int j = 0; j < m; j++) compressed[j] += a[bottom][j];
            ckmax(best, kadane(compressed));
        }
    }
    return best;
}

// Maximum number of groups of groupSize using both colors in every group.
i64 maxBalancedGroups(i64 first, i64 second, int groupSize = 3) {
    if (first < 0 || second < 0 || groupSize < 2) return 0;
    return min({first, second, (first + second) / groupSize});
}

i64 knapsack01(const vector<int> &w, const vector<i64> &v, int cap) {
    vector<i64> dp(cap + 1);
    for (int i = 0; i < (int)w.size(); i++)
        for (int j = cap; j >= w[i]; j--)
            ckmax(dp[j], dp[j - w[i]] + v[i]);
    return dp[cap];
}

// Minimum coins; INF64 if impossible.
i64 minCoins(const vector<int> &coins, int target) {
    vector<i64> dp(target + 1, INF64);
    dp[0] = 0;
    for (int c : coins) {
        if (c <= 0) continue;
        for (int x = c; x <= target; x++)
            if (dp[x - c] != INF64) ckmin(dp[x], dp[x - c] + 1);
    }
    return dp[target];
}

// Number of ordered sequences. O(target * |coins|).
i64 countCoinSequences(const vector<int> &coins, int target, i64 mod) {
    vector<i64> dp(target + 1);
    dp[0] = 1;
    for (int x = 1; x <= target; x++)
        for (int c : coins)
            if (c <= x) dp[x] = (dp[x] + dp[x - c]) % mod;
    return dp[target];
}

i64 countSubsetSum(const vector<int> &a, int target, i64 mod) {
    vector<i64> dp(target + 1);
    dp[0] = 1;
    for (int x : a)
        for (int s = target; s >= x; s--)
            dp[s] = (dp[s] + dp[s - x]) % mod;
    return dp[target];
}

// Enumerate all submasks of mask in O(number of submasks).
template <class F>
void forEachSubmask(unsigned mask, F f) {
    for (unsigned s = mask;; s = (s - 1) & mask) {
        f(s);
        if (!s) break;
    }
}

// f[mask] becomes sum of the original f[submask] for all submask <= mask.
void subsetZetaSum(vector<i64> &f) {
    int k = 0;
    while ((1u << k) < f.size()) k++;
    for (int b = 0; b < k; b++)
        for (int mask = 0; mask < (int)f.size(); mask++)
            if (mask & (1 << b))
                f[mask] += f[mask ^ (1 << b)];
}

// Smallest integer in [lo, hi] where pred becomes true.
// pred must be false...false,true...true on the interval.
template <class Pred>
i64 firstTrue(i64 lo, i64 hi, Pred pred) {
    if (lo > hi || !pred(hi)) return -1;
    while (lo < hi) {
        i64 mid = lo + (hi - lo) / 2;
        if (pred(mid)) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

// Largest integer in [lo, hi] where pred remains true.
// pred must be true...true,false...false on the interval.
template <class Pred>
i64 lastTrue(i64 lo, i64 hi, Pred pred) {
    if (lo > hi || !pred(lo)) return -1;
    while (lo < hi) {
        i64 mid = lo + (hi - lo + 1) / 2;
        if (pred(mid)) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

template <class Pred>
long double binarySearchReal(long double lo, long double hi, Pred pred,
                             int iterations = 100) {
    while (iterations--) {
        long double mid = (lo + hi) / 2;
        if (pred(mid)) lo = mid;
        else hi = mid;
    }
    return lo;
}

vector<i64> windowSums(const vector<i64> &a, int k) {
    if (k <= 0 || k > (int)a.size()) return {};
    vector<i64> answer;
    i64 sum = 0;
    for (int i = 0; i < (int)a.size(); i++) {
        sum += a[i];
        if (i >= k) sum -= a[i - k];
        if (i + 1 >= k) answer.push_back(sum);
    }
    return answer;
}

pair<vector<i64>, vector<i64>> windowExtrema(const vector<i64> &a, int k) {
    vector<i64> mins, maxs;
    if (k <= 0 || k > (int)a.size()) return {mins, maxs};
    deque<int> increasing, decreasing;
    for (int i = 0; i < (int)a.size(); i++) {
        while (!increasing.empty() && a[increasing.back()] >= a[i])
            increasing.pop_back();
        while (!decreasing.empty() && a[decreasing.back()] <= a[i])
            decreasing.pop_back();
        increasing.push_back(i);
        decreasing.push_back(i);
        if (increasing.front() <= i - k) increasing.pop_front();
        if (decreasing.front() <= i - k) decreasing.pop_front();
        if (i + 1 >= k) {
            mins.push_back(a[increasing.front()]);
            maxs.push_back(a[decreasing.front()]);
        }
    }
    return {mins, maxs};
}

// Requires nonnegative values. Counts nonempty subarrays with sum <= limit.
i64 countSubarraysAtMostSum(const vector<i64> &a, i64 limit) {
    i64 answer = 0, sum = 0;
    int left = 0;
    for (int right = 0; right < (int)a.size(); right++) {
        if (a[right] < 0) throw invalid_argument("negative value");
        sum += a[right];
        while (left <= right && sum > limit) sum -= a[left++];
        answer += right - left + 1;
    }
    return answer;
}

struct XorBasis {
    vector<u64> b = vector<u64>(64);

    bool insert(u64 x) {
        for (int i = 63; i >= 0; i--) {
            if (!(x >> i & 1)) continue;
            if (!b[i]) {
                b[i] = x;
                return true;
            }
            x ^= b[i];
        }
        return false;
    }

    bool contains(u64 x) const {
        for (int i = 63; i >= 0; i--)
            if (x >> i & 1) {
                if (!b[i]) return false;
                x ^= b[i];
            }
        return true;
    }

    int rank() const {
        return (int)count_if(b.begin(), b.end(), [](u64 x) { return x != 0; });
    }

    u64 maximum() const {
        u64 x = 0;
        for (int i = 63; i >= 0; i--) ckmax(x, x ^ b[i]);
        return x;
    }
};

// ---------------------------------------------------------------------------
// Parsing, dates and base conversion
// ---------------------------------------------------------------------------

string trim(string s) {
    int l = 0, r = (int)s.size();
    while (l < r && s[l] <= ' ') l++;
    while (l < r && s[r - 1] <= ' ') r--;
    return s.substr(l, r - l);
}

vector<string> split(const string &s, char delim = ',',
                     bool respectQuotes = true, bool doTrim = true) {
    vector<string> ans;
    string cur;
    bool quoted = false;
    for (int i = 0; i < (int)s.size(); i++) {
        char c = s[i];
        if (respectQuotes && c == '"') {
            if (quoted && i + 1 < (int)s.size() && s[i + 1] == '"') {
                cur.push_back('"');
                i++;
            } else {
                quoted = !quoted;
            }
        } else if (c == delim && !quoted) {
            ans.push_back(doTrim ? trim(cur) : cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    ans.push_back(doTrim ? trim(cur) : cur);
    return ans;
}

vector<string> words(const string &s) {
    vector<string> ans;
    string cur;
    for (char c : s) {
        if (c <= ' ') {
            if (!cur.empty()) {
                ans.push_back(cur);
                cur.clear();
            }
        } else {
            cur.push_back(c);
        }
    }
    if (!cur.empty()) ans.push_back(cur);
    return ans;
}

// next[d][i] is the first occurrence of alphabet[d] at or after i,
// or n when that occurrence does not exist.
vector<vector<int>> nextOccurrences(const string &s,
                                    const string &alphabet) {
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

class ExpressionParser {
    string s;
    int i = 0;

    void ws() { while (i < (int)s.size() && s[i] <= ' ') i++; }
    bool eat(char c) {
        ws();
        if (i < (int)s.size() && s[i] == c) {
            i++;
            return true;
        }
        return false;
    }

    long double atom() {
        ws();
        if (eat('+')) return atom();
        if (eat('-')) return -atom();
        if (eat('(')) {
            long double x = expr();
            eat(')');
            return x;
        }
        int j = i;
        while (i < (int)s.size() &&
               (isdigit((unsigned char)s[i]) || s[i] == '.' ||
                s[i] == 'e' || s[i] == 'E' ||
                ((s[i] == '+' || s[i] == '-') && i > j &&
                 (s[i - 1] == 'e' || s[i - 1] == 'E'))))
            i++;
        return strtold(s.substr(j, i - j).c_str(), nullptr);
    }

    long double power() {
        long double x = atom();
        if (eat('^')) x = powl(x, power());
        return x;
    }

    long double product() {
        long double x = power();
        while (true) {
            if (eat('*')) x *= power();
            else if (eat('/')) x /= power();
            else if (eat('%')) x = fmodl(x, power());
            else return x;
        }
    }

    long double expr() {
        long double x = product();
        while (true) {
            if (eat('+')) x += product();
            else if (eat('-')) x -= product();
            else return x;
        }
    }

public:
    explicit ExpressionParser(string text) : s(move(text)) {}
    long double eval() { i = 0; return expr(); }
};

i64 daysFromCivil(int y, unsigned m, unsigned d) {
    y -= m <= 2;
    i64 era = (y >= 0 ? y : y - 399) / 400;
    unsigned yoe = (unsigned)(y - era * 400);
    unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + (i64)doe - 719468;
}

tuple<int, unsigned, unsigned> civilFromDays(i64 z) {
    z += 719468;
    i64 era = (z >= 0 ? z : z - 146096) / 146097;
    unsigned doe = (unsigned)(z - era * 146097);
    unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    int y = (int)yoe + (int)era * 400;
    unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    unsigned mp = (5 * doy + 2) / 153;
    unsigned d = doy - (153 * mp + 2) / 5 + 1;
    unsigned m = mp + (mp < 10 ? 3 : -9);
    y += m <= 2;
    return {y, m, d};
}

bool leapYear(int y) {
    return y % 400 == 0 || (y % 4 == 0 && y % 100 != 0);
}

int daysInMonth(int y, int m) {
    static const int d[] = {0, 31, 28, 31, 30, 31, 30,
                            31, 31, 30, 31, 30, 31};
    return m == 2 && leapYear(y) ? 29 : d[m];
}

// Converts a nonnegative integer string between bases 2..36.
string convertBase(string value, int from, int to) {
    bool neg = !value.empty() && value[0] == '-';
    if (neg) value.erase(value.begin());
    BigInt x(0);
    for (char c : value) {
        int d = c >= '0' && c <= '9' ? c - '0'
              : c >= 'a' && c <= 'z' ? c - 'a' + 10
              : c >= 'A' && c <= 'Z' ? c - 'A' + 10
              : -1;
        if (d < 0 || d >= from) throw invalid_argument("invalid digit");
        x = x * from + d;
    }
    if (x.isZero()) return "0";
    string ans;
    while (!x.isZero()) {
        auto [q, r] = x.divSmall(to);
        ans.push_back(r < 10 ? char('0' + r) : char('A' + r - 10));
        x = q;
    }
    if (neg) ans.push_back('-');
    reverse(ans.begin(), ans.end());
    return ans;
}

// ---------------------------------------------------------------------------

} // namespace ib
