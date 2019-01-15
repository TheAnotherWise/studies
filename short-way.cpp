//====================================================================
//####################################################################
//====================================================================
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <limits>
#include <queue>
//====================================================================
//####################################################################
//====================================================================
using namespace std;
//====================================================================
//####################################################################
//====================================================================
static float maxLengthJump=0;
static unsigned int numberOfTrees=0;
static int ID=0;

//====================================================================
//####################################################################
//====================================================================
typedef vector< vector< pair<float,float> > > Graph;
struct dijkstraComparator{
    float operator() ( const pair<float,float> &p1, const pair<float,float> &p2){
        return p1.second >= p2.second;
    }
};
//####################################################################
bool dijkstra( Graph  graph, float start, float finish, vector<float> &path ){
    vector< int > work( graph.size() );
    vector< int > output( graph.size() );
    for( int i = 0 ; i < graph.size(); i++ ){
        work[i] = std::numeric_limits< int >::max();
        output[i] = -1;
    }
    priority_queue< pair< int, int >, vector< pair< int, int > >, dijkstraComparator> queue;
    work[start] = 0.0f;
    queue.push( make_pair( start, work[start] ) );
    while( !queue.empty() ){
        int elT = queue.top().first;
        if( elT == finish ) break;
        queue.pop();
        for(int i=0; i < graph[elT].size(); i++){
            int elF = graph[elT][i].first;
            float elS = graph[elT][i].second;
            if( work[elF] > work[elT]+elS){
                work[elF] = work[elT]+elS;
                output[elF] = elT;
                queue.push( make_pair( elF, work[elF] ) );
            }
        }
    }
    path.clear();
    int elP = finish;
    path.push_back( finish );
    if( output[elP] == -1) return 0;
    else{
        while( elP != start ){
            elP = output[elP];
            path.push_back( elP );
        }
        return 1;
    }
}
//====================================================================
//####################################################################
//====================================================================
struct Vertex{
    float xPos;
    float yPos;
    int vertexID;
    bool toCut;
    unsigned int weight;
    vector< Vertex* > edges;

    Vertex( float x, float y, unsigned int z ){
        this->xPos = x;
        this->yPos = y;
        this->toCut = 0;
        if(z>100) this->weight = 0;
        else this->weight = 1;
    }
};
//====================================================================
//####################################################################
//====================================================================
bool comparator( Vertex* first, Vertex* second ){
    if( first->xPos != second->xPos ){
        if( first->xPos < second->xPos ) return true;
        else return false;
    }else{
        if( first->yPos< second->yPos ) return true;
        else return false;
    }
}

float returnEdge( Vertex *vertext1, Vertex *vertex2){
    float max = 1000000000.0;
    float lenght = 0;
    if( vertext1->vertexID == vertex2->vertexID ) return max;
    if( fabs( vertext1->xPos - vertex2->xPos <= maxLengthJump) ){
        lenght = fabs( sqrt( ( vertex2->xPos - vertext1->xPos ) * ( vertex2->xPos - vertext1->xPos ) + ( ( vertex2->yPos - vertext1->yPos )*( vertex2->yPos - vertext1->yPos ) ) ) );
        return lenght;
    }
}
void searchEdges( vector< Vertex* > ret ){
    int k=0;
    for(  vector< Vertex* >::iterator itRet = ret.begin(); itRet != ret.end(); itRet++ ){
        Vertex *mov = dynamic_cast< Vertex* >(*itRet);
        float left = mov->xPos - maxLengthJump;
        float right = mov->xPos + maxLengthJump;
        vector< Vertex* >::iterator itLeft = itRet;
        vector< Vertex* >::iterator itRight = itRet;

        Vertex *vertexLeft = dynamic_cast< Vertex* >(*itLeft);
        int i = mov->vertexID;
        while( i >= 0 && vertexLeft->xPos >= left ){
            if( returnEdge( mov, vertexLeft ) <= maxLengthJump ){
                mov->edges.push_back( vertexLeft );
            }
            vertexLeft = dynamic_cast< Vertex* >(*itLeft);
            --itLeft, --i;
        }

        Vertex *vertexRight = dynamic_cast< Vertex* >(*itRight);
        int j = mov->vertexID;
        while( j < numberOfTrees && vertexRight->xPos <= right ){
            vertexRight = dynamic_cast< Vertex* >(*itRight);
            if( j == k );
            else if( returnEdge( mov, vertexRight) <= maxLengthJump){
                mov->edges.push_back( vertexRight );
            }
            ++itRight, ++j;
        }

    }
}
//====================================================================
//####################################################################
//====================================================================
void printOutPutForm( vector< Vertex* > vertexs ){
    int i=1;
    for( vector< Vertex* >::iterator it=vertexs.begin(); it!=vertexs.end(); it++ ){
        Vertex *vertex = dynamic_cast< Vertex* >(*it);
        if(vertex->toCut==0){
            cout << vertex->xPos << " " << vertex->yPos << endl;
        }
    }
}
void setID( vector< Vertex* > vertexs){
    for( vector< Vertex* >::iterator it=vertexs.begin(); it!=vertexs.end(); it++ ){
        Vertex *vertex = dynamic_cast< Vertex* >(*it);
        vertex->vertexID=ID;
        ID++;
    }
}
//====================================================================
//####################################################################
//====================================================================
main(){
    ios_base::sync_with_stdio(false);

    Graph g;
    int startPos, finishPos;
    float xPos, yPos;
    unsigned int ageTree;
    vector< Vertex* > vertexs;
    vector<float> path;

    cin >> maxLengthJump >> numberOfTrees;

    g.resize(numberOfTrees);

    //UTWORZENIE WIERZCHOLKOW
    for( int i=0; i<numberOfTrees; i++ ){
        cin >> xPos>> yPos >> ageTree;
        Vertex *vertex = new Vertex(xPos,yPos,ageTree);
        vertexs.push_back(vertex);
    }

    //SORTOWANIE
    sort(vertexs.begin(),vertexs.end(),comparator);
    //DODANIE POSORTOWANYM WIERZCHOLKOIM ID
    setID(vertexs);
    //WYSZUKANIE KRAWEDZI DLA KAZDEGO WIERZCHOLKA
    searchEdges(vertexs);

    //USTAWIENIE POZYCJI STARTOWEJ
    vector< Vertex* >::iterator startPosIt = vertexs.begin();
    Vertex *startVertex = dynamic_cast< Vertex* >(*startPosIt);
    startPos = startVertex->vertexID;

    //USTAWIENIE POZYCJI KONCOWEJ
    vector< Vertex* >::reverse_iterator finishVertexIt = vertexs.rbegin();
    Vertex *endVertex = dynamic_cast< Vertex* >(*finishVertexIt);
    finishPos = endVertex->vertexID;

    //DODANIE WIERZCHOLKOW I KRAWEDZI DO GRAFU
    for( vector< Vertex* >::iterator it=vertexs.begin(); it != vertexs.end(); it++ ){
        Vertex *vertex = dynamic_cast< Vertex* >(*it);
        for( vector< Vertex* >::iterator itDeep = vertex->edges.begin(); itDeep != vertex->edges.end(); itDeep++ ){
            Vertex *vertexDeep = dynamic_cast< Vertex* >(*itDeep);
            g[vertex->vertexID].push_back( make_pair(vertexDeep->vertexID, vertex->weight ) );
        }
    }

    //DIJKSTRA
    bool check = dijkstra( g, startPos, finishPos, path);

    //SPRAWDZENIE SCIEZKI I USTAWIENIE DRZEW DO KASACJI
    if( check ){
        int k = path.size()-1;
        for( int i = path.size()-1; i >= 0; i-- ){
            vertexs[path[i]]->toCut = 1;
        }
        cout << numberOfTrees-k-1 << endl;
        printOutPutForm( vertexs );
    }else cout << "brak sciezki";
}