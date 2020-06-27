#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <math.h>
#include <cstring>
using namespace std;

struct Point {
    int x, y;
	Point() : Point(0, 0) {}
	Point(float x, float y) : x(x), y(y) {}
	bool operator==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Point& rhs) const {
		return !operator==(rhs);
	}
	Point operator+(const Point& rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	Point operator-(const Point& rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
};

int player;
const int SIZE = 8;

//std::array<std::array<int, SIZE>, SIZE> board;
int board[8][8];
int valid_point[64][2];
int state[8][8];
std::vector<Point> next_valid_spots;
int ans_x, ans_y;
const int weight[8][8] =
{
     65,  -3, 6, 4, 4, 6, -3, 65,
     -3, -29, 3, 1, 1, 3, -29, -3,
     6, 3, 5, 3, 3, 5, 3, 6,
     4, 1, 3, 1, 1, 3, 1, 4,
     4, 1, 3, 1, 1, 3, 1, 4,
     6, 3, 5, 3, 3, 5, 3, 6,
     -3, -29, 3, 1, 1, 3, -29, -3,
     65, -3, 6, 4, 4, 6,  -3, 65,
};
const int d[8][2] =
{
    -1, 0, 1, 0, 0, -1, 0, 1,
    -1, -1, -1, 1, 1, -1, 1, 1
};
int getWeight(){
    int value;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            value += weight[i][j]*state[i][j];
        }
    }
    return value;
}
int alpha_beta(int depth, int alpha, int beta, bool maximizingPlayer){
     int value;
    if(depth > 4){//if depth = 0 || node is terminal node
       value = getWeight();
    }
    int temp[10][10];
    memcpy(temp, state, sizeof state);
    if(maximizingPlayer){
        value = alpha;
        for(int i=0 ;i< next_valid_spots.size(); i++){
            int val = alpha_beta(depth+1, alpha, beta, false);
            if(val > alpha){
                value = val;
                alpha = val;
            }
            memcpy(state, temp, sizeof temp);
            if(alpha >= beta)break;
        }
        return value;
    }
    else{
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                if(state[i][j] == 0){




                }

            }
        }


    }
    return value;
}
//
void read_board() {
    cin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cin >> board[i][j];
        }
    }
}

void read_valid_spots() {
    int n_valid_spots;
    cin >> n_valid_spots;
    int x, y;
    for (int i = 0; i < n_valid_spots; i++) {
        cin >> x >> y;
        valid_point[i][0]=x;
        valid_point[i][0]=y;
        next_valid_spots.push_back({x, y});
    }
}

void write_valid_spot() {
    int n_valid_spots = next_valid_spots.size();
    srand(time(NULL));
    int index = (rand() % n_valid_spots);
    Point p = next_valid_spots[index];
}

int main() {


    read_board();
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            state[i][j] = board[i][j] > 1 ? -1: board[i][j];
            //cout<<state[i][j]<<' ';
        }
        //cout<<endl;
    }
    ans_x = -1;
    ans_y = -1;
    alpha_beta(0, -10000000000, 10000000000, true);


    read_valid_spots();
    write_valid_spot();

    return 0;
}
