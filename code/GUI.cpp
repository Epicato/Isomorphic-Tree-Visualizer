
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include "GUI.h"
#include <iostream>
#include <string>

namespace Graph_lib {

//------------------------------------------------------------------------------

void Button::attach(Window& win)
{
    pw = new Fl_Button(loc.x, loc.y, width, height, label.c_str());
    pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
    own = &win;
}

void TreeButton::attach(Window& win)
{
    
    pw = new Fl_Button(loc.x, loc.y, width, height);
    pw->copy_label(label.c_str());
    pw->labelsize(10);

    label += "@" + to_string(idx);    // this means we add an index_tag(@1 or @2) after the label, to distinguish node on tree1 or tree2

    // add the map of button and label(with its idx) to a Buttonmap, so we can use the label to find the ptr of a button later
    Buttonmap.insert(pair <string,void*> (label,pw)); 
    
    pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
    
    own = &win;
}

//------------------------------------------------------------------------------

int In_box::get_int()
{
    Fl_Input& pi = reference_to<Fl_Input>(pw);
    // return atoi(pi.value());
    const char* p = pi.value();
    if (!isdigit(p[0])) return -999999;
    return atoi(p);
}

//------------------------------------------------------------------------------

void In_box::attach(Window& win)
{
    pw = new Fl_Input(loc.x, loc.y, width, height, label.c_str());
    own = &win;
}

//------------------------------------------------------------------------------

void Out_box::put(const string& s)
{
    reference_to<Fl_Output>(pw).value(s.c_str());
}

//------------------------------------------------------------------------------

void Out_box::attach(Window& win)
{
    pw = new Fl_Output(loc.x, loc.y, width, height, label.c_str());
    own = &win;
}

//------------------------------------------------------------------------------

int Menu::attach(Button& b)
{
    b.width = width;
    b.height = height;

    switch(k) {
    case horizontal:
        b.loc = Point(loc.x+offset,loc.y);
        offset+=b.width;
        break;
    case vertical:
        b.loc = Point(loc.x,loc.y+offset);
        offset+=b.height;
        break;
    }
    selection.push_back(b); // b is NOT OWNED: pass by reference
    return int(selection.size()-1);
}

//------------------------------------------------------------------------------

int Menu::attach(Button* p)
{
    Button& b = *p;
    b.width = width;
    b.height = height;

    switch(k) {
    case horizontal:
        b.loc = Point(loc.x+offset,loc.y);
        offset+=b.width;
        break;
    case vertical:
        b.loc = Point(loc.x,loc.y+offset);
        offset+=b.height;
        break;
    }
    selection.push_back(&b); // b is OWNED: pass by pointer
    return int(selection.size()-1);
}

//------------------------------------------------------------------------------

}; // of namespace Graph_lib
