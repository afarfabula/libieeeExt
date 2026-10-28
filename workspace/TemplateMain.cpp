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
namespace ib{
using i64=long long;
using u64=unsigned long long;
using i128=__int128_t;
using u128=__uint128_t;
const i64 INF64=(i64)4e18;
const int INF32=0x3f3f3f3f;
const long double EPS=1e-12L;
const long double PI=acosl(-1.0L);
template<class T>T absval(T x){return x<0?-x:x;}
template<class T>void ckmin(T&a,const T&b){if(b<a)a=b;}
template<class T>void ckmax(T&a,const T&b){if(b>a)a=b;}
template<class T>int sgn(T x){return(x>0)-(x<0);}
u64 splitmix64(u64 x){
x+=0x9e3779b97f4a7c15ULL;
x=(x^(x>>30))*0xbf58476d1ce4e5b9ULL;
x=(x^(x>>27))*0x94d049bb133111ebULL;
return x^(x>>31);
}
class Scanner{
static constexpr int SZ=1<<20;
char buf[SZ];
int pos=0,len=0;
char raw(){
if(pos==len){
len=(int)fread(buf,1,SZ,stdin);
pos=0;
if(len<=0)return 0;
}
return buf[pos++];
}
char skip(){
char c=raw();
while(c&&c<=' ')c=raw();
return c;
}
public:
template<class T,
typename enable_if<is_integral<T>::value,int>::type=0>
bool read(T&out){
char c=skip();
if(!c)return false;
bool neg=false;
if(c=='-'||c=='+'){
neg=c=='-';
c=raw();
}
using U=typename make_unsigned<T>::type;
U x=0;
while(c>='0'&&c<='9'){
x=x*10+(U)(c-'0');
c=raw();
}
out=neg?(T)(0-x):(T)x;
return true;
}
bool read(long double&out){
char c=skip();
if(!c)return false;
string s;
if(c=='-'||c=='+'){
s.push_back(c);
c=raw();
}
while(c&&c>' '&&c!=','){
s.push_back(c);
c=raw();
}
char*end=nullptr;
out=strtold(s.c_str(),&end);
return end!=s.c_str();
}
bool read(double&out){
long double x;
if(!read(x))return false;
out=(double)x;
return true;
}
bool read(char&out){
out=skip();
return out!=0;
}
bool read(string&out){
out.clear();
char c=skip();
if(!c)return false;
while(c&&c>' '){
out.push_back(c);
c=raw();
}
return true;
}
string line(){
string s;
char c=raw();
while(c!='\n'&&c){
if(c!='\r')s.push_back(c);
c=raw();
}
return s;
}
template<class T>
T next(){
T x{};
read(x);
return x;
}
template<class T>
vector<T>nextArray(int n){
vector<T>a(n);
for(auto&x:a)read(x);
return a;
}
};
class Printer{
static constexpr int SZ=1<<20;
char buf[SZ];
int pos=0;
void flushIfFull(){
if(pos==SZ)flush();
}
public:
~Printer(){flush();}
void flush(){
if(pos)fwrite(buf,1,pos,stdout);
pos=0;
}
void put(char c){
flushIfFull();
buf[pos++]=c;
}
void write(const string&s){
for(char c:s)put(c);
}
void write(const char*s){
while(*s)put(*s++);
}
template<class T,
typename enable_if<is_integral<T>::value,int>::type=0>
void write(T x){
if(x==0){
put('0');
return;
}
if(is_signed<T>::value&&x<0){
put('-');
using U=typename make_unsigned<T>::type;
U y=(U)(-(x+1))+1;
write(y);
return;
}
using U=typename make_unsigned<T>::type;
U y=(U)x;
char t[32];
int n=0;
while(y){
t[n++]=char('0'+y%10);
y/=10;
}
while(n)put(t[--n]);
}
void write(long double x,int digits=10){
char t[128];
snprintf(t,sizeof(t),"%.*Lf",digits,x);
write(t);
}
void write(double x,int digits=10){
write((long double)x,digits);
}
template<class T>
void line(const T&x){
write(x);
put('\n');
}
void space(){put(' ');}
};
inline Scanner in;
inline Printer out;
inline u64 mulMod(u64 a,u64 b,u64 mod){
if(!mod)return 0;
return(u64)((u128)a*b%mod);
}
inline u64 powMod(u64 a,u64 e,u64 mod){
if(!mod)return 0;
a%=mod;
u64 r=mod==1?0:1;
while(e){
if(e&1)r=mulMod(r,a,mod);
a=mulMod(a,a,mod);
e>>=1;
}
return r;
}
constexpr int NTT_MOD=998244353;
constexpr int NTT_ROOT=3;
int nttPow(int a,int e){
i64 r=1;
while(e){
if(e&1)r=r*a%NTT_MOD;
a=(int)((i64)a*a%NTT_MOD);
e>>=1;
}
return(int)r;
}
void ntt(vector<int>&a,bool invert){
int n=(int)a.size();
for(int i=1,j=0;i<n;i++){
int bit=n>>1;
for(;j&bit;bit>>=1)j^=bit;
j^=bit;
if(i<j)swap(a[i],a[j]);
}
for(int len=2;len<=n;len<<=1){
int wlen=nttPow(NTT_ROOT,(NTT_MOD-1)/len);
if(invert)wlen=nttPow(wlen,NTT_MOD-2);
for(int i=0;i<n;i+=len){
i64 w=1;
for(int j=0;j<len/2;j++){
int u=a[i+j];
int v=(int)(w*a[i+j+len/2]%NTT_MOD);
int sum=u+v;
if(sum>=NTT_MOD)sum-=NTT_MOD;
int diff=u-v;
if(diff<0)diff+=NTT_MOD;
a[i+j]=sum;
a[i+j+len/2]=diff;
w=w*wlen%NTT_MOD;
}
}
}
if(invert){
int invN=nttPow(n,NTT_MOD-2);
for(int&x:a)x=(int)((i64)x*invN%NTT_MOD);
}
}
int nttSize(int need){
int size=1;
while(size<need)size<<=1;
return size;
}
vector<vector<int>>convolution2D(const vector<vector<int>>&a,
const vector<vector<int>>&b){
int ar=(int)a.size(),ac=ar?(int)a[0].size():0;
int br=(int)b.size(),bc=br?(int)b[0].size():0;
int outR=ar+br-1,outC=ac+bc-1;
int width=nttSize(max(1,outC));
int size=nttSize(max(1,outR*width));
vector<int>x(size),y(size);
for(int i=0;i<ar;i++)
for(int j=0;j<ac;j++)x[i*width+j]=a[i][j];
for(int i=0;i<br;i++)
for(int j=0;j<bc;j++)y[i*width+j]=b[i][j];
ntt(x,false);
ntt(y,false);
for(int i=0;i<size;i++)
x[i]=(int)((i64)x[i]*y[i]%NTT_MOD);
ntt(x,true);
vector<vector<int>>result(outR,vector<int>(outC));
for(int i=0;i<outR;i++)
for(int j=0;j<outC;j++)
result[i][j]=x[i*width+j];
return result;
}
inline u64 gcdU(u64 a,u64 b){
while(b){
u64 t=a%b;
a=b;
b=t;
}
return a;
}
i64 extGcd(i64 a,i64 b,i64&x,i64&y){
if(!b){
x=a<0?-1:1;
y=0;
return absval(a);
}
i64 x1,y1;
i64 g=extGcd(b,a%b,x1,y1);
x=y1;
y=x1-(a/b)*y1;
return g;
}
i128 extGcd128(i128 a,i128 b,i128&x,i128&y){
if(!b){
x=a<0?-1:1;
y=0;
return a<0?-a:a;
}
i128 x1,y1;
i128 g=extGcd128(b,a%b,x1,y1);
x=y1;
y=x1-(a/b)*y1;
return g;
}
inline i64 modInverse(i64 a,i64 mod){
i128 x,y;
i128 g=extGcd128(a%mod,mod,x,y);
if(g!=1)throw domain_error("inverse does not exist");
x%=mod;
if(x<0)x+=mod;
return(i64)x;
}
inline bool isPrime(u64 n){
if(n<2)return false;
static const int small[]={2,3,5,7,11,13,17,19,23,29,31,37};
for(u64 p:small){
if(n%p==0)return n==p;
}
u64 d=n-1;
int s=0;
while(!(d&1)){
d>>=1;
s++;
}
static const u64 bases[]={
2ULL,325ULL,9375ULL,28178ULL,
450775ULL,9780504ULL,1795265022ULL
};
for(u64 a:bases){
if(a%n==0)continue;
u64 x=powMod(a,d,n);
if(x==1||x==n-1)continue;
bool witness=true;
for(int r=1;r<s;r++){
x=mulMod(x,x,n);
if(x==n-1){
witness=false;
break;
}
}
if(witness)return false;
}
return true;
}
u64 pollardRho(u64 n){
if(n%2==0)return 2;
if(n%3==0)return 3;
static mt19937_64 rng(
chrono::steady_clock::now().time_since_epoch().count());
while(true){
u64 c=uniform_int_distribution<u64>(1,n-1)(rng);
u64 x=uniform_int_distribution<u64>(0,n-1)(rng);
u64 y=x,d=1;
auto f=[&](u64 z){return(mulMod(z,z,n)+c)%n;};
while(d==1){
x=f(x);
y=f(f(y));
d=gcdU(x>y?x-y:y-x,n);
}
if(d!=n)return d;
}
}
void factorRec(u64 n,vector<u64>&fac){
if(n==1)return;
if(isPrime(n)){
fac.push_back(n);
return;
}
u64 d=pollardRho(n);
factorRec(d,fac);
factorRec(n/d,fac);
}
vector<u64>primeFactors(u64 n){
vector<u64>f;
factorRec(n,f);
sort(f.begin(),f.end());
return f;
}
map<u64,int>factorMap(u64 n){
map<u64,int>m;
for(u64 p:primeFactors(n))m[p]++;
return m;
}
vector<u64>divisors(u64 n){
auto f=factorMap(n);
vector<u64>d{1};
for(auto [p,e]:f){
int old=(int)d.size();
u64 pw=1;
for(int k=1;k<=e;k++){
pw*=p;
for(int i=0;i<old;i++)d.push_back(d[i]*pw);
}
}
sort(d.begin(),d.end());
return d;
}
vector<int>sievePrimes(int n){
vector<bool>prime(n+1,true);
if(n>=0)prime[0]=false;
if(n>=1)prime[1]=false;
for(int i=2;(i64)i*i<=n;i++){
if(!prime[i])continue;
for(i64 j=(i64)i*i;j<=n;j+=i)
prime[j]=false;
}
vector<int>p;
for(int i=2;i<=n;i++)
if(prime[i])p.push_back(i);
return p;
}
vector<int>smallestPrimeFactor(int n){
vector<int>spf(n+1);
for(int i=2;i<=n;i++){
if(spf[i])continue;
spf[i]=i;
if((i64)i*i<=n){
for(i64 j=(i64)i*i;j<=n;j+=i)
if(!spf[j])spf[j]=i;
}
}
return spf;
}
pair<i128,i128>crtCoprime(const vector<i64>&rem,
const vector<i64>&mod){
i128 x=0,M=1;
for(size_t i=0;i<mod.size();i++){
i64 inv=modInverse((i64)(M%mod[i]),mod[i]);
i128 t=((i128)((rem[i]-x)%mod[i]+mod[i])%mod[i])*inv;
x+=t%mod[i]*M;
M*=mod[i];
x%=M;
}
return{x,M};
}
class Comb{
int n;
i64 mod;
vector<i64>fact,invFact;
public:
Comb(int maxN,i64 modulus)
:n(max(0,maxN)),mod(modulus),fact(n+1),invFact(n+1){
fact[0]=1;
for(int i=1;i<=n;i++)fact[i]=fact[i-1]*i%mod;
invFact[n]=modInverse(fact[n],mod);
for(int i=n;i>=1;i--)invFact[i-1]=invFact[i]*i%mod;
}
i64 choose(int a,int b)const{
if(a<0||b<0||b>a||a>n)return 0;
return fact[a]*invFact[b]%mod*invFact[a-b]%mod;
}
i64 permute(int a,int b)const{
if(a<0||b<0||b>a||a>n)return 0;
return fact[a]*invFact[a-b]%mod;
}
i64 catalan(int k)const{
if(k<0||2*k>n)return 0;
i64 c=choose(2*k,k);
return c*modInverse(k+1,mod)%mod;
}
i64 factorial(int k)const{
return 0<=k&&k<=n?fact[k]:0;
}
};
struct DSU{
vector<int>p,sz;
int components;
explicit DSU(int n=0):p(n),sz(n,1),components(n){
iota(p.begin(),p.end(),0);
}
int find(int x){
while(p[x]!=x){
p[x]=p[p[x]];
x=p[x];
}
return x;
}
bool same(int a,int b){return find(a)==find(b);}
bool unite(int a,int b){
a=find(a);
b=find(b);
if(a==b)return false;
if(sz[a]<sz[b])swap(a,b);
p[b]=a;
sz[a]+=sz[b];
components--;
return true;
}
int size(int x){return sz[find(x)];}
};
struct WeightedDSU{
vector<int>p,sz;
vector<i64>pot;
explicit WeightedDSU(int n=0):p(n),sz(n,1),pot(n){
iota(p.begin(),p.end(),0);
}
pair<int,i64>find(int x){
if(p[x]==x)return{x,0};
auto [r,d]=find(p[x]);
pot[x]+=d;
p[x]=r;
return{p[x],pot[x]};
}
bool unite(int a,int b,i64 diff){
auto [ra,pa]=find(a);
auto [rb,pb]=find(b);
if(ra==rb)return pb-pa==diff;
if(sz[ra]<sz[rb]){
swap(ra,rb);
swap(pa,pb);
diff=-diff;
}
p[rb]=ra;
pot[rb]=pa+diff-pb;
sz[ra]+=sz[rb];
return true;
}
};
using WeightedAdj=vector<vector<pair<int,i64>>>;
vector<i64>dijkstra(const WeightedAdj&g,int source,
vector<int>*parent=nullptr){
int n=(int)g.size();
vector<i64>dist(n,INF64);
if(parent)parent->assign(n,-1);
priority_queue<pair<i64,int>,vector<pair<i64,int>>,
greater<pair<i64,int>>>pq;
dist[source]=0;
pq.push({0,source});
while(!pq.empty()){
auto [du,u]=pq.top();
pq.pop();
if(du!=dist[u])continue;
for(auto [v,w]:g[u]){
if(w<0)throw invalid_argument("negative edge");
if(dist[v]>du+w){
dist[v]=du+w;
if(parent)(*parent)[v]=u;
pq.push({dist[v],v});
}
}
}
return dist;
}
vector<int>restorePath(const vector<int>&parent,int target){
if(target<0||(size_t)target>=parent.size())return{};
vector<int>path;
while(target!=-1){
path.push_back(target);
target=parent[target];
}
reverse(path.begin(),path.end());
return path;
}
struct Edge{
int u,v;
i64 w;
};
bool bellmanFord(int n,const vector<Edge>&edges,int source,
vector<i64>&dist){
dist.assign(n,INF64);
dist[source]=0;
for(int it=0;it<n-1;it++){
bool changed=false;
for(auto e:edges){
if(dist[e.u]==INF64)continue;
if(dist[e.v]>dist[e.u]+e.w){
dist[e.v]=dist[e.u]+e.w;
changed=true;
}
}
if(!changed)break;
}
for(auto e:edges){
if(dist[e.u]!=INF64&&dist[e.v]>dist[e.u]+e.w)
return false;
}
return true;
}
void floydWarshall(vector<vector<i64>>&d){
int n=(int)d.size();
for(int k=0;k<n;k++)
for(int i=0;i<n;i++){
if(d[i][k]==INF64)continue;
for(int j=0;j<n;j++){
if(d[k][j]==INF64)continue;
ckmin(d[i][j],d[i][k]+d[k][j]);
}
}
}
pair<i64,vector<int>>kruskal(int n,vector<Edge>edges){
vector<int>id(edges.size());
iota(id.begin(),id.end(),0);
sort(id.begin(),id.end(),[&](int a,int b){
return edges[a].w<edges[b].w;
});
DSU dsu(n);
i64 total=0;
vector<int>used;
for(int i:id){
auto&e=edges[i];
if(dsu.unite(e.u,e.v)){
total+=e.w;
used.push_back(i);
}
}
return{total,used};
}
vector<int>topologicalSort(const vector<vector<int>>&g){
int n=(int)g.size();
vector<int>deg(n),ans;
for(int u=0;u<n;u++)
for(int v:g[u])deg[v]++;
priority_queue<int,vector<int>,greater<int>>pq;
for(int i=0;i<n;i++)
if(!deg[i])pq.push(i);
while(!pq.empty()){
int u=pq.top();
pq.pop();
ans.push_back(u);
for(int v:g[u])
if(--deg[v]==0)pq.push(v);
}
if((int)ans.size()!=n)ans.clear();
return ans;
}
struct TarjanSCC{
const vector<vector<int>>&g;
vector<int>dfn,low,stk,comp,on;
vector<char>inStack;
vector<vector<int>>components;
int timer=0;
explicit TarjanSCC(const vector<vector<int>>&graph)
:g(graph),dfn(g.size(),-1),low(g.size()),comp(g.size(),-1),
inStack(g.size(),false){}
void dfs(int u){
dfn[u]=low[u]=timer++;
stk.push_back(u);
inStack[u]=true;
for(int v:g[u]){
if(dfn[v]==-1){
dfs(v);
ckmin(low[u],low[v]);
}else if(inStack[v]){
ckmin(low[u],dfn[v]);
}
}
if(low[u]==dfn[u]){
int cid=(int)components.size();
components.push_back({});
while(true){
int v=stk.back();
stk.pop_back();
inStack[v]=false;
comp[v]=cid;
components.back().push_back(v);
if(v==u)break;
}
}
}
vector<vector<int>>run(){
for(int i=0;i<(int)g.size();i++)
if(dfn[i]==-1)dfs(i);
return components;
}
};
struct BinaryLifting{
int n,levels;
vector<int>depth;
vector<vector<int>>up;
BinaryLifting(const vector<vector<int>>&g,int root=0)
:n((int)g.size()),depth(n,-1){
levels=1;
while((1<<levels)<=max(1,n))levels++;
up.assign(levels,vector<int>(n,-1));
if(n==0||root<0||root>=n)return;
queue<int>q;
depth[root]=0;
up[0][root]=root;
q.push(root);
while(!q.empty()){
int u=q.front();
q.pop();
for(int v:g[u]){
if(depth[v]!=-1)continue;
depth[v]=depth[u]+1;
up[0][v]=u;
q.push(v);
}
}
for(int j=1;j<levels;j++)
for(int v=0;v<n;v++)
if(up[j-1][v]!=-1)
up[j][v]=up[j-1][up[j-1][v]];
}
int kthAncestor(int v,int k)const{
if(v<0||v>=n||depth[v]==-1||k<0)return-1;
if(k>depth[v])return-1;
for(int j=0;j<levels;j++)
if(k>>j&1){
v=up[j][v];
if(v==-1)return-1;
}
return v;
}
int lca(int a,int b)const{
if(a<0||a>=n||b<0||b>=n)return-1;
if(depth[a]==-1||depth[b]==-1)return-1;
if(depth[a]<depth[b])swap(a,b);
a=kthAncestor(a,depth[a]-depth[b]);
if(a==b)return a;
for(int j=levels-1;j>=0;j--)
if(up[j][a]!=up[j][b]){
a=up[j][a];
b=up[j][b];
}
return up[0][a];
}
int distance(int a,int b)const{
int c=lca(a,b);
return c==-1?-1:depth[a]+depth[b]-2*depth[c];
}
};
struct LowLinkResult{
vector<pair<int,int>>bridges;
vector<char>articulation;
vector<int>tin,low;
};
LowLinkResult lowLink(int n,vector<pair<int,int>>edges){
vector<vector<pair<int,int>>>g(n);
for(int i=0;i<(int)edges.size();i++){
auto [u,v]=edges[i];
g[u].push_back({v,i});
g[v].push_back({u,i});
}
LowLinkResult result;
result.articulation.assign(n,false);
result.tin.assign(n,-1);
result.low.assign(n,0);
int timer=0;
auto dfs=[&](auto&&self,int u,int parentEdge)->void{
result.tin[u]=result.low[u]=timer++;
int children=0;
for(auto [v,edgeId]:g[u]){
if(edgeId==parentEdge)continue;
if(result.tin[v]!=-1){
ckmin(result.low[u],result.tin[v]);
continue;
}
children++;
self(self,v,edgeId);
ckmin(result.low[u],result.low[v]);
if(result.low[v]>result.tin[u])
result.bridges.push_back({u,v});
if(parentEdge!=-1&&result.low[v]>=result.tin[u])
result.articulation[u]=true;
}
if(parentEdge==-1&&children>1)
result.articulation[u]=true;
};
for(int i=0;i<n;i++)
if(result.tin[i]==-1)dfs(dfs,i,-1);
return result;
}
struct Dinic{
struct E{
int to,rev;
i64 cap;
};
int n;
vector<vector<E>>g;
vector<int>level,it;
explicit Dinic(int count=0):n(count),g(count),level(count),it(count){}
void addEdge(int u,int v,i64 c,i64 rc=0){
E a{v,(int)g[v].size(),c};
E b{u,(int)g[u].size(),rc};
g[u].push_back(a);
g[v].push_back(b);
}
bool bfs(int s,int t){
fill(level.begin(),level.end(),-1);
queue<int>q;
level[s]=0;
q.push(s);
while(!q.empty()){
int u=q.front();
q.pop();
for(auto&e:g[u]){
if(e.cap>0&&level[e.to]==-1){
level[e.to]=level[u]+1;
q.push(e.to);
}
}
}
return level[t]!=-1;
}
i64 dfs(int u,int t,i64 f){
if(u==t)return f;
for(int&i=it[u];i<(int)g[u].size();i++){
E&e=g[u][i];
if(e.cap<=0||level[e.to]!=level[u]+1)continue;
i64 got=dfs(e.to,t,min(f,e.cap));
if(!got)continue;
e.cap-=got;
g[e.to][e.rev].cap+=got;
return got;
}
return 0;
}
i64 maxFlow(int s,int t){
i64 flow=0;
while(bfs(s,t)){
fill(it.begin(),it.end(),0);
while(i64 f=dfs(s,t,INF64))flow+=f;
}
return flow;
}
};
struct Kuhn{
int n,m;
vector<vector<int>>g;
vector<int>matchR,seen;
int stamp=0;
Kuhn(int left,int right):n(left),m(right),g(left),
matchR(right,-1),seen(right,0){}
void addEdge(int u,int v){g[u].push_back(v);}
bool dfs(int u){
for(int v:g[u]){
if(seen[v]==stamp)continue;
seen[v]=stamp;
if(matchR[v]==-1||dfs(matchR[v])){
matchR[v]=u;
return true;
}
}
return false;
}
int solve(){
int ans=0;
for(int u=0;u<n;u++){
stamp++;
ans+=dfs(u);
}
return ans;
}
};
struct GridBFSResult{
vector<vector<int>>dist;
vector<vector<pair<int,int>>>parent;
bool reachable(int r,int c)const{
return r>=0&&c>=0&&r<(int)dist.size()&&
c<(int)dist[0].size()&&dist[r][c]!=-1;
}
vector<pair<int,int>>path(int r,int c)const{
vector<pair<int,int>>p;
if(!reachable(r,c))return p;
while(r!=-1){
p.push_back({r,c});
auto q=parent[r][c];
r=q.first;
c=q.second;
}
reverse(p.begin(),p.end());
return p;
}
};
GridBFSResult gridBFS(const vector<string>&grid,int sr,int sc,
char wall='#',bool eightDirections=false){
int n=(int)grid.size();
int m=n?(int)grid[0].size():0;
GridBFSResult res;
res.dist.assign(n,vector<int>(m,-1));
res.parent.assign(n,vector<pair<int,int>>(m,{-1,-1}));
if(sr<0||sr>=n||sc<0||sc>=m||grid[sr][sc]==wall)
return res;
static const int dr4[]={1,-1,0,0};
static const int dc4[]={0,0,1,-1};
static const int dr8[]={1,-1,0,0,1,1,-1,-1};
static const int dc8[]={0,0,1,-1,1,-1,1,-1};
int dirs=eightDirections?8:4;
queue<pair<int,int>>q;
res.dist[sr][sc]=0;
q.push({sr,sc});
while(!q.empty()){
auto [r,c]=q.front();
q.pop();
for(int k=0;k<dirs;k++){
int nr=r+(eightDirections?dr8[k]:dr4[k]);
int nc=c+(eightDirections?dc8[k]:dc4[k]);
if(nr<0||nr>=n||nc<0||nc>=m)continue;
if(grid[nr][nc]==wall||res.dist[nr][nc]!=-1)continue;
res.dist[nr][nc]=res.dist[r][c]+1;
res.parent[nr][nc]={r,c};
q.push({nr,nc});
}
}
return res;
}
template<class T>
struct Fenwick{
int n;
vector<T>bit;
explicit Fenwick(int count=0):n(count),bit(count+1){}
void add(int idx,T val){
for(idx++;idx<=n;idx+=idx&-idx)bit[idx]+=val;
}
T prefix(int idx)const{
T s{};
for(idx++;idx>0;idx-=idx&-idx)s+=bit[idx];
return s;
}
T range(int l,int r)const{
if(l>r)return T{};
return prefix(r)-(l?prefix(l-1):T{});
}
};
struct LazySum{
int n;
vector<i64>tree,lazy;
explicit LazySum(int count=0)
:n(count),tree(4*count+4),lazy(4*count+4){}
void add(int ql,int qr,i64 v){add(1,0,n-1,ql,qr,v);}
i64 query(int ql,int qr){return query(1,0,n-1,ql,qr);}
void add(int p,int l,int r,int ql,int qr,i64 v){
if(ql<=l&&r<=qr){
tree[p]+=v*(r-l+1);
lazy[p]+=v;
return;
}
int mid=(l+r)>>1;
if(ql<=mid)add(p<<1,l,mid,ql,qr,v);
if(qr>mid)add(p<<1|1,mid+1,r,ql,qr,v);
tree[p]=tree[p<<1]+tree[p<<1|1]+
lazy[p]*(r-l+1);
}
i64 query(int p,int l,int r,int ql,int qr){
if(ql<=l&&r<=qr)return tree[p];
int mid=(l+r)>>1;
i64 ans=lazy[p]*(min(r,qr)-max(l,ql)+1);
if(ql<=mid)ans+=query(p<<1,l,mid,ql,qr);
if(qr>mid)ans+=query(p<<1|1,mid+1,r,ql,qr);
return ans;
}
};
struct RangeMin{
int n;
i64 neutral=INF64;
vector<i64>t;
explicit RangeMin(vector<i64>a){
n=1;
while(n<(int)a.size())n<<=1;
t.assign(2*n,neutral);
for(int i=0;i<(int)a.size();i++)t[n+i]=a[i];
for(int i=n-1;i;i--)t[i]=min(t[i<<1],t[i<<1|1]);
}
i64 query(int l,int r)const{
i64 ans=neutral;
for(l+=n,r+=n;l<=r;l>>=1,r>>=1){
if(l&1)ckmin(ans,t[l++]);
if(!(r&1))ckmin(ans,t[r--]);
}
return ans;
}
};
template<class T>
struct SparseTable{
vector<int>lg;
vector<vector<T>>st;
explicit SparseTable(const vector<T>&a){
int n=(int)a.size();
lg.assign(n+1,0);
for(int i=2;i<=n;i++)lg[i]=lg[i/2]+1;
int k=n?lg[n]+1:0;
st.assign(k,vector<T>(n));
if(n)st[0]=a;
for(int j=1;j<k;j++)
for(int i=0;i+(1<<j)<=n;i++)
st[j][i]=min(st[j-1][i],
st[j-1][i+(1<<(j-1))]);
}
T query(int l,int r)const{
int j=lg[r-l+1];
return min(st[j][l],st[j][r-(1<<j)+1]);
}
};
class FenwickKth{
int n;
vector<int>bit;
public:
explicit FenwickKth(int count):n(count),bit(count+1){}
void add(int idx,int v){
for(idx++;idx<=n;idx+=idx&-idx)bit[idx]+=v;
}
int prefix(int idx)const{
int s=0;
for(idx++;idx>0;idx-=idx&-idx)s+=bit[idx];
return s;
}
int kth(int k)const{
int idx=0;
int step=1;
while((step<<1)<=n)step<<=1;
for(;step;step>>=1){
int next=idx+step;
if(next<=n&&bit[next]<=k){
idx=next;
k-=bit[next];
}
}
return idx<n?idx:-1;
}
};
}
void solve(){
}
int main(){
ios::sync_with_stdio(false);
cin.tie(nullptr);
solve();
return 0;
}
