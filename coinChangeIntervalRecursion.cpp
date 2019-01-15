#include <iostream>
#include <vector>

#define ZERO(min) ((min == 0) ? 1 : min )

using namespace std;

int posibility = 0;
bool mode = 0;

int min_, max_;
int input;
int n;

struct coin{
    int value;
    int amount;
    int constamount;
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

void change( vector< coin > &coins, int i, int _min, int _max  ){
    if( _min == input ){
        if( mode == 0 ) posibility++;
        else{
            int sry = 1;
            for( int i = 0; i < n; i++ )
                if( coins[i].constamount != coins[i].amount )
                    sry *= newton(coins[i].constamount, coins[i].constamount-coins[i].amount);
            posibility += sry;
        }return;
    }else if( i == n || _min > input ) return;
    else{
        for( int j = 0; j <= coins[i].constamount; j++ ){
            min_ = _min + (j * coins[i].value);
            if( j != 0 ) coins[i].amount--;
            max_ = _max - (coins[i].value * coins[i].amount);
            if( i == 0 && min_ > input) return;
            change(coins, i+1 , min_, max_ );
            if( j == coins[i].constamount ) coins[i].amount = coins[i].constamount;
        }
    }
}

main(){
    cin >> n;
    vector< coin > coins;
    for(int i=0; i<n; i++){
        coin newCoin;
        cin >> newCoin.value >> newCoin.amount;
        newCoin.constamount = newCoin.amount;
        coins.push_back(newCoin);
    }
    cin >> input >> mode;
    max_ = takesum(coins);
    change( coins, 0, min_, max_ );
    cout << posibility;
}
