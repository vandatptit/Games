#ifndef __ZOMBIE__
#define __ZOMBIE__

#include <iostream>
#include "point.h"

#include <Windows.h>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <cstdlib>
#include <time.h>
//#include <stdint.h>
#include <iostream>
#include <conio.h>

#include "Animation.h"



using namespace sf;
using namespace std;

class Zombie{

private:
	/*RenderWindow*	m_window;
	Texture			m_texture_player;*/
	Sprite			m_sprite_zombie;
	Point position;
	Point location;
	int nextMove;
	char display;
	int status;
	int level;
	

public:
	int direction;
	Zombie(){
		this->position.setX(595);
		this->position.setY(45);
		this->location.setX(23);
		this->location.setY(1);
		this->nextMove = 4;
		this->status = 1;
		this->level = 1;
		this->display='Z';
		this->direction = -1;
		//showZombie(window, t_zombie);
		
	}
	void showZombie(RenderWindow &window, Texture &t_zombie){
		m_sprite_zombie.setTexture(t_zombie);
		m_sprite_zombie.setPosition(position.getY(), position.getX());
		window.draw(m_sprite_zombie);
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
