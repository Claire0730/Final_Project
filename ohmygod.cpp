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

class OthelloBoard {
public:
    enum SPOT_STATE {
        EMPTY = 0,
        BLACK = 1,
        WHITE = 2
    };
    static const int SIZE = 8;
    const std::array<Point, 8> directions{{
        Point(-1, -1), Point(-1, 0), Point(-1, 1),
        Point(0, -1), /*{0, 0}, */Point(0, 1),
        Point(1, -1), Point(1, 0), Point(1, 1)
    }};
    std::array<std::array<int, SIZE>, SIZE> board;
    std::vector<Point> next_valid_spots;
    std::array<int, 3> disc_count;
    int cur_player;
    bool done;
    int winner;
private:
    int get_next_player(int player) const {
        return 3 - player;
    }
    bool is_spot_on_board(Point p) const {
        return 0 <= p.x && p.x < SIZE && 0 <= p.y && p.y < SIZE;
    }//check if the spot is on board
    int get_disc(Point p) const {
        return board[p.x][p.y];
    }
    void set_disc(Point p, int disc) {
        board[p.x][p.y] = disc;
    }
    bool is_disc_at(Point p, int disc) const {
        if (!is_spot_on_board(p))
            return false;
        if (get_disc(p) != disc)
            return false;
        return true;
    }
    bool is_spot_valid(Point center) const {
        if (get_disc(center) != EMPTY)
            return false;
        for (Point dir: directions) {
            // Move along the direction while testing.
            Point p = center + dir;
            if (!is_disc_at(p, get_next_player(cur_player)))
                continue;
            p = p + dir;
            while (is_spot_on_board(p) && get_disc(p) != EMPTY) {
                if (is_disc_at(p, cur_player))
                    return true;
                p = p + dir;
            }
        }
        return false;
    }

    void flip_discs(Point center) {
        for (Point dir: directions) {
            // Move along the direction while testing.
            Point p = center + dir;
            if (!is_disc_at(p, get_next_player(cur_player)))
                continue;
            std::vector<Point> discs({p});
            p = p + dir;
            while (is_spot_on_board(p) && get_disc(p) != EMPTY) {
                if (is_disc_at(p, cur_player)) {
                    for (Point s: discs) {
                        set_disc(s, cur_player);
                    }
                    disc_count[cur_player] += discs.size();
                    disc_count[get_next_player(cur_player)] -= discs.size();
                    break;
                }
                discs.push_back(p);
                p = p + dir;
            }
        }
    }
public:
    OthelloBoard() {
        reset();
    }
    void reset() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = EMPTY;
            }
        }
        board[3][4] = board[4][3] = BLACK;
        board[3][3] = board[4][4] = WHITE;
        cur_player = BLACK;
        disc_count[EMPTY] = 8*8-4;
        disc_count[BLACK] = 2;
        disc_count[WHITE] = 2;
        next_valid_spots = get_valid_spots();
        done = false;
        winner = -1;
    }

    std::vector<Point> get_valid_spots() const {
        std::vector<Point> valid_spots;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                Point p = Point(i, j);
                if (board[i][j] != EMPTY)
                    continue;
                if (is_spot_valid(p))
                    valid_spots.push_back(p);
            }
        }
        return valid_spots;
    }//return valid spots as vector

    bool put_disc(Point p) {
        if(!is_spot_valid(p)) {
            winner = get_next_player(cur_player);
            done = true;
            return false;
        }
        set_disc(p, cur_player);
        disc_count[cur_player]++;
        disc_count[EMPTY]--;
        flip_discs(p);
        // Give control to the other player.
        cur_player = get_next_player(cur_player);
        next_valid_spots = get_valid_spots();
        // Check Win
        if (next_valid_spots.size() == 0) {
            cur_player = get_next_player(cur_player);
            next_valid_spots = get_valid_spots();
            if (next_valid_spots.size() == 0) {
                // Game ends
                done = true;
                int white_discs = disc_count[WHITE];
                int black_discs = disc_count[BLACK];
                if (white_discs == black_discs) winner = EMPTY;
                else if (black_discs > white_discs) winner = BLACK;
                else winner = WHITE;
            }
        }
        return true;
    }
};
int player;
const int SIZE = 8;

//std::array<std::array<int, SIZE>, SIZE> board;
int board[8][8];

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
    int value=0;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            value += weight[i][j]*state[i][j];
        }
    }
    return value;
}

int alpha_beta(int depth, int alpha, int beta, bool maximizingPlayer){
    int value;
    if(depth >=2){//if depth = 0 || node is terminal node
       value = getWeight();
       return value;
    }
    int temp[10][10];
    memcpy(temp, state, sizeof state);
    if(maximizingPlayer){
        value = alpha;
        int s = next_valid_spots.size();

        for(int i=0 ;i<s; i++){
            int val = alpha_beta(depth+1, alpha, beta, false);
            if(val > alpha){
                value = val;
                alpha = val;
            }
            if(alpha >= beta)break;
            cout<<i<<' '<<value<<endl;
        }
        return value;
    }
    else{

        value = beta;
        int s = next_valid_spots.size();
        for(int i=0; i<s; i++){
            int val = alpha_beta(depth+1, alpha, beta, true);
            if(val < beta){
                value = val;
                beta = val;
            }
           ;
            if(beta<=alpha)break;

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

    OthelloBoard cur;
    read_board();
    read_valid_spots();
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            state[i][j] = board[i][j] > 1 ? -1: board[i][j];
        }

    }
    ans_x = -1;
    ans_y = -1;
    int value = alpha_beta(0, -10000000000, 10000000000, true);

    write_valid_spot();

    return 0;
}
