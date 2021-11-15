#include <iostream>
#include <chrono>
#include <map>
#include <set>
#include <vector>
#include <iomanip>
#include <ctime>
using namespace std;
typedef chrono::high_resolution_clock Clock;
int random(int a, int b)
{
	int r = rand();
	if (a > 0) {
		return a + r % (b - a);
	}
	else {
		return a + r % (abs(a) + b);
	}
}
int fact(int n) {
	if (n == 0) {
		return 1;
	}
	return n * fact(n - 1);
}
void swap(int& a, int& b) {
	int c = 0;
	c = a;
	a = b;
	b = c;
	return;
}
int getAns(int* line, vector<vector<int> >& mtx, int start, int n, int countsitys) {
	int ans = mtx[start][line[0]];
	int begin;
	if (countsitys == 0) {
		begin == 0;
	}
	else {
		begin = n - countsitys - 1;
	}
	for (int j = begin; j < n - 1; j++) {
		ans += mtx[line[j]][line[j + 1]];
	}
	ans += mtx[line[n - 1]][start];
	return ans;
}
void FindPath(int start, vector<vector<int> >& mtx, int* line, int n, int& ans, int* ansLine, bool log, int val = 0, int countsitys = 0) {
	if ((countsitys != 0 && val == countsitys) || val == n) {
		int tans = getAns(line, mtx, start, n, countsitys);
		if (ans == -1 || ans > tans) {
			ans = tans;
			for (int i = 0; i < n; i++) {
				ansLine[i] = line[i];
			}
		}
		if (log) {
			cout << start << " ";
			for (int i = 0; i < n; i++) {
				cout << line[i] << " ";
			}
			cout << start << " |" << tans << "| " << endl;
		}
		return;
	}
	bool unique;
	for (int i = 0; i < n + 1; i++) {
		unique = true;
		for (int j = 0; j < val; j++) {
			unique &= i != line[j];
		}
		if (unique && i != start) {
			line[val] = i;
			FindPath(start, mtx, line, n, ans, ansLine, log, val + 1);
		}
	}
}
void generateMtx(vector<vector<int> >& mtx, int n, int a, int b) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int r = random(a, b);
			mtx[i][j] = r;
		}
	}
}

map<int, set<pair<int, int> > > MapToMatrix(vector<vector<int> > mtx) {
	map<int, set<pair<int, int> > > m_map;

	for (int i = 0; i < mtx.size(); i++) {
		for (int j = 0; j < mtx.size(); j++) {
			if (i != j) {
				m_map[i].insert({ mtx[i][j], j });
			}
		}
	}
	return m_map;
}

int solutionSipmleHeu(int n, int start, map<int, set<pair<int, int> > >& m_map) {
	cout << endl << "SipmleHeu" << endl;
	vector<int> line;
	set<int> be;
	int  ans = 0;
	start--;
	line.push_back(start);
	be.insert(start);
	for (int i = 0; i < n; i++) {
		for (auto it = m_map[start].begin(); it != m_map[start].end(); it++) {
			if (be.find((*it).second) == be.end()) {
				line.push_back((*it).second);
				be.insert((*it).second);
				ans += ((*it).first);
				start = (*it).second;
				break;
			}
		}
	}
	for (auto item : m_map[line[line.size() - 1]]) {
		if (item.second == line[0]) {
			ans += item.first;
		}
	}
	for (auto item : line) {
		cout << item + 1 << "->";
	}
	cout << line[0] + 1 << " | ans = " << ans << endl;
	return ans;
}
bool exist(int* line, int count, int element) {
	for (int i = 0; i < count; i++)
	{
		if (line[i] == element) {
			return true;
		}
	}
	return false;
}

int solutionHeu(int n, int start, map<int, set<pair<int, int> > >& m_map, vector<vector<int> >& mtx) {
	int ans = -1;
	int* line = new int[n - 1];
	int* ansLine = new int[n - 1];
	int coutcities = 0;
	FindPath(start - 1, mtx, line, n - 1, ans, ansLine, true, 0, 3);
	coutcities += 3;
	for (auto it = m_map[start].begin(); it != m_map[start].end(); it++) {
		if (exist(line, coutcities, (*it).second) == false) {
			line[coutcities] = (*it).second;
			ans += ((*it).first);
			start = (*it).second;
			break;
		}
	}
	coutcities++;
	return 0;
}

int solutionPer(int n, int start, vector<vector<int> >& mtx, bool log) {
	cout << endl << "PER" << endl;
	int ans = -1;
	int* line = new int[n - 1];
	int* ansLine = new int[n - 1];

	if (log)
		cout << "\\log per\\" << endl;
	FindPath(start - 1, mtx, line, n - 1, ans, ansLine, true);
	if (log)
		cout << "\\end log per\\" << endl;

	cout << start << "->";
	for (int i = 0; i < n - 1; i++) {
		cout << ansLine[i] + 1 << "->";
	}
	cout << start << " | ans = " << ans << endl;
	return ans;
}
void StartTests(int n, int* sizes, int p) {
	vector<vector<double> > timePer(n, vector<double>(p, 0)); // таблица с временем выполнения перебора
	vector<vector<double> > timeSipmleHeu(n, vector<double>(p, 0)); // таблица с временем выполнения эвристического алгоритма
	vector<vector<int> > ansPer(n, vector<int>(p, 0)); // таблица с ответами перебора
	vector<vector<int> > ansSipmleHeu(n, vector<int>(p, 0)); // таблица с ответами простейшего алгоритма


	for (int i = 0; i < n; i++) {
		vector<vector<int> > mtx(sizes[i], vector<int>(sizes[i])); // матрица смежности
		map<int, set<pair<int, int> > > map_mtx; // матрица смежности в map-е
		for (int pi = 0; pi < p; pi++) {
			// генерация матрицы смежности
			generateMtx(mtx, sizes[i], 1, 30);
			// перевод в map
			map_mtx = MapToMatrix(mtx);
			// определение стартовой точки
			int startCity = random(1, sizes[i]);
#pragma region PER
			auto t1 = Clock::now(); //фиксация времени перед началом выполнения алгоритма
			ansPer[i][pi] = solutionPer(sizes[i], startCity, mtx, true);
			auto t2 = Clock::now(); //фиксация времени после выполнения
			//конец перебора
#pragma endregion
			// подсчет времени выполнения
			double result = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000;
			// запись результатов в таблицу
			timePer[i][pi] = result;
#pragma region SipmleHeu
			t1 = Clock::now();
			ansSipmleHeu[i][pi] = solutionSipmleHeu(sizes[i], startCity, map_mtx);
			t2 = Clock::now();
#pragma endregion
			// посчет времени выполнения
			result = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000;
			// запись результатов выполнения
			timeSipmleHeu[i][pi] = result;
		}
	}
	cout << setprecision(1) << fixed;
	cout << endl << endl << endl << "----Tests----" << endl;
	for (int i = 0; i < n; i++) {
		double sumTimePer = 0;
		double sumTimeSipmleHeu = 0;
		int countOkAns = 0;
		double maxTimePer = -1;
		double maxTimeSipmleHeu = -1;
		double minTimePer = -1;
		double minTimeSipmleHeu = -1;
		const int indent = 20;
		cout << endl << endl << "Test (" << sizes[i] << " cities):" << endl << endl;
		cout << setw(indent) << "Per" << setw(indent) << "Time" << setw(indent) << "SipmleHeu" << setw(indent) << "Time" << setw(indent) << "Result" << endl;
		for (int j = 0; j < p; j++) {
			cout << setw(indent) << ansPer[i][j] << setw(indent) << timePer[i][j] << setw(indent) << ansSipmleHeu[i][j] << setw(indent) << timeSipmleHeu[i][j];
			if (ansPer[i][j] == ansSipmleHeu[i][j]) {
				cout << setw(indent + 11) << "[\x1b[32;1mOK\x1b[0m]";
				countOkAns++;
			}
			else {
				cout << setw(indent + 11) << "[\x1b[31;1mWA\x1b[0m]";
			}
			cout << endl;
		}
		for (int j = 0; j < p; j++) {
			sumTimePer += timePer[i][j];
			if (maxTimePer == -1) {
				maxTimePer = timePer[i][j];
			}
			if (maxTimePer == -1 || maxTimePer < timePer[i][j]) {
				maxTimePer = timePer[i][j];
			}
			if (minTimePer == -1 || minTimePer > timePer[i][j]) {
				minTimePer = timePer[i][j];
			}
			sumTimeSipmleHeu += timeSipmleHeu[i][j];
			if (maxTimeSipmleHeu == -1 || maxTimeSipmleHeu < timeSipmleHeu[i][j]) {
				maxTimeSipmleHeu = timeSipmleHeu[i][j];
			}
			if (minTimeSipmleHeu == -1 || minTimeSipmleHeu > timeSipmleHeu[i][j]) {
				minTimeSipmleHeu = timeSipmleHeu[i][j];
			}
		}
		cout << endl << endl << "----result----" << endl;
		cout << "OKs : " << countOkAns << endl;
		cout << "MidTimePer: " << (double)sumTimePer / p << endl;
		cout << "MidTimeSipmleHeu: " << (double)sumTimeSipmleHeu / p << endl;
		if (countOkAns > 0)
			cout << "% Ok: " << (double)countOkAns / (double)p * 100 << "%" << endl;
		else
			cout << "% Ok: 0%" << endl;
		cout << "maxTimePer:" << maxTimePer << endl;
		cout << "maxTimeSipmleHeu:" << maxTimeSipmleHeu << endl;
		cout << "minTimePer:" << minTimePer << endl;
		cout << "minTimeSipmleHeu:" << minTimeSipmleHeu;
	}

}
int main() {

	srand(time(NULL));
	int countSizes, countRepeat;
	cin >> countSizes >> countRepeat;
	int* sizes = new int[countSizes];
	for (int i = 0; i < countSizes; i++) {
		cin >> sizes[i];
	}
	StartTests(countSizes, sizes, countRepeat);
	system("pause");
}
