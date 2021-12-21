#include<bits/stdc++.h>
using namespace std;
class Node{   //public calss.it can be usable without making friend.
public:
  int val;
  int height;   //node height is that which is greatest among the height of left and right sub tree
  Node* left;
  Node* right;
};
class AVL_Tree{
  Node* t=NULL;
  Node* m=NULL;
  Node* r=NULL;
  public:
    Node* Root=NULL;

    AVL_Tree(int arr[],int n){
      Root=new Node;
      m=Root;
      m->val=arr[0];
      m->left=NULL;
      m->right=NULL;
      for(int i=1;i<n;i++){
        insert_AVL(arr[i],Root,Root);
      }
    }

  void insert_AVL(int key,Node* p,Node* parent){
    Node* t=NULL;

    if(p==NULL){
      t=new Node;
      t->val=key;
      t->height=0;
      t->left=NULL;
      t->right=NULL;
      if(key>parent->val){parent->right=t; p=t;}
      else{parent->left=t; p=t;}
    }
    else{
      if(key< p->val){insert_AVL(key,p->left,p);}
      else{insert_AVL(key,p->right,p);}
    }

    set_NodeHeight(p);

  //checking the balance factor for each node.
 int bf=Balance_Factor(p);
 int c=0;
 if(p->val>parent->val){c=1;}
 else{c=-1;}

  if(bf==2 && Balance_Factor(p->left)==1){
        LL_rotation(p,parent,c);
   }
  else if(bf==-2 && Balance_Factor(p->right)==-1){
       RR_rotation(p,parent,c);
  }
  else if(bf==2 && Balance_Factor(p->left)==-1){
       LR_rotation(p,parent,c);
  }
  else if(bf==-2 && Balance_Factor(p->right)==1){
      RL_rotation(p,parent,c);
  }
  else{}

  }



  void set_NodeHeight(Node* p){
    int lh,rh;
    Node* t=NULL;
    if(p->left==NULL){lh=0;}
    else{t=p->left; lh=t->height+1;}

    if(p->right==NULL){rh=0;}
    else{t=p->right; rh=t->height+1;}

    if(lh<rh){p->height=rh;}
    else{p->height=lh;}
  }

 int Balance_Factor(Node* p){
   int lh,rh;
   Node* t=NULL;
   if(p->left==NULL){lh=0;}
   else{t=p->left; lh=t->height+1;}

   if(p->right==NULL){rh=0;}
   else{t=p->right; rh=t->height+1;}

   return lh-rh;
 }

void LL_rotation(Node* p,Node* parent,int indicator){
  Node* pl=p->left;     //p's left child
  Node* plr=pl->right;  //pl's right child
  pl->right=p;
  p->left=plr;
  set_NodeHeight(pl);
  set_NodeHeight(p);

  if(p==Root){Root=pl;}
  else{
    if(indicator==1){parent->right=pl;}
    else{parent->left=pl;}}
}

void RR_rotation(Node* p,Node* parent,int indicator){
  Node* pr=p->right;     //p's right child
  Node* prl=pr->left;  //pr's left child
  pr->left=p;
  p->right=prl;
  set_NodeHeight(pr);
  set_NodeHeight(p);

  if(p==Root){Root=pr;}
  else{
    if(indicator==1){parent->right=pr;}
    else{parent->left=pr;}}
}


void LR_rotation(Node* p,Node* parent,int indicator){
  Node* pl=p->left;   //p's left child
  Node* plr=pl->right;  //pl's right child
  Node* plrl=plr->left;  //plr's left child
  Node* plrr=plr->right;  //plr's right child

  pl->right=plrl;
  p->left=plrr;
  plr->left=pl;
  plr->right=p;

  set_NodeHeight(pl);
  set_NodeHeight(p);
  set_NodeHeight(plr);

  if(p==Root){Root=plr;}
  else{
    if(indicator==1){parent->right=plr;}
    else{parent->left=plr;}
  }
}

void RL_rotation(Node* p,Node* parent,int indicator){
  Node* pr=p->right;   //p's right child
  Node* prl=pr->left;  //pr's right child
  Node* prll=prl->left;  //prl's left child
  Node* prlr=prl->right;  //prl's right child

  pr->left=prlr;
  p->right=prll;
  prl->left=p;
  prl->right=pr;

  set_NodeHeight(pr);
  set_NodeHeight(p);
  set_NodeHeight(prl);

  if(p==Root){Root=prl;}
  else{
    if(indicator==1){parent->right=prl;}
    else{parent->left=prl;}
  }
}


void AVL_delete(Node* m, int key, Node* p, int i=0){      //p will initially Root but it points to parent node of the node which has to be deleted.
    int node_degree=0;
    Node* w=NULL;  //rotation would be done across w(for 2 degree node deletion w=same position where deletion has done and for 0/1 degree node deletion w=parent of that node where deletion has done)
    Node* parent=NULL;  //parent is the parent node of w
  if(key==m->val){                                                    //i indicates to left or right node of p (for left=-1,for right=1)

       if(m->left==NULL && m->right==NULL){ node_degree=0;
                                            w=p;
                                           if(i==-1){p->left=NULL; delete m;}   //when target node is leaf node.
                                           else if(i==1){p->right=NULL; delete m;}
                                           else{delete m;}}
       else if((m->left==NULL && m->right!=NULL)||(m->left!=NULL && m->right==NULL)){  //when target node is a one degree node.
              node_degree=1;
              w=p;
              if(i==1){
                if(m->left!=NULL){p->right=m->left; delete m;}
                else{p->right=m->right; delete m;}
              }
              else{
                if(m->left!=NULL){p->left=m->left; delete m;}
                else{p->left=m->right; delete m;}
              }
       }
       else{                          //when target node is a two degree node.
          node_degree=2;
          Node* predecessor=inorder_predecessor(m);
          Node* parent_predecessor=parent_of_predecessor_or_successor(predecessor);
          Node* successor=inorder_successor(m);
          Node* parent_successor=parent_of_predecessor_or_successor(successor);

          //here we take predecessor as our 1st priority to take place of m

          if(predecessor!=NULL){       //if m have predecessor then set it at place of m
             w=predecessor;
            if(predecessor!=m->left){
            if(predecessor->left==NULL && predecessor->right!=NULL){   //if predecessor is leaf degree node
                 parent_predecessor->right=NULL;
                 predecessor->right=m->right;
                 predecessor->left=m->left;
                 m->left=NULL;
                 m->right=NULL;
                 delete m;
                 if(i==1){p->right=predecessor;}
                 else{p->left=predecessor;}
            }
            else{    //if predecessor is 1 degree node
                parent_predecessor->right=predecessor->left;
                predecessor->right=m->right;
                predecessor->left=m->left;
                m->left=NULL;
                m->right=NULL;
                delete m;
                if(i==1){p->right=predecessor;}
                else{p->left=predecessor;}}
              }
              else{                  //if predecessor is left child of m.
                if(predecessor->left==NULL && predecessor->right!=NULL){
                  predecessor->right=m->right;
                  predecessor->left=NULL;
                  m->left=NULL;
                  m->right=NULL;
                  delete m;
                  if(i==1){p->right=predecessor;}
                  else{p->left=predecessor;}
                }
                else{
                  predecessor->right=m->right;
                  m->left=NULL;
                  m->right=NULL;
                  delete m;
                  if(i==1){p->right=predecessor;}
                  else{p->left=predecessor;}
                }
               }
             }
          else{                       //if m dont have predecessor then set successor at place of m
            w=successor;
            if(successor!=m->right){
            if(successor->left==NULL && successor->right!=NULL){   //if successor is leaf node
                 parent_successor->left=NULL;
                 successor->right=m->right;
                 successor->left=m->left;
                 m->left=NULL;
                 m->right=NULL;
                 delete m;
                 if(i==1){p->right=successor;}
                 else{p->left=successor;}
            }
            else{    //if successor is 1 degree node
                parent_successor->left=successor->right;
                successor->right=m->right;
                successor->left=m->left;
                m->left=NULL;
                m->right=NULL;
                delete m;
                if(i==1){p->right=successor;}
                else{p->left=successor;}}
              }
              else{             //if successor is right child of m.
                if(successor->left==NULL && successor->right!=NULL){    //when successor is leaf node
                  successor->left=m->left;
                  successor->right=NULL;
                  m->left=NULL;
                  m->right=NULL;
                  delete m;
                  if(i==1){p->right=successor;}
                  else{p->left=successor;}
                }
                else{                  //when successor is 1 degree node
                  successor->left=m->left;
                  m->left=NULL;
                  m->right=NULL;
                  delete m;
                  if(i==1){p->right=successor;}
                  else{p->left=successor;}
                }
              }
            }
          }


      //now balance the tree by rotations after deletion.
          if(node_degree==2){parent=p;}
          else{}                       //due.......for 0/1 degree node deletion, parent of w is not defined(as now w=q);

          set_NodeHeight(w);

          //checking the balance factor for w node.
          int bf=Balance_Factor(w);
          int c=0;
          if(w->val>parent->val){c=1;}
          else{c=-1;}

          if(bf==2 && Balance_Factor(w->left)==1){
              LL_rotation(w,parent,c);
          }
          else if(bf==-2 && Balance_Factor(w->right)==-1){
             RR_rotation(w,p,c);
          }
          else if(bf==2 && Balance_Factor(w->left)==-1){
             LR_rotation(w,parent,c);
          }
          else if(bf==-2 && Balance_Factor(w->right)==1){
            RL_rotation(w,parent,c);
          }
          else{}


     }
     else if(key<m->val){AVL_delete(m->left,key,m,-1);}
     else{AVL_delete(m->right,key,m,1);}
  }


Node* inorder_predecessor(Node* q){
  q=q->left;
  while(true){
    if((q->left==NULL && q->right==NULL) || (q->left!=NULL && q->right==NULL)){ break; }  //when target node is 1/2 degree
      q=q->right;
  }
  return q;
}


Node* parent_of_predecessor_or_successor(Node* k){
    Node* a=Root;
    while(true){
      if(a->left==k || a->right==k){break;}

      if(k->val<a->val){a=a->left;}
      else{a=a->right;}
    }
    return a;
}

Node* inorder_successor(Node* q){
  q=q->right;
  while(true){
    if((q->left==NULL && q->right==NULL) || (q->left==NULL && q->right!=NULL)){ break; }  //when target node is 1/2 degree
      q=q->left;
  }
  return q;
}





void inorder_treversal(Node* ptr){
  if(ptr!=NULL){
  inorder_treversal(ptr->left);
  cout<<ptr->val<<" ";
  inorder_treversal(ptr->right);}
}




};

int main(){
int list[9]={9,15,5,20,16,8,12,3,6};
AVL_Tree t(list,9);
t.AVL_delete(t.Root,5,t.Root);
t.inorder_treversal(t.Root);

  return 0;
}
