#pragma once
#include <vector>
#include <iostream>
#include <queue>
using namespace std;

class Gameplay {
private:
    int n;
    vector<vector<int>> goal;
    vector<vector<int>> a;
    vector<int> b;
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> OPEN;
    vector<pair<vector<vector<int>>, int>> FRINGE;
    int cnt, check, zero, t;
public:
    // Constructor
    Gameplay(int size) : n(size), goal(vector<vector<int>>(size + 2, vector<int>(size + 2))), a(vector<vector<int>>(size + 2, vector<int>(size + 2))), b(vector<int> (size + 2)),
        FRINGE(1000000, pair<vector<vector<int>>, int>(vector<vector<int>>(size + 2, vector<int>(size + 2)), 0)) {
        cnt = 0;
        check = 0;
        t = 0;
    }
    void checkrand();
    void random();
    void display();
    void solve();
    pair<int, int> pos(int x, vector<vector<int>> v);
    void Play();
    void CheckTrung(vector<vector<int>> a);
    bool CheckGoal(vector<vector<int>> a);


};
