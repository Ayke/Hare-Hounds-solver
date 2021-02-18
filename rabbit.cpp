#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>

#include <string>
#include <array>
#include <algorithm>
#include <vector>

#define DOGWIN 1
#define RABWIN -1

using namespace std;

struct pos
{
	int rabbit;
	array<int,3> dog; //Make sure dog[] is sorted.
	bool dogturn; //It's time for dogs to move
};

const int N = 11;
const int N4 = N*N*N*N;
const int M = N4*2;
bool eR[N][N] = {0};             // map for rabbit
bool eD[N][N] = {0};             // map for dog

bool vis[M] = {false};     // 0 for unvisited, -1 for in calculation, 
bool illegal[M] = {false};
int key[M] = {0};          // if f[x] is not 0, key[x] is the next state for x to win the game
int f[M] = {0};
// -1 兔子必胜, 1 猎犬必胜


int encode(pos state)
{
	int answer = 0;
	answer += state.dog[0];
	answer *= N;
	answer += state.dog[1];
	answer *= N;
	answer += state.dog[2];
	answer *= N;
	answer += state.rabbit;
	if (state.dogturn) answer += N4;
	return answer;
}

pos decode(int code)
{
	pos answer;
	if (code >= N4) answer.dogturn = true;
	else answer.dogturn = false;
	code %= N4;
	answer.rabbit = code % N;
	code /= N;
	answer.dog[2] = code % N;
	code /= N;
	answer.dog[1] = code % N;
	code /= N;
	answer.dog[0] = code % N;
	return answer;    
}


void printPos(pos state, bool newline = true)
{
	cout << state.rabbit << " " << state.dog[0] << " " << state.dog[1] << " " << state.dog[2];
	if (state.dogturn) cout << " dog's turn";
	else cout << " rabbit's turn";
	if (newline) cout << endl;
}

bool legal(pos state)
{
	if (state.rabbit >= N) return false;
	if (state.dog[0] >= N) return false;
	if (state.dog[1] >= N) return false;
	if (state.dog[2] >= N) return false;
	if (!(state.dog[0] < state.dog[1] && state.dog[0] < state.dog[2] && state.dog[1] < state.dog[2])) return false;
	if (state.rabbit == state.dog[0] || state.rabbit == state.dog[1] || state.rabbit == state.dog[2]) return false;
	return true;
}

void save(string filename)
{
	ofstream ofs;
	ofs.open(filename);
	ofs << f[0];
	for(int i = 1; i < M; i++) ofs << " " << f[i];
	ofs << endl;
	ofs.close();
}


vector<pos> genNextStates(pos state)
{
	// cout << "Generating new states..." << endl;
	vector<pos> answer;
	pos newState = state;
	newState.dogturn = !state.dogturn;
	if (state.dogturn) {
		//Dog's move
		for(int i = 0; i < N; i++) {
			if (state.rabbit == i || state.dog[0] == i || state.dog[1] == i || state.dog[2] == i) continue;
			for(int k = 0; k < 3; k++) {
				if (eD[state.dog[k]][i]) {
					newState.dog[k] = i;
					sort(newState.dog.begin(), newState.dog.end());
					answer.push_back(newState);
					newState.dog = state.dog;
				}
			}
		}
	} else {
		//Rabbit's move
		for(int i = N-1; i >= 0; i--) {
			if (state.rabbit == i || state.dog[0] == i || state.dog[1] == i || state.dog[2] == i) continue;
			if (eR[state.rabbit][i]) {
				newState.rabbit = i;
				answer.push_back(newState);
				newState.rabbit = state.rabbit;
			}
		}
	}
	return answer;
}


vector<pos> genPrevStates(pos state)
{
	// cout << "Generating previous states..." << endl;
	vector<pos> answer;
	pos newState = state;
	newState.dogturn = !state.dogturn;
	if (newState.dogturn) {
		//Dog's move
		for(int i = N-1; i > 0; i--) {
			if (state.rabbit == i || state.dog[0] == i || state.dog[1] == i || state.dog[2] == i) continue;
			for(int k = 0; k < 3; k++) {
				if (eD[i][state.dog[k]]) {
					newState.dog[k] = i;
					sort(newState.dog.begin(), newState.dog.end());
					answer.push_back(newState);
					newState.dog = state.dog;
				}
			}
		}
	} else {
		//Rabbit's move
		for(int i = 0; i < N; i++) {
			if (state.rabbit == i || state.dog[0] == i || state.dog[1] == i || state.dog[2] == i) continue;
			if (eR[i][state.rabbit]) {
				newState.rabbit = i;
				answer.push_back(newState);
				newState.rabbit = state.rabbit;
			}
		}
	}
	return answer;
}


void init(void)
{
	int x;
	x = 0; vector<int> gg{1,4,7};           for(int y : gg) eR[x][y] = true;
	x = 1;  gg.assign({0,2,4,5});           for(int y : gg) eR[x][y] = true;
	x = 2;  gg.assign({1,3,5});             for(int y : gg) eR[x][y] = true;
	x = 3;  gg.assign({2,5,6,10});          for(int y : gg) eR[x][y] = true;
	x = 4;  gg.assign({0,1,5,7});           for(int y : gg) eR[x][y] = true;
	x = 5;  gg.assign({1,2,3,4,6,7,8,9});   for(int y : gg) eR[x][y] = true;
	x = 6;  gg.assign({3,5,9,10});          for(int y : gg) eR[x][y] = true;
	x = 7;  gg.assign({0,4,5,8});           for(int y : gg) eR[x][y] = true;
	x = 8;  gg.assign({5,7,9});             for(int y : gg) eR[x][y] = true;
	x = 9;  gg.assign({5,6,8,10});          for(int y : gg) eR[x][y] = true;
	x = 10; gg.assign({3,6,9});             for(int y : gg) eR[x][y] = true;

	x = 0;
	x = 1;  gg.assign({0,4});			for(int y : gg) eD[x][y] = true;
	x = 2;  gg.assign({1,5});			for(int y : gg) eD[x][y] = true;
	x = 3;  gg.assign({2,5,6});			for(int y : gg) eD[x][y] = true;
	x = 4;  gg.assign({0,1,7});			for(int y : gg) eD[x][y] = true;
	x = 5;  gg.assign({1,2,4,7,8});		for(int y : gg) eD[x][y] = true;
	x = 6;  gg.assign({3,5,9});			for(int y : gg) eD[x][y] = true;
	x = 7;  gg.assign({0,4});			for(int y : gg) eD[x][y] = true;
	x = 8;  gg.assign({5,7});			for(int y : gg) eD[x][y] = true;
	x = 9;  gg.assign({5,6,8});			for(int y : gg) eD[x][y] = true;
	x = 10; gg.assign({3,6,9});			for(int y : gg) eD[x][y] = true;
	cout << "eR:" << endl;
	for(int i = 0; i < N; i++) {
	    for(int j = 0; j < N; j++) {
	        cout << eR[i][j] << " ";
	    }
	    cout << endl;
	}
	cout << endl << "eD:" << endl;
	for(int i = 0; i < N; i++) {
	    for(int j = 0; j < N; j++) {
	        cout << eD[i][j] << " ";
	    }
	    cout << endl;
	}

	pos start;
	start.rabbit = 0; start.dog[0] = 3; start.dog[1] = 9; start.dog[2] = 10; start.dogturn = true;
	for(int i = 0; i < M; i++) {
		pos state = decode(i);
		if (!legal(state) || genPrevStates(state).size() == 0) illegal[i] = true;
	}


	
	for(int code = 0; code < M; code++) {
		pos state = decode(code);
		if (illegal[code]) continue;
		if (state.rabbit == 10 || state.dog[0] == 0) {
			f[code] = RABWIN;
			continue;
		}
		if (state.dog[2] < 10) {
			int foorabbit = state.rabbit;
			while (foorabbit > 3) foorabbit -= 3;
			int foodog0 = state.dog[0], foodog1 = state.dog[1], foodog2 = state.dog[2];
			while (foodog0 > 3) foodog0 -= 3;
			while (foodog1 > 3) foodog1 -= 3;
			while (foodog2 > 3) foodog2 -= 3;
			if (foorabbit >= foodog0 && foorabbit >= foodog1 && foorabbit >= foodog2) {
				f[code] = RABWIN;
				continue;
			}
		}
		if (!state.dogturn) {
			// It's turn for the rabbit and it has no where to go
			bool dogwinning = true;
			for(int i = 0; i < N; i++) if (eR[state.rabbit][i] && (state.dog[0] != i) && (state.dog[1] != i) && (state.dog[2] != i)) dogwinning = false;
			if (dogwinning) {
				f[code] = DOGWIN;
				continue;
			}
		}
	}

	// cout << "where the fuck is init.txt" << endl;
	save("init.txt");
}


void search(pos state)
{
	int code = encode(state);
	if (f[code] != 0) return;
	cout << "Searching... ";
	printPos(state);
	vector<pos> nextStates = genNextStates(state);
	assert (nextStates.size() > 0);
	bool dogwinning = true, rabwinning = true;
	for(pos nextState : nextStates) {
		cout << "The next state is: ";
		printPos(nextState);
		int nextCode = encode(nextState);
		if (vis[nextCode]) continue;
		vis[nextCode] = true;
		search(nextState);
		vis[nextCode] = false;
		if (f[nextCode] != DOGWIN) dogwinning = false;
		if (f[nextCode] != RABWIN) rabwinning = false;
		if (state.dogturn && f[nextCode] == DOGWIN) {
			f[code] = DOGWIN;
			key[code] = nextCode;
			return;
		}
		if (!state.dogturn && f[nextCode] == RABWIN) {
			f[code] = RABWIN;
			key[code] = nextCode;
			return;
		}
	}
	if (dogwinning) {
		f[code] = DOGWIN;
		key[code] = -1;
	}
	if (rabwinning) {
		f[code] = RABWIN;
		key[code] = -1;
	}
	return;	
}

void getAns(int code, int level)
{
	string winner;
	if (f[code] == DOGWIN) winner = "dog win";
	else {
		if (f[code] == RABWIN) winner = "rabbit win";
		else winner = "unknown";
	}
	for(int i = 0; i < level; i++) cout << "*";
	pos state = decode(code);
	printPos(state, false);
	cout << "  " << winner << endl;
	if (key[code] != 0) {
		// if (level > 8) return;
		if (key[code] > 0) getAns(key[code], level+1);
		else {
			// cout << "All following nodes end up the same." << endl;
			vector<pos> nextStates = genNextStates(state);
			pos nextState = nextStates[0];
			if (state.dogturn) {
				int nextCode = encode(nextState);
				getAns(nextCode,level+1);
			} else {
				for(pos nextState : nextStates) {
					int nextCode = encode(nextState);
					getAns(nextCode, level+1);
				}
			}
		}
	} 
	return;
}

void work()
{
	pos goal;
	goal.rabbit = 0; goal.dog[0] = 3; goal.dog[1] = 9; goal.dog[2] = 10; goal.dogturn = true;
	// cout << "After encode:" << goalx << endl;
	// pos goal2 = decode(goalx);
	// cout << "After decode:";
	// printPos(goal2);

	// cout << "Get to work" << endl;
	vis[encode(goal)] = true;
	search(goal);
	
	// printPos(goal);
	// cout << "Its following states:" << endl;
	// vector<pos> gg = genNextStates(goal);
	// for(pos state : gg) {
	// 	printPos(state);
	// }
}



void work2()
{
	int changed;
	do {
		changed = 0;
		for(int code = 0; code < M; code++) {
			pos state = decode(code);
			if (illegal[code]) continue;
			if (f[code] == RABWIN || f[code] == DOGWIN) {
				// It's a dog node or a rabbit node
				if (f[code] == DOGWIN && !state.dogturn) {
					// DOG went to the right track! Where could it come from?
					vector<pos> posList = genPrevStates(state);
					for(pos prevState : posList) {
						int prevCode = encode(prevState);
						if (f[prevCode] == 0) {
							f[prevCode] = DOGWIN;
							key[prevCode] = code;
							changed++;
						}
						assert (f[prevCode] != RABWIN);
					}
				}
				if (f[code] == RABWIN && state.dogturn) {
					// RABBIT went to the right track! Where could it come from?
					vector<pos> posList = genPrevStates(state);
					for(pos prevState : posList) {
						int prevCode = encode(prevState);
						if (f[prevCode] == 0) {
							f[prevCode] = RABWIN;
							key[prevCode] = code;
							changed++;
						}
						assert (f[prevCode] != DOGWIN);
					}
				}
			} else {
				// check if all its succesors are dog/rab node
				vector<pos> nextStates = genNextStates(state);
				assert (nextStates.size() > 0);
				bool dogwinning = true, rabwinning = true;
				for(pos nextState : nextStates) {
					int nextCode = encode(nextState);
					if (f[nextCode] != DOGWIN) dogwinning = false;
					if (f[nextCode] != RABWIN) rabwinning = false;
				}
				if (dogwinning) {
					f[code] = DOGWIN;
					key[code] = -1;
					changed++;
				}
				if (rabwinning) {
					f[code] = RABWIN;
					key[code] = -1;
					changed++;
				}
			}
		}
		cout << "Updated " << changed << " states" << endl;
	} while (changed > 0);
}

void show()
{
	pos goal;
	goal.rabbit = 0; goal.dog[0] = 3; goal.dog[1] = 9; goal.dog[2] = 10; goal.dogturn = true;
	cout << "Final answer:" << endl;
	cout << f[encode(goal)] << endl;
	getAns(encode(goal), 0);

	pos state;
	state.rabbit = 8; state.dog[0] = 6; state.dog[1] = 9; state.dog[2] = 10; state.dogturn = false;
	// getAns(encode(state), 0);
}


int main()
{
	init();
	work2();
	// work();
	show();
	save("final.txt");
}