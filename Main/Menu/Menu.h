#ifndef MENU_H
#define MENU_H

class QueueSystem;
class ExpressionCalculator;
void mainMenu();
void queueMenu(QueueSystem&qu);
void calculatorMenu(ExpressionCalculator&expr);

void printMainMenu();
void printQueueMenu();
void printCalculatorMenu();

#endif
