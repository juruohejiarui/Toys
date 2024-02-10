#include <bits/stdc++.h>
#include<sys/ioctl.h>

inline int rand(int l, int r) { return rand() % (r - l + 1) + l; }

struct Element {
    int x, y, st;
    int col;
    Element(int x, int y, int col, int st = 0) : x(x), y(y), col(col), st(st) {}
};

std::queue<Element> ele;
std::vector<Element> elist;
struct winsize size;

unsigned char mp[1000006], mp1[1000005];

inline int getid(int x, int y) { return x * size.ws_col + y; }
char ch[] = {'/','|','\\','-','.','-','\\','|','/'};

void updateState() {
    if (ele.size() <= 10) ele.push(Element(0, rand(0, size.ws_col), rand(0, 7), 0));
    while (rand(1, 2) == 1 && ele.size() > 0 && ele.front().x > (size.ws_row >> 1))
        elist.push_back(ele.front()), ele.pop();
    int sz = ele.size();
    
    
    memset(mp, 0, sizeof(unsigned char) * size.ws_col * size.ws_row);
    memset(mp1, ' ', sizeof(unsigned char) * size.ws_col * size.ws_row);
    for (int i = 0; i < elist.size(); i++) {
        if (elist[i].st > 4) {
            elist.erase(elist.begin() + i), i--;
            continue;
        }
        elist[i].st++;
        int st = 0;
        for (int x = -1; x <= 1; x++)
            for (int y = -1; y <= 1; y++) {
                int px = x * elist[i].st + elist[i].x, py = y * elist[i].st + elist[i].y;
                if (px >= 0 && py >= 0 && px < size.ws_row && py < size.ws_col)
                    mp[getid(px, py)] = elist[i].col,
                    mp1[getid(px, py)] = ch[st];
                st++;
            }
    }
    for (int i = 0; i < sz; i++) {
        Element e = ele.front();
        ele.pop(), e.x++, ele.push(e);
        mp[getid(e.x, e.y)] = e.col, mp1[getid(e.x, e.y)] = '|';
    }
}

void drawState() {
    system("clear");
    // printf("%d %d %d %d\n", size.ws_col, size.ws_row, size.ws_xpixel, size.ws_ypixel);
    auto setColor = [&](int col) -> int {
        switch (col) {
            case 0:printf("\033[30m");break;
            case 1:printf("\033[31m");break;
            case 2:printf("\033[32m");break;
            case 3:printf("\033[33m");break;
            case 4:printf("\033[34m");break;
            case 5:printf("\033[35m");break;
            case 6:printf("\033[36m");break;
            case 7:printf("\033[37m");break;
        }
        return 1;
    };
    for (int i = 0; i < size.ws_row; i++) {
        for (int j = 0; j < size.ws_col; j++)
            mp1[getid(size.ws_row - i - 1, j)] == ' ' || setColor(mp[getid(size.ws_row - i - 1, j)]), 
            putchar(mp1[getid(size.ws_row - i - 1, j)]);
        putchar('\n');
    }
}
int main() {
    srand(time(NULL));
    ioctl(STDIN_FILENO,TIOCGWINSZ, &size);
    while (1) {
        updateState();
        drawState();
        usleep(100000);
    }
    return 0;
}