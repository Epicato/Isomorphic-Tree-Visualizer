
#include "Tree_window.h"
#include <iostream>
#include <FL/Fl_Button.H>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <queue>
//------------------------------------------------------------------------------

map<string,void*> Buttonmap;    // it stores the map relationship between treenode's name and its button pointer
map<string,string> Nodemap;     // it stores the map relationship between each isomorphic node pair

Tree_window::Tree_window(Point xy, int w, int h, const string& title) :
    Window(xy,w,h,title),
    next_button(Point(x_max()-70,0), 70, 20, "Exit", cb_next),
    reset_button(Point(x_max()-150,0), 70, 20, "Reset", cb_reset),
    button_pushed(false)
{
    attach(next_button);    
    attach(reset_button);    
}

//------------------------------------------------------------------------------

bool Tree_window::wait_for_button()
// modified event loop:
// handle all events (as per default), quit when button_pushed becomes true
// this allows graphics without control inversion
{
    show();
    button_pushed = false;
#if 1
    // Simpler handler
    while (!button_pushed) Fl::wait();
    Fl::redraw();
#else
    // To handle the case where the user presses the X button in the window frame
    // to kill the application, change the condition to 0 to enable this branch.
    Fl::run();
#endif
    return button_pushed;
}

//------------------------------------------------------------------------------

void Tree_window::cb_next(Graph_lib::Address, Graph_lib::Address pw)
// call Tree_window::next() for the window located at pw
{
    Graph_lib::reference_to<Tree_window>(pw).next();

}

void Tree_window::cb_reset(Graph_lib::Address, Graph_lib::Address pw)
// call Tree_window::reset() for the window located at pw
{
    Graph_lib::reference_to<Tree_window>(pw).reset();

}

//------------------------------------------------------------------------------

void Tree_window::next()
{
    button_pushed = true;
    hide();
}

void Tree_window::reset()
{
    
    for(auto &node:Buttonmap)
    {   //reset all nodes' colors
        Graph_lib::reference_to<Fl_Button>(node.second).color(FL_BACKGROUND_COLOR);
        Graph_lib::reference_to<Fl_Button>(node.second).redraw();
    }
}

//------------------------------------------------------------------------------
void Tree_window::cb_node(Graph_lib::Address me, Graph_lib::Address pw)
{
    if(!Nodemap.empty())//if the trees are isomorphic
    {
        //highlight the node you click on
        Graph_lib::reference_to<Fl_Button>(me).color(FL_YELLOW);

        //get the name with idx(@1 or @2) of the node we click on
        //We don't directly use ##string name = Graph_lib::reference_to<Fl_Button>(me).label(); ## here because we want to know it's @1 or @2
        string name;
        for(auto &meNode:Buttonmap)
        {
            if(meNode.second == me)
            {
                name = meNode.first;
                break;
            }
        }
            
        //find the isomorphic node on the other tree
        //we actually find the pointer to the node button here
        map<string,void*>::iterator itButton;
        
        for(auto &itNode:Nodemap)
        {
            if(itNode.first == name)
            {
                itButton=Buttonmap.find(itNode.second);
                break;
            }
            else if(itNode.second == name)
            {
                itButton=Buttonmap.find(itNode.first);
                break;
            }
        }
        
        //highlight the isomorphic node
        Graph_lib::reference_to<Fl_Button>(itButton->second).color(FL_GREEN);
        Graph_lib::reference_to<Fl_Button>(itButton->second).redraw();
    }  
}



void Tree_window::drawButtons(Node* t,int x,int y,int treeIdx)
{
    //draw the node buttons recursively
    Graph_lib::TreeButton father_button(Point(x-10,y-5), 20, 10,t->name,cb_node,treeIdx);
    attach(father_button);
    
    int childrenCNT = t->Children.size();
    for(int i=0;i<childrenCNT;i++)
    {
        //decide the location of the children buttons
        int vertical_interval = 80;
        int child_y = y + vertical_interval; //the vertical interval is fixed
        int depth = t->depth+1;
        t->Children[i]->depth = depth;  //update the depth of the child node
        int horizontal_interval;    //the horizontal interval is up to depth and childrencount
        if(depth==1)    horizontal_interval = 200;
        else if(depth==2)   horizontal_interval = 15*(10-childrenCNT);
        else if(depth==3)   horizontal_interval = 10*(10-childrenCNT/1.2);
        else if(depth==4)   horizontal_interval = 4*(10-childrenCNT);
        else if(depth==5)   horizontal_interval = 3*(10-childrenCNT);
        else    horizontal_interval = 2*(10-childrenCNT);

        int child_x = x-(horizontal_interval*(childrenCNT-1))/2 + horizontal_interval*i;
        

        //then draw the child node
        drawButtons(t->Children[i],child_x,child_y,treeIdx);
    }
    
}

void Tree_window::solveIsomorphic(Node* t)  //assign canonical names (tag)
{
    vector<string> tagList;
    if(t->Children.empty())     //leaf node
    {
        t->tag = "10";
    }
    else
    {
        vector<string> tagList;
        for(auto &i:t->Children) 
        {
            solveIsomorphic(i);
            tagList.push_back(i->tag);
        }
        sort(tagList.begin(),tagList.end());
        string temp;
        for(auto &i:tagList) temp += i;
        t->tag = "1" + temp + "0";
    }   
}


bool compareNode(Node* x,Node* y)	//compare function for sorting by level
{
    if(x->depth < y->depth) return true;
    else if(x->depth == y->depth)    return (x->tag < y->tag);
    else return false;
}

void Tree_window::mapNodes(Node* t1, Node* t2)
{
    //In this function, we want to save the pair of isomorphic nodes in a Nodemap
    vector<Node*> q;
    q.push_back(t1);
    while(!q.empty())
    {   
        tree1.push_back(q[0]);
        for(auto &i:q[0]->Children)  q.push_back(i);    // visit the tree by level
        q.erase(q.begin());
        sort(q.begin(),q.end(),compareNode);    //sort each level of the tree
    }
    q.push_back(t2);
    while(!q.empty())
    {   
        tree2.push_back(q[0]);
        for(auto &i:q[0]->Children)  q.push_back(i);    // visit the tree by level
        q.erase(q.begin());
        sort(q.begin(),q.end(),compareNode);    //sort each level of the tree
    }
    
    //after sorting, the nodes with the same index are isomorphic nodes
    for(int i=0;i<tree1.size();i++)
    {
        // we plus "@1/2" after the original node name in case there are same names in different trees
        Nodemap.insert(pair <string,string> (tree1[i]->name + "@1",tree2[i]->name + "@2")); 
    }
}
