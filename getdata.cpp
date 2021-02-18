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
int key[M] = {0};          // if f[x] is 1, key[x] is the next state for x to win the game
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

void printPos(pos state)
{
	cout << state.rabbit << " " << state.dog[0] << " " << state.dog[1] << " " << state.dog[2];
	if (state.dogturn) cout << " dog's turn";
	else cout << " rabbit's turn";
	cout << endl;
}


void load()
{
	ifstream ifs;
	// ifs.open("final.txt");
    ifs.open("init.txt");
	for(int i = 0; i < M; i++) ifs >> f[i];
}

int main()
{
    load();
    for(int i = 0; i < M; i++) if (f[i] != 0) {
        // if (f[i] == RABWIN) continue;
        pos state = decode(i);
        // cout << "dogturn:" << state.dogturn << endl;
        // cout << "!state.dogturn:" << !state.dogturn << endl;
        printPos(decode(i));
        string winner = (f[i]==DOGWIN) ? ("dog win") : ("rabbit win");
        cout << winner << endl;
    }
}
