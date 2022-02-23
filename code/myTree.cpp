#include "MyTree.h"




myTree::myTree(int x, int y)
{
	err = false;
	root = NULL;
    root_x = x;
    root_y = y;
}

myTree::~myTree()
{
	clear(root);
}

void myTree::clear(Node* t)
{
	if(t->Children.empty())	return;
	else
	for(int i=0;i<t->Children.size();i++)
	{
		clear(t->Children[i]);
	}
}

void myTree::printNodes(Node* t)// utils function if we want to print out the nodes
{
    cout<<t->name<<endl;
	if(t->Children.empty()) return;

	for(int i=0;i<t->Children.size();i++)
	{
		printNodes(t->Children[i]);
	}
}

void myTree::scanfile(string s) //scan the file to create a tree
{
    ifstream ifs(s,ios::in);
    if(!ifs)
    {
        cout<<"Fail to open file"+s+"!!!";
    }
    string input;
    vector <Node*> Nodelist;   // the nodes been read already

    while(getline(ifs,input))   //read every line in the file 
    {
        int idx=0;
        string n1,n2;   //n1 is father's name, n2 is child's name
        bool after = false;     //whether we are reading after "->" 
        while(idx<input.size()) //solve the nodes' relationships
        {
            if(!after)  //not after"->" yet,reading at father node
            {
                if(input[idx]=='-'&&input[idx+1]=='>') //->
                {
                    after = true;
                    idx += 2;
                }
                else
                {
                    n1+=input[idx];
                    idx++;
                }
            }
            else       //after"->",reading at child node
            {
                n2+=input[idx];
                idx++;
            }
        }
        if(n1==n2) //a node can't point to itself
        {
            err = true;
            cout<<"A node can't point to itself!!!"<<endl;
            break;
        }; 

        bool fatherExists = false;
        bool childExists = false;
        Node* father=NULL;
        Node* child=NULL;

        for(int i=0;i<Nodelist.size();i++) //check whether father or child node in this line has already appeared
        {
            if(!fatherExists && Nodelist[i]->name == n1) //father node exists
            {
                father = Nodelist[i];
                fatherExists = true;
            }
            if(!childExists && Nodelist[i]->name == n2) //child node exists
            {
                child = Nodelist[i];
                childExists = true;
            }
            
        }

        //if the child node has two different fathers, it's an invalid tree
        if(childExists) 
        {   if(child->hasFather)    // it means the current node appeared as a child before
            {
                err = true; //if father doesn't exist, the child must have two different fathers
                if(fatherExists)    //if father exists, check whether the same line appeared before
                {
                    for(int i=0;i<father->Children.size();i++)
                    {
                        if(father->Children[i]->name == child->name)
                        {
                            err = false;
                            break;
                        }
                    }
                }
                if(err == true)
                {
                    cout<<"Node "<<child->name<<" has two different fathers!!!"<<endl;
                    break;
                }
                else continue; // because this line appeared once before, so we just jump to the next line
            }
        }

        if(!fatherExists)   //allocate memory for the new nodes if they appear for the first time
        {
            father = new Node;
            father->name = n1;
            Nodelist.push_back(father);
        }
        if(!childExists)
        {
            child = new Node;
            child->name = n2;
            Nodelist.push_back(child);
        }
        
        father->Children.push_back(child);
        child->hasFather = true;
                        
    }
    int rootcnt=0;
    for(int i=0;i<Nodelist.size();i++)  //find the root
    {
        if(! Nodelist[i]->hasFather)
        {
            root = Nodelist[i];
            rootcnt++;
        }
    }
    if(rootcnt!=1)  //if the root number is not one, it's an invalid tree
    {
        err = true;
        cout<<"There are more than one root!!!"<<endl;
    };   

}

void myTree::draw_lines() const
{
    drawNodes(root,root_x,root_y);
}

void myTree::drawNodes(Node* t,int x,int y) const   //this function draws the tree's framework(lines and boxes)
{
    //draw this node
    Graph_lib::Rectangle node(Point(x-10,y-5),20,10);
    
    node.draw();
    

    // Text nodename(Point(x-6,y+3),t->name);   //label can be drawn when you draw the button
    // nodename.set_font_size(10);
    // nodename.draw();
    
    //draw this node's children nodes recursively
    int childrenCNT = t->Children.size();
    for(int i=0;i<childrenCNT;i++)
    {
        int vertical_interval = 80;
        int child_y = y + vertical_interval; //the vertical interval is fixed
        int depth = t->depth + 1;   //depth is calculated when you draw the buttons before

        int horizontal_interval;    //the horizontal interval is up to depth and childrencount
        if(depth==1)    horizontal_interval = 200;
        else if(depth==2)   horizontal_interval = 15*(10-childrenCNT);
        else if(depth==3)   horizontal_interval = 10*(10-childrenCNT/1.2);
        else if(depth==4)   horizontal_interval = 4*(10-childrenCNT);
        else if(depth==5)   horizontal_interval = 3*(10-childrenCNT);
        else    horizontal_interval = 2*(10-childrenCNT);

        int child_x = x-(horizontal_interval*(childrenCNT-1))/2 + horizontal_interval*i;
        
        //draw a line to connect father and child first
        Line fatherTOchild(Point(x,y+5),Point(child_x,child_y-5));
        fatherTOchild.draw();
        //then draw the child node
        
        drawNodes(t->Children[i],child_x,child_y);
    }
    
}
