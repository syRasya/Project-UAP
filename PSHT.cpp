#include <curses.h>
#include <windows.h>
#include <vector>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

using namespace std;
using Clock = chrono::steady_clock;
#define SLEEP_MS(x) Sleep(x)

// Struktur Entitas
struct Vec2 { int x, y; };

struct Peluru {
    int x, y;
    bool active;
    Peluru(int x_, int y_) : x(x_), y(y_), active(true) {}
};

struct Asteroid {
    int x, y, speed, frameCount;
    Asteroid(int x_, int y_, int s) : x(x_), y(y_), speed(s), frameCount(0) {}
};

struct Pemain {
    int x, y, lives;
    bool immune;
    Clock::time_point immune_until;
    Pemain(int x_, int y_) : x(x_), y(y_), lives(3), immune(false) {}
};

int maxX = 80, maxY = 24;
vector<Peluru> peluru;
vector<Asteroid> asteroids;
vector<Vec2> stars;

int score = 0;
int highScore = 0;
bool running = true;

// Music

void play_music() {
    PlaySound(TEXT("music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void stop_music() {
    PlaySound(NULL, 0, 0);
}

// Score

int load_highscore() {
    ifstream ifs("scores.txt");
    int hs = 0;
    if (ifs >> hs) return hs;
    return 0;
}

void save_highscore(int sc) {
    if (sc > highScore) {
        highScore = sc;
        ofstream ofs("scores.txt", ios::trunc);
        ofs << highScore;
    }
}

// Ncurses

void spawn_bintang(int count);

void init_ncurses() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(0);
    getmaxyx(stdscr, maxY, maxX);
    spawn_bintang(max(10, (maxX * maxY) / 80));
}

void shutdown_ncurses() {
    endwin();
}

