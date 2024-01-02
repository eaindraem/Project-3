//
//  main.cpp
//  project3
//
//  Created by Melody Myae on 10/22/23.
//
#include "grid.h"
#include <iostream>
#include <cassert>
#include <cctype>
#include <cmath>
#include <string>

using namespace std;

//void plotHorizontalLine(int r, int c, int distance, char ch);
//void plotVerticalLine(int r, int c, int distance, char ch);
//void plotRectangle(int r, int c, int height, int width, char ch);
const int HORIZ = 0;
const int VERT = 1;

const int FG = 0;
const int BG = 1;

bool plotLine(int r, int c, int d, int dir, char plotChar, int fgbg);
int performCommands(string commandString, char& plotChar, int& mode, int& badPos);
int distanceFunction (string& commandString, int& d, int&i);
int parseCommandString(string& commandString, int& r, int& c, int& dir, int& fgbg, char& plotChar, int& i);


int main()
{
    for (;;)
    {
        cout << "Enter the number of grid rows and columns (max 30 each): ";
        int nRows;
        int nCols;
        cin >> nRows >> nCols;
        cin.ignore(10000, '\n');
        if (nRows >= 1  &&  nRows <= MAXROWS  &&  nCols >= 1  &&  nCols <= MAXCOLS)
        {
            setSize(nRows, nCols);
            break;
        }
        cout << "The numbers must be between 1 and 30." << endl;
    }
    char currentChar = '*';
    int currentMode = FG;
    for (;;)
    {
        cout << "Enter a command string (empty line to quit): ";
        string cmd;
        getline(cin, cmd);
        if (cmd == "")
            break;
        int position = 0;
        int status = performCommands(cmd, currentChar, currentMode, position);
        switch (status)
        {
          case 0:
            draw();
            break;
          case 1:
            cout << "Syntax error at position " << position << endl;
            break;
          case 2:
            if (!isprint(currentChar))
                cout << "Current character is not printable" << endl;
            if (currentMode != FG  &&  currentMode != BG)
                cout << "Current mode is " << currentMode << ", not FG or BG" << endl;
            break;
          case 3:
            cout << "Cannot perform command at position " << position << endl;
            break;
          default:
              // It should be impossible to get here.
            cout << "performCommands returned " << status << "!" << endl;
        }
    }
}
    
    
    //function to get distance
    int distanceFunction (string& commandString, int& d, int& i)
    {
        char c = commandString.at(i);
//        int number = 0;
        bool isNegative = false;
        
        do
        {
            char c = commandString.at(i);
            
            //check it's a number & previous index is an alphabet
            if ((!isdigit(c) || !(isalpha(commandString[i-1]) || commandString[i-1] == '-')) && c != '-')
            {
                return 1;
            }
            
            
            if (isdigit(c))
            {

                d = 0;
                //distance can only be 2 digits, alphabet before
                //this loop will get the distance
                while (d < 30 && i < commandString.size() && isdigit(commandString[i]))
                {
                    d *= 10;
                    d += commandString[i] - '0';
                    i++;
                }
                if (d > 30)
                {
                    i--;
                    return 1; //distance cannot be bigger than 30
                }
                if (isNegative)
                {
                    d *= -1;
                }
                return 0;
            }
            
            //check if negative, no digits or negative spotted yet,
            else if (c == '-' && d == 0 && !isNegative)
            {
                isNegative = true;
                i++;
        
                //  distanceFunction(commandString, distance);
            }
            
            //if not digit or negative, break out of while loop
        }while (c == '-' || isdigit(c));
        
        return 1;
    }

int parseCommandString(string& commandString, int& r, int& c, int& dir, int& fgbg, char& plotChar, int& i, int&d, int& badPos)
{
    if (commandString[i] == ' ' && isalpha(commandString[i-1]) && isdigit(commandString[i+1])){
        i++;
        return 1; //return 1 if space between a command and a digit
    }
    
    else if (commandString[i] == 'v' || commandString[i] == 'V')
    {
        dir = VERT;
        if ((commandString[i+1] != '-' && !isdigit(commandString[i+1])))
        {
            i++;
            return 1; //if a number doesn't follow the v command, return 1
        }
        
        //next index to deal with integers
        i++;

        if (distanceFunction (commandString, d, i) == 1)
        {
            return 1;
        }

        if ((plotLine(r, c, d, dir, plotChar, fgbg)) == false)
        {
            i--;
            //if attempt to plot off grid, return the h or v command
            while (commandString[i] != 'h'&& commandString[i] != 'H' && commandString[i] != 'v' && commandString[i] != 'V')
            {
                i--;
            }
            badPos = i;
            return 3; //check if the line is plottable
        }

        return 0;
    }
    else if (commandString[i] == 'h' || commandString[i] == 'H')
    {
        dir = HORIZ;
        if (commandString[i+1] != '-' && !isdigit(commandString[i+1]))
        {
            i++;
            return 1; //if a number doesn't follow the h command, return 1
        }
        
        //deal with integers/distance now
        i++;
        
        if (distanceFunction (commandString, d, i) == 1)
        {
            return 1;
        }
        if ((plotLine(r, c, d, dir, plotChar, fgbg)) == false)
        {
            i--;
            //if attempt to plot off grid, return the h or v command
            while (commandString[i] != 'h'&& commandString[i] != 'H' && commandString[i] != 'v' && commandString[i] != 'V')
            {
                i--;
                if (i<0)
                {
                    break;
                }
            }
            badPos = i;
            return 3; //check if the line is plottable
        }
        
        return 0;

    }
    else if (commandString[i] == 'f' || commandString[i] == 'F')
    {
        fgbg = FG;
        i++;
        plotChar = commandString[i]; //set next char after fgbg as the new character
        if (!isprint(plotChar))
        {
            return 2; //if character isn't printable, return 2
        }
        else
        {
            i++;
            return 0;
        }
    }
    else if (commandString[i] == 'b' || commandString[i] == 'B')
    {
        fgbg = BG;
        i++;
        plotChar = commandString[i]; //set next char after fgbg as the new character
        
        if (!isprint(plotChar))
        {
            return 2; //if character isn't printable, return 2
        }
        else
        {
            i++;
            return 0;
        }
    }
    else if (commandString[i] == 'c' || commandString[i] == 'C')
    {
        clearGrid();
        i++;
        plotChar = '*';
        fgbg = FG;
        return 0;
        
    }
        
    else{
        return 1; //syntax error
    }

}

        

int performCommands(string commandString, char& plotChar, int& mode, int& badPos)
{
    int i = 0;
    int r = 1;
    int c = 1;
    int dir = -1;
    
    do{
        dir = -1;
        int d = 0;
        
        //if mode isn't foreground or background, return 2
        if (mode != FG && mode != BG)
        {
            i++;
            return 2;
        }
        
        //if empty string, quit
        if (commandString == "")
            break;
        
        //store values so that when we test for return values, the functions won't be called twice
      int parseCommandFunc = parseCommandString(commandString, r, c, dir, mode, plotChar, i, d, badPos);

        //if function return values are 1, then this function return 1
        //if syntax error, return i at syntax error
        if (parseCommandFunc == 1 )
        {
            badPos = i;
            return 1;
        }
        else if (parseCommandFunc == 3)
        {
            return 3;
        }
        else if (parseCommandFunc == 2)
        {
            return 2;
        }
                 
        
        //set where to start for the next pair of commands
        if (dir == 0 )
        {
    
            c = c + d ;
        }
       
        else if (dir == 1 )
        {
            r = r + d ;
        }

    }
    while (i < commandString.size());
    
    return 0;

}



bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg)
{

    //TESTS IF STARTING POINTS ARE WITHIN GRID AND (FOREGROUND OR BACKGROUND IS PUT IN)
    // AND CHARACTER IS VALID
    bool valid = false;

    if (!(r > 0 && c >0 && r <= getRows() && c <= getCols() && (fgbg == FG || fgbg == BG) && isprint(plotChar)))
    {
        return false;
    }
        
    //CONFIRMS THE WHOLE LINE IS WITHIN THE GRID
    if (dir == 0 && c+distance <= getCols() && c+distance > 0) //horizontal
    {
        valid = true;
    }
    else if (dir == 1 && r+distance <= getRows() && r+distance >0) //vertical
        valid = true;
    else
    {
        //cout << "Came into here 1" << endl;
        return false;
    }
        
        
    //SET WHERE TO PRINT THE LINE
    int newrow = r, newcol = c;
    
    //PRINT IF FOREGROUND OR SPACE IS AT THE POSITION AND PASSED THE CASES ABOVE
    if ((!fgbg || getChar(newrow, newcol) == ' ') && valid)
    {
        setChar(newrow, newcol, plotChar);
    }
    
    for (int i = 0; i< abs(distance) ; i++)
    {
        if (dir == 0  && distance >= 0)
        {
            newcol += 1 ;
        }
        else if (dir == 0 && distance < 0)
        {
            newcol -= 1 ;
        }
        else if (dir == 1 && distance >= 0)
        {
            newrow += 1 ;
        }
        else if (dir == 1 && distance < 0)
        {
            newrow -= 1 ;
        }
        
        //PRINT IF FOREGROUND OR SPACE IS AT THE POSITION AND PASSED THE CASES ABOVE
        if ((!fgbg || getChar(newrow, newcol) == ' ') && valid){
            setChar(newrow, newcol, plotChar);
        }
            
    }

    
    return true;

}


////function to plot Horizontal line
//void plotHorizontalLine(int r, int c, int distance, char ch)
//{
//    if (distance >= 0)
//        for (int i = 0 ; i <= distance ;  i++ )
//            setChar(r, c + i, ch);
//        
//}
//
////function to plot Vertical line
//void plotVerticalLine(int r, int c, int distance, char ch)
//{
//    if ( distance >= 0 )
//        for (int i = 0; i <= distance ; i++)
//            setChar(r+i, c, ch);
//}
//
////function to plot a rectangle
//void plotRectangle(int r, int c, int height, int width, char ch)
//{
//    if (height>=0 && width >=0)
//    {
//        plotHorizontalLine(r, c, width, ch);
//        plotHorizontalLine(r + height, c, width, ch);
//        plotVerticalLine( r, c, height,ch);
//        plotVerticalLine(r, c+width, height, ch);
//    }
//}
