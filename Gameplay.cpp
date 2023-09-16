#include "Gameplay.h"
#include "Game.h"

void Gameplay::CheckRand()
{
    //Kiểm tra điều kiện random
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
    //Hàm random + tính tọa độ ảnh
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
                //Tính tọa độ ảnh
                posIMG[a[i][j]] = { Height/n * (j - 1), Height/n * (i - 1) };
                //Tìm hàng chứa ô trống
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
    //Hàm in ra màn hình
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
    // Tính 4 vị trí xung quanh ô đang xét
    int x = a[p.first - 1][p.second], y = a[p.first][p.second - 1], z = a[p.first + 1][p.second], t = a[p.first][p.second + 1];
    // Kiểm tra xem ô nào trống thì đổi chỗ và vị trí của ô trống với ô đang xét
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
    //Hàm trả về tọa độ của số x ở trong vector v
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
    //Hàm kiểm tra xem vector được sinh ra đã nằm trong CLOSE chưa
    for (int i = 0; i < CLOSE.size(); i++) {
        if (a == CLOSE[i]) return true;
    }
    return false;
}

bool Gameplay::CheckGoal(vector<vector<int>> a)
{
    // Hàm kiểm tra xem trạng thái a có phải là trạng thái đích cần xét đến không
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (a[i][j] != Goal[i][j]) return false;
        }
    }
    return true;
}

int Gameplay::Heuristic(vector<vector<int>> a)
{
    // Hàm Heuristic
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
    // Hàm khởi tạo thêm vào FRINGE các trạng thái hợp lệ xung quanh ô trống
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
        // Thêm vào hàng đợi ưu tiền OPEN f ( = g + h), vị trí của mảng thêm vào (res), chi phí đi từ lúc đầu đến trạng thái hiện tại(g)
        OPEN.push({ g + h, {g, res} });
        //Lưu cha của mảng thứ res là 0;
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
    //Khởi tạo trạng thái ban đầu.
    KhoiTao();
    // Curent là trạng thái đang xét hiện tại
    vector<vector<int>> Curent;
    int k = 1, l;
    while (!OPEN.empty()) {
        int g = OPEN.top().second.first;
        // lấy ra vị trí của trạng thái có chi phí thấp nhất trong OPEN
        l = OPEN.top().second.second;
        // Cho trạng thái hiện tại đang xét là trạng thái tối ưu (vị trí l đã tính)
        Curent = FRINGE[l].first;
        // Đẩy trạng thái hiện tại vào CLOSE ( xác nhận trạng thái này đã xét)
        CLOSE.push_back(Curent);
        // Xóa đi trạng thái vừa xét ra khỏi OPEN
        OPEN.pop();
        // Nếu trạng thái đang xét là trạng thái đích thì thoát vòng lặp
        if (CheckGoal(Curent)) break;

        pair<int, int> p = Pos(0, Curent);
        int x = Curent[p.first - 1][p.second], y = Curent[p.first][p.second - 1], z = Curent[p.first + 1][p.second], t = Curent[p.first][p.second + 1];
        // Thêm các trạng thái hợp lệ được sinh ra bởi trạng thái đang xét vào OPEN
        if (x != n * n) {
            vector<vector<int> > tmp = Curent;
            swap(tmp[p.first][p.second], tmp[p.first - 1][p.second]);
            // Nếu trạng thái này chưa nằm trong CLOSE thì ta thêm vào OPEN
            if (!CheckTrung(tmp)) {
                int h = Heuristic(tmp);
                // Thêm vào hàng đợi ưu tiền OPEN f ( = g + h), vị trí của mảng thêm vào (res), chi phí đi từ lúc đầu đến trạng thái hiện tại(g)
                OPEN.push({ g + h, {g, res} });
                // Đặt cha của trạng thái đang xét hiện tại là vị trí của trạng thái vừa sinh ra nó (l)
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
        //k++;
    }
    // Đẩy trạng thái đích vào KQ
    KQ.push_back(l);
    // Tìm các trạng thái đã suy ra được trạng thái đích
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
    //Đặt tráng thái ban đầu của vecto a
    for (int i = 0; i <= n + 1; i++) {
        for (int j = 0; j <= n + 1; j++) {
            a[i][j] = n * n;
        }
    }
}

void Gameplay::setGoal()
{
    // Đặt trạng thái đích
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
    // Kiểm tra xem ô nào đang chứa tọa độ p ( dùng để xử lí sự kiện )
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
GameObject** StartGame;
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
}

void Gameplay::SetStart()
{
    // Khởi tạo các đối tượng giao diện của game ( hình nền và các chế độ chơi )
    StartGame = new GameObject * [5];
    const char* s1 = "Data/Anh";
    const char* s2 = "x";
    const char* s3 = ".png";
    //string N = to_string(n);
    for (int i = 0; i <= 3; i++) {
        string str = to_string(i + 3);
        string s = s1 + str + s2 + str + s3;
        const char* filename = s.c_str();
        StartGame[i] = new GameObject(filename, renderer, PosG[i].first, PosG[i].second, 5);
    }
    StartGame[4] = new GameObject("Data/Background2.png", renderer, 0, 0, 5);
}

void Gameplay ::StartUpdate()
{
    // Cập nhật tọa độ của các đối tượng giao diện lúc đầu của game ( hình nền và các chế độ chơi )
    StartGame[4]->Update(0, 0, 1050, 1400, 1000, 600);
    for (int i = 0; i <= 3; i++)
    {
        StartGame[i]->Update(PosG[i].first, PosG[i].second, 150, 450, WH[i].first, WH[i].second);
        if (WH[i].first == 210) isRunning2 = false;
    }
}

void Gameplay::StartRenderer()
{
    // Hàm hiển thị các đối tượng giao diện game ra màn hình
    SDL_RenderClear(renderer);
    StartGame[4]->Render();
    for (int i = 0; i <= 3; i++)
    {
        StartGame[i]->Render();
    }
    SDL_RenderPresent(renderer);
}

void Gameplay::StartEvents()
{
    // Hàm xử lí sự kiện lúc đầu ( để chọn chế độ chơi )
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        break;
    case SDL_MOUSEBUTTONDOWN:
    {
        int x = event.motion.x, y = event.motion.y;
        if (410 <= x && x <= 590)
        {
            if (y >= 130 && y <= 190)
            {
                //ô này là ô 3x3
                if (!n)
                {
                    // Đặt lại tọa độ của đối tượng nút cho hợp lí vì kích thươc nút đã đc tăng lên
                    PosG[0] = { 395, 135 };
                    // Đặt lại kích thước của đối tượng nút ( Giúp tạo cảm giác nhấn nút khi kick vào )
                    WH[0] = { 210, 70 };
                    // Đặt n = 3, biểu thị chế độ chơi 3x3
                    n = 3;
                }
            }
            else if (y >= 230 && y <= 290)
            {
                //ô này là ô 4x4
                if (!n)
                {
                    PosG[1] = { 395, 235 };
                    WH[1] = { 210, 70 };
                    n = 4;
                }
            }
            else if (y >= 330 && y <= 390)
            {
                //ô này là ô 5x5
                if (!n)
                {
                    PosG[2] = { 395, 335 };
                    WH[2] = { 210, 70 };
                    n = 5;
                }
            }
            else if (y >= 430 && y <= 490)
            {
                //ô này là ô 6x6
                if (!n)
                {
                    PosG[3] = { 395, 435 };
                    WH[3] = { 210, 70 };
                    n = 6;
                }
            }
        }
        break;
    }
    default:
        break;
    }
}

void Gameplay::SetUpGame(int height)
{
    // Hàm tạo các đối tượng ( các mảnh của puzzle ) gồm tọa độ, file ảnh, vị trí ảnh
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
        Number[i] = new GameObject(filename, renderer, posIMG[i].first, posIMG[i].second, n);
    }
}

void Gameplay::handleEvents() {
    // Hàm xử lí sự kiện để chơi trò chơi
    bool isQuit = false;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            isQuit = true;
            break;
        case SDL_KEYDOWN:
        {
            // Khi nhấn xuống phím bất kì thì ta sẽ tìm tọa độ vị trí ô trống
            pair<int, int> pa = Pos(0, a);
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
            {
                // nếu phím được nhấn là phím Up thì ta xét vị trí dưới ô trống, nếu vị trí đó hợp lí thì hoán đổi 2 ô
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
            // Xử lí sự kiện bằng chuột, lấy tọa độ của vị trí chuột nhấn vào (x, y)
            int x = event.motion.x, y = event.motion.y;
            //if (x > 600)
            //{
            //    SolveGame();
            //}
            int H = Height / n;
            // Tính toán xem vị trí click đang nằm ở ô nào ( đưa về vị trí lúc đầu ta xét để dễ tính )
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
    // Nếu trạng thái hiện tại là trạng thái đích thì kết thúc vòng lặp, đánh giấu trò chơi đã dừng
    isRunning = !CheckGoal(a) && !isQuit;
}

void Gameplay::SolveGame()
{
    // Hàm giải game bằng AI
    AuToRun();
    //handleEvents();
    update();
    render();
    SDL_Delay(1000);
    bool isQuit = false;
    SDL_Event e;
    for (int i = KQ.size() - 1; i >= 0 && !isQuit; i--)
    {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) isQuit = true;
        }
        // Tìm vị trị trong mảng KQ và cập nhật lại các ảnh của trạng thái đang xét đến
        int x = FRINGE[KQ[i]].second;
        //display(FRINGE[i].first);
        Solve(Pos(x, a));
        //StartEvents();
        update();
        render();

        SDL_Delay(500);
        /*frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }*/
    }
    isRunning = false;
    if (!isQuit) {
        update();
        render();
        while (!isQuit)
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) isQuit = true;
        }
        //SDL_Delay(500);
        //SDL_Delay(10000);
    }
}



void Gameplay::update() {
    // Hàm cập nhật lại các tọa độ của các đối tượng của game ( các mảnh puzzle )
    for (int i = 0; i <= n * n - 1; i++)
    {
        Number[i]->Update(posIMG[i].first, posIMG[i].second, Height/n, Height/n, Height/n, Height/n);
    }
}

void Gameplay::render() {
    // Hàm hiển thị hình ảnh các đối tượng của game ra màn hình ( các mảnh puzzle )
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
    // Hàm để chơi game bằng bàn phím và chuột
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
            if (CheckGoal(a))
            {
                SDL_Delay(500);
                update();
                render();
                SDL_Delay(10000);
            }
        }
    }
}

void Gameplay::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
