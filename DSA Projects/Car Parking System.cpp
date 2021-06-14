#include<ctime>
#include<chrono>
#include<random>
#include<map>
#include<bits/stdc++.h>
#include<iterator>
#include <queue>

using namespace std;
enum NODE_COLOR { RED, BLACK };

class NODE_RB {
public:
  int valw;
  NODE_COLOR color;
  NODE_RB *le, *ri, *par;

  NODE_RB(int valw) : valw(valw) {
    par = le = ri = NULL;


    color = RED;
  }


  NODE_RB *uncle1N_node() {

    if (par == NULL or par->par == NULL)
      return NULL;

    if (par->isOnLeft())

      return par->par->ri;
    else

      return par->par->le;
  }


  bool isOnLeft() { return this == par->le; }


  NODE_RB *sibli() {

    if (par == NULL)
      return NULL;

    if (isOnLeft())
      return par->ri;

    return par->le;
  }


  void moved1(NODE_RB *nParent) {
    if (par != NULL) {
      if (isOnLeft()) {
        par->le = nParent;
      } else {
        par->ri = nParent;
      }
    }
    nParent->par = par;
    par = nParent;
  }

  bool hadredchild() {
    return (le != NULL and le->color == RED) or
           (ri != NULL and ri->color == RED);
  }
};

class RedBlackTree {
  NODE_RB *root;


  void leRotate(NODE_RB *x) {

    NODE_RB *nParent = x->ri;


    if (x == root)
      root = nParent;

    x->moved1(nParent);


    x->ri = nParent->le;

    if (nParent->le != NULL)
      nParent->le->par = x;


    nParent->le = x;
  }

  void riRotate(NODE_RB *x) {

    NODE_RB *nParent = x->le;


    if (x == root)
      root = nParent;

    x->moved1(nParent);


    x->le = nParent->ri;

    if (nParent->ri != NULL)
      nParent->ri->par = x;

    nParent->ri = x;
  }

  void swapColors(NODE_RB *x1, NODE_RB *x2) {
    NODE_COLOR temp;
    temp = x1->color;
    x1->color = x2->color;
    x2->color = temp;
  }

  void swapValues(NODE_RB *u, NODE_RB *v) {
    int temp;
    temp = u->valw;
    u->valw = v->valw;
    v->valw = temp;
  }


  void fixRedRed(NODE_RB *x) {

    if (x == root) {
      x->color = BLACK;
      return;
    }


    NODE_RB *par = x->par, *grandpar = par->par,
         *uncle1N_node = x->uncle1N_node();

    if (par->color != BLACK) {
      if (uncle1N_node != NULL && uncle1N_node->color == RED) {

        par->color = BLACK;
        uncle1N_node->color = BLACK;
        grandpar->color = RED;
        fixRedRed(grandpar);
      } else {

        if (par->isOnLeft()) {
          if (x->isOnLeft()) {

            swapColors(par, grandpar);
          } else {
            leRotate(par);
            swapColors(x, grandpar);
          }

          riRotate(grandpar);
        } else {
          if (x->isOnLeft()) {

            riRotate(par);
            swapColors(x, grandpar);
          } else {
            swapColors(par, grandpar);
          }


          leRotate(grandpar);
        }
      }
    }
  }


  NODE_RB *successor(NODE_RB *x) {
    NODE_RB *temp = x;

    while (temp->le != NULL)
      temp = temp->le;

    return temp;
  }


  NODE_RB *BSTreplace(NODE_RB *x) {

    if (x->le != NULL and x->ri != NULL)
      return successor(x->ri);


    if (x->le == NULL and x->ri == NULL)
      return NULL;


    if (x->le != NULL)
      return x->le;
    else
      return x->ri;
  }

  // deletes the given node
  void deleteNODE_RB(NODE_RB *v) {
    NODE_RB *u = BSTreplace(v);

    // True when u and v are both black
    bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK));
    NODE_RB *par = v->par;

    if (u == NULL) {
      // u is NULL therefore v is leaf
      if (v == root) {
        // v is root, making root null
        root = NULL;
      } else {
        if (uvBlack) {
          // u and v both black
          // v is leaf, fix double black at v
          fixDoubleBlack(v);
        } else {
          // u or v is red
          if (v->sibli() != NULL)
            // sibli is not null, make it red"
            v->sibli()->color = RED;
        }

        // delete v from the tree
        if (v->isOnLeft()) {
          par->le = NULL;
        } else {
          par->ri = NULL;
        }
      }
      delete v;
      return;
    }

    if (v->le == NULL or v->ri == NULL) {
      // v has 1 child
      if (v == root) {
        // v is root, assign the valwue of u to v, and delete u
        v->valw = u->valw;
        v->le = v->ri = NULL;
        delete u;
      } else {
        // Detach v from tree and move u up
        if (v->isOnLeft()) {
          par->le = u;
        } else {
          par->ri = u;
        }
        delete v;
        u->par = par;
        if (uvBlack) {
          // u and v both black, fix double black at u
          fixDoubleBlack(u);
        } else {
          // u or v red, color u black
          u->color = BLACK;
        }
      }
      return;
    }

    // v has 2 children, swap valwues with successor and recurse
    swapValues(u, v);
    deleteNODE_RB(u);
  }

  void fixDoubleBlack(NODE_RB *x) {
    if (x == root)
      // Reached root
      return;

    NODE_RB *sibli = x->sibli(), *par = x->par;
    if (sibli == NULL) {
      // No sibiling, double black pushed up
      fixDoubleBlack(par);
    } else {
      if (sibli->color == RED) {
        // Sibling red
        par->color = RED;
        sibli->color = BLACK;
        if (sibli->isOnLeft()) {
          // le case
          riRotate(par);
        } else {
          // ri case
          leRotate(par);
        }
        fixDoubleBlack(x);
      } else {
        // Sibling black
        if (sibli->hadredchild()) {
          // at least 1 red children
          if (sibli->le != NULL and sibli->le->color == RED) {
            if (sibli->isOnLeft()) {
              // le le
              sibli->le->color = sibli->color;
              sibli->color = par->color;
              riRotate(par);
            } else {
              // ri le
              sibli->le->color = par->color;
              riRotate(sibli);
              leRotate(par);
            }
          } else {
            if (sibli->isOnLeft()) {
              // le ri
              sibli->ri->color = par->color;
              leRotate(sibli);
              riRotate(par);
            } else {
              // ri ri
              sibli->ri->color = sibli->color;
              sibli->color = par->color;
              leRotate(par);
            }
          }
          par->color = BLACK;
        } else {
          // 2 black children
          sibli->color = RED;
          if (par->color == BLACK)
            fixDoubleBlack(par);
          else
            par->color = BLACK;
        }
      }
    }
  }
  void levelOrder(NODE_RB *x) {
    if (x == NULL)

      return;

    queue<NODE_RB *> q;
    NODE_RB *curr;

    q.push(x);

    while (!q.empty()) {

      curr = q.front();
      q.pop();


      cout << curr->valw << " ";

      if (curr->le != NULL)
        q.push(curr->le);
      if (curr->ri != NULL)
        q.push(curr->ri);
    }
  }

  void inorder(NODE_RB *x) {
    if (x == NULL)
      return;
    inorder(x->le);
    cout << x->valw << " ";
    inorder(x->ri);
  }

public:

  RedBlackTree() { root = NULL; }

  NODE_RB *getRoot() { return root; }


  NODE_RB *search(int n) {
    NODE_RB *temp = root;
    while (temp != NULL) {
      if (n < temp->valw) {
        if (temp->le == NULL)
          break;
        else
          temp = temp->le;
      } else if (n == temp->valw) {
        break;
      } else {
        if (temp->ri == NULL)
          break;
        else
          temp = temp->ri;
      }
    }

    return temp;
  }


  void insert(int n) {
    NODE_RB *newNODE_RB = new NODE_RB(n);
    if (root == NULL) {

      newNODE_RB->color = BLACK;
      root = newNODE_RB;
    } else {
      NODE_RB *temp = search(n);

      if (temp->valw == n) {

        return;
      }


      newNODE_RB->par = temp;

      if (n < temp->valw)
        temp->le = newNODE_RB;
      else
        temp->ri = newNODE_RB;


      fixRedRed(newNODE_RB);
    }
  }


  void deleteByVal(int n) {
    if (root == NULL)

      return;

    NODE_RB *v = search(n), *u;

    if (v->valw != n) {
      cout << "No node found to delete with valwue:" << n << endl;
      return;
    }

    deleteNODE_RB(v);
  }

  void printInOrder() {
    cout << "Inorder: " << endl;
    if (root == NULL)
      cout << "Tree is empty" << endl;
    else
      inorder(root);
    cout << endl;
  }


  void printLevelOrder() {
    cout << "Level order: " << endl;
    if (root == NULL)
      cout << "Tree is empty" << endl;
    else
      levelOrder(root);
    cout << endl;
  }
};

std::string random_string(std::size_t length)
{
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    std::string random_string;

    for (std::size_t i = 0; i < length; ++i)
    {
        random_string += CHARACTERS[distribution(generator)];
    }

    return random_string;
}
int removeColon(string s)
{
    if (s.size() == 4)
        s.replace(1, 1, "");

    if (s.size() == 5)
        s.replace(2, 1, "");

    return stoi(s);
}
int diff(string s1, string s2)
{
    int time1 = removeColon(s1);
    int time2 = removeColon(s2);

    int hourDiff = time2 / 100 - time1 / 100 - 1;
    int minDiff = time2 % 100 + (60 - time1 % 100);

    if (minDiff >= 60) {
        hourDiff++;
        minDiff = minDiff - 60;
    }
    int x=hourDiff*3600+minDiff*60;
    return x;
}
void insertion_of_a_car(map<int,string>&data, int d)
{
    RedBlackTree tree;
    time_t start;
     string arrivalwtime;
     string c, text, coupon;
     cout<<"\tENTER CAR NUMBER (without spaces) : ";
     cin>>c;
     cout<<endl;
     cout<<"\tEnter the arrivalw time of the car : ";
     cin>>arrivalwtime;
     cout<<endl;
     cout<<"\tYour token number is : "<<d;
      tree.insert(d);
      cout<<endl<<endl<<"\tYOUR CAR HAS BEEN SUCCESSFULLY PARKED!"<<endl<<endl;
       cout<<"\tThis car was parked at : ";
       auto givemetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
   cout << ctime(&givemetime) << endl;
   data[d]=arrivalwtime;
}
void deletion_of_a_car(map<int,string>&data)
{
    RedBlackTree tree;
    string deptime;
    cout<<"\tPlease enter your token number : ";
    int s;
    cin>>s;
    cout<<endl;
    cout<<"\tEnter the departure time of the car : ";
    cin>>deptime;
    cout<<endl;
    tree.deleteByVal(s);
    cout<<"\tExit time :";
    auto givemetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    cout << ctime(&givemetime) << endl;
    auto it=data.find(s);
    string starttime=(*it).second;
    int x=diff(starttime,deptime);
    string yes;

    int hours=x/3600;
    int minutes=(x%3600)/60;
    cout << "\tThis Car Stayed here for = " << hours <<" hours "<< minutes<<" minutes "<<endl<<endl<<endl;
    cout<<"\t==== PARKING CHARGES ===="<<endl<<endl;
    cout<<"\tDo you have a VIP number?"<<endl<<endl;
    cout<<"\tEnter YES/ NO : ";
    cin>>yes;
    cout<<endl;
    if(yes=="YES" || yes=="yes"){
    cout<<"\tHURRY YOU'VE GOT A 10% DISCOUNT !!"<<endl<<endl;
    int z=x*10;
    z=z/10;
    cout<<"\tThe amount to be paid is : Rs "<<x*2-z<<endl<<endl;
}
else{
    cout<<"\tThe amount to be paid is : Rs "<<x*2<<endl<<endl;
}
}
int login(){
    string str;
    cout <<"\n\n\n\n\n\n\n\t\t\t\t\t\t\tCarParking Reservation System Login";
    cout << "\n\n\n\n\n\n\n\t\t\t\t\t\t\t\tEnter Password: ";
    cin>>str;
    if(str=="pass"){
        cout << "\n\n\n\n\t\t\t\t\t\t\tAccess Granted! Welcome To Our System \n\n";
        system("PAUSE");
    }
    else{
      cout << "\n\n\n\n\t\t\t\t\t\t\tAccess Aborted...Please Try Again!!\n";
      system("PAUSE");
      system("CLS");
      login();
    }
}
int main() {
    login();
    map<int,string> data;
    int t;
    int d=0;
    t=0;
    while(t<50){
    cout<<endl;
    cout << "                                                             CAR PARKING RESERVATION SYSTEM"<<endl<<endl<<"                                                              *Number of Slots Available : "<<50-t<<"*";
    cout <<endl<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"                       1. Arrivalw of a Car"<<endl<<endl;
    cout<<"                       2. Departure of the car"<<endl<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"                       Select Your Choice :: ";
    int x;
    cin>>x;
    cout<<endl<<endl;
    if(x==1)
    {
       insertion_of_a_car(data, d);
       t++;
       d++;
    }
    else
    {
       deletion_of_a_car(data);
       t--;
    }
}
}
