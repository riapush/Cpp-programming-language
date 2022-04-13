#include <iostream>

class Square {
protected:
	int _h;
	double _a;

	virtual double _area() { return _h * _h; }

public:
	Square() :_h(0) { _a = _area(); }
	Square(int h) :_h(h){ _a = _area(); }

	void h(int h){
		_h = h;
		_a = _area();
	}

	virtual void draw() {
		for (int i = 0; i < _h; ++i) {
			for (int j = 0; j < _h; ++j)
				std::cout << "#" << std::endl;
			std::cout << "\n" << std::endl;
		}
	}

	virtual double area() {
		return _a;
	}
};

class Rectangle:Square {
protected:
	int _w;
	double _area()override{ return _h * _w; }
public:
	Rectangle() : Square(), _w(0) { _a = _area(); }
	Rectangle(int h, int w):Square(h), _w(w){ _a = _area(); }

	double area() {
		return _a;
	}

	void w(int w) {
		_w = w;
		_a = _area();
	}
	void set(int h, int w) {
		_w = w;
		_h = h;
		_a = _area();
	}

	virtual void draw() {
		for (int i = 0; i < _h; ++i) {
			for (int j = 0; j < _w; ++j)
				std::cout << "#";
			std::cout << std::endl;
		}
	}

};

int main(void) {
	Rectangle a{ Rectangle(4,10) };
	a.draw();
	std::cout << a.area();
}