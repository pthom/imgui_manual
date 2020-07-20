# ImGui Playground

This is the playground. You can here test a feature in two quick steps:
* #include "playground.h"
* create a function named "Playground" and fill it with whatever you want to test

For example, if you want to play with checkboxes, create a file `test_checkbox.cpp` with the following content

````cpp
#include "playground.h"
void Playground() {
    static bool selected = false;
    /*bool changed =*/ ImGui::Checkbox("Check me", &selected);
}
````

After adding a new cpp file, just run cmake, so that your new file gets discovered:

````bash
    cmake .
````

And then compile it:
````bash
make
````    

After the build, your program should be visible in the playground build folder.
