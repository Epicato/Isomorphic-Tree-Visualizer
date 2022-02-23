

#ifndef TREE_WINDOW_GUARD
#define TREE_WINDOW_GUARD 1

#include "GUI.h"    
#include "Graph.h"
#include <FL/Fl_Button.H>
#include <vector>
//using namespace Graph_lib;
struct Node     //tree node 
{
	string name; //real name
    string tag; //canonical name (1...0)
	vector<Node*> Children;
    bool hasFather = false;
    int depth=0;
};
//------------------------------------------------------------------------------

struct Tree_window : Graph_lib::Window {
    Tree_window(Point xy, int w, int h, const string& title);

    bool wait_for_button(); // simple event loop
    void drawButtons(Node* t,int x,int y,int treeIdx);
    void solveIsomorphic(Node* t);
    void mapNodes(Node* t1, Node* t2);
private:
    
    Graph_lib::Button next_button;     // the "exit" button
    Graph_lib::Button reset_button;     // the "reset" button
    static void cb_next(Graph_lib::Address, Graph_lib::Address); // callback for next_button
    static void cb_node(Graph_lib::Address, Graph_lib::Address); // callback for node_button
    static void cb_reset(Graph_lib::Address, Graph_lib::Address); // callback for reset_button
    bool button_pushed;     // implementation detail
    vector<Node*> tree1;    // root ptr
    vector<Node*> tree2;
    void next();            // action to be done when next_button is pressed
    void reset();           // set all buttons dark
    
};

//------------------------------------------------------------------------------

#endif 
