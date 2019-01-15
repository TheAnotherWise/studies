#include <iostream>

using namespace std;

static int counter=0;

class Queue{
    public:
        Queue * head, * tail, * next, * prev;
        int nQueue, x, y;
        Queue(){ next=prev=head=tail=NULL; nQueue=0; }
        void put( int x, int y){
            if(head!=NULL && tail!=NULL){
                Queue * work = new Queue;
                work->x=x;
                work->y=y;
                work->prev=tail;
                tail->next=tail=work;
                work->next=NULL;
                this->nQueue++;
            }else{
                Queue * work = new Queue;
                work->x=x;
                work->y=y;
                work->prev=work->next=NULL;
                head=tail=work;
                this->nQueue=1;
            }
        }
        void removeFirst(){
            if(this->nQueue>1){
                Queue * work=head;
                head=work->next;
                work->next->prev=NULL;
                delete work;
                if(this->nQueue!=0) this->nQueue--;
            }else if(this->nQueue==1){
                Queue * work=head;
                head=tail=NULL;
                delete work;
                if(this->nQueue==1) this->nQueue--;
            }
        }
};
class Rescue{
public:
    char **mine;
    int mineX, mineY, rescueX, rescueY, minersX, minersY;
    Rescue( int mineX, int mineY ){
        cin >> this->mineX >> this->mineY;
        mine=new char*[this->mineX];
        for(int i=0; i<this->mineX; i++) mine[i]=new char[this->mineY];
        for(int i=0; i<this->mineX; i++)
            for(int j=0; j<this->mineY; j++) mine[i][j]=' ';
        for(int i=0; i<this->mineX; i++){
            cin >>mineX;
            for(int j=0; j<mineX; j++){
                cin >> mineY;
                mine[i][mineY]='*';
            }
        }
        cin >> this->minersY >> this->minersX;
        cin >> this->rescueY >> this->rescueX;
        mine[this->minersX][this->minersY]='+';
    }
    bool checkOrReplace( int x, int y, Queue *roadQueue, Queue *wallsQueue ){
        if(mine[x][y]==' '){
            mine[x][y]='-';
            roadQueue->put(x, y);
        }else if(mine[x][y]=='*'){
            mine[x][y]='-';
            wallsQueue->put(x, y);
        }else if(mine[x][y]=='+')return 1;
        return 0;
    }
    bool floodFill( int x, int y, Queue *roadQueue, Queue *wallsQueue ){
        if(x+1<mineX) if(checkOrReplace(x+1, y, roadQueue, wallsQueue)==1) return 1;
        if(x-1>=0) if(checkOrReplace(x-1, y, roadQueue, wallsQueue)==1) return 1;
        if(y+1<mineY) if(checkOrReplace(x, y+1, roadQueue, wallsQueue)==1) return 1;
        if(y-1>=0)if(checkOrReplace(x, y-1, roadQueue, wallsQueue)==1) return 1;
        return 0;
    }
    int floodFillTrigger( bool spr, bool test, Queue *tmpI, Queue *tmpII ){
        while(true){
            if(spr==0){
                while(tmpI->head!=NULL){
                    test=floodFill(tmpI->head->x,tmpI->head->y,tmpI,tmpII);
                    tmpI->removeFirst();
                }spr=1;
            }else if(spr==1){
                while(tmpII->head!=NULL){
                    test=floodFill(tmpII->head->x,tmpII->head->y,tmpII,tmpI);
                    tmpII->removeFirst();
                }spr=0;
            }
            if(test==1)return counter;
            counter++;
        }
    }
};
main(){
    ios_base::sync_with_stdio(false);
    Rescue attempt(0,0);
    Queue * tmpI = new Queue;
    Queue * tmpII = new Queue;
    tmpI->put(attempt.rescueX,attempt.rescueY);
    cout << attempt.floodFillTrigger(0,0,tmpI,tmpII);
}