#include <iostream>
#include <vector>
using namespace std;

int posibility = 0;
bool mode = 0;

int input;
int n;

struct coin{
    int value;
    int amount;
    int used;
};

int newton( int n, int k ){
    double out = 1;
    for( int i = 1; i <= k; i++ ) out = out * ( n-i+1 ) / i;
    return (int)out;
}

int takesum( vector< coin > &coins ){
    int tmp = 0;
    for( int i = 0; i < n; i++ )
        tmp += coins[i].value * coins[i].amount;
    return tmp;
}

void change( vector< coin > &coins, int i, int _min ){
    coins[i].used = coins[i].amount;

    int min_ = _min + (coins[i].value * coins[i].amount);

    for( int j = coins[i].amount; j >= 0; j-- ) {
        coins[i].used = j;
        if ( i == n-1 ) {
            int sum = _min + j*coins[i].value;
            if ( sum == input )//ZAKONCZENIE
                if( mode == 0 ) posibility++;
                else {
                    int sry = 1;
                    for( int j = 0; j < n; j++ )
                        sry *= newton( coins[j].amount, coins[j].used );
                    posibility += sry;
                }
            else if ( sum < input ) break;
        }
        else if ( min_ <= input  )
            change( coins, i+1 , min_ );
        min_ -= coins[i].value;
    }
}

int main(){
    ios_base::sync_with_stdio(0);
    cin >> n;
    vector< coin > coins(n);
    for(int i=0; i<n; i++){
        cin >> coins[i].value >> coins[i].amount;
        coins[i].used = 0;
    }
    cin >> input >> mode;
    change( coins, 0, 0 );
    cout << posibility << endl;
    return 0;
}