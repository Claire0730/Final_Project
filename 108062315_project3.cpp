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


public:
    OthelloBoard(const OthelloBoard& Co) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = Co.board[i][j];

            }
        }
        cur_player = Co.cur_player;
        disc_count[EMPTY] = Co.disc_count[EMPTY];
        disc_count[BLACK] = Co.disc_count[BLACK];
        disc_count[WHITE] = Co.disc_count[WHITE];
        //next_valid_spots = get_valid_spots();
        for(int i=0; i<Co.next_valid_spots.size();i++){
//Point p = Co.next_valid_spots[i];
            next_valid_spots.push_back(Co.next_valid_spots[i]);
        }

        done = false;
        winner = -1;
    }
    OthelloBoard(){
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
        cur_player = player;
        disc_count[EMPTY] = 8*8-4;
        disc_count[BLACK] = 2;
        disc_count[WHITE] = 2;
        next_valid_spots = get_valid_spots();
        done = false;
        winner = -1;
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

const int SIZE = 8;

//std::array<std::array<int, SIZE>, SIZE> board;
//int board[8][8];

//int state[8][8];
std::vector<Point> next_valid_spots;
OthelloBoard cur;


const int weight[8][8] =
{
     65,  -3, 6, 4, 4, 6, -3, 65,
     -3, -50, 3, 1, 1, 3, -50, -3,
     6, 3, 5, 3, 3, 5, 3, 6,
     4, 1, 3, 1, 1, 3, 1, 4,
     4, 1, 3, 1, 1, 3, 1, 4,
     6, 3, 5, 3, 3, 5, 3, 6,
     -3, -50, 3, 1, 1, 3, -50, -3,
     65, -3, 6, 4, 4, 6,  -3, 65,
};

int ans;

int alpha_beta(OthelloBoard &input_board,int depth, int alpha, int beta, bool maximizingPlayer){
    int value=0;
    OthelloBoard f_board = input_board;
    if(depth >= 8){//if depth = 0 || node is terminal node
        int temp[8][8];
        int value=0;
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
               // cout<<input_board.board[i][j]<<' ';
                //temp[i][j] = f_board.board[i][j]>1 ? -1:f_board.board[i][j];
                value += weight[i][j]*f_board.board[i][j];
            }
            //cout<<endl;
        }
        value -= f_board.get_valid_spots().size()*2;
        return value;
    }

    if(maximizingPlayer){
        //cout<<"player: "<<input_board.cur_player<<endl;
       // cout<<"depth: "<<depth<<endl;
        value = alpha;
        int s = input_board.next_valid_spots.size();
        for(int i=0 ;i<s; i++){
            Point p = input_board.next_valid_spots[i];
            if(depth == 0){
                if((p.x == 0 && p.y == 0)||(p.x == 0 && p.y == 7)||(p.x == 7 && p.y == 0)||(p.x == 7 && p.y == 7)){
                   // cout<<"get corner!!!!"<<endl;
                    ans = i;
                    value = 0;
                    break;
                }
            }
            //cout<<"flip: "<<p.x<<' '<<p.y<<endl;
            for(int k=0; k<8; k++){
                for(int j=0; j<8; j++){
                    f_board.board[k][j]=input_board.board[k][j];
                }
            }

            f_board.put_disc(p);
            /*
            cout<<"------------------------------"<<endl;
            for(int k=0; k<8; k++){
                for(int j=0; j<8; j++){
                    cout<<f_board.board[k][j]<<' ';
                }
                cout<<endl;
            }
            cout<<"------------------------------"<<endl;*/
            int val = alpha_beta(f_board, depth+1, alpha, beta, false);
            if(val > alpha){
                value = val;
                alpha = val;
                if(depth == 0)ans = i;
            }
            if(alpha >= beta)break;

        }
        //cout<<"depth: "<<depth<<endl;
        //cout<<"value_player_1 :"<<value<<endl;
        return value;
    }
    else{
        value = beta;
        //cout<<"player: "<<input_board.cur_player<<endl;
        //cout<<"depth: "<<depth<<endl;
        int s = input_board.next_valid_spots.size();
        for(int i=0; i<s; i++){
            Point p = input_board.next_valid_spots[i];
           // cout<<"flip: "<<p.x<<' '<<p.y<<endl;

            for(int k=0; k<8; k++){
                for(int j=0; j<8; j++){
                    f_board.board[k][j]=input_board.board[k][j];

                }
            }
            f_board.put_disc(p);
            /*
            cout<<"------------------------------"<<endl;
            for(int k=0; k<8; k++){
                for(int j=0; j<8; j++){
                    cout<<f_board.board[k][j]<<' ';
                }
                cout<<endl;
            }
            cout<<"------------------------------"<<endl;*/
            int val = alpha_beta(f_board, depth+1, alpha, beta, true);
            if(val < beta){
                value = val;
                beta = val;
                if(depth == 0)ans = i;
            }

            if(beta<=alpha)break;

        }
        //cout<<"depth: "<<depth<<endl;
        //cout<<"value_player_2 :"<<value<<endl;
        return value;
    }
}

void read_board(std::ifstream& fin) {
    int temp;
    fin >> player;
    cur.cur_player = player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin>>cur.board[i][j];
        }
    }
    //cur.next_valid_spots = cur.get_valid_spots();

}

void read_valid_spots(std::ifstream& fin) {
    int n_valid_spots;
    fin >> n_valid_spots;
    int x, y;
    for (int i = 0; i < n_valid_spots; i++) {
        fin >> x >> y;
        cur.next_valid_spots.push_back({x, y});
    }

}

void write_valid_spot(std::ofstream& fout) {
    int n_valid_spots = cur.next_valid_spots.size();
    int value = alpha_beta(cur, 0, -10000000000, 10000000000, true);

    Point p = cur.next_valid_spots[ans];
    fout << p.x <<" "<<p.y<<std::endl;
    fout.flush();
//    cout<<cur.next_valid_spots[ans].x<<' '<<cur.next_valid_spots[ans].y<<endl;
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    read_valid_spots(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
