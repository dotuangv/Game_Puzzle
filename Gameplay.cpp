#include "Gameplay.h"
#include "LButton.h"

LButton gButton[TOTAL_GAMEPLAY_BUTTONS];
LTexture GamePlayButton[TOTAL_GAMEPLAY_BUTTONS];
SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

bool Gameplay::LoadMedia() 
{
    bool success = true;
    if (!GamePlayButton[BUTTON_AUTO_RUN].loadFromFile("IMG//AutoRun.png"))
    {
        success = false;
    }
    else if (!GamePlayButton[BUTTON_BACK].loadFromFile("IMG//Back.png"))
    {
        success = false;
    }
    else if (!GamePlayButton[BUTTON_RELOAD].loadFromFile("IMG//Reload.png"))
    {
        success = false;
    }
    else if (!GamePlayButton[BUTTON_SELF_SOLVING].loadFromFile("IMG//SelfSolving.png"))
    {
        success = false;
    }
    else {
        for (int i = 0; i < 2; ++i) {
            gSpriteClips[i].x = 50;
            gSpriteClips[i].y = 190 * i;
            gSpriteClips[i].w = 527;
            gSpriteClips[i].h = 180;
        }
        gButton[BUTTON_AUTO_RUN].SetAllValue(810, 490, 339, 64);
        gButton[BUTTON_RELOAD].SetAllValue(810, 360, 360, 180);
        gButton[BUTTON_SELF_SOLVING].SetAllValue(806, 570, 332, 128);
        gButton[BUTTON_BACK].SetAllValue(SCREEN_WIDTH,0, 128, 128);

    }
    return success;
}

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
        int tam = 0;
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
                posIMG[a[i][j]] = { Height / n * (j - 1) + 89, Height / n * (i - 1) + 87 };
                //Tìm hàng chứa ô trống
                if (a[i][j] == 0) zero = i;
                b.erase(b.begin() + tmp);
            }
        }
        CheckRand();
        if ((check + zero * (n % 2 + 1)) % 2 == 0) break;
    }
}

//void Gameplay::display(vector<vector<int>> a)
//{
//    //Hàm in ra màn hình
//    cnt = 1;
//    for (int i = 1; i <= n; i++) {
//        for (int j = 1; j <= n; j++) {
//            printf("%3d ", a[i][j]);
//            if (a[i][j] != Goal[i][j]) cnt = 0;
//        }
//        cout << endl;
//    }
//}

void Gameplay::Solve(pair<int, int> p)
{

    int W = a[p.first][p.second];
    // Tính 4 vị trí xung quanh ô đang xét
    int x = a[p.first - 1][p.second], y = a[p.first][p.second - 1], z = a[p.first + 1][p.second], t = a[p.first][p.second + 1];
    // Kiểm tra xem ô nào trống thì đổi chỗ và vị trí của ô trống với ô đang xét
    if (!x) {
        swap(a[p.first - 1][p.second], a[p.first][p.second]);
        swap(posIMG[W], posIMG[0]);
        return;
    }
    if (!y) {
        swap(a[p.first][p.second - 1], a[p.first][p.second]);
        swap(posIMG[W], posIMG[0]);
        return;
    }
    if (!z) {
        swap(a[p.first + 1][p.second], a[p.first][p.second]);
        swap(posIMG[W], posIMG[0]);
        return;
    }
    if (!t) {
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

int Gameplay::checkPos(pair<int, int> p)
{
    // Kiểm tra xem ô nào đang chứa tọa độ p ( dùng để xử lí sự kiện )
    for (int i = 1; i <= n * n - 1; i++)
    {
        if (posIMG[i] == p) return i;
    }
    return 0;
}

//Gameplay::~Game() {};

GameObject** Number;
GameObject** StartGame;
SDL_Event Gameplay::event;
SDL_Texture* PlayerTex;
SDL_Rect srcR, destR;

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
        Number[i] = new GameObject(filename, gRenderer, posIMG[i].first, posIMG[i].second, n);
    }
}

void Gameplay::handleEvents() {
    // Hàm xử lí sự kiện để chơi trò chơi
    bool isQuit = false;
    SDL_PollEvent(&event);
    switch (event.type) 
    {
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
            cout << x << " " << y << endl;
            int H = 589 / n;
            // Tính toán xem vị trí click đang nằm ở ô nào ( đưa về vị trí lúc đầu ta xét để dễ tính )
            int Px = x / H * H, Py = y / H * H;
            cout << "Px = " << Px << " Py = " << Py << endl;
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
        Solve(Pos(x, a));
        update();
        render();

        SDL_Delay(500);
    }
    isRunning = false;
    if (!isQuit) {
        update();
        render();
        while (!isQuit)
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) isQuit = true;
            }
    }
}



void Gameplay::update() {
    // Hàm cập nhật lại các tọa độ của các đối tượng của game ( các mảnh puzzle )
    for (int i = 0; i <= n * n - 1; i++)
    {
        Number[i]->Update(posIMG[i].first, posIMG[i].second, Height / n, Height  / n, 589 / n, 589 / n);
    }
}

void Gameplay::render() {
    // Hàm hiển thị hình ảnh các đối tượng của game ra màn hình ( các mảnh puzzle )
    SDL_RenderClear(gRenderer);
    LTexture Background, GoalImage;
    Background.loadFromFile("IMG//PlayGame.PNG");
    GoalImage.loadFromFile("Data//anhYourName.png");
    GoalImage.Resize(234, 234);
    Background.render(0, 0);
    GoalImage.render(928, 52);
    for (int i = 0; i < TOTAL_GAMEPLAY_BUTTONS; ++i)
        gButton[i].HandleEvent(&event);
    for (int j = 0; j < TOTAL_GAMEPLAY_BUTTONS; ++j)
        gButton[j].render(GamePlayButton[j], gSpriteClips);
    for (int i = 1; i <= n * n - 1; i++)
    {
        Number[i]->Render();
    }
    if (!isRunning) Number[0]->Render();
    SDL_RenderPresent(gRenderer);
}

void Gameplay::Play()
{
    // Hàm để chơi game bằng bàn phím và chuột
    while (running()) {
        handleEvents();
        update();
        render();
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
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    SDL_Quit();
}


void Gameplay::Run(bool isPlay) {
 
    if (LoadMedia())
    {
        SetUpGame(594);
        if (isPlay)
            Play();
        else
            SolveGame();
        clean();
        Clear();
    }
}