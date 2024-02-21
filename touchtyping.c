/*future ideas
    -> make a settings file for settings such as (max) word length, random word length or const, home row or others, default number of rounds (in the input screen could say press d for default) etc.
    change the amount correct display to reflect correct characters instead of words
    add a words mode (using linux standard dictionary? or c dictionary library??) - put in settings??
    add a speed evaluator????
 */

#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

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

// rewrites letters with a coloured background
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

void writeKeysSettingsScreen(){
    FILE *settings = fopen("settings.txt", "r+");
    char input = 0;
    while(input != 'q') {
        clear();
        addstr("You have selected which keys to be tested on\n");

        //reads the first number in settings.txt for keys settings info
        //1 = home row, 2 = home + top, 3 = home + bottom, 4 = all
        rewind(settings);
        int n = getc(settings) - '0';
        if (n == 1) addstr("keys are currently set to: Home Row\n");
        else if (n == 2) addstr("keys are currently set to: Home + Top Rows\n");
        else if (n == 3) addstr("keys are currently set to: Home + Bottom Rows\n");
        else if (n == 4) addstr("keys are currently set to: All Rows\n");

        addstr("\npress:\n(1) for home row,              (2) for home + top rows,\n(3) for home + bottom rows,    (4) for all rows\nor q to quit\n");
        input = getch();
        napms(200);

        if (input == '1') {
            fseek(settings, -1, 2);
            fwrite(&input, sizeof(char), 1, settings);
        }
        else if (input == '2') {
            fseek(settings, -1, 2);
            fwrite(&input, sizeof(char), 1, settings);
        }
        else if (input == '3') {
            fseek(settings, -1, 2);
            fwrite(&input, sizeof(char), 1, settings);
        }
        else if (input == '4') {
            fseek(settings, -1, 2);
            fwrite(&input, sizeof(char), 1, settings);
        }

    }
    fclose(settings);
}

void writeSettingsScreen(){
    curs_set(0);
    clear();
    addstr("--------------           (press q to exit)\n|  Settings  |\n--------------\n"
           "What do you want to edit?     Enter the corresponding number\n\n"
           "(1) Word length, (2) Number of rounds, (3) which keys to be tested on\n");
    char input = getch();
    //not implemented
    if (input == '1') {
        addstr("\nYou have selected Word length (1)\n");
    }
    //not implemented
    else if (input == '2') {
        addstr("\nYou have selected Number of rounds (2)\n");
    }
    else if (input == '3') {
        writeKeysSettingsScreen();
        writeSettingsScreen();
        return;

    } else if (input == 'q') {
        return;
    }
    if (getch() == 'q') {
        return;
    }
}

// returns number of rounds inputted
// returns -1 if user quits
int writeIntroScreen(){
    clear();
    addstr("---------------------------             (press s to access settings)\n|     Asher's amazing     |\n"
           "| touch-typing challenge! |\n---------------------------\n\n");
    char strIn[4];
    bool redoInput = true;
    int rounds;

    while (redoInput == true) {
        curs_set(1);
        addstr("enter the number of rounds you want to play\nin a three-digit format (i.e 005, 042 or 247)     (max = 999)\nor q to quit\n");
        refresh();
        for (int i = 0; i < 3; i++) {
            strIn[i] = getch();
            if (strIn[i] == 'q'){
                return -1;
            }
            if (strIn[i] == 's'){
                writeSettingsScreen();
                rounds = writeIntroScreen();
                return rounds;
            }
        }
        strIn[3] = '\0';
        rounds = atoi(strIn);

        addstr("\npress any key to start...");
        addstr("\nor r to reinput the number of rounds\n");
        curs_set(0);
        refresh();
        if (getch() != 'r') redoInput = false;
        addstr("\n\n");
    }
    clear();
    curs_set(1);

    move(4, 13);
    printw("0/%d correct",rounds);
    return rounds;
}

//returns whether player has quit
bool writeEndScreen(int roundsCorrect, int rounds){
    bool isEnding = false;
    char winMessage[25];
    if (roundsCorrect >= rounds * 0.8) strcpy(winMessage, "AMAZING!");
    else if (roundsCorrect >= rounds * 0.5) strcpy(winMessage ,"Well Done!");
    else if (roundsCorrect >= rounds * 0.3) strcpy(winMessage, "Better Luck Next Time!");
    else strcpy(winMessage, "Oh Dear...");
    clear();
    curs_set(0);
    printw("---------------\n|  Game Over  |\n---------------\n\n"
           "%s\nyou got %d out of %d right\n\n\n",winMessage, roundsCorrect, rounds);
    addstr("press any key to try again or q to quit");
    flushinp();
    refresh();
    if (getch() == 'q') isEnding = true;
    curs_set(1);
    return isEnding;
}

//returns number of correct rounds
int runTouchtypingTests(char* charSet, int rounds){
    int correctCount = 0;
    bool fail;
    bool currentFail;
    char input;
    for (int i = 0; i < rounds; i++) {
        int length = (rand() % 8 + 1);
        char word[length + 1];
        for (int i = 0; i < length; i++) {
            word[i] = charSet[rand() % 10];
        }
        word[length] = '\0';
        fail = false;
        move(0, 0);
        printw("%d.", i + 1);

        move(1, 0);
        addstr(word);
        move(2, 0);
        flushinp();
        refresh();
        for (int i = 0; i < length; i++) {
            input = getch();
            if (word[i] != input) {
                fail = true;
                currentFail = true;
            } else currentFail = false;
            overwriteLetters(i, currentFail, word);
        }
        napms(500);
        if (fail == false) correctCount += 1;
        curs_set(0);
        writeOutcome(fail, correctCount, rounds);
        napms(680);
        clear();
        writeOutcome(fail, correctCount, rounds);
        curs_set(1);
    }
    return correctCount;
}



int main(){

    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    init_pair(2, COLOR_WHITE, COLOR_RED);
    srand(time(NULL));
    char homerowChars[10] = {'a','s','d','f','g','h','j','k','l',';'};
    bool isEnding = false;

    while (isEnding == false) {
        //intro screen
        int rounds = writeIntroScreen();
        if (rounds == -1){
            flushinp();
            endwin();
            return 0;
        }

        // main loop for touchtyping tests
        int correctCount = runTouchtypingTests(homerowChars, rounds);

        // end screen
        isEnding = writeEndScreen(correctCount, rounds);
        flushinp();
    }
    endwin();
    return 0;
}

