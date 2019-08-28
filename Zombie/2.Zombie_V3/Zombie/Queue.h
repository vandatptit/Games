class Queue
{
private:
	int *matrix;
	int first;
	int last;

	int *iX;
	int *iY;
	int *oX;
	int *oY;

	int lengthMap;

public:
	Queue(){
		this->first = this->last = -1;
		this->matrix = new int[10000];

		iX = new int[1000];
		iY = new int[1000];
		oX = new int[1000];
		oY = new int[1000];

		lengthMap = 0;
	};

	void clear(){
		delete this->matrix;
	}

	void map(int x1,int y1,int x2,int y2){
		iX[lengthMap] = x1;
		iY[lengthMap] = y1;
		oX[lengthMap] = x2;
		oY[lengthMap] = y2;
		lengthMap ++;
	}

	int *getMap(int x,int y){
		for(int i=0;i<lengthMap;i++){
			if(iX[i] == x && iY[i] == y){
				int * out = new int[2];			
				out[0] = oX[i];
				out[1] = oY[i];
				return out;
			}
		}
		return NULL;
	}





	int size(){
		if(this->first == -1 || this->last == -1){
			return 0;
		}else{
			return ( this->last - this->last) + 1;
		}
	};

	void push(int a){
		if(this->size() == 0){
			this->first = this->last = 0;
			this->matrix[0] = a;
		}else{
			this->matrix[++this->last] = a;
		}
	};

	int pop(){
		int a = this->matrix[this->first];
		if(this->first == this->last){
			this->first = this->last = -1;
		}else{
			this->first ++;
		}
		return a;
	};
};