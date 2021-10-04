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
int getAns(int* line, vector<vector<int> >& mtx, int start, int n) {
	int ans = mtx[start][line[0]];
	for (int j = 0; j < n - 1; j++) {
		ans += mtx[line[j]][line[j + 1]];
	}
	ans += mtx[line[n - 1]][start];
	return ans;
}
void FindPath(int start, vector<vector<int> >& mtx, int* line, int n, int& ans, int* ansLine, int val = 0) {
	if (val == n) {
		int tans = getAns(line, mtx, start, n);
		if (ans == -1 || ans > tans) {
			ans = tans;
			for (int i = 0; i < n; i++) {
				ansLine[i] = line[i];
			}
		}
		cout << start << " ";
		for (int i = 0; i < n; i++) {
			cout << line[i] << " ";
		}
		cout << start << " |" << tans << "| " << endl;
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
			FindPath(start, mtx, line, n, ans, ansLine, val + 1);
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
int solutionHeu(int n, int start, vector<vector<int> >& mtx) {
	cout << endl << "HEU" << endl;
	map<int, set<pair<int, int> > > m_map;
	vector<int> line;
	set<int> be;
	int  ans = 0;
	start--;
	line.push_back(start);
	be.insert(start);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i != j) {
				m_map[i].insert({ mtx[i][j], j });
			}
		}
	}
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
int solutionPer(int n, int start, vector<vector<int> >& mtx) {
	cout << endl << "PER" << endl;
	int ans = -1;
	int* line = new int[n - 1];
	int* ansLine = new int[n - 1];

	cout << "\\log per\\" << endl;
	FindPath(start - 1, mtx, line, n - 1, ans, ansLine);
	cout << "\\end log per\\" << endl;

	cout << start << "->";
	for (int i = 0; i < n - 1; i++) {
		cout << ansLine[i] + 1 << "->";
	}
	cout << start << " | ans = " << ans << endl;
	return ans;
}
void StartTests(int n, int* sizes, int p) {
	vector<vector<double> > timePer(n, vector<double>(p, 0));
	vector<vector<double> > timeHeu(n, vector<double>(p, 0));
	vector<vector<int> > ansPer(n, vector<int>(p, 0));
	vector<vector<int> > ansHeu(n, vector<int>(p, 0));


	for (int i = 0; i < n; i++) {
		vector<vector<int> > mtx(sizes[i], vector<int>(sizes[i]));
		for (int pi = 0; pi < p; pi++) {
			generateMtx(mtx, sizes[i], 1, 30);
			int startCity = random(1, sizes[i]);
			auto t1 = Clock::now();
			ansPer[i][pi] = solutionPer(sizes[i], startCity, mtx);
			auto t2 = Clock::now();
			double result = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000;
			timePer[i][pi] = result;
			t1 = Clock::now();
			ansHeu[i][pi] = solutionHeu(sizes[i], startCity, mtx);
			t2 = Clock::now();
			result = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000;
			timeHeu[i][pi] = result;
		}
	}
	cout << setprecision(1) << fixed;
	cout << endl << endl << endl << "----Tests----" << endl;
	for (int i = 0; i < n; i++) {
		double sumTimePer = 0;
		double sumTimeHeu = 0;
		int countOkAns = 0;
		double maxTimePer = -1;
		double maxTimeHeu = -1;
		double minTimePer = -1;
		double minTimeHeu = -1;
		const int indent = 20;
		cout << endl << endl << "Test (" << sizes[i] << " cities):" << endl << endl;
		cout << setw(indent) << "Per" << setw(indent) << "Time" << setw(indent) << "Heu" << setw(indent) << "Time" << setw(indent) << "Result" << endl;
		for (int j = 0; j < p; j++) {
			cout << setw(indent) << ansPer[i][j] << setw(indent) << timePer[i][j] << setw(indent) << ansHeu[i][j] << setw(indent) << timeHeu[i][j];
			if (ansPer[i][j] == ansHeu[i][j]) {
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
			sumTimeHeu += timeHeu[i][j];
			if (maxTimeHeu == -1 || maxTimeHeu < timeHeu[i][j]) {
				maxTimeHeu = timeHeu[i][j];
			}
			if (minTimeHeu == -1 || minTimeHeu > timeHeu[i][j]) {
				minTimeHeu = timeHeu[i][j];
			}
		}
		cout << endl << endl << "----result----" << endl;
		cout << "OKs : " << countOkAns << endl;
		cout << "MidTimePer: " << (double)sumTimePer / p << endl;
		cout << "MidTimeHeu: " << (double)sumTimeHeu / p << endl;
		if (countOkAns > 0)
			cout << "% Ok: " << (double)countOkAns / (double)p * 100 << "%" << endl;
		else
			cout << "% Ok: 0%" << endl;
		cout << "maxTimePer:" << maxTimePer << endl;
		cout << "maxTimeHeu:" << maxTimeHeu << endl;
		cout << "minTimePer:" << minTimePer << endl;
		cout << "minTimeHeu:" << minTimeHeu;
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
