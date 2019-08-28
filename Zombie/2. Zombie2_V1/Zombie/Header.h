
#ifndef _SNAKE_HPP_
#define _SNAKE_HPP_

#include <SFML\Graphics.hpp>
#include "Animation.h"
using namespace sf;
using namespace std;

// Trạng Thái Của Con Rắn
enum STATUS {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	ORTHER
};

// Tọa Độ Cho Một Đốt Của Con Rắn
struct POSITION
{
	float Column;
	float Line;
};

class SNAKE{
private:
	CircleShape sprite;
	STATUS Status;
	POSITION Position[120];
	int Amount;       // Số Đốt Của Con Rắn
	int Mana;         // Thang Điểm Thưởng
public:
	// Constructor
		SNAKE(){
		Die = false;

		Status = RIGHT;

		sprite.setRadius(5);
		Amount = 1;
		Position[0].Column = 100;
		Position[0].Line = 100;
		Position[1].Column = 90;
		Position[1].Line = 100;

		for (int i = 2; i < 120; i++)
		{
			Position[i].Column = NULL;
			Position[i].Line = NULL;
		}
	}

	bool Die;               // True Là Rắn Chết
	                        // False Là Rắn Chưa Chết

	void ShowSnake(RenderWindow &window, Texture & Head){
		Animation mov(&window, Head, 0, 0, 25, 25, 0.8, 3, Position[0].Column, Position[0].Line);
		// Vẽ Cái Đầu
		/*sprite.setTexture(&Head);
		sprite.setPosition(Position[0].Column, Position[0].Line);
		window.draw(sprite);*/
		mov.update();
		mov.draw();
		
	}
	// Hàm Để Xử Lí Di Chuyển
	bool Process(float Column, float Line){
		/*for (int i = Amount - 1; i > 0; i--)
			Position[i] = Position[i - 1];*/
	
		switch (Status)
		{
		case UP:
			Position[0].Line -= 25;
			Status = ORTHER;
			break;
		case DOWN:
			Position[0].Line += 25;
			Status = ORTHER;
			break;
		case LEFT:
			Position[0].Column -= 25;
			Status = ORTHER;
			break;
		case RIGHT:
			Position[0].Column += 25;
			Status = ORTHER;
			break;
		}

		// Xử Lí Chạm Thân
		for (int i = 1; i < Amount; i++){
			if (Position[0].Column == Position[i].Column && Position[0].Line == Position[i].Line){
				Die = true;
				break;
			}
		}

		if (Position[0].Column < 50.0 || Position[0].Column > 1100.0) Die = true;
		if (Position[0].Line < 50.0 || Position[0].Line > 600.0) Die = true;

		if (Position[0].Column == Column && Position[0].Line == Line){
			Amount++;
			for (int i = Amount - 1; i > 0; i--)
				Position[i] = Position[i - 1];
			return true;
		}
		else return false;
	}

	void setStatus(STATUS Tus){
		Status = Tus;
	}
	STATUS getStatus(){
		return Status;
	}
};

#endif