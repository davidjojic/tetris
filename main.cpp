#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>
using namespace std;
set<pair<int, int>>s;
vector<pair<int, int>> rotateBlock(vector<pair<int, int>>& fig) {
    vector<pair<int, int>> rotated;

    int pivotX = fig[0].first;
    int pivotY = fig[0].second;

    for (auto& p : fig) {
        int newX = pivotX - (p.second - pivotY);
        int newY = pivotY + (p.first - pivotX);
        if (newX < 0 || newX>9)
            return fig;
        rotated.push_back({ newX, newY });
    }

    return rotated;
}
vector<pair<int, int>> opada(vector<pair<int, int>>& fig,bool& pauz)
{
    vector<pair<int, int>> rotated;
    for (auto& p : fig) {
        if (p.second + 1 > 19 || s.find({p.first,p.second+1}) != s.end())
        {
            pauz = true;
            return fig;
        }
        rotated.push_back({p.first, p.second+1});
    }
    return rotated;
}

vector<pair<int, int>>pomera(vector<pair<int, int>>& fig, int index)
{
    vector<pair<int, int>> rotated;
    for (auto& p : fig) {
        if (p.first + index < 0 || p.first + index>9 || s.find({ p.first+index,p.second + 1 }) != s.end())
            return fig;
        rotated.push_back({ p.first+index, p.second });
    }
    return rotated;
}
pair<int, int>opd(pair<int, int> p) {
    pair<int, int>temp;
    if (p.second + 1 > 19)
        return p;
    temp = { p.first,p.second + 1 };
    return temp;
}
int main()
{
    bool gameOver = false, pauz = false;
    int width = 10, height = 20, cellsize = 30,count = 0,score = 0;
    sf::Font font;
    font.loadFromFile("Roboto-Italic-VariableFont_wdth,wght.ttf");
    sf::RenderWindow window(sf::VideoMode(14 * cellsize, 25 * cellsize), "Tetris", sf::Style::Default);
    sf::RectangleShape cell(sf::Vector2f(cellsize - 1, cellsize - 1)); cell.setOutlineThickness(1); cell.setOutlineColor(sf::Color::White); cell.setFillColor(sf::Color::Black);
    srand(static_cast<unsigned>(time(0)));
    sf::Sprite sprite;
    sf::Texture texture;
    texture.loadFromFile("game over.png");
    sf::Text text;
    text.setFont(font);
    sf::Event ev;
    char move = ' ';
    int matrix[10][20] = { 0 };
    int index = 0;
    map<pair<int,int>, int>boj;
    vector<vector<pair<int, int>>> figure = {
     {{4, 0}, {4, 1}, {4, 2}, {4, 3}}, // I piece (vertical)
    {{4, 0}, {5, 0}, {4, 1}, {5, 1}}, // O piece
    {{4, 0}, {5, 0}, {5, 1}, {5, 2}}, // L piece
    {{5, 0}, {4, 1}, {5, 1}, {6, 1}}, // T piece
    {{4, 0}, {5, 0}, {5, 1}, {5, 2}}, // J piece
    {{4, 0}, {5, 0}, {5, 1}, {6, 1}}, // Z piece
    {{5, 0}, {4, 1}, {5, 1}, {4, 2}}  // S piece
    };
    vector<pair<int, int>> obj;
    int koji = rand() % 7, boja = rand() % 3 + 1;
    obj = figure[koji];


    while (window.isOpen())
    {
        if(!gameOver){
            while (window.pollEvent(ev)) {
                switch (ev.type)
                {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (ev.key.code == sf::Keyboard::Escape)
                        window.close();
                    else if (ev.key.code == sf::Keyboard::D)
                        move = 'D';
                    else if (ev.key.code == sf::Keyboard::A)
                        move = 'A';
                    else if (ev.key.code == sf::Keyboard::W)
                        obj = rotateBlock(obj);
                    else if (ev.key.code == sf::Keyboard::S)
                    {
                        move = 'S';
                    }
                    else if (ev.key.code == sf::Keyboard::Enter) {
                        while (!pauz)
                        {
                            obj = opada(obj, pauz);
                        }
                    }

                }
            }

            window.clear();
            if (move == 'D')
            {
                cout << move << endl;
                index += 1;
                cout << index;
                obj = pomera(obj, 1);
                move = ' ';
            }
            else if (move == 'A')
            {
                index--;
                obj = pomera(obj, -1);
                move = ' ';
            }
            else if (move == 'W')
            {
                obj = rotateBlock(obj);
                move = ' ';
            }
            else if (move == 'S')
            {
                obj = opada(obj, pauz);
                move = ' ';
            }
            for (int i = 0; i < 10; i++)
                for (int j = 0; j < 20; j++)
                {
                    cell.setFillColor(sf::Color::Black);
                    cell.setPosition(60 + i * cellsize, 60 + j * cellsize);
                    window.draw(cell);
                }
            for (auto& k : s)
            {
                int i = k.first, j = k.second;
                if (s.find({ i,j }) != s.end())
                    if (boj[{i, j}] == 1)
                    {
                        cell.setFillColor(sf::Color::Red);
                        cell.setPosition(60 + i * cellsize, 60 + j * cellsize);
                        window.draw(cell);
                    }
                    else if (boj[{i, j}] == 2)
                    {
                        cell.setFillColor(sf::Color::Yellow);
                        cell.setPosition(60 + i * cellsize, 60 + j * cellsize);
                        window.draw(cell);
                    }
                    else if (boj[{i, j}] == 3) {
                        cell.setFillColor(sf::Color::Magenta);
                        cell.setPosition(60 + i * cellsize, 60 + j * cellsize);
                        window.draw(cell);
                    }
            }
            for (auto& p : obj) {
                cell.setPosition(60 + p.first * cellsize, 60 + p.second * cellsize);
                if (boja == 1)
                    cell.setFillColor(sf::Color::Red);
                else if (boja == 2)
                    cell.setFillColor(sf::Color::Yellow);
                else if (boja == 3)
                    cell.setFillColor(sf::Color::Magenta);
                window.draw(cell);
            }
            count++;
            cout << boja << endl;
            if (count == 4)
            {
                obj = opada(obj, pauz);
                count = 0;
            }
            if (pauz)
            {
                for (auto& p : obj)
                {
                    s.insert(p);
                    boj[p] = boja;
                }
                pauz = false;
                obj.clear();
                koji = rand() % 7;
                boja = rand() % 3 + 1;
                obj = figure[koji];
                for (auto& l : obj)
                    if (s.find(l) != s.end())
                        gameOver = true;
            }
            int inc = 0;
            for (int j = 0;j < 20;j++)
            {
                for (int j = 0; j < 20; j++) {
                    int cn = 0;
                    for (int i = 0; i < 10; i++) {
                        if (s.find({ i, j }) != s.end())
                            cn++;
                    }
                    if (cn == 10) {
                        inc++;
                        for (int i = 0; i < 10; i++) {
                            s.erase({ i, j });
                            boj.erase({ i, j });
                        }

                        set<pair<int, int>> newS;
                        map<pair<int, int>, int> newBoj;

                        for (auto& k : s) {
                            if (k.second < j) {
                                pair<int, int> temp = { k.first, k.second + 1 };
                                newS.insert(temp);
                                newBoj[temp] = boj[k];
                            }
                            else {
                                newS.insert(k);
                                newBoj[k] = boj[k];
                            }
                        }
                        s = newS;
                        boj = newBoj;

                        j--;
                    }
                }
            }
            score += pow(inc, 10)*100;
            text.setString("SCORE :" + to_string(score));
            text.setPosition(0, 10);
            window.draw(text);
            window.display();
            sf::sleep(sf::seconds(0.03f));
        }
else {
    window.clear();

    sprite.setTexture(texture);

    sf::Vector2u textureSize = texture.getSize();
    sf::Vector2u windowSize = window.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    sprite.setScale(scaleX, scaleY);

    // Center the sprite
    sprite.setPosition((windowSize.x - textureSize.x * scaleX) / 2,
        (windowSize.y - textureSize.y * scaleY) / 2);

    window.draw(sprite); // Draw the game over sprite
    window.display();
        }
    }
}