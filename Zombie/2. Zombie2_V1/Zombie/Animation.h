#ifndef __ANIMATION__
#define __ANIMATION__


#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <vector>

using namespace sf;
using namespace std;


class Animation
{
private:
    RenderWindow * window;
    Sprite sprite;
    float speed;
    float Frame;
    vector <IntRect> Frames;

public:
    Animation(RenderWindow * window, Texture &t, int column, int line, int width, int height, float speed, int count, int x, int y)
    {
        this->window = window;
        this->speed = speed;
        this->Frame = 0;

        for (int i = 0; i < count; i++)
            this->Frames.push_back(IntRect(column + width*i, line, width, height));

        this->sprite.setTexture(t);
		this->sprite.setPosition(x, y);
        this->sprite.setScale(1, 1);
        this->sprite.setTextureRect(this->Frames[count-1]);
    }

    void update()
    {
        this->Frame += this->speed;
        int n = this->Frames.size();
        if (this->Frame > n)
            this->Frame -= n;
        this->sprite.setTextureRect(this->Frames[int(this->Frame)]);
    }
    void draw()
    {
        this->window->draw(this->sprite);
    }
};


#endif