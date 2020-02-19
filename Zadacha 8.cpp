#include <iostream> 
#include <algorithm> 
#include <vector>
#include <stack>
#include <math.h>
#include<iomanip>
using namespace std;

struct vect {
	double x, y;
};

//зададим разность двух векторов
vect subtraction(vect a, vect b) {
	vect c;
	c.x = b.x - a.x;
	c.y = b.y - a.y;
	return c;
}

//зададим векторное произведение двух векторов на плоскости
double vector_mupltipl(vect a, vect b) {
	return a.x * b.y - a.y * b.x;
}

//зададим скалярное произведение на плоскости двух векторов
double scalar_miltipl(vect a, vect b) {
	return (a.x * b.x + a.y * b.y);
}

//длина вектора
double vector_len(vect a) {
	return sqrt(scalar_miltipl(a, a));
}

//оператор сравнения. включает случаи вравнения с вектором - центром О(0, 0), пограничный случай когда y векторов 0. В таком случае мы в оболочку добавим самую дальнюю от O точку.
struct InOperator {
	bool operator()(vect l, vect r) const {
		if (vector_len(l) == 0) {
			return false;
		}
		else if (vector_len(r) == 0) {
			return true;
		}
		else if (abs(l.y) + abs(r.y) == 0) {
			return l.x > r.x;
		}
		else {
			return vector_mupltipl(l, r) > 0;
		}
	}
};


int main() {
	int n;
	cin >> n;
	//вектор-точка самая нижняя (если такова не одна, самая левая). Дальше от каждой точки вычтем координаты точки О (сместим начало координат в выбранную точку О)
	vect O;
	O.x = 0;
	O.y = INFINITY;
	vector<vect> points;
	for (int i = 0; i < n; i++) {
		vect a;
		cin >> a.x >> a.y;
		points.push_back(a);
		if ((O.y > a.y) or (O.y == a.y and O.x > a.x)) {
			O = a;
		}
	}
	for (int i = 0; i < n; i++) {
		points[i] = subtraction(O, points[i]);
	}
	//сортируем точки по углу, при чем точка О улетает в самый конец
	sort(points.begin(), points.end(), InOperator());
	//разбиваю ответ на три варианта - если точек 0 или 1, тогда длина оболочки = 0, если 2 точки - тогда длина оболочки это туда-назад = 2 * длина расстояния между точками, и если более 2 точек, тогда через стэк определяем точки в оболочку, и посчитаем суммарную длину
	if (n <= 1) {
		cout << 0 << endl;
	}
	else if (n == 2) {
		cout << 2 * vector_len(points[0]) << endl;
	}
	else {
		stack<vect> geom_shell;
		geom_shell.push(points[n - 1]);
		geom_shell.push(points[0]);
		geom_shell.push(points[1]);
		int j = 2;
		while (j < n) {
			vect top, before_top;
			top = geom_shell.top();
			geom_shell.pop();
			before_top = geom_shell.top();
			if (vector_mupltipl(subtraction(points[j], top), subtraction(before_top, top)) > 0) {
				geom_shell.push(top);
				geom_shell.push(points[j]);
				j++;
			}
			else if (top.y == 0) {
				geom_shell.push(points[j]);
				j++;
			}
		}
		int count_points_shell = geom_shell.size();
		double len_shell = 0.0;
		for (int i = 0; i < count_points_shell - 1; i++) {
			vect top, before_top;
			top = geom_shell.top();
			geom_shell.pop();
			before_top = geom_shell.top();
			len_shell += vector_len(subtraction(before_top, top));
		}
		cout << setprecision(9);
		cout << len_shell << endl;
	}
}