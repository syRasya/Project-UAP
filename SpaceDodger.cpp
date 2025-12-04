#include <curses.h>
#include <windows.h>

int maxX = 80, maxY = 24;

void init_ncurses() {
    initscr();
    cbreak();
    noecho();
    getmaxyx(stdscr, maxY, maxX);
}

void shutdown_ncurses() {
    nodelay(stdscr, FALSE);
    getch();
    endwin();
}

void tampilan_rank() {
    erase();
    mvprintw(2, maxX/2 - 8, "== RANKINGS ==");
    mvprintw(maxY-3, 2, "Press any key to continue...");
    nodelay(stdscr, FALSE);
    getch();
    nodelay(stdscr, TRUE);
}

bool tampilan_menu() {
    erase();
    mvprintw(maxY/2 -3, maxX/2 - 10, "SPACE SHOOTER");
    mvprintw(maxY/2 -1, maxX/2 - 12, "1) Start Game");
    mvprintw(maxY/2 +0, maxX/2 - 12, "2) Show Rankings");
    mvprintw(maxY/2 +1, maxX/2 - 12, "3) Quit");
    mvprintw(maxY/2 +3, maxX/2 - 18, "Use number keys (1-3) to select");
    refresh();

    nodelay(stdscr, FALSE);
    int ch;
    while (true) {
        ch = getch();
        if (ch == '1') { nodelay(stdscr, TRUE); return true; }
        if (ch == '2') { tampilan_rank(); erase(); mvprintw(maxY/2 -3, maxX/2 - 10, "SPACE SHOOTER"); mvprintw(maxY/2 -1, maxX/2 - 12, "1) Start Game"); mvprintw(maxY/2 +0, maxX/2 - 12, "2) Show Rankings"); mvprintw(maxY/2 +1, maxX/2 - 12, "3) Quit"); mvprintw(maxY/2 +3, maxX/2 - 18, "Use number keys (1-3) to select"); refresh(); }
        if (ch == '3') { nodelay(stdscr, TRUE); return false; }
    }
}

int main() {
    init_ncurses();

    while (true) {
        getmaxyx(stdscr, maxY, maxX);
        bool start = tampilan_menu();
        if (!start) break;
    }

    shutdown_ncurses();
    return 0;
}
