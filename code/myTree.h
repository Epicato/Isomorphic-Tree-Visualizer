#ifndef MYTREE_H_
#define MYTREE_H_


#include "Tree_window.h"    // get access to our window library
#include "Graph.h"            // get access to our graphics library facilities
#include <iostream>
#include <vector>


using namespace Graph_lib;


class myTree : public Graph_lib::Shape
{
	
    void draw_lines() const;
    void drawNodes(Node* t, int x, int y) const;
    int root_x;
    int root_y;
public:
	Node* root;
	bool err=false;
	myTree(int x,int y);
	~myTree();
	void scanfile(string s);
	void clear(Node* t);
    void printNodes(Node* t);
    
};





#endif
