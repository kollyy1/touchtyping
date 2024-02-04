/*future ideas
    make an end screen
    make a settings file for settings such as (max) word length, random word length or const, home row or others, default number of rounds (in the input screen could say press d for default) etc
    put things into discrete functions
    change the amount correct display to reflect correct characters instead of words
    add a words mode (using linux standard dictionary? or c dictionary library??) - put in settings??
    add a speed evaluator???
 */

#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <stdbool.h>

void writeOutcome(bool fail, int correctCount, int rounds){
    int colour;
    char outcomeStr[10];
    if (fail == true){
        colour = 2;
        strcpy(outcomeStr, "  WRONG  ");
    }
    else{
        colour = 1;
        strcpy(outcomeStr, " CORRECT ");
    }
    attrset(COLOR_PAIR(colour));
    move(4, 0);
    addstr(outcomeStr);
    attroff(COLOR_PAIR(colour));
    move(4, 13);
    printw("%d/%d correct", correctCount, rounds);
    refresh();
}

void overwriteLetters(int currentL, bool fail, char* word){
    int colour;
    if (fail == true) colour = 2;
    else colour = 1;
    attrset(COLOR_PAIR(colour));
    move(1,currentL);
    addch(word[currentL]);
    attroff(COLOR_PAIR(colour));
    move(2,currentL+1);
    refresh();
}

int main(){

    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    init_pair(2, COLOR_WHITE, COLOR_RED);
    srand(time(NULL));
    char homerowChars[10] = {'a','s','d','f','g','h','j','k','l',';'};
    char input;
    bool fail;
    bool currentFail;
    int correctCount = 0;

    //intro screen
    addstr("---------------------------\n|     Asher's amazing     |\n"
           "| touch-typing challenge! |\n---------------------------\n\n");
    char strIn[4];
    bool redoInput = true;
    int rounds = 0;

    while (redoInput == true) {
        curs_set(1);
        addstr("enter the number of rounds you want to play\nin a three-digit format (i.e 005, 042 or 247)     (max = 999)\n");
        refresh();
        for (int i = 0; i < 3; i++) {
            strIn[i] = getch();
        }
        strIn[3] = '\0';
        rounds = atoi(strIn);

        addstr("\npress any key to start...");
        addstr("\nor r to reinput the number of rounds");
        curs_set(0);
        refresh();
        if (getch() != 'r') redoInput = false;
        addstr("\n\n");
    }
    clear();
    curs_set(1);

    move(4, 13);
    printw("0/%d correct",rounds);

    // main loop for touchtyping tests
    for(int i = 0; i < rounds; i++){
        int length = (rand() %8 + 1);
        char word[length+1];
        for (int i = 0; i < length; i++){
            word[i] = homerowChars[rand() % 10];
        }
        word[length] = '\0';
        fail = false;
        move(0,0);
        printw("%d.", i+1);

        move(1, 0);
        addstr(word);
        move(2, 0);
        flushinp();
        refresh();
        for(int i = 0; i < length; i++) {
            input = getch();
            if (word[i] != input) {
                fail = true;
                currentFail = true;
            }
            else currentFail = false;
            overwriteLetters(i, currentFail, word);
        }
        napms(500);
        if (fail == false) correctCount += 1;
        curs_set(0);
        writeOutcome(fail, correctCount, rounds);
        napms(700);
        clear();
        writeOutcome(fail, correctCount, rounds);
        curs_set(1);
    }

    // end screen



    napms(2000);
    flushinp();
    endwin();
    return 0;
}

