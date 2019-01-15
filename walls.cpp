#include <iostream>
#include <vector>
using namespace std;

struct query {
    int x, y;
    int step;
    int answer;
};

#define TOP    (1 << 0)
#define BOTTOM (1 << 1)
#define LEFT   (1 << 2)
#define RIGHT  (1 << 3)

#define SET(var, mask)   ((var) |= (mask))
#define UNSET(var, mask) ((var) &= ~(mask))
#define IS(var, mask)    (((var) & (mask)) == (mask))

int type, x, y;
int steps;

int walls_count;

int size;

int** current;
int** prevous;

int** walls;

query* queries;
int queries_count;


void reset(int **array, int value){
    for( int i = 0; i < size; i++ )
        for( int j = 0; j < size; j++ )
            array[i][j] = value;
}

void init(int** array) {
    reset(array, LEFT | RIGHT | TOP | BOTTOM);

    for (int i=0; i<size; i++) {
        UNSET(array[0][i], LEFT);
        UNSET(array[i][0], TOP);

        UNSET(array[size-1][i], RIGHT);
        UNSET(array[i][size-1], BOTTOM);
    }
}

int** alloc() {
    int** arr = new int*[size];
    for( int i = 0; i < size; i++ )
        arr[i] = new int[size];
    return arr;
}

void stash(int step) {
    for(int i=0; i<queries_count; i++) {
        if (queries[i].step != step)
            continue;

        int x = queries[i].x, y = queries[i].y;
        queries[i].answer = current[x][y];
    }

    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            prevous[i][j] = current[i][j];
}

void print() {
    for(int i=0; i<queries_count; i++)
        cout << queries[i].answer << endl;
}

void counting() {
    for(int x=0; x<size; x++) {
        for(int y=0; y<size; y++) {
            int sum = 0;

            if (IS(walls[x][y], LEFT)) {
                sum += prevous[x-1][y];
                sum %= 1000000007;
            }

            if (IS(walls[x][y], RIGHT)) {
                sum += prevous[x+1][y];
                sum %= 1000000007;
            }

            if (IS(walls[x][y], TOP)) {
                sum += prevous[x][y-1];
                sum %= 1000000007;
            }

            if (IS(walls[x][y], BOTTOM)) {
                sum += prevous[x][y+1];
                sum %= 1000000007;
            }

            current[x][y] = sum;
        }
    }
}


int main() {
    cin >> size >> walls_count;

    walls = alloc();
    init(walls);

    for(int i = 0; i < walls_count; i++) {
        cin >> type >> x >> y;

        if (type) { // poziomy
            UNSET(walls[x][y], RIGHT);
            UNSET(walls[x+1][y], LEFT);
        } else { // pionowy
            UNSET(walls[x][y], BOTTOM);
            UNSET(walls[x][y+1], TOP);
        }
    }

    prevous = alloc();
    current = alloc();

    cin >> x >> y >> queries_count;

    queries = new query[queries_count];
    for( int i = 0; i < queries_count; i++ ){
        cin >> queries[i].step >> queries[i].x >> queries[i].y;
        steps = max(steps, queries[i].step);
    }

    for(int i=0; i<steps+1; i++) {
        if (i == 0) {
            reset(current, 0);
            current[x][y] = 1;
        } else {
            counting();
        }
        
        stash(i);
    }
    
    print();
    
    return 0;
}