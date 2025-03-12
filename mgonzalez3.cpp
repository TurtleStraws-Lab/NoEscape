//Moises Gonzalez
//2/11/25
//
//
//No Escape
//mgonzalez3.cpp

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "mgonzalez3.h"
#include <string>




using namespace std;

void printCenteredText(const string& text) {
    // Get terminal width using tput (it returns the number of columns)
    int consoleWidth = 0;
    FILE* pipe = popen("tput cols", "r");  // Executes tput cols
    if (pipe) {
        fscanf(pipe, "%d", &consoleWidth);
        fclose(pipe);
    }
    else {
        cerr << "Error retrieving terminal width!" << endl;
        return;
    }

    // Calculate the position to start printing
    int x = (consoleWidth - text.length()) / 2;  // Horizontal center

    // Print spaces to center the text and then print the text itself
    cout << string(x, ' ') << text <<endl;
}



void mgonzalez3::sayHello() {

string answer;
        
    system( "@cls || clear");
    printCenteredText("Make your window fullscreen");
    printCenteredText("Did you do it?");
    cin >> answer;
    if (answer == "yes")
    { 
    system( "@cls || clear");
    }
    else if(answer == "no"){
     system( "@cls || clear");
     printCenteredText("Well do it!");
     sleep(2);
     system( "@cls || clear");
     printCenteredText("Did ya? :)");
     cin >> answer;
    }
    
    if (answer == "yes")
    { 
    system( "@cls || clear");
    printCenteredText("About time");
    }
    else if(answer == "no")
    {
     system( "@cls || clear");
     printCenteredText("I know where you live");
     sleep(1.5);
     system( "@cls || clear");
     printCenteredText("I mean, come on friend its a really fun game");
     printCenteredText("So are you gonna make it full screen (yes/no)");
     cin >> answer;
    }
    
    if (answer == "yes")
    { 
    system( "@cls || clear");
    printCenteredText("Enjoy ~-~");
    }
    else if (answer == "no")
    {
     system( "@cls || clear");
     printCenteredText("THEN YOU DON'T GET TO PLAY");

    printCenteredText("     _______     ");
    printCenteredText("    /       \\    ");
    printCenteredText("   |  O   O  |   ");
    printCenteredText("   |    ^    |   ");
    printCenteredText("   |   ---   |   ");
    printCenteredText("    \\_______/    ");
    printCenteredText("      |   |       ");
    printCenteredText("      |   |       ");
    printCenteredText("     /     \\      ");
    printCenteredText("    /       \\     ");
    printCenteredText("   /         \\    ");
    printCenteredText("  /           \\   ");
    printCenteredText(" /_____________\\  ");
     
     
    printCenteredText("               _____  ");
    printCenteredText("             /       \\ ");
    printCenteredText("            |  O   O  |");
    printCenteredText("            |    ^    |");
    printCenteredText("            |   ___   |");
    printCenteredText("             \\_______/ ");
    printCenteredText("            /         \\ ");
    printCenteredText("           /  /\\     /\\ \\ ");
    printCenteredText("          /  /  \\___/  \\ \\ ");
    printCenteredText("         (  (           )  )");
    printCenteredText("          \\  \\         /  / ");
    printCenteredText("           \\  \\_______/  / ");
    printCenteredText("            \\___________/  ");


     exit(0);
    }


    system( "@cls || clear");
    string text1 = "Welcome to the most amazing";
    printCenteredText(text1);
    sleep(2);
    system( "@cls || clear");
    string text2 = "Spectactular";
    printCenteredText(text2);
    sleep(2);
    system( "@cls || clear");
    string text3 = "MAZE GAME NO ESCAPE";
    printCenteredText(text3);
    sleep(2);
    system( "@cls || clear");
    string text4 = "Wait, it didnt work";
    printCenteredText(text4);
    sleep(2);
    system( "@cls || clear");
    string text5 = "THERE IT IS";
    printCenteredText(text5);
    system( "@cls || clear");
}
