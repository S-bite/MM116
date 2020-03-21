#include <bits/stdc++.h>
#define _overload3(_1, _2, _3, name, ...) name
#define _rep(i, n) repi(i, 0, n)
#define repi(i, a, b) for (int i = (a); i < (b); ++i)
#define rep(...) _overload3(__VA_ARGS__, repi, _rep, )(__VA_ARGS__)
#define ALL(x) x.begin(), x.end()
#define chmax(x, y) x = max(x, y)
#define chmin(x, y) x = min(x, y)
using namespace std;
using ll = long long;
using lld = long double;
using VI = vector<int>;
using VVI = vector<VI>;
using VL = vector<ll>;
using VVL = vector<VL>;
using PII = pair<int, int>;
random_device rnd;
mt19937 mt(rnd());

const int IINF = 1 << 30;
const ll INF = 1ll << 60;
const ll MOD = 1e9 + 7;

class Tile
{
public:
    int h, w;
    vector<string> g;
    Tile(){};
    ~Tile(){};
};
class Tactics
{
public:
    int h, w;
    lld score;
    vector<string> g;
    vector<PII> pos;
    Tactics(){};
    ~Tactics(){};
};

bool operator>(const Tactics &t1, const Tactics &t2)
{
    return t1.score > t2.score;
};

lld p;
int n;
int t = 0;
vector<Tile> tiles;

void eval(Tactics &tact)
{
    tact.score = 0;
    lld compressionScore = (lld)tact.h * tact.w / (lld)t;
    lld sumdiff = 0;
    rep(i, n)
    {
        auto tile = tiles[i];
        int sy = tact.pos[i].first;
        int sx = tact.pos[i].second;
        rep(y, tile.h)
        {
            rep(x, tile.w)
            {
                // cerr << sx << " " << x << " " << tact.w << endl;
                assert(sy + y <= tact.h);
                assert(sx + x <= tact.w);
                sumdiff += abs(tile.g[y][x] - tact.g[sy + y][sx + x]);
            }
        }
    }
    lld lossinessScore = sumdiff / ((lld)12.5 * t);
    tact.score = compressionScore * p + lossinessScore * (1 - p);
}
void init()
{
    cin >> p;
    cin >> n;
    tiles.resize(n);
    rep(i, n)
    {
        int ch, cw;
        cin >> ch;
        tiles[i].g.resize(ch);
        rep(j, ch)
        {
            string s;
            cin >> s;
            cw = s.size();
            tiles[i].g[j] = s;
        }
        tiles[i].h = ch;
        tiles[i].w = cw;
        t += ch * cw;
    }
}

void initTactics(Tactics &tact, int mh, int mw)
{
    tact.h = mh;
    tact.w = mw;
    tact.g.resize(tact.h);
    tact.pos.resize(n);

    rep(i, mh)
    {
        rep(j, mw)
        {
            tact.g[i] += 'A' + mt() % 26;
        }
    }
    rep(i, n)
    {
        tact.pos.push_back({0, 0});
    }
}

void printTactics(Tactics &tact)
{
    cout << tact.h << endl;
    rep(i, tact.h)
    {
        cout << tact.g[i] << endl;
    }
    rep(i, n)
    {
        cout << tact.pos[i].first << " " << tact.pos[i].second << endl;
    }
}

bool canPut(Tactics &tact, int index, int py, int px)
{
    auto th = tiles[index].h;
    auto tw = tiles[index].w;
    return (py + th <= tact.h && px + tw <= tact.w);
}

void changePos(Tactics &tact)
{
    int ri, rx, ry;
    int cnt = 0;
    do
    {
        ri = mt() % n;
        ry = mt() % tact.h;
        rx = mt() % tact.w;
        cnt++;
    } while (canPut(tact, ri, ry, rx) == false && cnt < 999);
    if (cnt == 1000)
    {
        cerr << "combination not found." << endl;
        return;
    }
    tact.pos[ri] = {ry, rx};
    eval(tact);
}
void changeCol(Tactics &tact)
{
    int ry = mt() % tact.h;
    int rx = mt() % tact.w;
    tact.g[ry][rx] = 'A' + mt() % 26;
    eval(tact);
}
Tactics search(int mh, int mw)
{
    int beamsize = 50;
    clock_t starttime = clock();
    priority_queue<Tactics, vector<Tactics>, greater<Tactics>> pque;
    rep(j, beamsize)
    {
        Tactics base;
        initTactics(base, mh, mw);
        eval(base);
        pque.push(base);
    }
    int loop = 0;
    while (double(clock() - starttime) / CLOCKS_PER_SEC < 9)
    {
        auto tmp = pque;
        while (!pque.empty())
        {
            auto tact = pque.top();
            pque.pop();
            changePos(tact);
            tmp.push(tact);
            changeCol(tact);
            tmp.push(tact);
        }
        for (int i = 0; !tmp.empty() && i < beamsize; i++)
        {

            pque.push(tmp.top());
            tmp.pop();
        }
        loop++;
    }
    cerr << "loop: " << loop << endl;
    return pque.top();
}

int main()
{
    init();
    int mh = -1, mw = -1;
    for (auto tile : tiles)
    {
        chmax(mh, tile.h);
        chmax(mw, tile.w);
    }
    Tactics ans = search(mh, mw);
    printTactics(ans);
    cerr << fixed << setprecision(18) << ans.score << endl;
    return 0;
}