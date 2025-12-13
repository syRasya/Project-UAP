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

// Bintang

void spawn_bintang(int count) {
    stars.clear();
    for (int i = 0; i < count; i++)
        stars.push_back({ rand() % maxX, rand() % maxY });
}

void draw_bintang() {
    for (auto& s : stars)
        mvaddch(s.y, s.x, '.');
}

void respawn_stars() {
    for (auto& s : stars) {
        s.y++;
        if (s.y >= maxY) {
            s.y = 0;
            s.x = rand() % maxX;
        }
    }
}

// Asteroid

void spawn_asteroid() {
    if (maxX < 3) return;
    int x = rand() % (maxX - 2) + 1;
    int speed = 1 + rand() % 3;
    asteroids.emplace_back(x, 0, speed);
}

void update_asteroids() {
    for (auto& a : asteroids) {
        a.frameCount++;
        if (a.frameCount >= a.speed) {
            a.y++;
            a.frameCount = 0;
        }
    }
    asteroids.erase(remove_if(asteroids.begin(), asteroids.end(),
        [](auto& a) { return a.y >= maxY; }), asteroids.end());
}

// Peluru

void update_peluru() {
    for (auto& b : peluru) {
        if (!b.active) continue;
        b.y--;
        if (b.y < 0) b.active = false;
    }
    peluru.erase(remove_if(peluru.begin(), peluru.end(),
        [](auto& b) { return !b.active; }), peluru.end());
}

// Gambar Pemain, Peluru, Asteroid, UI

void draw_player(Pemain& p) {
    if (p.immune && Clock::now() >= p.immune_until)
        p.immune = false;

    bool blink = (Clock::now().time_since_epoch().count() / 100000000) % 2 == 0;

    if (!p.immune || blink) {
        mvaddch(p.y, p.x, '^');
        mvaddch(p.y, p.x - 1, '/');
        mvaddch(p.y, p.x + 1, '\\');
    }
}

void draw_peluru() {
    for (auto& b : peluru)
        if (b.active)
            mvaddch(b.y, b.x, '|');
}

void draw_asteroids() {
    for (auto& a : asteroids)
        mvaddch(a.y, a.x, 'O');
}

void draw_ui(Pemain& p) {
    mvprintw(0, 2, "Score: %d", score);
    mvprintw(0, maxX / 2 - 6, "High: %d", highScore);
    mvprintw(0, maxX - 12, "Lives: %d", p.lives);
}

