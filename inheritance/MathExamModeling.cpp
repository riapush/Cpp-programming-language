#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <queue>
#include <ctime>
#include <fstream>
using namespace std;

enum class NumOfRoots { Zero, One, Two };

class Answer {
public:
	double x1;
	double x2;
	NumOfRoots n;
	
	Answer() :x1(0), x2(0), n(NumOfRoots::Zero) {}

	friend bool operator== (const Answer& c1, const Answer& c2);
};

bool operator== (const Answer& c1, const Answer& c2)
{
	return (c1.x1 == c2.x1 &&
		c1.x2 == c2.x2 && c1.n == c2.n);
}

class QuadEq {

public:
	double _a, _b, _c;
	double _d{ _b * _b - 2 * _a * _c };
	//QuadEq() : _a(0), _b(0), _c(0) {}
	//QuadEq(double a, double b, double c) : _a(a), _b(b), _c(c) {}
	//double getA() const { return _a; }
	//double getB() const { return _b; }
	//double getC() const { return _c; }

	//double setA(double a) {
	//	_a = a;
	//	_d = _b * _b - 2 * _a * _c;
	//}
	//double setB(double b) { 
	//	_b = b; 
	//	_d = _b * _b - 2 * _a * _c;
	//}
	//double setC(double c) { 
	//	_c = c; 
	//	_d = _b * _b - 2 * _a * _c;
	//}

	//double set(double a, double b, double c) {
	//	_a = a;
	//	_b = b;
	//	_c = c;

	//}

	Answer solve(NumOfRoots& n) {
		Answer ans;
		if (_d >= 0) {
			double* x{ new double[2] };
			ans.x1 = (-_b + sqrt(_d)) / (2 * _a);
			ans.x2 = (-_b - sqrt(_d)) / (2 * _a);
		}
		if (_d > 0) { ans.n = NumOfRoots::Two; }
		if (_d == 0) { ans.n = NumOfRoots::One; }
		if (_d < 0) { ans.n = NumOfRoots::Zero; }
		return ans;
	}

};

class Letter {
public:
	QuadEq eq;
	Answer ans;
	string name;

	Letter(QuadEq eq, Answer ans, string name) :eq(eq), ans(ans), name(name) {}
};

class Teacher {

	class Result {
	public:
		string name;
		int successes;

	};
	vector<Result> results;

public:
	queue<Letter> letters;

	bool isCorrectAns(QuadEq eq, Answer ans) {
		NumOfRoots n;
		return (eq.solve(n) == ans);
	}

	void checkLetters() {
		while (!letters.empty()) {
			bool student_in_result = false;
			for (int i = 0; i < results.size(); ++i)
			{
				if (results.at(i).name == letters.front().name)
				{
					student_in_result = true;
					results.at(i).successes += isCorrectAns(letters.front().eq, letters.front().ans);
				}
			}

			if (!student_in_result) {
				Result res;
				res.name = letters.front().name;
				res.successes = isCorrectAns(letters.front().eq, letters.front().ans);
				results.insert(results.end(), res);
			}
			letters.pop();
		}
	}

	void postResults() {
		for (int i = 0; i < results.size(); ++i)
		{
			cout << results.at(i).name << " " << results.at(i).successes << endl;
		}
	}
};

class Student {
	string _name;

public:
	Student() :_name("No name") {}
	Student(const string& name) : _name(name){}
	string getName()const{ return _name; }
	virtual Answer solveEq(QuadEq eq, NumOfRoots& n) {
		return eq.solve(n);
	}

	void sendToTeacher(const QuadEq& eq, Teacher& teacher) {
		NumOfRoots n;
		Answer ans = solveEq(eq, n);
		Letter letter = Letter(eq, ans, _name);
		teacher.letters.push(letter);
	}
};

class BadStudent : public Student {
public:
	BadStudent() : Student(){}
	BadStudent(const string& name) : Student(name){}
	virtual Answer solveEq(QuadEq eq, NumOfRoots& n)override {
		Answer ans;
		ans.n = NumOfRoots::One;
		ans.x1 = ans.x2 = 0;
		return ans;
	}
};

class GoodStudent : public Student {
public:
	GoodStudent() : Student() {}
	GoodStudent(const string& name) : Student(name) {}
	virtual Answer solveEq(QuadEq eq, NumOfRoots& n)override{
		return eq.solve(n);
	}
};

class AverageStudent : public Student {
public:
	AverageStudent() : Student() {}
	AverageStudent(const string& name) : Student(name) {}
	virtual Answer solveEq(QuadEq eq, NumOfRoots& n)override{
		if (rand() % 2) {
			return eq.solve(n);
		}
		else {
			Answer ans;
			ans.n = NumOfRoots::One;
			ans.x1 = ans.x2 = 0;
			return ans;
		}
	}
};

const int count_of_eqs = 100;
const int count_of_students = 10;

void generateEqs(const string filename) {
	ofstream file(filename);
	for (int i = 0; i < count_of_eqs; ++i)
	{
		file << rand() % 100 + 1 << " " << rand() % 100 + 1 << " " << rand() % 100 + 1 << std::endl;
	}
	file.close();
}

const string name[10] = { "Lera Rubanova", "Vanya Eroshkin", "Olya Embekova", "Roma Krasnikov", "Artyom Petroshenko", "Katya Zinyakova", "Pasha Popov", "Sasha Tronyagina", "Marina Otto", "Sasha Sorokin" };

vector<unique_ptr<Student>> generateStudents() {
	vector<unique_ptr<Student>> students;
	for (int i = 0; i < count_of_students; ++i) {
		int situation = rand() % 3;
		unique_ptr<Student> A;
		switch (situation) {
		case 0:
			A = move(make_unique<BadStudent>(name[i]));
			break;
		case 1:
			A = move(make_unique<GoodStudent>(name[i]));
			break;
		case 2:
			A = move(make_unique<AverageStudent>(name[i]));
			break;
		}
		students.push_back(move(A));
	}
	return students;
}

void studentsSolveEqs(const char* filename, vector < unique_ptr < Student>>& students, Teacher& teacher) {
	std::ifstream file(filename);
	for (int i = 0; i < count_of_eqs; ++i)
	{
		QuadEq eq;
		file >> eq._a >> eq._b >> eq._c;
		Answer ans;
		for (int i = 0; i < count_of_students; ++i){ students.at(i)->sendToTeacher(eq, teacher); }
	}
	file.close();

}

int main(void) {
	srand(time(0));

	generateEqs("Eqs.txt");
	vector<unique_ptr<Student>> students = generateStudents();

	Teacher tchr;

	studentsSolveEqs("Eqs.txt", students, tchr);

	tchr.checkLetters();

	tchr.postResults();
}