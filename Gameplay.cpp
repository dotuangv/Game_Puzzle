#include "Gameplay.h"
#include "Game.h"
void Gameplay::CheckRand()
{
    set<int> s;
    for (int i = n; i >= 1; i--) {
        for (int j = n; j >= 1; j--) {
            if (a[i][j] != 0) {
                s.insert(a[i][j]);
                set<int> ::iterator it = s.find(a[i][j]);
                if (it != s.begin()) {
                    check += distance(s.begin(), it);
                }
            }
        }
    }
}

void Gameplay::Random(int height)
{
    Height = height;
    while (1) {
        check = 0;
        b.resize(n * n);
        for (int i = 0; i <= n * n - 1; i++) b[i] = i;
        srand(time(NULL));
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++) {
                int tmp = (rand() + tam++) % b.size();
                a[i][j] = b[tmp];
                posIMG[a[i][j]] = { Height/n * (j - 1), Height/n * (i - 1) };
                if (a[i][j] == 0) zero = i;
                b.erase(b.begin() + tmp);
            }
        }
        CheckRand();
        if ((check + zero * (n % 2 + 1)) % 2 == 0) break;
    }
}

void Gameplay::display(vector<vector<int>> a)
{
    cnt = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            printf("%3d ", a[i][j]);
            if (a[i][j] != Goal[i][j]) cnt = 0;
        }
        cout << endl;
    }
}

void Gameplay::Solve(pair<int, int> p)
{
    int W = a[p.first][p.second];
    int x = a[p.first - 1][p.second], y = a[p.first][p.second - 1], z = a[p.first + 1][p.second], t = a[p.first][p.second + 1];
    if (!x) {
        //cout << "HELLO x: " << x << endl;
        swap(a[p.first - 1][p.second], a[p.first][p.second]);
        swap(posIMG[W], posIMG[0]);
        return;
    }
    if (!y) {
        //cout << "HELLO y: " << y << endl;
        swap(a[p.first][p.second - 1], a[p.first][p.second]);
        swap(posIMG[W], posIMG[0]);
        return;
    }
    if (!z) {
        //cout << "HELLO z: " << z << endl;
        swap(a[p.first + 1][p.second], a[p.first][p.second]);
        swap(posIMG[W], posIMG[0]);
        return;
    }
    if (!t) {
        //cout << "HELLO t: " << t << endl;
        swap(a[p.first][p.second + 1], a[p.first][p.second]);
        swap(posIMG[W], posIMG[0]);
        return;
    }
}

pair<int, int> Gameplay::Pos(int x, vector<vector<int>> v)
{

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (v[i][j] == x) {
                return { i, j };
            }
        }
    }
}

bool Gameplay::CheckTrung(vector<vector<int>> a)
{
    for (int i = 0; i < CLOSE.size(); i++) {
        if (a == CLOSE[i]) return true;
    }
    return false;
}

bool Gameplay::CheckGoal(vector<vector<int>> a)
{
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (a[i][j] != Goal[i][j]) return false;
        }
    }
    return true;
}

int Gameplay::Heuristic(vector<vector<int>> a)
{
    double d = 0, A = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (a[i][j] != 0) {
                int row = (a[i][j] + n - 1) / n;
                int col = (a[i][j] - 1) % n + 1;
                d += pow(i - row, 2) + pow(j - col, 2);
                //				cout << a[i][j] << " " << pow(i - row, 2) + pow(j - col, 2) << endl;
            }
            if (a[i][j] == Goal[i][j + 1] && a[i][j + 1] == Goal[i][j]) A++;
            if (a[i][j] == Goal[i + 1][j] && a[i + 1][j] == Goal[i][j]) A++;
        }
    }
    double h = round(d - 0.15 * d + A * 2);
    return int(h);
}

void Gameplay::KhoiTao()
{
    pair<int, int> p = Pos(0, a);
    FRINGE[0].first = a;
    int g = 1;
    int x = a[p.first - 1][p.second], y = a[p.first][p.second - 1], z = a[p.first + 1][p.second], t = a[p.first][p.second + 1];
    if (x != n * n) {
        vector<vector<int> > tmp = a;
        swap(tmp[p.first][p.second], tmp[p.first - 1][p.second]);
        FRINGE[res].first = tmp;
        FRINGE[res].second = x;
        int h = Heuristic(tmp);
        OPEN.push({ g + h, {g, res} });
        FATHER[res] = 0;
        res++;
    }
    if (y != n * n) {
        vector<vector<int> > tmp = a;
        swap(tmp[p.first][p.second], tmp[p.first][p.second - 1]);
        FRINGE[res].first = tmp;
        FRINGE[res].second = y;
        int h = Heuristic(tmp);
        OPEN.push({ g + h, {g, res} });
        FATHER[res] = 0;
        res++;
    }
    if (z != n * n) {
        vector<vector<int> > tmp = a;
        swap(tmp[p.first][p.second], tmp[p.first + 1][p.second]);
        FRINGE[res].first = tmp;
        FRINGE[res].second = z;
        int h = Heuristic(tmp);
        OPEN.push({ g + h, {g, res} });
        FATHER[res] = 0;
        res++;
    }
    if (t != n * n) {
        vector<vector<int> > tmp = a;
        swap(tmp[p.first][p.second], tmp[p.first][p.second + 1]);
        FRINGE[res].first = tmp;
        FRINGE[res].second = t;
        int h = Heuristic(tmp);
        OPEN.push({ g + h, {g, res} });
        FATHER[res] = 0;
        res++;
    }
}

void Gameplay::AuToRun()
{
    KhoiTao();
    vector<vector<int>> Curent;
    int k = 1, l;
    while (!OPEN.empty()) {
        int g = OPEN.top().second.first;
        l = OPEN.top().second.second;
        Curent = FRINGE[l].first;
        CLOSE.push_back(Curent);
        OPEN.pop();
        if (CheckGoal(Curent)) break;
        //display(Curent);
        pair<int, int> p = Pos(0, Curent);
        int x = Curent[p.first - 1][p.second], y = Curent[p.first][p.second - 1], z = Curent[p.first + 1][p.second], t = Curent[p.first][p.second + 1];
        if (x != n * n) {
            vector<vector<int> > tmp = Curent;
            swap(tmp[p.first][p.second], tmp[p.first - 1][p.second]);
            if (!CheckTrung(tmp)) {
                int h = Heuristic(tmp);
                OPEN.push({ g + h, {g, res} });
                FATHER[res] = l;
                FRINGE[res].first = tmp;
                FRINGE[res].second = x;
                res++;
            }
        }
        if (y != n * n) {
            vector<vector<int> > tmp = Curent;
            swap(tmp[p.first][p.second], tmp[p.first][p.second - 1]);
            if (!CheckTrung(tmp)) {
                int h = Heuristic(tmp);
                OPEN.push({ g + h, {g, res} });
                FATHER[res] = l;
                FRINGE[res].first = tmp;
                FRINGE[res].second = y;
                res++;
            }
        }
        if (z != n * n) {
            vector<vector<int> > tmp = Curent;
            swap(tmp[p.first][p.second], tmp[p.first + 1][p.second]);
            if (!CheckTrung(tmp)) {
                int h = Heuristic(tmp);
                OPEN.push({ g + h, {g, res} });
                FATHER[res] = l;
                FRINGE[res].first = tmp;
                FRINGE[res].second = z;
                res++;
            }
        }
        if (t != n * n) {
            vector<vector<int> > tmp = Curent;
            swap(tmp[p.first][p.second], tmp[p.first][p.second + 1]);
            if (!CheckTrung(tmp)) {
                int h = Heuristic(tmp);
                OPEN.push({ g + h, {g, res} });
                FATHER[res] = l;
                FRINGE[res].first = tmp;
                FRINGE[res].second = t;
                res++;
            }
        }
        //cout << OPEN.size() << endl;
        //cout << k << endl;
        //        cout << endl;
        k++;
    }
    //    cout << k << endl;
    KQ.push_back(l);
    //    display(Curent);

    while (FATHER[l] != 0) {
        KQ.push_back(FATHER[l]);
        l = FATHER[l];
    }
    //cout << KQ.size() << endl;
    //for (int i = KQ.size() - 1; i >= 0; i--) {
    //    cout << FRINGE[KQ[i]].second << endl;
    //    display(FRINGE[KQ[i]].first);
    //    cout << endl;
    //}
}

void Gameplay::Clear()
{
    FATHER.clear();
    FRINGE.clear();
    CLOSE.clear();
    KQ.clear();
    while (!OPEN.empty()) {
        OPEN.pop();
    }
}

void Gameplay::setA()
{
    for (int i = 0; i <= n + 1; i++) {
        for (int j = 0; j <= n + 1; j++) {
            a[i][j] = n * n;
        }
    }
}

void Gameplay::setGoal()
{
    int tmp = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            Goal[i][j] = tmp;
                        //cin >> a[i][j];
            tmp++;
        }
    }
    Goal[n][n] = 0;
}

vector<vector<int>> Gameplay::getA()
{
    return a;
}

vector<vector<int>> Gameplay::getGoal()
{
    return Goal;
}

pair<int, int> Gameplay::getPosImg(int i) {
    return posIMG[i];
}

int Gameplay::getN() {
    return n;
}

int Gameplay::checkPos(pair<int, int> p)
{
    for (int i = 1; i <= n * n - 1; i++)
    {
        if (posIMG[i] == p) return i;
    }
    return 0;
}

void Gameplay::setN(int n) {
    this->n = n;
}
//Gameplay::~Game() {};

GameObject** Number;

SDL_Event Gameplay::event;
SDL_Texture* PlayerTex;
SDL_Rect srcR, destR;
void Gameplay::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;

    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        cout << "Subsystems Initialised!..." << endl;
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) {
            cout << "Window created!" << endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            cout << "Renderer created!" << endl;
        }

        isRunning = true;
    }
    else {
        isRunning = false;
    }
    setA();
    Random(height);
    setGoal();
    Number = new GameObject * [n * n + 1];
    const char* s1 = "Data/YourName";
    const char* s2 = "-";
    const char* s3 = ".png";
    string N = to_string(n);
    for (int i = 0; i <= n * n - 1; i++) {
        string str = to_string(i);
        string s = s1 + N + s2 + str + s3;
        const char* filename = s.c_str();
        Number[i] = new GameObject(filename, renderer, posIMG[i].first, posIMG[i].second, height/n, n);
    }
}

void Gameplay::handleEvents() {

    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
        {
            pair<int, int> pa = Pos(0, a);
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
            {
                int x = a[pa.first + 1][pa.second];
                if (x != n * n) {
                    swap(a[pa.first + 1][pa.second], a[pa.first][pa.second]);
                    swap(posIMG[x], posIMG[0]);
                }
                break;
            }
            case SDLK_DOWN:
            {
                int x = a[pa.first - 1][pa.second];
                if (x != n * n) {
                    swap(a[pa.first - 1][pa.second], a[pa.first][pa.second]);
                    swap(posIMG[x], posIMG[0]);
                }
                break;
            }
            case SDLK_LEFT:
            {
                int x = a[pa.first][pa.second + 1];
                if (x != n * n) {
                    swap(a[pa.first][pa.second + 1], a[pa.first][pa.second]);
                    swap(posIMG[x], posIMG[0]);
                }
                break;
            }
            case SDLK_RIGHT:
            {
                int x = a[pa.first][pa.second - 1];
                if (x != n * n) {
                    swap(a[pa.first][pa.second - 1], a[pa.first][pa.second]);
                    swap(posIMG[x], posIMG[0]);
                }
                break;
            }
            default:
                break;
            }
         }
        case SDL_MOUSEBUTTONDOWN:
        {
            int x = event.motion.x, y = event.motion.y;
            int H = Height / n;
            int Px = x / H * H, Py = y / H * H;
            int P = checkPos({ Px, Py });
            if (P)
            {
                Solve(Pos(P, a));
            }
            break;
        }
        default:
            break;
    }
    isRunning = !CheckGoal(a);
}

void Gameplay::SolveGame()
{
    AuToRun();
    handleEvents();
    update();
    render();
    SDL_Delay(1000);
    for (int i = KQ.size() - 1; i >= 0; i--)
    {
        int x = FRINGE[KQ[i]].second;
        //display(FRINGE[i].first);
        Solve(Pos(x, a));
        handleEvents();
        update();
        render();

        SDL_Delay(500);
        /*frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }*/
    }
    CheckGoal(a);
    SDL_Delay(500);
    update();
    render();
    SDL_Delay(10000);
}



void Gameplay::update() {
    for (int i = 0; i <= n * n - 1; i++)
    {
        Number[i]->Update(posIMG[i].first, posIMG[i].second);
    }
}

void Gameplay::render() {
    SDL_RenderClear(renderer);
    for (int i = 1; i <= n * n - 1; i++)
    {
        Number[i]->Render();
    }
    if (!isRunning) Number[0]->Render();
    SDL_RenderPresent(renderer);
}

void Gameplay::Play()
{
    while (running()) {
        //frameStart = SDL_GetTicks();
        handleEvents();
        update();
        render();
        //frameTime = SDL_GetTicks() - frameStart;
        //if (frameDelay > frameTime) {
        //    SDL_Delay(frameDelay - frameTime);
        //}
        if (!running())
        {
            SDL_Delay(500);
            update();
            render();
            SDL_Delay(10000);
        }
    }
}

void Gameplay::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
