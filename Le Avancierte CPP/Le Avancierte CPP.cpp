// Le Avancierte CPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Task.h"
#include <iostream>
#include <string>
using std::cout;
using std::cin;
using std::string;

const int currentTasks = 6;

void ExplainPrograms();
void PickProgram();
void RunProgram(int i);

int main()
{
    std::locale::global(std::locale("swedish"));
    PickProgram();
}

void ExplainPrograms() 
{
    cout << "There are currently " << currentTasks << " tasks which can be run.";
    cout << "\nEnter a number between 1 and " << currentTasks << " to run a task:\n";
}

void PickProgram() 
{
    ExplainPrograms();
    string userInput;
    int taskNumber;
    while (cin >> userInput)
    {
        taskNumber = -1;
        try
        {
             taskNumber = std::stoi(userInput);
        }
        catch (const std::exception&)
        {
            cout << "Could not convert " << userInput << " to an integer.\n";
        }

        if (taskNumber > 0 && taskNumber <= currentTasks) {
            RunProgram(taskNumber);
            break;
        }
        else {
            cout << userInput << " is not a valid task.\n";
            ExplainPrograms();
        }
    }
}

void RunProgram(int i) 
{
    cout << "Starting task " << i << ".\n";
    Task task;
    switch (i)
    {
        case 1: task.RunTask1(); break;
        case 2: task.RunTask2(); break;
        case 3: task.RunTask3(); break;
        case 4: task.RunTask4(); break;
        case 5: task.RunTask5(); break;
        //case 6: task.RunTask6(); break;
        default: cout << "Could not find the task";
        break;
    }
}
