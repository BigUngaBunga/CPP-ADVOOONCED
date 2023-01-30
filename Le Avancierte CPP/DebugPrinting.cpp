#include "DebugPrinting.h"

void FinishedTest(int testNumber) {std::cout << "Finished test " << testNumber << ".\n";}
void FinishedTest(std::string message) { std::cout << "Finished testing " << message << ".\n"; }
void Println(std::string message) {std::cout << message << ".\n";}