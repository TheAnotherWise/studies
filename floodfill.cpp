#include <iostream>
#define PUSTO 'O'
#define SCIANA 'X'
#define RATOWNICY 'R'
#define GORNICY 'G'
#define DOUSUNIECIA 'U'

using namespace std;

struct Node{
    int x;
    int y;
    Node *next;
    Node(int x, int y, Node *next = NULL):x(x),y(y),next(next){}
};

class Kolejka{
    Node *first;
    Node *last;
public:
    Kolejka();
    ~Kolejka();
    void enqueue(int, int);
    Node* dequeue();
    bool isEmpty();
    friend void zamien(Kolejka &, Kolejka &);
};

void reset(char **mapa, const int &max_x, const int &max_y);

void printMap(char **mapa, const int &max_x, const int &max_y);
void floodFill(char **mapa, const int &max_x, const int &max_y, const int &start_x, const int &start_y);

int main()
{
    ios_base::sync_with_stdio(false);
    int x,y;
    char **mapa;
    cin >> x >> y;
    mapa = new char*[x];

    for(int i = 0; i < x; i++)
    {
        mapa[i] = new char[y];
    }

    for(int i = 0; i < x; i++)
    {
        for(int j = 0; j < y; j++)
        {
            mapa[i][j] = PUSTO;
        }

        int r,tmp;
        cin >> r;

        for(int k = 0; k < r; k++)
        {
            cin >> tmp;
            mapa[i][tmp] = SCIANA;
        }
    }

    int gornicyX, gornicyY, ratownicyX, ratownicyY;
    cin >> gornicyY >> gornicyX;
    cin >> ratownicyY >> ratownicyX;

    //mapa[ratownicyX][ratownicyY] = RATOWNICY;
    //mapa[gornicyX][gornicyY] = GORNICY;

    int usunietychScian = 0;

    while(true)
    {
        floodFill(mapa,x,y,ratownicyX,ratownicyY);
        if(mapa[gornicyX][gornicyY] == RATOWNICY){
            break;
        }else{
            usunietychScian++;
            reset(mapa,x,y);
            //mapa[ratownicyX][ratownicyY] = RATOWNICY;
        }
    }

    //printMap(mapa,x,y);

    for(int i = 0; i < x; i++)
    {
        delete [] mapa[i];
    }
    delete [] mapa;


    cout << usunietychScian << endl;

    return 0;
}

void reset(char **mapa, const int &max_x, const int &max_y)
{
    for(int i = 0; i < max_x; i++)
    {
        for(int j = 0; j < max_y; j++)
        {
            if(mapa[i][j] == DOUSUNIECIA)
            {
                mapa[i][j] = PUSTO;
            }
        }
    }
}

Kolejka scianyDoUsuniecia = Kolejka();

void floodFill(char **mapa, const int &max_x, const int &max_y, const int &start_x, const int &start_y)
{
    Kolejka k = Kolejka();
    if(scianyDoUsuniecia.isEmpty())
    {
        k.enqueue(start_x,start_y);
    }else{
        zamien(k,scianyDoUsuniecia);
    }
    register Node *current = NULL;

    while(!k.isEmpty())
    {
        current = k.dequeue();
        int wx, ex;
        wx = ex = current->x;
        if(mapa[current->x][current->y] != PUSTO){
            if(mapa[current->x][current->y] == SCIANA){
                mapa[current->x][current->y] = DOUSUNIECIA;
                scianyDoUsuniecia.enqueue(current->x,current->y);
            }
            delete current;
            continue;
        }

        while(wx - 1 >= 0)
        {
            wx--;
            if(mapa[wx][current->y] != PUSTO){
                if(mapa[wx][current->y] == SCIANA){
                    mapa[wx][current->y] = DOUSUNIECIA;
                    scianyDoUsuniecia.enqueue(wx,current->y);
                }
                wx++;
                break;
            }
        }

        while(ex + 1 < max_x)
        {
            ex++;
            if(mapa[ex][current->y] != PUSTO){
                if(mapa[ex][current->y] == SCIANA){
                    mapa[ex][current->y] = DOUSUNIECIA;
                    scianyDoUsuniecia.enqueue(ex,current->y);
                }
                ex--;
                break;
            }
        }

        bool check_above = (current->y - 1 >= 0)?true:false;
        bool check_below = (current->y + 1 < max_y)?true:false;

        for(int i = wx; i <= ex; i++)
        {
            mapa[i][current->y] = RATOWNICY;

            if(check_above){
                if(mapa[i][current->y - 1] == PUSTO){
                    k.enqueue(i,current->y - 1);
                }else if(mapa[i][current->y - 1] == SCIANA){
                    mapa[i][current->y - 1] = DOUSUNIECIA;
                    scianyDoUsuniecia.enqueue(i,current->y - 1);
                }
            }

            if(check_below){
                if(mapa[i][current->y + 1] == PUSTO){
                    k.enqueue(i,current->y + 1);
                }else if(mapa[i][current->y + 1] == SCIANA){
                    mapa[i][current->y + 1] = DOUSUNIECIA;
                    scianyDoUsuniecia.enqueue(i,current->y + 1);
                }
            }

        }
        delete current;
    }
}

void printMap(char **mapa, const int &max_x, const int &max_y)
{
    for(int i = 0; i < max_x; i++)
    {
        for(int j = 0; j < max_y; j++)
        {
            cout << mapa[i][j] << " ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void zamien(Kolejka &k1, Kolejka &k2)
{
    Node *temp = k1.first;
    k1.first = k2.first;
    k2.first = temp;

    temp = k1.last;
    k1.last = k2.last;
    k2.last = k1.last;
}

Kolejka::Kolejka()
{
    first = last = NULL;
}

Kolejka::~Kolejka()
{
    Node *temp = first;
    while(temp != NULL)
    {
        first = first->next;
        delete temp;
        temp = first;
    }
}

void Kolejka::enqueue(int x, int y)
{
    if(first == NULL){
        first = new Node(x,y);
        last = first;
    }else{
        last->next = new Node(x,y);
        last = last->next;
    }
}

Node* Kolejka::dequeue()
{
    Node *temp = first;
    //temp->next = NULL;
    first = first->next;
    if(first == NULL) last = NULL;
    return temp;
}

bool Kolejka::isEmpty()
{
    return first == NULL;
}
