#ifndef __POINT__
#define __POINT__



int dx[5]={0,-1,0,1,0};
int dy[5]={0,0,1,0,-1};

class Point {
private:
	int x;
	int y;
public:
	Point() {
		this->x = 0;
		this->y = 0;
	};

	Point(int x, int y) {
		this->x = x;
		this->y = y;
	};

	int getX() {
		return this->x;
	};

	int getY() {
		return this->y;
	};

	void setX(int x) {
		this->x = x;
	};

	void setY(int y) {
		this->y = y;
	};

	bool operator == (Point point) {
		if(this->x == point.getX() && this->y == point.getY())
			return true;
		else
			return false;
	};
};

#endif