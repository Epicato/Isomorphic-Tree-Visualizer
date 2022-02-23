#include "MyTree.h"
#include "Tree_window.h"    // get access to our window library


int main()
{
    /********** Begin **********/
    string file1,file2;
    cout<<"Please enter the name of file 1:\n";
    cin>>file1;
    cout<<"Please enter the name of file 2:\n";
    cin>>file2;
    int x1,x2,y1,y2;
    x1 = 350;   y1 = 20;    x2 = 900;   y2 = 20;    //location of root nodes
    myTree Tree1(x1,y1);    //class myTree is used to scan files to create trees and draw the frameworks
    myTree Tree2(x2,y2);
    //read from files
    Tree1.scanfile(file1);
    Tree2.scanfile(file2);
    if (Tree1.err)  cout<<"The tree in file "+file1+" is invalid!!!\n";
    if (Tree2.err)  cout<<"The tree in file "+file2+" is invalid!!!\n";
    if (!Tree1.err && !Tree2.err) //both trees are valid, draw them on the canvas
    {
        using namespace Graph_lib;   // our graphics facilities are in Graph_lib

        Point tl(0,0);           // to become top left  corner of window

        Tree_window win(tl,1260,780,"Canvas");    // make a Tree window and draw the Buttons

        win.drawButtons(Tree1.root,x1,y1,1);    //draw buttons on the window recursively
        win.drawButtons(Tree2.root,x2,y2,2);

        win.attach (Tree1);           // connect Tree framework to the window
        win.attach (Tree2);


        win.solveIsomorphic(Tree1.root);    //check whether two trees are isomorphic
        win.solveIsomorphic(Tree2.root);
        if(Tree1.root->tag==Tree2.root->tag)
        {
            win.mapNodes(Tree1.root,Tree2.root);    //save pairs of isomorphic nodes to a nodeMap
            cout<<"The two trees are isomorphic!!!";
        }
        else 
        {
            cout<<"The two trees are not isomorphic!!!";
        }
      
        //win.wait_for_button();       // give control to the display engine
        return gui_main();
    }


	/********** End **********/
  	return 0;
}
