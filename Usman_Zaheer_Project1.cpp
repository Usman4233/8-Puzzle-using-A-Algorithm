
/*
PUROJECT: *8 PUZZLE PROBLEM*
SUBMITTED TO: *Dr. Yasar Ayaz
SUBMITTED By: *Usman Zaheer

PROBLEM STATEMENT: Write a program (using C / C++) that can solve the 8 Puzzle problem from any initial
configuration input by the user using at least two different heuristics.
*/

/* CODE START*/

#include <iostream>
#include <list>
#include <algorithm>
#include <ctime>
using namespace std;

const int n = 3;
bool random_start = false, generate_start = true; // State Initializer conditions.

// State Class Definition.
class State {
public:
	int A[n][n] = {{9, 9, 9}, {9, 9, 9}, {9, 9, 9}}, g, h, t;
	State *parent; //Parent Pointer.
	State();
	void reset();
	bool is_goal();

	bool operator==(const State &) const;
	bool operator<(const State &) const;
	void print();
	void Heurs();
};
int Goal[n][n] = {{9, 9, 9}, {9, 9, 9}, {9, 9, 9}};
int space = 1, runtime = 1, timer = 0; // Stored States, Runtime, and initial timer Counters
list< State > closed, fringe; // Tested States, Active States Lists.
State start, cur, temp; // Start, Current, Temporary States.
void Astar();
void Expand(); // State Expander function.
void PrintPath(State *s);
bool InClosed(State &s); // to search for state in the Closed List.
bool alreadyExists(int inputVariable, int Goal[n][n]);

int main() {
	cout << "Enter Goal State values from 0 to 8:\n";
	int inputVariable;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            bool already = false;
            do {
                cout << "Enter Value At Position "<< i + 1 << " "<< j + 1 << ": " ;
                cin >> inputVariable;
                already = alreadyExists(inputVariable, Goal);
                if (already) {
                    cout << "Number already entered!! Please enter new Number" << endl;
                }
            } while(already);
            Goal[i][j] = inputVariable;
        }
    }

	start.g = 0;
	start.Heurs();
	start.t = start.g + start.h;
	start.parent = NULL;
	timer = (clock() * 1000) / CLOCKS_PER_SEC;
    Astar();
	getchar();
	return 0;
}

State::State() {
	reset();
}

void State::reset() {
	int i, j, k;
	g = h = t = 0;
	parent = NULL;

	int inputVariable;
	if (generate_start) {
		if (!random_start) {
			cout << "Enter Initial State values from 0 to 8:" << endl;
			    for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        bool already = false;
                        do {
                            cout << "Enter Value At Position "<< i + 1 << " "<< j + 1 << ": " ;
                            cin >> inputVariable;
                            already = alreadyExists(inputVariable, A);
                            if (already) {
                                cout << "Number already entered!! Please enter new Number" << endl;
                            }
                        } while(already);
                        A[i][j] = inputVariable;
                    }
                }

		}
		else {
			list< int > l;
			list< int >::iterator it;
			srand(time(0));
			for (i = 0; i < n * n; i++)
				l.push_back(i);
			for (i = 0; i < n; i++) {
				for (j = 0; j < n; j++) {
					it = l.begin();
					for (k = rand() % l.size(); k > 0; k--)
						it++;
					A[i][j] = (*it);
					l.erase(it);
				}
			}
		}
		generate_start = false;
	}
}

bool State::is_goal() {
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (A[i][j] != Goal[i][j])
				return false;
		}
	}

	return true;
}

void State::print() {
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			cout << A[i][j] << ' ';
		cout << endl;
	}
	cout << endl;
}

bool State::operator==(const State &r) const {
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (A[i][j] != r.A[i][j])
				return false;
		}
	}
	return true;
}
//
bool State::operator<(const State &r) const {

    return t < r.t;

}

void State::Heurs() {
	int i, i2, j, j2, Heuristic = 0;
	bool found;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			found = false;
			for (i2 = 0; i2 < n; i2++) {
				for (j2 = 0; j2 < n; j2++) {
					if (Goal[i][j] == A[i2][j2]) {
						Heuristic += abs(i - i2) + abs(j - j2);
						found = true;
					}
					if (found)
						break;
				}
				if (found)
					break;
			}
		}
	}

	h = Heuristic;
}

void Astar() {
	cout << "Executing Algorithm \n";
	cur = start;
	fringe.push_front(cur);
	while (true) {
		cur = fringe.front();
		for (list<State>::iterator it = fringe.begin(); it != fringe.end(); ++it) {
			if ((*it) < cur ) {
				cur = (*it);
			}
		}

		if (cur.is_goal()) {
			runtime = ((clock() * 1000) / CLOCKS_PER_SEC) - timer;
			cout << "Time= " << runtime << "\n";
			cout << "Space= " << space << "\n";
			cout << "Cost= " << cur.g << "\n";
			cout << "Path:\n";
			PrintPath(&cur);
			return;
		}
		else {
			Expand();
		}
	}
}

void Expand() {

	closed.push_back(cur);
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (cur.A[i][j] == 0) {
				if (i > 0) {
					temp = cur;
					temp.parent = &(closed.back());
					swap(temp.A[i][j], temp.A[i - 1][j]);
					if (!InClosed(temp)) {
						temp.g += 1;
						temp.Heurs();
						temp.t = temp.g + temp.h;
						fringe.push_front(temp);
						space++;
					}
				}

				if (i < n-1) {
					temp = cur;
					temp.parent = &(closed.back());
					swap(temp.A[i][j], temp.A[i + 1][j]);
					if (!InClosed(temp)) {
						temp.g += 1;
						temp.Heurs();
						temp.t = temp.g + temp.h;
						fringe.push_front(temp);
						space++;
					}
				}
				if (j > 0) {
					temp = cur;
					temp.parent = &(closed.back());
					swap(temp.A[i][j], temp.A[i][j - 1]);
					if (!InClosed(temp)) {
						temp.g += 1;
						temp.Heurs();
						temp.t = temp.g + temp.h;
						fringe.push_front(temp);
						space++;
					}
				}
				if (j < n-1) {
					temp = cur;
					temp.parent = &(closed.back());
					swap(temp.A[i][j], temp.A[i][j + 1]);
					if (!InClosed(temp)) {
						temp.g += 1;
						temp.Heurs();
						temp.t = temp.g + temp.h;
						fringe.push_front(temp);
						space++;
					}
				}
			}
		}
	}
	fringe.remove(cur);
}

void PrintPath(State *s)
{

	if (s != NULL) {
		(*s).print();
		PrintPath((*s).parent);
	}
}

bool InClosed(State &s)
{
	for (list<State>::iterator it = closed.begin(); it != closed.end(); ++it) {
		if ((*it) == s) {
			return true;
		}
	}
	return false;
}

bool alreadyExists(int x, int Array[n][n])
{
    bool found = false;
    for (int i = 0; i < n; i++) {
     for (int j = 0; j < n; j++) {
            if ( x == Array[i][j])
                found = true;
		}
    }

	return found;
}
