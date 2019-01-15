#include <stack>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class Point{
private:
    int x, y;
public:
    bool operator <(Point b){
        if (y != b.y)
            if(y < b.y)
                return 1;
            else
                return 0;
        else
            if(x < b.x)
                return 1;
            else
                return 0;
    }
    Point(){}
    Point( int _x, int _y ){
        this->x = _x;
        this->y = _y;
    }
    int getX(){ return this->x; }
    int getY(){ return this->y; }
    void setX( int _x ){ this->x = _x;}
    void setY( int _y ){ this->y = _y;}
};

int n;
int n_Query;

int x, y; //TMP

Point lowest;

int square(Point first, Point second)  {
    int x = first.getX() - second.getX();
    int y = first.getY() - second.getY();
    int square =  x * x  + y * y;

    return square;
}

//ILOCZYK WEKTOROWY
int vectorProduct(Point &root, Point first, Point second) {
    int det =
              (first.getX() - root.getX())
              *
              (second.getY() - root.getY())
              -
              (first.getY() - root.getY())
              *
              (second.getX() - root.getX());
    if(det > 0)
        return -1;
    else if(det < 0)
        return 1;
    else
        return 0;
}

bool comparator(const Point &first, const Point &second){
    int det = vectorProduct(lowest, first, second);
    if(det == 0)
        if(square(lowest, first) < square(lowest, second))
            return 1;
        else
            return 0;
    else
        if(det == -1)
            return 1;
        else
            return 0;
}

// GRAHAM NA KWADRACIE(BEZ FLOATINGU)
// SZUKANIE PUNKTU KTORY LEZY NAJBRADZIEJ PO PRAWEJ
// OD NAJNIZSZEGO PUNKTU
vector<Point> graham(Point *points){
    vector<Point> out;
    stack<Point> hull;
	
	//WYSZUKANIE NAJNIZSZEGO ELEMENTU PO Y
    int last = 0;
    for(int i = 1; i < n; i++)
        if(points[i] < points[last]) last = i;

    // PRZESTAWIENIE NA POCZATEK TABLICY
    // NAJNIZSZEGO ELEMENTU Z OSI XY
    Point temp = points[0];
    points[0] = points[last];
    points[last] = temp;

    // NAJNIZSZY ELEMENT Z OSI XY DO LOWEST(TMP)
	// UZYCIE OD POCZATKU 
    lowest = points[0];
    // POSORTOWANIE "PO Y"
    sort(points + 1, points + n, comparator);

    // PIERWSZE TRZY PUNKTU NA STOSL(WEKTOR DWOCH)
    hull.push(points[0]);
    hull.push(points[1]);
    hull.push(points[2]);

    for(int i = 3; i < n; i++) {
        Point top = hull.top();
        hull.pop();
		
        // DOPÓKI ILOCZYK WEKTOROWY RÓ¯NY OD SKRETU W LEWO
		// CZYLI OTOCZKA MA MIEC OD PUNKTU AKTUALNEGO
		// NAJBARDZIEJ ODLEGLY I NAJBARDZIEJ PO PRAWEJ PUNKT
        while(vectorProduct(hull.top(), top, points[i]) != -1){
            top = hull.top();
            hull.pop();
        }
        hull.push(top);
        hull.push(points[i]);
    }
    while(!hull.empty()){
        Point p = hull.top();
        hull.pop();
        out.push_back(p);
    }
    return out;
}

// PRZESZUKANIE OTOCZKI
//
// -1 WEW
// 0 NA
// 1 ZEW
//
// GDY 0 A PO NIM -1 TO PUNKT JEST NA ODCINKU
// GDY ZBADANO WSZYSTKIE ODCINKI I SAME -1 TO NA PEWNO W SRODKU
// GDY CHOC RAZ 1 TO NA ZEW BO OTOCZKA NIE MA KATOW WKLESLYCH
int lol = 0;

int checkIf( int j, vector<Point> work, Point *query ){
    //7 1 2 -1 2 2 3 3 2 3 3 1 4 1 1 6 0 3 1 3 1 0 2 2 4 3 3 2
    int test = 1 , next = 1;
    Point A, B, C;
    for(int i = 0; i < work.size()-1; i++){
        next = test;

        A = work[i];
        B = work[i+1];
        C = query[j];

        test = vectorProduct(A, B, C);
        if(next == 0 && test == -1){
            test = next;
            //cout << A.getX() << " " << A.getY() << "-" << B.getX() << " " << B.getY() << endl;
            //cout << square(work[i],work[i-1]) << " " << square(work[i],query[j]) << endl;
            if( square(work[i],work[i-1]) < square(work[i],query[j]))
                test = 1;
            break;
        }
        if(test == 1)
            break;
    }
    if(test == 1)
        cout << "-1" << endl;
    if(test == 0)
        cout << "0" << endl;
    if(test == -1)
        cout << "1" << endl;
}

main(){
    cin >> n;
    Point points[n];
    for(int i = 0; i < n; i++){
        cin >> x >> y;
        Point p(x,y);
        points[i] = p;
    }

    cin >> n_Query;
    Point query[n_Query];
    for(int i = 0; i < n_Query; i++){
        cin >> x >> y;
        Point p(x,y);
        query[i] = p;
    }

    vector<Point> work = graham(points);
    vector<Point> done;
    for(int i = work.size()-1; i >= 0 ; i--){
        done.push_back(work[i]);
    }

    done.push_back(done[0]);
    for(int j = 0; j < n_Query; j++){
        checkIf(j, done, query);
    }

    return 0;
}