#include <Windows.h>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include "Header.h"
#include "Player.h"
#include "Zombie.h"
#include "Queue.h"
#include <thread>
#include <list>
#include <mutex>
#include "bullet.h"
#include <chrono>


std::mutex mtx; 

RenderWindow* WIN;


using namespace std;
using namespace sf;
const string PathImagesHuman2  = "Resource/Images/Human2/"; 
const string PathImagesHuman  = "Resource/Images/Human/"; 
const string PathImagesZombie = "Resource/Images/zombie/"; 
const string PathImages = "Resource/Images/";
const string PathFonts = "Resource/Fonts/MarkerFelt.ttf";
const string PathMusics = "Resource/Musics/";
const string PathFiles = "Resource/Files/HighScore.txt";
unsigned int Score;

char MAP[25][45];

Player player;
Zombie zombie;
Player player2;

list<Zombie *> listZombie;


int random(int min, int max) //range : [min, max)
{
   static bool first = true;
   if (first) 
   {  
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

void setMap(int x,int y,char in){
	mtx.lock();
	MAP[x][y] = in;
	mtx.unlock();
}

void drawMap(){
	freopen("map.txt","r",stdin);
	for(int i = 0; i < 25; i++){
		for(int j = 0; j < 45 ;j++)
			cin >> MAP[i][j];
	}
	fclose(stdin);
}

unsigned __int64 getTime(){
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


void Delay(int delay){
	unsigned __int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	while(true){
		unsigned __int64 _now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		if(_now - now > delay){
			return;
		}
	}
}
//BFS
int *BFS(char *a,int lengthX,int lengthY,int x1,int y1,int x2,int y2, int &lengtResult){
	Queue queue =  Queue();
	int result[1000];

	queue.push(x1);
	queue.push(y1);
	while(queue.size() > 0){
		int tmpX = queue.pop();
		int tmpY = queue.pop();

		int xi[] = {1,0,-1,0};
		int yi[] = {0,1,0,-1};

		for(int i=0;i<4;i++){
			int xx = tmpX + xi[i];
			int yy = tmpY + yi[i];

			if((xx == x2 && yy == y2)){
				int length = 0;
				result[length++] = xx;
				result[length++] = yy;

				result[length++] = tmpX;
				result[length++] = tmpY;

				if(!(tmpX == x1 && tmpY == y1)){
					int *t = queue.getMap(tmpX,tmpY);
					do{
						result[length++] = t[0];
						result[length++] = t[1];
						t = queue.getMap(t[0],t[1]);
					}while (t != NULL);
				}

				
				 // dao mang

				int _result[1000];

				int count = 0;
				for(int k=length-1;k>=0;k=k-2){
					_result[count++] = result[k-1];
					_result[count++] = result[k];
				}
				queue.clear();
				lengtResult = count;
				return _result;
			}
			if(!(xx == x1 && yy == y1) && xx >= 0 && yy >=0 && xx <lengthX && yy <lengthY /*&& a[xx*lengthY + yy] != 'Z'*/ && a[xx*lengthY + yy] != '1' && a[xx*lengthY + yy] != '*' && a[xx*lengthY + yy] != 'o' && queue.getMap(xx,yy) == NULL){
				// map duong di
				queue.map(xx,yy,tmpX,tmpY);

				queue.push(xx);
				queue.push(yy);
			}
		}
	}
	queue.clear();
	return NULL;

}

int *BFS2(char *a,int lengthX,int lengthY,int x1,int y1,int x2,int y2, int x3, int y3, int &lengtResult){
	Queue queue =  Queue();
	int result[1000];

	queue.push(x1);
	queue.push(y1);
	while(queue.size() > 0){
		int tmpX = queue.pop();
		int tmpY = queue.pop();

		int xi[] = {1,0,-1,0};
		int yi[] = {0,1,0,-1};

		for(int i=0;i<4;i++){
			int xx = tmpX + xi[i];
			int yy = tmpY + yi[i];

			if((xx == x2 && yy == y2)||(xx == x3 && yy == y3)){
				int length = 0;
				result[length++] = xx;
				result[length++] = yy;

				result[length++] = tmpX;
				result[length++] = tmpY;

				if(!(tmpX == x1 && tmpY == y1)){
					int *t = queue.getMap(tmpX,tmpY);
					do{
						result[length++] = t[0];
						result[length++] = t[1];
						t = queue.getMap(t[0],t[1]);
					}while (t != NULL);
				}

				
				 // dao mang

				int _result[1000];

				int count = 0;
				for(int k=length-1;k>=0;k=k-2){
					_result[count++] = result[k-1];
					_result[count++] = result[k];
				}

				lengtResult = count;
				return _result;
			}
			if(!(xx == x1 && yy == y1) && xx >= 0 && yy >=0 && xx <lengthX && yy <lengthY /*&& a[xx*lengthY + yy] != 'Z'*/ && a[xx*lengthY + yy] != '1' && a[xx*lengthY + yy] != '*' && queue.getMap(xx,yy) == NULL){
				// map duong di
				queue.map(xx,yy,tmpX,tmpY);

				queue.push(xx);
				queue.push(yy);
			}
		}
	}

	return NULL;

}


struct WOOD{
	Point posWood[400];
	int quantity;
	Sprite sprite;
	WOOD(){
		
		//sprite.setRadius(5);
	}
	void showWood(RenderWindow &window, Texture &wood){
		sprite.setTexture(wood);
		for(int i = 0; i < quantity; i++){
			sprite.setPosition(posWood[i].getY(), posWood[i].getX());
			window.draw(sprite);
		}
	}
	void updateWood(){
		quantity = 0;
		for(int i = 0; i < 25; i++){
			for(int j = 0; j < 45 ;j++){
				if(MAP[i][j] == '*'){
					posWood[quantity].setX(20 + i*25);
					posWood[quantity].setY(20 + j*25);
					quantity++;
				}
			}
			
		}
	}
};

struct ZOMSHOW{
	Point posZOM[20];
	int quantity;
	Sprite sprite;
	ZOMSHOW(){
		
		//sprite.setRadius(5);
	}
	void showZOM(RenderWindow &window, Texture &ZOM){
		sprite.setTexture(ZOM);
		for(int i = 0; i < quantity; i++){
			sprite.setPosition(posZOM[i].getY(), posZOM[i].getX());
			window.draw(sprite);
		}
	}
	void updateZOM(){
		quantity = 0;
		for(int i = 0; i < 25; i++){
			for(int j = 0; j < 45 ;j++){
				if(MAP[i][j] == 'Z'){
					posZOM[quantity].setX(20 + i*25);
					posZOM[quantity].setY(20 + j*25);
					quantity++;
				}
			}
			
		}
	}
};


void showBullet(RenderWindow &window, Texture &wood){
	CircleShape sprite;
	for(int i = 0; i < 25; i++){
		for(int j = 0; j < 45 ;j++){
			if(MAP[i][j] == 'o'){
				sprite.setTexture(&wood);
				sprite.setRadius(8);
				sprite.setPosition(20 + j*25, 20 + i*25);
				window.draw(sprite);
			}
		}
	}
}

//Slash
void slash(int x, int y, int d){
	switch(d){
	case 1:{
			
				if (MAP[x-1][y]=='*') MAP[x-1][y]='0';
			break;
		   }
	case 2:{
			
				if (MAP[x][y+1]=='*') MAP[x][y+1]='0';
			break;
		   }
	case 3:{
			
				if (MAP[x+1][y]=='*') MAP[x+1][y]='0';
			break;
		   }
	case 4:{
			
				if (MAP[x][y-1]=='*') MAP[x][y-1]='0';
			break;
		   }
	}

}

void move(Player &player, int direction){
	player.updateDirection(direction);

	if(MAP[player.getLocation().getX()+dx[direction]][player.getLocation().getY()+dy[direction]]=='0'){
		//Set Position
		player.setPosX(dx[direction] * 25);
		player.setPosY(dy[direction] * 25);

		MAP[player.getLocation().getX()][player.getLocation().getY()]='0';
		player.updateLocation(direction);

		char test = player.getDisplay();

		MAP[player.getLocation().getX()][player.getLocation().getY()]=test;
	}
}

void moveZ(Zombie *zombie, int x, int y){
	MAP[zombie->getLocation().getX()][zombie->getLocation().getY()]='0';
	zombie->setLocation(x,y);
	MAP[zombie->getLocation().getX()][zombie->getLocation().getY()] = zombie->getDisplay();
}



void threadZombie(int x, int y){
	//Sleep(1000);


	//Texture t_zombie;
	//t_zombie.loadFromFile(PathImagesZombie + "left.png");
	Zombie *zombie = new Zombie();
	/*Texture t_zombie;
	t_zombie.loadFromFile(PathImagesZombie + "right.png");
	Sprite sprite;
	sprite.setTexture(t_zombie);
	sprite.setPosition(x, y);

	WIN->draw(sprite);*/



	int length;
	int counter=0;
	int xZombie,yZombie,xPlayer,yPlayer;
	zombie->setLocation(x,y);

	listZombie.push_back(zombie);

	int timeSleep = 500;

	setMap(zombie->getLocation().getX(),zombie->getLocation().getX(),zombie->getDisplay());

	while(zombie->getStatus()==1){
		xZombie=zombie->getLocation().getX();
		yZombie=zombie->getLocation().getY();
		xPlayer=player.getLocation().getX();
		yPlayer=player.getLocation().getY();
		int* zombieWay = BFS((char*)MAP,25,45,xZombie,yZombie,xPlayer,yPlayer,length);
		if(zombieWay==NULL) {
			bool check = false;
			if(zombie->direction == -1){
				int x_ = zombie->getLocation().getX();
				int y_ = zombie->getLocation().getY();
				int ran = random(50,100);

				for (int ii=ran; ii<=ran+4; ii++){
					int _tm = ii % 4 + 1;
					int x__=x_+dx[_tm];
					int y__=y_+dy[_tm];
					if(MAP[x__][y__] == '0'){
						zombie->direction = _tm;
						moveZ(zombie,x__,y__);
						check = true;
						Sleep(timeSleep);
						break;
					}
				}
				
			}else{
				int x_ = zombie->getLocation().getX();
				int y_ = zombie->getLocation().getY();
				int ii = zombie->direction;
				x_+=dx[ii];
				y_+=dy[ii];
				if(MAP[x_][y_] == '0'){
					zombie->direction = ii;
					moveZ(zombie,x_,y_);
					check = true;
					Sleep(timeSleep);
				}else{
					x_ = zombie->getLocation().getX();
					y_ = zombie->getLocation().getY();
					int ran = random(50,100);
					for (ii=ran; ii<=ran+4; ii++){
						int _tm = ii % 4 + 1;
						int x__=x_+dx[_tm];
						int y__=y_+dy[_tm];
						if(MAP[x__][y__] == '0'){
							zombie->direction = _tm;
							moveZ(zombie,x__,y__);
							check = true;
							Sleep(timeSleep);
							break;
						}
					}
				
				}
			}

			if(check){
				continue;
			}else{
				Sleep(5000);
				continue;
			}
		};
		if(xPlayer==zombieWay[2] && yPlayer==zombieWay[3]){ 
			player.setStatus(0);	
			break;
		}
		if(zombie->getStatus()==0){
			return;
		}

		if(listZombie.size() == 0){
			return;
		}

		moveZ(zombie,zombieWay[2],zombieWay[3]);
		Sleep(timeSleep);
	}
	return ;
}
void threadZombie2(int x, int y){
	//Sleep(1000);


	//Texture t_zombie;
	//t_zombie.loadFromFile(PathImagesZombie + "left.png");
	Zombie *zombie = new Zombie();
	/*Texture t_zombie;
	t_zombie.loadFromFile(PathImagesZombie + "right.png");
	Sprite sprite;
	sprite.setTexture(t_zombie);
	sprite.setPosition(x, y);

	WIN->draw(sprite);*/



	int length;
	int counter=0;
	int xZombie,yZombie,xPlayer,yPlayer,xPlayer2,yPlayer2;
	zombie->setLocation(x,y);

	listZombie.push_back(zombie);

	int timeSleep = 500;


	MAP[zombie->getLocation().getX()][zombie->getLocation().getY()]=zombie->getDisplay();
	while(zombie->getStatus()==1){
		xZombie=zombie->getLocation().getX();
		yZombie=zombie->getLocation().getY();
		xPlayer=player.getLocation().getX();
		yPlayer=player.getLocation().getY();
		xPlayer2=player2.getLocation().getX();
		yPlayer2=player2.getLocation().getY();
		int* zombieWay = BFS2((char*)MAP,25,45,xZombie,yZombie,xPlayer,yPlayer,xPlayer2,yPlayer2,length);
		if(zombieWay==NULL) {
			bool check = false;
			if(zombie->direction == -1){
				int x_ = zombie->getLocation().getX();
				int y_ = zombie->getLocation().getY();
				int ran = random(50,100);

				for (int ii=ran; ii<=ran+4; ii++){
					int _tm = ii % 4 + 1;
					int x__=x_+dx[_tm];
					int y__=y_+dy[_tm];
					if(MAP[x__][y__] == '0'){
						zombie->direction = _tm;
						moveZ(zombie,x__,y__);
						check = true;
						Sleep(timeSleep);
						break;
					}
				}
				
			}else{
				int x_ = zombie->getLocation().getX();
				int y_ = zombie->getLocation().getY();
				int ii = zombie->direction;
				x_+=dx[ii];
				y_+=dy[ii];
				if(MAP[x_][y_] == '0'){
					zombie->direction = ii;
					moveZ(zombie,x_,y_);
					check = true;
					Sleep(timeSleep);
				}else{
					x_ = zombie->getLocation().getX();
					y_ = zombie->getLocation().getY();
					int ran = random(50,100);
					for (ii=ran; ii<=ran+4; ii++){
						int _tm = ii % 4 + 1;
						int x__=x_+dx[_tm];
						int y__=y_+dy[_tm];
						if(MAP[x__][y__] == '0'){
							zombie->direction = _tm;
							moveZ(zombie,x__,y__);
							check = true;
							Sleep(timeSleep);
							break;
						}
					}
				
				}
			}

			if(check){
				continue;
			}else{
				Sleep(5000);
				continue;
			}
		};
		if(xPlayer==zombieWay[2] && yPlayer==zombieWay[3]){ 
			player.setStatus(0);	
			break;
		}
		if(zombie->getStatus()==0){
			return;
		}
		moveZ(zombie,zombieWay[2],zombieWay[3]);
		Sleep(timeSleep);
	}
	return ;
}
void shoot(int x, int y, int d){
	Bullet bullet(x,y,d);
	int check=1;
	while(MAP[x+dx[d]][y+dy[d]]!='1' && MAP[x+dx[d]][y+dy[d]]!='*' && check==1){
		if(MAP[x][y]=='o') setMap(x,y,'0');
		x+=dx[d];
		y+=dy[d];
		setMap(x,y,bullet.getDisplay());
		for (Zombie* z : listZombie) {
			if(z->getStatus()==1 && (( z->getLocation().getX()==x && z->getLocation().getY()==y)||(z->getLocation().getX()==x+dx[d] && z->getLocation().getY()==y+dy[d]))){
				check=0;
				setMap(z->getLocation().getX(),z->getLocation().getY(),'0');
				z->setStatus(0);
				break;
			}
		}

		
		Sleep(100);
	}
	if(MAP[x][y]=='o') setMap(x,y,'0');
}

void SetHighScore(){
	// Đọc File
	ifstream Read;
	Read.open(PathFiles, ios_base::in);

	unsigned int Score[5];

	for (int i = 0; i < 5; i++)
		Read >> Score[i];

	Read.close();

	if (Score[4] < ::Score)
	{
		Score[4] = ::Score;
		for (int i = 0; i < 5; i++)
			for (int j = i; j < 5; j++)
				if (Score[i] < Score[j])
				{
					Score[i] = Score[i] + Score[j];
					Score[j] = Score[i] - Score[j];
					Score[i] = Score[i] - Score[j];
				}
		ofstream Write;
		Write.open(PathFiles, ios_base::out);
		Write.clear();
		for (int i = 0; i < 5; i++)
		{
			Write << Score[i];
			Write << "\n";
		}

		Write.close();
	}
}

void HighScore(RenderWindow &window){
	// Đọc File
	ifstream Read;
	Read.open(PathFiles, ios_base::in);

	unsigned int * Score = new unsigned int[5];
	string SCORE[5];
	for (int i = 0; i < 5; i++)
	{
		Read >> *(Score + i);
		SCORE[i] = to_string(*(Score + i));
	}
	delete Score;
	Read.close();

	Texture Background;
	Background.loadFromFile(PathImages + "HighScore.png");
	Sprite MainBackground(Background);

	Font font;
	font.loadFromFile(PathFonts);
	Text Point;
	Point.setFont(font);
	Point.setCharacterSize(18);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(MainBackground);
		for (int i = 0; i < 5; i++)
		{
			Point.setString(SCORE[i]);
			Point.setPosition(280, i * 40 + 150);
			window.draw(Point);
		}
		window.display();
		sleep(microseconds(50));
		if (Mouse::isButtonPressed(Mouse::Left) && Mouse::getPosition(window).x >= 0 && Mouse::getPosition(window).x <= 600 && Mouse::getPosition(window).y >= 0 && Mouse::getPosition(window).y <= 400) break;
		if (Keyboard::isKeyPressed(Keyboard::Escape)) break;
	}
}

void Author(RenderWindow &window){
	Texture Background;
	Background.loadFromFile(PathImages + "Author.png");
	Sprite MainBackground(Background);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(MainBackground);
		window.display();
		sleep(microseconds(50));
		if (Mouse::isButtonPressed(Mouse::Left) && Mouse::getPosition(window).x >= 0 && Mouse::getPosition(window).x <= 600 && Mouse::getPosition(window).y >= 0 && Mouse::getPosition(window).y <= 400) break;
		
	}
}

void Guide(RenderWindow &window){
	Texture Background;
	Background.loadFromFile(PathImages + "Guide.png");
	Sprite MainBackground(Background);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(MainBackground);
		window.display();
		sleep(microseconds(50));
		if (Mouse::isButtonPressed(Mouse::Left) && Mouse::getPosition(window).x >= 0 && Mouse::getPosition(window).x <= 600 && Mouse::getPosition(window).y >= 0 && Mouse::getPosition(window).y <= 400) break;
		
	}
}


void testthread(){
	Delay(1000);

	return;
}




void randomZombie(){
	int _X[1000];
	int _Y[1000];
	int count =0;

	if(listZombie.size() > 7){
		return;
	}

	for(int i=0;i<25;i++){
		for(int j=0;j<45;j++){
			if(j > 0 && i>0 &&MAP[i][j] == '0'){
				_X[count] = i;
				_Y[count] = j;
				count ++;
			}
		}
		
	}

	int ran = random(0,count);


	while(true){
		int x = _X[ran];
		int y = _Y[ran];

		int length;
		int xPlayer=player.getLocation().getX();
		int yPlayer=player.getLocation().getY();

		int* zombieWay = BFS((char*)MAP,25,45,x,y,xPlayer,yPlayer,length);
		if(zombieWay == NULL || length > 20){
			thread dfs (threadZombie,x,y);
			dfs.detach();
			dfs.~thread();

			return;
		}else{
			continue;
		}
		
		ran = random(0,count);
	}
}

void randomZombie2(){

	while(true){
		int _X[1000];
		int _Y[1000];
		int count =0;

		int length;
		int xPlayer=player.getLocation().getX();
		int yPlayer=player.getLocation().getY();
		int xPlayer2=player2.getLocation().getX();
		int yPlayer2=player2.getLocation().getY();
		for(int i=0;i<25;i++){
			for(int j=0;j<45;j++){
				if(j > 0 && i>0 && MAP[i][j] == '0' 
					&& BFS2((char*)MAP,25,45,i,j,xPlayer,yPlayer,xPlayer2,yPlayer2,length) != NULL){
					if(length < 20){
						continue;
					}
					_X[count] = i;
					_Y[count] = j;
					count ++;
				}
			}
		
		}
		int ran = random(0,count);

		int MAX = 100;

		while(MAX > 0 && count > 0){
			MAX --;
			int x = _X[ran];
			int y = _Y[ran];

			int* zombieWay = BFS((char*)MAP,25,45,x,y,xPlayer,yPlayer,length);
			if(length > 20){
				thread dfs (threadZombie,x,y);
				dfs.detach();
				dfs.~thread();
				break;
			}
			
			ran = random(0,count);
		}
		Sleep(5000);
	
	}


}





void PlayGame(RenderWindow &window){
	window.clear();
	listZombie.clear();
	drawMap();
	player = Player();
	Texture t_player, t_wood, Background, t_zombie, t_bullet;
	t_player.loadFromFile(PathImagesHuman + "down.png");
	t_zombie.loadFromFile(PathImagesZombie + "right.png");
	t_bullet.loadFromFile(PathImages + "bullet.png");
	/*Texture t_zombie;
	Sprite zom;
	t_zombie.loadFromFile(PathImagesZombie + "left.png");
	zom.setTexture(t_zombie);*/
	//zom.setPosition(zombie-> getPosY(), zombie-> getPosX());
	

	t_wood.loadFromFile(PathImages + "wood.jpg");
	Background.loadFromFile(PathImages + "BG1.png");

	Sprite MainBackground(Background);

	//Zombie zombie;
	WOOD wood;
	ZOMSHOW zom;
	//threadZombie(23, 1, window);
	/*if(zombie.joinable()){
		zombie.join();
	}*/

	Score = 0;

	Font font;
	font.loadFromFile(PathFonts);

	Text SCORE;
	SCORE.setFont(font);
	SCORE.setString("Score: " + to_string(Score));
	SCORE.setColor(Color::Yellow);
	SCORE.setPosition(540, 650);
	SCORE.setCharacterSize(32);

	SoundBuffer start, eatfruit, end, beep, ban;
	start.loadFromFile(PathMusics + "Start.wav");
	eatfruit.loadFromFile(PathMusics + "EatFruit.wav");
	end.loadFromFile(PathMusics + "EndGame.wav");
	beep.loadFromFile(PathMusics + "Beep.wav");
	
	Sound Start(start), End(end), Beep(beep), EatFruit(eatfruit);
	Start.play();


	/*thread zombie2 (threadZombie,1,4);
	zombie2.detach();
	zombie2.~thread();*/


	unsigned __int64 latestBullet = 0;

	unsigned __int64 latestShot = 0;

	unsigned __int64 latestMove = 0;

	int delayMove = 200;

	int countBullet = 5;

	unsigned __int64 latestReloadShot = -1;
	
	thread dslfkdj(randomZombie2);

	while (player.getStatus() == 1){

		if(getTime() - latestBullet > 3000){
			randomZombie();
			latestBullet = getTime();
		}

		if(latestReloadShot!= -1 && getTime() - latestReloadShot > 5000){
			countBullet = 5;
		}

		if(getTime() - latestShot > 3000){
			countBullet = 5;
		}

		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(MainBackground);


		for(int i=1;i<=countBullet;i++){
			CircleShape __sprite;

			__sprite.setTexture(&t_bullet);
			__sprite.setRadius(10);
			__sprite.setPosition(20 + i*25, 20 + 26*25);
			window.draw(__sprite);
		}
		

		int _count = 0;
		for (Zombie* z : listZombie) {
			if(z->getStatus() == 0){
				_count ++;
				continue;
				
			}
			

			Texture t__zombie;
			t__zombie.loadFromFile(PathImagesZombie + "right.png");
			Sprite sprite;
			sprite.setTexture(t__zombie);
			sprite.setPosition(20+z->getLocation().getY()*25,20+z->getLocation().getX()*25);
			window.draw(sprite);
		}

		Score = _count;	
		SCORE.setString("Score: " + to_string(Score));


		window.draw(SCORE);	

		
		
		
		/*thread zombie3 (threadZombie,14,28);
		
		thread zombie4 (threadZombie,18,2);*/

		//window.draw(Wood.sprite);

		//Kiem---------------------------------
		if(Keyboard::isKeyPressed(Keyboard::Num1) || Keyboard::isKeyPressed(Keyboard::Numpad1)){
			slash(player.getLocation().getX(),player.getLocation().getY(), player.getDirection());
			Beep.play();
		}
		if(countBullet > 0 && getTime() - latestShot > 200 && Keyboard::isKeyPressed(Keyboard::Num2) || Keyboard::isKeyPressed(Keyboard::Numpad2)){
			EatFruit.play();
			countBullet --;
			latestShot = getTime();
			thread dsfkd(shoot,player.getLocation().getX(),player.getLocation().getY(), player.getDirection());
			dsfkd.detach();
			dsfkd.~thread();

			if(countBullet == 0){
				latestReloadShot = getTime();
			}else{
				latestReloadShot = -1;
			}

		}
		//------------------------------------

		wood.updateWood();
		wood.showWood(window, t_wood);
		// zom.updateZOM();
		zom.showZOM(window, t_zombie);

		/*CircleShape __sprite;

		__sprite.setTexture(&t_bullet);
		__sprite.setRadius(5);
		__sprite.setPosition(20 + 4*25, 20 + 1*25);
		window.draw(__sprite);*/


		showBullet(window,t_bullet);




		//move(player, 1);
		

		//direction
		int direction1 = player.getDirection();
		//int direction2 = player2.getDirection();

		if (getTime() -latestMove > delayMove && (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))){
			direction1 = 1;
			t_player.loadFromFile(PathImagesHuman + "up.png");
			move(player,direction1);
			Beep.play();
			latestMove = getTime();
		}
		if (getTime() -latestMove > delayMove &&(Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))){
			direction1 = 2;
			t_player.loadFromFile(PathImagesHuman + "right.png");
			move(player,direction1);
			Beep.play();
			latestMove = getTime();
		}
		if (getTime() -latestMove > delayMove &&(Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))){
			direction1 = 3;
			t_player.loadFromFile(PathImagesHuman + "down.png");
			move(player,direction1);
			Beep.play();
			latestMove = getTime();
		}
		if (getTime() -latestMove > delayMove &&(Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))){
			/*Snake.setStatus(LEFT);*/
			direction1 = 4;
			t_player.loadFromFile(PathImagesHuman + "left.png");
			move(player,direction1);
			Beep.play();
			latestMove = getTime();
		}
		if (Keyboard::isKeyPressed(Keyboard::P)){

			randomZombie();
		}
		player.showPlayer(window, t_player);
		window.display();
		sleep(milliseconds(50));
	}
	End.play();
	
	while (End.getStatus() == End.Playing && window.isOpen()){
		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed) window.close();
		}
	}

	Background.loadFromFile(PathImages + "GameOver.png");
	MainBackground.setTexture(Background);

	while (window.isOpen()){
		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(MainBackground);
		window.display();
		if (Mouse::isButtonPressed(Mouse::Left) && Mouse::getPosition(window).x >= 0 && Mouse::getPosition(window).x <= 600 && Mouse::getPosition(window).y >= 0 && Mouse::getPosition(window).y <= 400) break;
		sleep(microseconds(50));
	}

	SetHighScore();
}

void PlayGame2(RenderWindow &window){
	drawMap();
	player = Player(22, 43);
	player2 = Player(1, 1);

	Texture t_player, t_player2, t_wood, Background, t_zombie, t_bullet;
	t_player.loadFromFile(PathImagesHuman + "down.png");
	t_player2.loadFromFile(PathImagesHuman2 + "down.png");
	t_zombie.loadFromFile(PathImagesZombie + "right.png");
	t_bullet.loadFromFile(PathImages + "bullet.png");
	/*Texture t_zombie;
	Sprite zom;
	t_zombie.loadFromFile(PathImagesZombie + "left.png");
	zom.setTexture(t_zombie);*/
	//zom.setPosition(zombie-> getPosY(), zombie-> getPosX());
	

	t_wood.loadFromFile(PathImages + "wood.jpg");
	Background.loadFromFile(PathImages + "BG1.png");

	Sprite MainBackground(Background);

	//Zombie zombie;
	WOOD wood;
	ZOMSHOW zom;
	//threadZombie(23, 1, window);
	/*if(zombie.joinable()){
		zombie.join();
	}*/

	Score = 0;

	Font font;
	font.loadFromFile(PathFonts);

	Text SCORE;
	SCORE.setFont(font);
	SCORE.setString("Score: " + to_string(Score));
	SCORE.setColor(Color::Yellow);
	SCORE.setPosition(540, 650);
	SCORE.setCharacterSize(32);

	SoundBuffer start, eatfruit, end, beep, ban;
	start.loadFromFile(PathMusics + "Start.wav");
	eatfruit.loadFromFile(PathMusics + "EatFruit.wav");
	end.loadFromFile(PathMusics + "EndGame.wav");
	beep.loadFromFile(PathMusics + "Beep.wav");
	
	Sound Start(start), End(end), Beep(beep), EatFruit(eatfruit);
	Start.play();


	/*thread zombie2 (threadZombie,1,4);
	zombie2.detach();
	zombie2.~thread();*/


	unsigned __int64 latestBullet = 0;


	unsigned __int64 latestMove = 0;
	unsigned __int64 latestMove2 = 0;

	int delayMove = 200;
	
	


	while (player.getStatus() == 1 && player2.getStatus()==1){

		if(getTime() - latestBullet > 5000){
			randomZombie();
			latestBullet = getTime();
		}


		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(MainBackground);
		

		int _count = 0;
		for (Zombie* z : listZombie) {
			if(z->getStatus() == 0){
				_count ++;
				continue;
				
			}
			

			Texture t__zombie;
			t__zombie.loadFromFile(PathImagesZombie + "right.png");
			Sprite sprite;
			sprite.setTexture(t__zombie);
			sprite.setPosition(20+z->getLocation().getY()*25,20+z->getLocation().getX()*25);
			window.draw(sprite);
		}

		Score = _count;	
		SCORE.setString("Score: " + to_string(Score));


		window.draw(SCORE);	

		
		
		
		/*thread zombie3 (threadZombie,14,28);
		
		thread zombie4 (threadZombie,18,2);*/

		//window.draw(Wood.sprite);

		//Kiem---------------------------------
		if(Keyboard::isKeyPressed(Keyboard::Numpad1)){
			slash(player.getLocation().getX(),player.getLocation().getY(), player.getDirection());
			Beep.play();
		}
		if(Keyboard::isKeyPressed(Keyboard::T)){
			slash(player2.getLocation().getX(),player2.getLocation().getY(), player2.getDirection());
			Beep.play();
		}
		if(Keyboard::isKeyPressed(Keyboard::Numpad2)){
			EatFruit.play();
			thread dsfkd(shoot,player.getLocation().getX(),player.getLocation().getY(), player.getDirection());
			dsfkd.detach();
			dsfkd.~thread();
		}
		if(Keyboard::isKeyPressed(Keyboard::Y)){
			EatFruit.play();
			thread dsfkd(shoot,player2.getLocation().getX(),player2.getLocation().getY(), player2.getDirection());
			dsfkd.detach();
			dsfkd.~thread();
		}
		//------------------------------------

		wood.updateWood();
		wood.showWood(window, t_wood);
		// zom.updateZOM();
		zom.showZOM(window, t_zombie);

		/*CircleShape __sprite;

		__sprite.setTexture(&t_bullet);
		__sprite.setRadius(5);
		__sprite.setPosition(20 + 4*25, 20 + 1*25);
		window.draw(__sprite);*/


		showBullet(window,t_bullet);




		//move(player, 1);
		

		//direction
		int direction1 = player.getDirection();
		int direction2 = player2.getDirection();

		if (getTime() -latestMove > delayMove && (Keyboard::isKeyPressed(Keyboard::Up) )){
			direction1 = 1;
			t_player.loadFromFile(PathImagesHuman + "up.png");
			move(player,direction1);
			Beep.play();
			latestMove = getTime();
		}
		if (getTime() -latestMove2 > delayMove && (Keyboard::isKeyPressed(Keyboard::W) )){
			direction2 = 1;
			t_player2.loadFromFile(PathImagesHuman2 + "up.png");
			move(player2,direction2);
			Beep.play();
			latestMove2 = getTime();
		}
		if (getTime() -latestMove > delayMove &&(Keyboard::isKeyPressed(Keyboard::Right))){
			direction1 = 2;
			t_player.loadFromFile(PathImagesHuman + "right.png");
			move(player,direction1);
			Beep.play();
			latestMove = getTime();
		}
		if (getTime() -latestMove2 > delayMove &&(Keyboard::isKeyPressed(Keyboard::D))){
			direction2 = 2;
			t_player2.loadFromFile(PathImagesHuman2 + "right.png");
			move(player2,direction2);
			Beep.play();
			latestMove2 = getTime();
		}
		if (getTime() -latestMove > delayMove &&(Keyboard::isKeyPressed(Keyboard::Down) )){
			direction1 = 3;
			t_player.loadFromFile(PathImagesHuman + "down.png");
			move(player,direction1);
			Beep.play();
			latestMove = getTime();
		}
		if (getTime() -latestMove2 > delayMove &&( Keyboard::isKeyPressed(Keyboard::S))){
			direction2 = 3;
			t_player2.loadFromFile(PathImagesHuman2 + "down.png");
			move(player2,direction2);
			Beep.play();
			latestMove2 = getTime();
		}
		if (getTime() -latestMove > delayMove &&(Keyboard::isKeyPressed(Keyboard::Left))){
	
			direction1 = 4;
			t_player.loadFromFile(PathImagesHuman + "left.png");
			move(player,direction1);
			Beep.play();
			latestMove = getTime();
		}
		if (getTime() -latestMove2 > delayMove &&(Keyboard::isKeyPressed(Keyboard::A))){
			direction2 = 4;
			t_player2.loadFromFile(PathImagesHuman2 + "left.png");
			move(player2,direction2);
			Beep.play();
			latestMove2 = getTime();
		}
		if (Keyboard::isKeyPressed(Keyboard::P)){
			randomZombie();
		}
		player.showPlayer(window, t_player);
		player2.showPlayer(window, t_player2);
		window.display();
		sleep(milliseconds(50));
	}
	End.play();
	
	while (End.getStatus() == End.Playing && window.isOpen()){
		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed) window.close();
		}
	}

	Background.loadFromFile(PathImages + "GameOver.png");
	MainBackground.setTexture(Background);

	while (window.isOpen()){
		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(MainBackground);
		window.display();
		if (Mouse::isButtonPressed(Mouse::Left) && Mouse::getPosition(window).x >= 0 && Mouse::getPosition(window).x <= 600 && Mouse::getPosition(window).y >= 0 && Mouse::getPosition(window).y <= 400) break;
		sleep(microseconds(50));
	}

	SetHighScore();
}
void Menu(RenderWindow &window){
	Texture Back;
	Back.loadFromFile(PathImages + "bg.jpg");

	Sprite Background(Back);

	Font font;
	font.loadFromFile(PathFonts);

	Text Play, Aut, High, Quit, Gui;
	Play.setString("Play");
	Aut.setString("Human vs Human");
	High.setString("High Score");
	Gui.setString("How To Play");
	Quit.setString("Quit");

	Play.setFont(font);
	Aut.setFont(font);
	High.setFont(font);
	Quit.setFont(font);
	Gui.setFont(font);

	Play.setCharacterSize(24);
	Aut.setCharacterSize(24);
	High.setCharacterSize(24);
	Quit.setCharacterSize(24);
	Gui.setCharacterSize(24);

	Play.setPosition(100, 220);
	Aut.setPosition(100, 260);
	High.setPosition(100, 300);
	Quit.setPosition(100, 380);
	Gui.setPosition(100,340);

	Play.setColor(Color::Yellow);
	Aut.setColor(Color::Yellow);
	High.setColor(Color::Yellow);
	Quit.setColor(Color::Yellow);
	Gui.setColor(Color::Yellow);

	Music music;
	music.openFromFile(PathMusics + "Main.wav");
	music.setLoop(true);
	music.play();

	while (window.isOpen()){
		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(Background);
		window.draw(Play);
		window.draw(Aut);
		window.draw(High);
		window.draw(Gui);
		window.draw(Quit);
		window.display();
		sleep(microseconds(50));
		
		if (Mouse::getPosition(window).x >= 100 && Mouse::getPosition(window).x <= 130 && Mouse::getPosition(window).y >= 220 && Mouse::getPosition(window).y <= 244){
			Play.setColor(Color::Magenta);
			if (Mouse::isButtonPressed(Mouse::Left)){
				music.pause();
				PlayGame(window);
				music.play();
			}
		}
		else Play.setColor(Color::Yellow);

		if (Mouse::getPosition(window).x >= 100 && Mouse::getPosition(window).x <= 190 && Mouse::getPosition(window).y >= 260 && Mouse::getPosition(window).y <= 284){
			Aut.setColor(Color::Magenta);
			if (Mouse::isButtonPressed(Mouse::Left)) PlayGame2(window);
		}
		else Aut.setColor(Color::Yellow);

		if (Mouse::getPosition(window).x >= 100 && Mouse::getPosition(window).x <= 177 && Mouse::getPosition(window).y >= 300 && Mouse::getPosition(window).y <= 320){
			High.setColor(Color::Magenta);
			if (Mouse::isButtonPressed(Mouse::Left)) HighScore(window);
		}
		else High.setColor(Color::Yellow);

		if (Mouse::getPosition(window).x >= 100 && Mouse::getPosition(window).x <= 190 && Mouse::getPosition(window).y >= 340 && Mouse::getPosition(window).y <= 364){
			Gui.setColor(Color::Magenta);
			if (Mouse::isButtonPressed(Mouse::Left)) Guide(window);
		}
		else Gui.setColor(Color::Yellow);

		if (Mouse::getPosition(window).x >= 100 && Mouse::getPosition(window).x <= 130 && Mouse::getPosition(window).y >= 380 && Mouse::getPosition(window).y <= 404){
			Quit.setColor(Color::Magenta);
			if (Mouse::isButtonPressed(Mouse::Left)) break;
		}
		else Quit.setColor(Color::Yellow);
	}
	music.stop();
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){ //WINAPI
	
	RenderWindow window(VideoMode(1200, 700), "Zombies - Rules Of Survival // WaterMelon", Style::Titlebar);

	//srand(time(NULL));

	WIN = &window;

	Menu(window);
	return EXIT_SUCCESS;
}