#ifndef __BULLET__
#define __BULLET__

#include <iostream>
#include "point.h"

class Bullet{
private:
	Point location;
	int direction;
	int status;
	char display;
public:
	Bullet (int x, int y, int d){
		this->location.setX(x);
		this->location.setY(y);
		this->direction=d;
		this->status=1;
		this->display='o';
	}

	char getDisplay(){
		return this->display;
	}

	void fire(){
		int x=this->location.getX();
		int y=this->location.getY();
		this->location.setX(x+dx[direction]);
		this->location.setY(y+dx[direction]);
	};

};

#endif