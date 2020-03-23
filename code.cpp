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
uniform_real_distribution<> urand(0, 1.0); // [1.0, 2.0] 範囲の一様乱数
const int LOOPNUM = 10 * 1000;
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
    lld lossinessScore;
    lld compressionScore;
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
int loop = 0;
vector<Tile> tiles;
vector<PII> shuffledPos;

bool canPut(Tactics &tact, Tile &tile, int py, int px)
{
    auto th = tile.h;
    auto tw = tile.w;
    return (py + th <= tact.h && px + tw <= tact.w);
}

int evalPiece(Tactics &tact, Tile &tile, PII pos)
{
    int ret = 0;
    rep(i, tile.h) rep(j, tile.w)
    {
        ret += abs(tact.g[pos.first + i][pos.second + j] - tile.g[i][j]);
    }
    return ret;
}

void fitPosAndEval(Tactics &tact)
{
    tact.score = 0;
    tact.compressionScore = (lld)tact.h * tact.w / (lld)t;
    lld sumdiff = 0;
    rep(i, n)
    {
        auto tile = tiles[i];
        int sy = -1;
        int sx = -1;
        int bestscore = IINF;
        rep(_, 30)
        {

            int y, x;
            do
            {
                y = mt() % tact.h;
                x = mt() % tact.w;
            } while (canPut(tact, tile, y, x) == false);
            //if (canPut(tact, tile, y, x) == false)
            //    continue;
            int curscore = evalPiece(tact, tile, {y, x});
            if (bestscore >= curscore)
            {
                bestscore = curscore;
                sy = y;
                sx = x;
            }
        }
        /*        rep(y, tile.h)
        {
            rep(x, tile.w)
            {
            }
            if (canPut(tact, tile, y, 0) == false)
                continue;
        }*/
        sumdiff += bestscore;
        tact.pos[i] = {sy, sx};
    }
    tact.lossinessScore = sumdiff / ((lld)12.5 * t);
    tact.score = tact.compressionScore * p + tact.lossinessScore * (1 - p);
}
void fitColorAndEval(Tactics &tact)
{
    tact.score = 0;
    tact.compressionScore = (lld)tact.h * tact.w / (lld)t;
    lld sumdiff = 0;
    vector<vector<vector<int>>> tmp(tact.h, VVI(tact.w, VI(0)));
    rep(i, n)
    {
        auto tile = tiles[i];
        auto pos = tact.pos[i];
        rep(y, tile.h)
        {
            rep(x, tile.w)
            {
                tmp[pos.first + y][pos.second + x].push_back(tile.g[y][x]);
            }
        }
    }
    rep(i, tact.h)
    {
        rep(j, tact.w)
        {
            auto v = tmp[i][j];
            sort(ALL(v));
            int med;
            if (v.size() == 0)
            {
                med = 'A';
            }
            else if (v.size() % 2 == 1)
            {
                med = v[v.size() / 2];
            }
            else
            {
                med = (v[v.size() / 2 - 1] + v[v.size() / 2]) / 2;
            }
            tact.g[i][j] = med;
            for (auto col : v)
                sumdiff += abs(med - col);
        }
    }

    tact.lossinessScore = sumdiff / ((lld)12.5 * t);
    tact.score = tact.compressionScore * p + tact.lossinessScore * (1 - p);
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
        int y;
        int x;
        do
        {
            y = mt() % mh;
            x = mt() % mw;
        } while (canPut(tact, tiles[i], y, x) == false);
        tact.pos.push_back({y, x});
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

void changePos(Tactics &tact)
{
    rep(i, 2)
    {
        int ri, rx, ry;
        int cnt = 0;
        do
        {
            ri = mt() % n;
            ry = mt() % tact.h;
            rx = mt() % tact.w;
            cnt++;
        } while (canPut(tact, tiles[ri], ry, rx) == false && cnt < 999);
        tact.pos[ri] = {ry, rx};
    }
}

lld f(int n)
{
    return (lld)6.5 * n - 6.0;
}

void printWrapCount(Tactics &tact)
{
    vector<vector<vector<int>>> tmp(tact.h, VVI(tact.w, VI(0)));

    rep(i, n)
    {
        auto tile = tiles[i];
        auto pos = tact.pos[i];
        rep(y, tile.h)
        {
            rep(x, tile.w)
            {
                tmp[pos.first + y][pos.second + x].push_back(tile.g[y][x]);
            }
        }
    }
    lld rough = 0;
    rep(i, tact.h)
    {
        rep(j, tact.w)
        {
            cerr << tmp[i][j].size() << " ";
            rough += f(tmp[i][j].size());
        }
        cerr << endl;
    }
    cerr << fixed << setprecision(18) << (rough / (12.5 * t)) << endl;
}

void changeCol(Tactics &tact)
{
    rep(i, 1)
    {
        int ry = mt() % tact.h;
        int rx = mt() % tact.w;
        tact.g[ry][rx] = 'A' + mt() % 26;
    }
    tact.score = -1;
}
Tactics search(int mh, int mw)
{
    int beamsize = 30;
    clock_t starttime = clock();
    priority_queue<Tactics, vector<Tactics>, greater<Tactics>> pque;
    rep(j, 50)
    {
        Tactics base;
        initTactics(base, mh, mw);
        //initTactics(base, mh * (6), mw * (6));
        fitColorAndEval(base);
        pque.push(base);
    }
    while (double(clock() - starttime) / CLOCKS_PER_SEC < 9)
    {
        auto tmp = pque;
        while (!pque.empty())
        {
            auto tact = pque.top();
            pque.pop();
            changePos(tact);
            fitColorAndEval(tact);
            tmp.push(tact);
        }
        for (int i = 0; !tmp.empty() && i < beamsize; i++)
        {

            pque.push(tmp.top());
            tmp.pop();
        }
        loop++;
#ifdef LOCAL
        /*if (loop % 1000 == 0)
        {
            cerr << loop / 1000 << "," << fixed << setprecision(18) << pque.top().score << endl;
        }*/
#endif
    }
    return pque.top();
}

lld estimateScore(lld area)
{
    return (lld)area / (lld)t * p + max((lld)0, (13.0 / 25.0 - (12.0 / 25.0) * ((lld)area / (lld)t) * (1 - p)));
}

lld getOptimalArea(int mh, int mw)
{
    lld optimalArea = 0;
    lld curArea = mh * mw;
    lld optimalEstimatedScore = 1010101;
    while (curArea <= 100 * 100)
    {
        lld curScore = estimateScore(curArea);
        if (curScore < optimalEstimatedScore)
        {
            optimalEstimatedScore = curScore;
            optimalArea = curArea;
        }
        curArea++;
    }
    return optimalArea;
}

void runtest(int seed)
{
    mt19937 mt_testgen(seed);
    tiles.resize(n);
    rep(i, n)
    {
        int ch, cw;
        ch = mt() % 9 + 2;
        cw = mt() % 9 + 2;
        tiles[i].g.resize(ch);
        rep(j, ch)
        {

            string s;
            rep(k, cw)
            {
                s += 'A' + mt() % 26;
            }
            tiles[i].g[j] = s;
        }
        tiles[i].h = ch;
        tiles[i].w = cw;
        t += ch * cw;
    }
    int mh = -1, mw = -1;
    for (auto tile : tiles)
    {
        chmax(mh, tile.h);
        chmax(mw, tile.w);
    }
    lld optimalArea = getOptimalArea(mh, mw);
    chmax(optimalArea, (lld)100.5);
    mh = 10;
    mw = optimalArea / mh;
    auto best = search(mh, mw);
    cerr << "N = " << n << " ,P = " << p << " ,Seed = " << seed << endl;
    cerr << "areaOfTiles     : " << t << endl;
    cerr << "areaOfAnswer    : " << best.h * best.w << endl;
    cerr << "compressionScore: " << fixed << setprecision(18) << best.compressionScore << endl;
    cerr << "lossinessScore  : " << fixed << setprecision(18) << best.lossinessScore << endl;
    cerr << "rawScore        : " << fixed << setprecision(18) << best.score << endl;
    cerr << "loop              : " << loop << endl;
    //printWrapCount(best);
}
int main(int argc, char *argv[])
{
#ifdef LOCAL
    //cerr << "local test. loop " << LOOPNUM << " times." << endl;
    p = atof(argv[1]);
    n = atoi(argv[2]);
    int seed;
    if (argc == 3)
    {
        seed = mt();
    }
    else
    {
        seed = atoi(argv[3]);
    }

    //cerr << "P=" << p << " N=" << n << " seed=" << seed << endl;
    runtest(seed);
#else

    init();
    int mh = -1, mw = -1;
    for (auto tile : tiles)
    {
        chmax(mh, tile.h);
        chmax(mw, tile.w);
    }
    lld optimalArea = getOptimalArea(mh, mw);
    cerr << n << " " << p << " " << t << endl;
    cerr << optimalArea << endl;
    mw = max((int)(optimalArea / mh), mw);
    Tactics ans = search(mh, mw);
    printTactics(ans);

    cerr << fixed << setprecision(18) << ans.score << endl;
    cerr << "loop: " << loop << endl;
    cerr << "base size   : (" << mh << "," << mw << ")" << endl;
    cerr << "actual size : (" << ans.h << "," << ans.w << ")" << endl;
#endif
    return 0;
}