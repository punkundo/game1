#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <Windows.h>
using namespace sf;

int N = 40, M = 30;
int size = 16;
int w = size * N;
int h = size * M;
int size_wall;
int dir_s1, dir_s2, len_s1, len_s2, end_game, app_running;

struct Snake {
    int x, y;
}  s1[400], s2[400];

struct Wall {
    int x, y;
} wa[400];

struct Food
{
    int x, y;
} f;

bool food_OK() {
    for (int i = 0; i < len_s1; i++) {
        if (s1[i].x == f.x && s1[i].y == f.y) {
            return 0;
        }
    }
    for (int i = 0; i < len_s2; i++) {
        if (s2[i].x == f.x && s2[i].y == f.y) {
            return 0;
        }
    }
    for (int i = 0; i < size_wall; i++) {
        if (wa[i].x == f.x && wa[i].y == f.y) {
            return 0;
        }
    }
    return 1;
}

void Tick_s1() {
    for (int i = len_s1; i > 0; --i) {
        s1[i].x = s1[i - 1].x; s1[i].y = s1[i - 1].y;
    }

    if (dir_s1 == 0) s1[0].y += 1;
    if (dir_s1 == 1) s1[0].x -= 1;
    if (dir_s1 == 2) s1[0].x += 1;
    if (dir_s1 == 3) s1[0].y -= 1;

    if (s1[0].x >= N) s1[0].x = 0;  if (s1[0].x < 0) s1[0].x = N - 1;
    if (s1[0].y >= M) s1[0].y = 0;  if (s1[0].y < 0) s1[0].y = M - 1;

    if ((s1[0].x == f.x) && (s1[0].y == f.y)) {
        len_s1++;
        while (!food_OK()) {
            f.x = rand() % N;
            f.y = rand() % M;
        }
    }

    //Break game//
    for (int i = 1; i < len_s1; i++) {//tu dam vao minh
        if (s1[0].x == s1[i].x && s1[0].y == s1[i].y) {
            end_game = 1;
            return;
        }
    }
    for (int i = 0; i < len_s2; i++) {//dam vao doi thu
        if (s1[0].x == s2[i].x && s1[0].y == s2[i].y) {
            if (!i) {//2 con ran dam dau vao nhau//
                end_game = -1;
                return;
            }
            end_game = 1;
            return;
        }
    }
    for (int i = 0; i < size_wall; i++) {//dam vao chuong ngai vat
        if (s1[0].x == wa[i].x && s1[0].y == wa[i].y) {
            end_game = 1;
            return;
        }
    }
    end_game = 0;
}

void Tick_s2()
{
    for (int i = len_s2; i > 0; --i) {
        s2[i].x = s2[i - 1].x; s2[i].y = s2[i - 1].y;
    }

    if (dir_s2 == 0) s2[0].y += 1;
    if (dir_s2 == 1) s2[0].x -= 1;
    if (dir_s2 == 2) s2[0].x += 1;
    if (dir_s2 == 3) s2[0].y -= 1;

    if (s2[0].x >= N) s2[0].x = 0;  if (s2[0].x < 0) s2[0].x = N - 1;
    if (s2[0].y >= M) s2[0].y = 0;  if (s2[0].y < 0) s2[0].y = M - 1;

    if ((s2[0].x == f.x) && (s2[0].y == f.y)) {
        len_s2++;
        do {
            f.x = rand() % N;
            f.y = rand() % M;
        } while (!food_OK());
    }

    //Break game//
    for (int i = 1; i < len_s2; i++) {//tự đâm vào bản thân mình
        if (s2[0].x == s2[i].x && s2[0].y == s2[i].y) {
            end_game = 1;
            return;
        }
    }
    for (int i = 0; i < len_s1; i++) {//dam vao doi thu
        if (s2[0].x == s1[i].x && s2[0].y == s1[i].y) {
            end_game = 1;
            return;
        }
    }
    for (int i = 0; i < size_wall; i++) {//dam vao chuong ngai vat
        if (s2[0].x == wa[i].x && s2[0].y == wa[i].y) {
            end_game = 1;
            return;
        }
    }
    end_game = 0;

}

int main(){

    srand(time(0));

    RenderWindow screen(VideoMode(w + 100, h), "Snake Game!");

    Texture background, food, snake1, snake2, wall,bg_point;
    background.loadFromFile("images/white2.png");
    snake1.loadFromFile("images/green.png");
    snake2.loadFromFile("images/red.png");
    food.loadFromFile("images/blue.png");
    wall.loadFromFile("images/black.png");
    bg_point.loadFromFile("images/bg_point.png");

    Sprite sprite_bg(background);
    Sprite sprite_s1(snake1);
    Sprite sprite_s2(snake2);
    Sprite sprite_food(food);
    Sprite sprite_wall(wall);
    Sprite point(bg_point);

    Clock clock;
    float timer = 0, delay = 0.12;

    app_running = 1;
    end_game = 0;
    size_wall = 20;

    while (app_running) {
        dir_s1 = 0; dir_s2 = 3;
        s1[0].x = N - 1; s1[0].y = 0;
        s2[0].x = 0; s2[0].y = 0;
        len_s1 = 1; len_s2 = 1;
        f.x = 10;
        f.y = 10;

        //set wall//
        for (int i = 0; i < size_wall; i++) {
            wa[i].x = rand() % (N - 2) + 1;
            wa[i].y = rand() % (M - 2) + 1;
        }

        while (screen.isOpen()) {
            {
            Event e;
            while (screen.pollEvent(e)) {
                if (e.type == Event::Closed) {
                    if (MessageBox(NULL, TEXT("Are you sure??\n\n:(("), TEXT("Thảo Cute <3"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        app_running = 0;
                        screen.close();
                    }
                }
                while (e.type == sf::Event::LostFocus) {
                    Event e2;
                    int brk = 0;
                    while (screen.pollEvent(e2)) {
                        if (e2.type == Event::GainedFocus) {
                            brk = 1;
                        }
                    }
                    if (brk) break;
                }
            }
            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;
            
                if (Keyboard::isKeyPressed(Keyboard::Left) && dir_s1 != 2) dir_s1 = 1;
                if (Keyboard::isKeyPressed(Keyboard::Right) && dir_s1 != 1) dir_s1 = 2;
                if (Keyboard::isKeyPressed(Keyboard::Up) && dir_s1 != 0) dir_s1 = 3;
                if (Keyboard::isKeyPressed(Keyboard::Down) && dir_s1 != 3) dir_s1 = 0;

                if (Keyboard::isKeyPressed(Keyboard::A) && dir_s2 != 2) dir_s2 = 1;
                if (Keyboard::isKeyPressed(Keyboard::D) && dir_s2 != 1) dir_s2 = 2;
                if (Keyboard::isKeyPressed(Keyboard::W) && dir_s2 != 0) dir_s2 = 3;
                if (Keyboard::isKeyPressed(Keyboard::S) && dir_s2 != 3) dir_s2 = 0;
            }

            if (timer > delay) {
                timer = 0;
                Tick_s1();
                if (end_game) {
                    if (end_game == -1) {//2 con ran dam dau vao nhau//
                        if (len_s1 > len_s2) {
                            if (MessageBox(NULL, TEXT("Green is Winner!\n\nPlay again??"), TEXT("Thảo Cute <3"), MB_YESNO) == IDNO) {
                                app_running = 0;
                            }
                            break;
                        }
                        else if (len_s2 > len_s1) {
                            if (MessageBox(NULL, TEXT("Red is Winner!\n\nPlay again??"), TEXT("Thảo Cute <3"), MB_YESNO) == IDNO) {
                                app_running = 0;
                            }
                            break;
                        }
                        else {
                            if (MessageBox(NULL, TEXT("No one wins!\n\nPlay again??"), TEXT("Thảo Cute <3"), MB_YESNO) == IDNO) {
                                app_running = 0;
                            }
                            break;
                        }
                    }
                    if (MessageBox(NULL, TEXT("Red is Winner!\n\nPlay again??"), TEXT("Thảo Cute <3"), MB_YESNO) == IDNO) {
                        app_running = 0;
                    }
                    break;
                }
                Tick_s2();
                if (end_game) {
                    if (MessageBox(NULL, TEXT("Green is Winer!\n\nPlay again??"), TEXT("Thảo Cute <3"), MB_YESNO) == IDNO) {
                        app_running = 0;
                    }
                    break;
                }
            }

            ////// draw  ///////
            screen.clear();

            //POINT//
            point.setPosition(w, 0); screen.draw(point);
            
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < M; j++) {
                    sprite_bg.setPosition(i * size, j * size);  screen.draw(sprite_bg);
                }
            }

            for (int i = 0; i < size_wall; i++) {
                sprite_wall.setPosition(wa[i].x * size, wa[i].y * size); screen.draw(sprite_wall);
            }

            for (int i = 0; i < len_s1; i++) {
                sprite_s1.setPosition(s1[i].x * size, s1[i].y * size);  screen.draw(sprite_s1);
            }

            for (int i = 0; i < len_s2; i++) {
                sprite_s2.setPosition(s2[i].x * size, s2[i].y * size);  screen.draw(sprite_s2);
            }

            sprite_food.setPosition(f.x * size, f.y * size);  screen.draw(sprite_food);

            screen.display();

        }

    }
    return 0;
}