#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <list>
#include <iostream>

using namespace std;

void handleCommand(char*);
void setPcName();
void setCommandsList();
int getCmdId(const char*);
bool isACmd(char*, const char*);

char* getInput();
void echo(char*, ...);
void clearxy(int, int);
void gotoxy(int, int);
COORD getxy();

struct tCmd;

bool isRunning = true;
char userName[64];
char pcName[64];

list<tCmd> cmdList = *new list<tCmd>();

enum {
    LS = 100,
    YES = 1, NO = 0, EXIT = -1
};

struct tCmd {
    int id;
    const char* name;

    tCmd(int id, const char* name) {
        this->id = id;
        this->name = name;
    }
};

int main () {
    setPcName();
    setCommandsList();

    cout << "Introduceti numele: ";
    cin >> userName;

    while(isRunning) {
        handleCommand(getInput());
    }

    return 0;
}

void setPcName() {
    TCHAR tpcName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD tpcNameLen = sizeof(pcName);

    if(GetComputerName(tpcName, &tpcNameLen) == TRUE)
        strcpy(pcName, tpcName);
    else
        sprintf(pcName, "`unknown`");
}

void setCommandsList() {
	// cmdList.push_back(*new tCmd( enum { command code }, {command input}));
    cmdList.push_back(*new tCmd(LS, "ls"));
    cmdList.push_back(*new tCmd(YES, "yes"));
    cmdList.push_back(*new tCmd(NO, "no"));
    cmdList.push_back(*new tCmd(EXIT, "exit"));
    cmdList.push_back(*new tCmd(EXIT, "quit"));
}

int getCmdId(char* name) {
    int cvarId = -10001;

    for(list<tCmd>::iterator i = cmdList.begin(); i != cmdList.end(); i++) {
        const char* cvarName = (*i).name;

        if(isACmd(name, cvarName)) {
            cvarId = (*i).id;
        }
    }

    return cvarId;
}

bool isACmd(char* name, const char* cvarName) {

    if(name == NULL || cvarName == NULL || strlen(name) == 0 || strlen(cvarName) == 0)
        return false;

    if(strlen(name) > strlen(cvarName))
        return false;

    bool hasBadRes = false;
    for(unsigned int i=0; i < strlen(name); i++) {
        if(hasBadRes)
            continue;
        if(name[i] != cvarName[i])
            hasBadRes = true;
    }
    if(hasBadRes)
        return false;

    return true;
}

void handleCommand(char* cmd) {
	//////////////////////////////////////
	//Manage and create new commands, taking parameters too
    char* cmdPart = new char[32];
    cmdPart = cmd;

	//////////////////////////////////////

    switch(getCmdId(cmdPart)) {
    case YES:
        echo("Command YES sent");
        break;
    case NO:
        echo("Command NO sent");
        break;
    case EXIT:
        echo("Closing...");
        isRunning = false;
        break;
    case LS:
        system("dir");
        break;
    default:
        system(cmdPart);
        //echo("Unknown command line");
    }
}

char* getInput() {
    char tch = -1;
    char *input = new char[1024];

    int posX = 0;
    COORD cursPos = getxy();

    cout << userName << "@" << pcName << " ~# ";
    cursPos.X += strlen(userName) + strlen(pcName) + 5;

    while(tch != 10 && tch != 13) {
        tch = _getch();  // ori getch();
        if(tch == 10 || tch == 13) {
            if(posX == 0) {
                tch = -1;
            }
            continue;
        }

        if(tch == 8) {
            if(posX > 0)
                input[--posX] = 0;
            clearxy(cursPos.X + posX, cursPos.Y);
        }else {
            //if(posX > 2)
            //    continue;
            input[posX++] = tch;
            input[posX] = 0;
            cout << tch;
        }
    }

    gotoxy(0, ++cursPos.Y);

    return input;
}

void clearxy(int x, int y) {
    gotoxy(x, y);
    cout << ' ';
    gotoxy(x, y);
}

void gotoxy(int x, int y) {
    COORD axes;
    axes.X = x;
    axes.Y = y;

    HANDLE cslHdl = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleCursorPosition(cslHdl, axes);
}

COORD getxy() {
    COORD axes;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE cslHdl = GetStdHandle(STD_OUTPUT_HANDLE);

    axes.X = -1;
    axes.Y = -1;

    if(GetConsoleScreenBufferInfo(cslHdl, &csbi)) {
        axes.X = csbi.dwCursorPosition.X;
        axes.Y = csbi.dwCursorPosition.Y;
    }

    return axes;
}

void echo(char* message, ...) {
    char *decalaj = "    ";
    cout << decalaj << message << endl;
}
