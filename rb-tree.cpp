#include <iostream>

using namespace std;

void endln( int n ){ for(int i=0; i<n; i++) cout << endl; }

class RB;
static RB * inLast=NULL;
static int counter=0;
static bool numbExist=0;



class RB{
private:
	int numb;
	bool color;
	int set;
	RB *root;
	RB *left;
	RB *right;
	RB *parent;
public:
 	RB();
	RB( int  );
	void print();
	void main();
	//###############
	void search( RB *, int );
	void insert( RB *&, RB *& );
	void fixup( RB *&, RB *& );
	void leftRotate( RB *&, RB *& );
	void rightRotate( RB *&, RB *& );
};

RB::RB(){ root=NULL; }

RB::RB(int n){
    this->numb=n;
    this->color=1;
    this->set=1;
    left=NULL;
    right=NULL;
    parent=NULL;
}

void RB::print() {
    if(left!=NULL) left->print();
    cout << "->"<<this->numb << ", <=" << this->set << " color:" << this->color << endl;
    if(right!=NULL) right->print();
}

void RB::main(){
  bool spr=0;
  int n, tmp, nWe, nWy;
  cin >>n;
  for(int i=0; i<n; i++){
    cin >> nWe;
    for(int j=0; j<nWe; j++){
        cin>> tmp;
        RB * work = new RB(tmp);
        insert(root,work);
        fixup(root,work);
    }
    cin >> nWy;
    for(int j=0; j<nWy; j++ ){
        cin >>tmp;
        search(root,tmp);
        if(numbExist==1) cout << "T," << counter << " ";
        else cout << "N," << counter << " ";
        counter=0;
        numbExist=0;
    }
    cout << endl;
  }
  /*cout << "counter: "<<counter << " Exist:"<< numbExist ;
  endln(0);
  cout << "[1->red, 0:black]" << endl;*/
  //root->print();
}
//############################################################
void RB::search( RB *work, int numb ){
       /* if(work!=NULL){
        if(work->numb<=numb){
            counter++;
            if(work->numb==numb) numbExist=1;
        }
        if(work->right!=NULL)search(work->right,numb);
        if(work->left!=NULL)search(work->left,numb);
    }*/
    if(work!=NULL){
        if(numb<work->numb) search(work->left,numb);
        //else if(work->left!=NULL && numb<work->left->numb) search(work->left,numb);
        else if(numb==work->numb){
            numbExist=1;
            counter+=work->set;
        }
        else{
            counter+=work->set;
            search(work->right,numb);
        }
    }
}
void RB::insert( RB *&root, RB *&work ){
    if(root==NULL){
        root=work;
        work->parent=inLast;
        return;
    }else{
        inLast=root;
        if(work->numb<root->numb){
            root->set++;
            insert(root->left,work);
        }
        else if(work->numb>root->numb) insert(root->right,work);
    }
}

void RB::fixup( RB *&root, RB *&work ){
	RB *x=NULL;
	RB *y=NULL;
	RB *under=NULL;
	while(work!=root && work->color!=0 && work->parent->color==1){
		x=work->parent;
		y=work->parent->parent;
		if(x==y->left){
			RB *under=y->right;
			if(under!=NULL && under->color==1){
				y->color=1;
				x->color=0;
				under->color=0;
				work=y;
			}else{
				if(work==x->right){
					leftRotate(root,x);
					//cout << "x: "<<x->numb<<endl;
					//if(x->left==NULL) x->set=1;
					work=x;
					x=work->parent;
				}
				rightRotate(root,y);
                if(y->left==NULL) y->set=1;
				x->color=0;
				y->color=1;
				work=x;
			}
		}else{
			RB *under=y->left;
			if(under!=NULL && under->color==1){
				y->color=1;
				x->color=0;
				under->color=0;
				work=y;
			}else{
				if(work==x->left){
					rightRotate(root,x);
					//cout << "x: "<<x->numb<<endl;
					//if(x->left==NULL) x->set=1;
					work=x;
					x=work->parent;
				}
				leftRotate(root,y);
				if(y->left==NULL) y->set=1;
				x->color=0;
				y->color=1;
				work=x;
			}
		}
	}
	root->color=0;
}

void RB::leftRotate( RB *&root, RB *&work ){
    //cout << "L" << endl;
	RB *y=work->right;
	work->right=y->left;
	if(work->right!=NULL){
        //cout << "(1)" << endl;
        work->right->parent=work;
	}
	y->parent=work->parent;
	if(work->parent==NULL){
	    //cout << "(2)" << endl;
        root=y;
        y->set+=work->set;
	}
	else if(work==work->parent->left){
	    //cout << "(3)" << endl;
	    work->parent->left=y;
	    y->set+=work->set;
	}
	else{
	    //cout << "(4)" << endl;
        work->parent->right=y;
        y->set+=work->set;
	}
	y->left=work;
	work->parent=y;
}
void RB::rightRotate( RB *&root, RB *&work ){
    //cout << "R" << endl;
	RB *y=work->left;
	work->left=y->right;
	if(work->left!=NULL){
	    //cout << "(1)" << endl;
        work->left->parent=work;
	}
	y->parent=work->parent;
	if(work->parent==NULL){
	    //cout << "(2)" << endl;
        root=y;
        work->set-=y->set;
	}
	else if(work==work->parent->left){
	    //cout << "(3)" << endl;
        work->parent->left=y;
        work->set-=y->set;
	}
	else{
	    //cout << "(4)" << endl;
        work->parent->right=y;
        work->set-=y->set;
	}
	y->right=work;
	work->parent=y;
}
//#################################################################
int main() {
  RB rb_tree;
  rb_tree.main();
  return 0;
}