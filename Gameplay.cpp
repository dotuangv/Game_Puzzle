#include "Gameplay.h"
#include "LButton.h"
#include "Main_Menu.h"

LTexture ButtonBack, ButtonReload, ButtonAutoRun, ButtonMode, StepTexture, SolveMode;
LButton gButtonBack, gButtonReload, gButtonAutoRun;

SDL_Rect ButtonReloadRect[] = { {0, 0, 142, 142}, {0, 175, 142, 142}, {0,350 , 142, 142} };
SDL_Rect ButtonBackRect[] = { {0, 0, 156, 156}, {0, 180, 156, 156}, {0, 361, 156, 156} };
SDL_Rect ButtonAutoRunRect[] = { {0, 0, 327, 65}, {0, 182, 327, 65}, {0, 356, 327, 65} };
SDL_Rect ButtonModeRect[] = { {0, 0, 186, 58}, {0, 70, 186, 58}, {0, 140, 186, 58}, {0, 210, 186, 58} };
LTimer timer;
LTexture timing;
Mix_Chunk* gSlide = NULL;
Mix_Music* gSoundTrack = NULL;

LTexture Background, GoalImage;

bool Gameplay::LoadMedia() 
{
    std::string index = std::to_string(Order % TOTAL_IMAGE);
    std::string imagePath = "Data//MenuImage//MenuImage" + index + ".png";
    bool success = true;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    if (!ButtonAutoRun.loadFromFile("IMG//AutoRun.png"))
    {
        success = false;
    }
    else if (!ButtonBack.loadFromFile("IMG//Back.png"))
    {
        success = false;
    }
    else if (!ButtonReload.loadFromFile("IMG//Reload.png"))
    {
        success = false;
    }
    else if (!ButtonMode.loadFromFile("IMG//Mode.png"))
    {
        success = false;
    }
    else if (!Background.loadFromFile("IMG//PlayGame.PNG"))
    {
        success = false;
    }
    else if (!GoalImage.loadFromFile(imagePath))
    {
        success = false;
    }
    else if (!(gSoundTrack = Mix_LoadMUS("Music//Rondo Alla.mp3")))
    {
        printf("Failed to load SoundTrack! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    else if (!(gSlide = Mix_LoadWAV("Music//low.wav")))
    {
        printf("Failed to load gSlide sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    else {
        gButtonReload.SetAllValue(985, 357, 142, 142);
        gButtonAutoRun.SetAllValue(896, 523, 327, 65);
        gButtonBack.SetAllValue((SCREEN_WIDTH - 156 / 2), - (156 / 2), 156, 156);
    }
    return success;
}

std::string millisecondsToTimeString(Uint32 milliseconds) {
    Uint32 seconds = milliseconds / 1000; // Chuyển từ mili giây sang giây
    Uint32 minutes = seconds / 60; // Chuyển từ giây sang phút
    seconds %= 60; // Lấy số giây còn lại sau khi tính phút

    // Sử dụng std::to_string để chuyển số thành chuỗi và đảm bảo rằng chúng có hai chữ số
    std::string mm = (minutes < 10 ? "0" : "") + std::to_string(minutes);
    std::string ss = (seconds < 10 ? "0" : "") + std::to_string(seconds);

    return mm + " : " + ss;
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
                posIMG[a[i][j]] = { Height / n * (j - 1) + 89, Height / n * (i - 1) + 85 };
                //Tìm hàng chứa ô trống
                if (a[i][j] == 0) zero = i;
                b.erase(b.begin() + tmp);
                //cout << posIMG[a[i][j]].first << " " << posIMG[a[i][j]].second << endl;
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
        swap(a[p.first - 1][p.second], a[p.first][p.second]);
        swap(posIMG[W], posIMG[0]);
        if (!checksolve)
        {
            ++step;
            if (!timer.isStarted())
                timer.start();
        }
        return;
    }
    if (!y) {
        swap(a[p.first][p.second - 1], a[p.first][p.second]);
        swap(posIMG[W], posIMG[0]);
        if (!checksolve)
        {
            ++step;
            if (!timer.isStarted())
                timer.start();
        }
        return;
    }
    if (!z) {
        swap(a[p.first + 1][p.second], a[p.first][p.second]);
        swap(posIMG[W], posIMG[0]);
        if (!checksolve)
        {
            ++step;
            if (!timer.isStarted())
                timer.start();
        }
        return;
    }
    if (!t) {
        swap(a[p.first][p.second + 1], a[p.first][p.second]);
        swap(posIMG[W], posIMG[0]);
        if (!checksolve)
        {
            ++step;
            if (!timer.isStarted())
                timer.start();
        }
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
    cout << "BYE\n";
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
        // Thêm vào hàng đợi ưu tiên OPEN f ( = g + h), vị trí của mảng thêm vào (res), chi phí đi từ lúc đầu đến trạng thái hiện tại(g)
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

void Gameplay::AuToRun(bool CheckQuit)
{
    cout << "HELLO\n";
    display(a);
    //Khởi tạo trạng thái ban đầu.
    KhoiTao();
    // Curent là trạng thái đang xét hiện t ại
    vector<vector<int>> Curent;
    int k = 1, l;
    bool isQuit = false;
    SDL_Event e;
    while (!OPEN.empty()) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT)
            {
                isQuit = true;
                CheckQuit = true;
                return;
            }
        }
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
        k++;
        //cout << k << endl;
        //display(Curent);
    }
    // Đẩy trạng thái đích vào KQ
    KQ.push_back(l);
    // Tìm các trạng thái đã suy ra được trạng thái đích
    while (FATHER[l] != 0) {
        KQ.push_back(FATHER[l]);
        l = FATHER[l];
    }
    cout << "KQ: " << KQ.size() << endl;
    index = KQ.size();
}

void Gameplay::Clear()
{
    // Đặt isRunning thành false để kết thúc vòng lặp chính
    isRunning = false;
    FATHER.clear();
    FRINGE.clear();
    CLOSE.clear();
    KQ.clear();
    while (!OPEN.empty()) {
        OPEN.pop();
    }
    res = 0;
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
        if (p.first >= posIMG[i].first && p.first <= posIMG[i].first + Height / n
            && p.second >= posIMG[i].second && p.second <= posIMG[i].second + Height / n) return i;
    }
    return 0;
}

//Gameplay::~Game() {};

GameObject** Number;
GameObject** StartGame;
SDL_Event Gameplay::event;
SDL_Texture* PlayerTex;
SDL_Rect srcR, destR;

std::vector<SDL_Texture*> CutTextureIntoPieces(SDL_Texture* texture, int n) {
    std::vector<SDL_Texture*> textures;

    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

    int pieceWidth = textureWidth / n;
    int pieceHeight = textureHeight / n;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            SDL_Rect srcRect = { j * pieceWidth, i * pieceHeight, pieceWidth, pieceHeight };
            SDL_Texture* pieceTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, pieceWidth, pieceHeight);

            // Thiết lập mục tiêu vẽ cho texture
            SDL_SetRenderTarget(gRenderer, pieceTexture);

            // Xóa texture
            SDL_RenderClear(gRenderer);

            // Sao chép một phần của texture gốc vào texture đích
            SDL_RenderCopy(gRenderer, texture, &srcRect, NULL);

            // Đặt lại mục tiêu vẽ về texture gốc
            SDL_SetRenderTarget(gRenderer, NULL);

            textures.push_back(pieceTexture);
        }
    }

    return textures;
}
void Gameplay::SetNguoc(int height)
{
    Height = height;
    int tmp = n * n;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            a[i][j] = tmp--;
            if (i == 1 && j == 1) a[i][j] = 0;
            //cin >> a[i][j];
            //Tính tọa độ ảnh
            posIMG[a[i][j]] = { Height / n * (j - 1) + 89, Height / n * (i - 1) + 85 };
            //Tìm hàng chứa ô trống
            if (a[i][j] == 0) zero = i;
        }
    }
    CheckRand();
    if ((check + zero * (n % 2 + 1)) % 2 == 0) cout << "Duoc\n";
    else cout << "Khong the giai\n";
}
void Gameplay::SetUpGame(int height)
{
    setA();
    if (Mode == 1)
        Random(height);
    else if (Mode == 2)
        SetNguoc(height);
    setGoal();
    //Load ảnh lớn
    std::string index = std::to_string(Order % TOTAL_IMAGE);
    std::string imagePath = "Data//MenuImage//MenuImage" + index + ".png";

    SDL_Surface* imageSurface = IMG_Load(imagePath.c_str());

    SDL_Texture* largeTexture = SDL_CreateTextureFromSurface(gRenderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    //Tính kích thước của mỗi phần nhỏ
    int smallWidth = 589 / n;
    vector<SDL_Texture*> CutPicture;
    CutPicture = CutTextureIntoPieces(largeTexture, n);
    Number = new GameObject * [n * n + 1];

    // Tạo và lưu các phần nhỏ vào mảng Number
    Number[0] = new GameObject(CutPicture[n * n - 1], gRenderer, posIMG[0].first, posIMG[0].second);
    for (int i = 1; i < n * n; i++)
    {
        Number[i] = new GameObject(CutPicture[i - 1], gRenderer, posIMG[i].first, posIMG[i].second);
    }
    for (int i = 0; i < CutPicture.size(); ++i)
    {
        SDL_DestroyTexture(largeTexture);
        CutPicture[i] = NULL;
    }
    CutPicture.clear();
}

void Gameplay::HandleAuto()
{
    bool isQuit = false;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        isQuit = true;
        break;

    case SDL_MOUSEBUTTONDOWN:
    {
        // Xử lí sự kiện bằng chuột, lấy tọa độ của vị trí chuột nhấn vào (x, y)
        int x = event.motion.x, y = event.motion.y;
        if (x >= 870 && x <= 1220 && y >= 540 && y <= 590)
        {
            if (!checksolve)
            {
                //gButtonAutoRun.render(ButtonAutoRun, ButtonAutoRunRect, BUTTON_SPRITE_MOUSE_DOWN);
                checksolve = 1;
            }
            else
            {
                checksolve = 0;
            }
        }
        break;
    }
    default:
        break;
    }
}

void Gameplay::handleEvents() {
    //Hàm xử lí sự kiện để chơi trò chơi
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
        if (!checksolve)
        {
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
                    if (!checksolve)
                    {
                        ++step;
                        if (!timer.isStarted())
                            timer.start();
                        Mix_PlayChannel(-1, gSlide, 0);
                    }
                    checkmove = true;
                }
                break;
            }
            case SDLK_DOWN:
            {
                int x = a[pa.first - 1][pa.second];
                if (x != n * n) {
                    swap(a[pa.first - 1][pa.second], a[pa.first][pa.second]);
                    swap(posIMG[x], posIMG[0]);
                    if (!checksolve)
                    {
                        ++step;
                        if (!timer.isStarted())
                            timer.start();
                        Mix_PlayChannel(-1, gSlide, 0);
                    }
                    checkmove = true;
                }
                break;
            }
            case SDLK_LEFT:
            {
                int x = a[pa.first][pa.second + 1];
                if (x != n * n) {
                    swap(a[pa.first][pa.second + 1], a[pa.first][pa.second]);
                    swap(posIMG[x], posIMG[0]);
                    if (!checksolve)
                    {
                        ++step;
                        if (!timer.isStarted())
                            timer.start();
                        Mix_PlayChannel(-1, gSlide, 0);
                    }
                    checkmove = true;
                }
                break;
            }
            case SDLK_RIGHT:
            {
                int x = a[pa.first][pa.second - 1];
                if (x != n * n) {
                    swap(a[pa.first][pa.second - 1], a[pa.first][pa.second]);
                    swap(posIMG[x], posIMG[0]);
                    if (!checksolve)
                    {
                        ++step;
                        if (!timer.isStarted())
                            timer.start();
                        Mix_PlayChannel(-1, gSlide, 0);
                    }
                    checkmove = true;
                }
                break;
            }
            default:
                break;
            }
        }
    }
    case SDL_MOUSEBUTTONDOWN:
    {
            // Xử lí sự kiện bằng chuột, lấy tọa độ của vị trí chuột nhấn vào (x, y)
        int x = event.motion.x, y = event.motion.y;
        if (x >= 870 && x <= 1223 && y >= 523 && y <= 590)
        {
            if (!checksolve)
            {
                checksolve = 1;
                if (timer.isStarted() && !timer.isPaused())
                    timer.pause();
            }
            else
            {
                checksolve = 0;
            }
            break;
        }
        //Button Reload
        else if(x >= 987 && x <= 1809 && y >= 371 && y <= 447 && checksolve == 0)
        {
            if (!Mode) Random(Height);
            else SetNguoc(Height);
            KQ.clear();
            CLOSE.clear();
            while (!OPEN.empty()) {
                OPEN.pop();
            }
            res = 1;
            checkmove = true;
            step = 0;
            if (timer.isStarted())
                timer.stop();
            break;
        }
        //Button Back
        else if (x >= 1207 && x <= 1278 && y >= 0 && y <= 71)
        {
            checksolve = 0;
            isUSE = true;
            isQuit = true;
            isRunning = false;
        }
        if (!checksolve)
        {
            cout << x << " " << y << endl;
             // Tính toán xem vị trí click đang nằm ở ô nào ( đưa về vị trí lúc đầu ta xét để dễ tính )
             int P = checkPos({ x, y });
             if (P)
             {
                 Solve(Pos(P, a));
                 checkmove = true;
                 Mix_PlayChannel(-1, gSlide, 0);
             }
             break;
            }
        }
            break;
    default:
        break;
    }
    // Nếu trạng thái hiện tại là trạng thái đích thì kết thúc vòng lặp, đánh giấu trò chơi đã dừng
    isRunning = !CheckGoal(a) && !isQuit;
}

void Gameplay::SolveGame()
{
    // Hàm giải game bằng AI
    if (checkmove)
    {
        KQ.clear();
        CLOSE.clear();
        while (!OPEN.empty()) {
            OPEN.pop();
        }
        res = 1;
        bool CheckQuit = false;
        AuToRun(CheckQuit);
        if (CheckQuit) return;

    }
    checkmove = false;
    update();
    render();
    SDL_Delay(250);
    bool isQuit = false;
    SDL_Event e;
    while(index && !isQuit)
    {
        HandleAuto();
        if (!checksolve)
        {
            return;
        }
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) isQuit = true;
        }
        // Tìm vị trị trong mảng KQ và cập nhật lại các ảnh của trạng thái đang xét đến
        int x = FRINGE[KQ[index - 1]].second;
        cout << index << " " << x << endl;
        //display(a);
        Solve(Pos(x, a));
        update();
        render();
        SDL_Delay(250);
        index--;
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
    GoalImage.Resize(234, 234);
    Background.render(0, 0);
    GoalImage.render(928, 52);

    gButtonReload.HandleEvent(&event);
    gButtonReload.render(ButtonReload, ButtonReloadRect);

    gButtonBack.HandleEvent(&event);
    gButtonBack.render(ButtonBack, ButtonBackRect);

    if (checksolve == 1)
        gButtonAutoRun.render(ButtonAutoRun, ButtonAutoRunRect, TRUE);
    else 
        gButtonAutoRun.render(ButtonAutoRun, ButtonAutoRunRect);

    StepTexture.loadFromRenderedText(to_string(step), { 0xFF, 0xFF, 0xFF });
    StepTexture.render(660, 28);
    
    string time = millisecondsToTimeString(timer.getTicks());
    timing.loadFromRenderedText(time, { 0xFF, 0xFF, 0xFF });
    timing.render(92, 28);
    ButtonMode.render(281, 9, &ButtonModeRect[n - 3]);

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
    if (Mix_PlayingMusic() == 0)
    {
        //Play the music
        Mix_PlayMusic(gSoundTrack, -1);
    }
    while (running()) {
        handleEvents();
        if (checksolve)
        {
            //Nếu thời gian đang chạy, dừng thời gian
            if (!timer.isPaused())
                timer.pause();
            SolveGame();
        }
        update();
        render();
        if (!running())
        {
            if (CheckGoal(a))
            {
                checksolve = 0;
                SDL_Delay(500);
                update();
                render();
                SDL_Event e;
                bool IsQuit = false;
                while (!IsQuit)
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT)
                    {
                        IsQuit = true;
                        isRunning = false;
                        outGame = true;
                    }
                }
            }
        }
    }
}

void Gameplay::clean() {
    // Hàm giải phóng các đối tượng đã tạo
    delete[] Number;
    Number = NULL;
    delete[] StartGame;
    StartGame = NULL;
    Mix_FreeMusic(gSoundTrack);
    gSoundTrack = NULL;
    Mix_FreeChunk(gSlide);
    gSlide = NULL;
    ButtonBack.free();
    ButtonReload.free();
    ButtonAutoRun.free();
    ButtonMode.free();
    Background.free();
    GoalImage.free();
    SDL_DestroyTexture(PlayerTex);
    PlayerTex = NULL;
}


void Gameplay::Run() {
    if (LoadMedia())
    {
        SetUpGame(594);
        Play();
        Clear();
        clean();
    }
}