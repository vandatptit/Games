#ifndef __PLAYER__
#define __PLAYER__

#include <iostream>
#include "point.h"

#include <Windows.h>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <cstdlib>
#include <time.h>
#include <fstream>
//#include <stdint.h>
#include <iostream>
#include <conio.h>
#include "Animation.h"
using namespace sf;
using namespace std;

class Player{

private:
	/*RenderWindow*	m_window;
	Texture			m_texture_player;
	Sprite			m_sprite_player;*/
	Point position;
	Point location;
	int nextMove;
	char display;
	int status;

public:
	
	Player(){
		this->position.setX(45);
		this->position.setY(45);
		this->location.setX(1);
		this->location.setY(1);
		this->nextMove=2;
		this->status=1;
		this->display = 'P';
		
	}
	Player(int x, int y){
		this->position.setX(20 + x*25);
		this->position.setY(20 + y*25);
		this->location.setX(x);
		this->location.setY(y);
		this->nextMove=2;
		this->status=1;
		this->display = 'P';
		
	}
	void showPlayer(RenderWindow &window, Texture & player){
		Animation mov(&window, player, 0, 0, 25, 25, 0.1, 5, position.getY(), position.getX());
		mov.update();
		mov.draw();
	}
	
	void setPosX(int posX){
		this->position.setX(position.getX() + posX);
	}
	void setPosY(int posY){
		this->position.setY(position.getY() + posY);
	}
	int getPosX(){
		return position.getX();
	}
	int getPosY(){
		return position.getY();
	}

	//void draw(){
	//	this-> m_sprite_player.setPosition(45,45);
	//	m_window-> draw(m_sprite_player);
	//}

	Point getLocation() {
		return this->location;
	};

	char getDisplay() {
		return this->display;
	};

	void setLocation(int X, int Y){
		this->location.setX(X);
		this->location.setY(Y);
	}

	void setDisplay(char display) {
		this->display=display;
	}

	void  setStatus(int newStatus){
		this->status=newStatus;
	};

	int getStatus(){
		return this->status;
	};

	int getDirection(){
		return this->nextMove;
	};

	void updateLocation(int nextMove) {
		int x=dx[nextMove] + this->location.getX();
		int y=dy[nextMove] + this->location.getY();
		this->setLocation(x,y);
	};

	void updateDirection(int nextMove){
		this->nextMove=nextMove;
	}
		
	
};


#endif 
