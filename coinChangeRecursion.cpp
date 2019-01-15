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
    int constamount;
};

int newton( int n, int k ){
    double out = 1;
    for( int i = 1; i <= k; i++ ) out = out * ( n - i + 1 ) / i;
    return (int)out;
}

int sum( vector< coin > &coins, int i ){
    int tmp = 0;
    for( ; i < n; i++){
        int sry=1;
        if( coins[i].amount >= 0 ) sry = coins[i].amount;
        tmp += (coins[i].value * sry );
    }
    return tmp;
}

void change( vector< coin > coins, int in, int work ){
    if( in == input ){
        if( mode == 0 ) posibility++;
        else{
            int sry = 1;
            for( int i = 0; i < n; i++ )
                if( coins[i].constamount != coins[i].amount)
                    sry *= newton(coins[i].constamount,coins[i].constamount-coins[i].amount);
            posibility += sry;
        }
        return;
    }else if( sum(coins,work)+in < input ) return;
    else
        for( int i = work; i < n; i++ )
            if( coins[i].amount > 0 && (coins[i].value + in) <= input ){
                coins[i].amount -= 1;
                change(coins, in + coins[i].value, i);
                coins[i].amount += 1;
            }
}

main(){
    cin >> n;
    vector< coin > coins;
    for( int i=0; i<n; i++ ){
        coin newCoin;
        cin >> newCoin.value >> newCoin.amount;
        newCoin.constamount = newCoin.amount;
        coins.push_back(newCoin);
    }
    cin >> input >> mode;
    change( coins, 0, 0 );
    cout << posibility;
}