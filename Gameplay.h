#pragma once
#include "CommonFunc.h"

using namespace std;

class Gameplay {
private:
    int n;
    vector<vector<int>> Goal;
    vector<vector<int>> a;
    vector<int> b;
    vector<pair<int, int>> posIMG;
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> OPEN;
    vector<pair<vector<vector<int>>, int>> FRINGE;
    vector<vector<vector<int>>> CLOSE;
    vector<int> KQ;
    vector<int> FATHER;
    vector<pair<int, int >> p;

    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    int cnt, check, zero, tam, res;
public:
    // Constructor
    Gameplay(int size) : n(size), Goal(vector<vector<int>>(size + 2, vector<int>(size + 2))), a(vector<vector<int>>(size + 2, vector<int>(size + 2))), b(vector<int>(size + 2)),
        posIMG(vector<pair<int, int>>(n*n+1)), FRINGE(100000, pair<vector<vector<int>>, int>(vector<vector<int>>(size + 2, vector<int>(size + 2)), 0)), FATHER(vector<int>(100000)) {
        cnt = 0;
        check = 0;
        tam = 0;
        res = 1;
    }
    void CheckRand();
    void Random();
    void display(vector<vector<int>> a);
    void Solve(pair<int, int> p);
    pair<int, int> Pos(int x, vector<vector<int>> v);
    void Play();
    bool CheckTrung(vector<vector<int>> a);
    bool CheckGoal(vector<vector<int>> a);
    int Heuristic(vector<vector<int>> a);
    void KhoiTao();
    void AuToRun();
    void Clear();
    void setA();
    void setGoal();
    void setN(int n);
    vector<vector<int>> getA();
    vector<vector<int>> getGoal();
    pair<int, int> getPosImg(int i);
    int getN();
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() {
        return isRunning;
    };
    int getCnt() {
        return cnt;
    }
    static SDL_Event event;
};
