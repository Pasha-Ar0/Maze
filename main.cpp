#include <iostream>
#include <string>
#include <vector>
#include <memory.h>
#include <stdalign.h>
#include <fstream>
#include <time.h>

using namespace std;

struct player_info {string name; int level; bool devel_mode; int money; int fake_money; int dynamite;};
struct portals {int x; int y;};

int GetRandNum(int min, int max) {
    int random = min + rand() % (max - min + 1);
    return random;
}

void Help(void) {

}

bool Pass(void) {
    string command, code = "xxxx";
    char k;
    code[0] = GetRandNum(0, 5) + '0';
    for(int i = 0; i < 3; i++)
        code[i+1] = GetRandNum(0, 25) + 'a';
    cout << "Введите расшифровку " << code << ":" << endl;
    cout << ">>> ";
    cin >> command;
    cout << endl;
    if(command.size() == 4) {
        for(int i = command[0]; i > 0; i--) {
            if(i%2 == 0) {
                k = command[1];
                command[1] = command[2];
                command[2] = k;
            }
            else {
                k = command[2];
                command[2] = command[3];
                command[3] = k;
            }
        }
        if(command == code) {
            cout << "#OK" << endl << endl;
            return true;
        }
    }
    cout << "#Неверно" << endl << endl;
    return false;
}

int *LabGener(string settings) {
    int n = settings[0], x = settings[1], y = settings[2], rand, pass, maxi;
    int lab[n][n];
    bool portal = false, chest = false, dragon = false;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            lab[i][j] = 0;

    /*  0 - wall
        1 - empty seat
        2 - entry
        3 - exit
        4 - portal
        5 - chest
        6 - dragon      */

    for(int k = 0; k < 2; k++) {
        if(k == 0) lab[x][y] = 2;
        else lab[x][y] = 3;
        portal = false;
        for(int i = 0; i < n; i++) {
            int snake_pass[4], cnt = 0;
            //up
            if(x-1 < 0) snake_pass[0] = 0;
            else if(lab[x-1][y] != 0) snake_pass[0] = 0;
            else {snake_pass[0] = 1; cnt++;}
            //right
            if(y+1 >= n) snake_pass[1] = 0;
            else if(lab[x][y+1] != 0) snake_pass[1] = 0;
            else {snake_pass[1] = 1; cnt++;}
            //down
            if(x+1 >= n) snake_pass[2] = 0;
            else if(lab[x+1][y] != 0) snake_pass[2] = 0;
            else {snake_pass[2] = 1; cnt++;}
            //left
            if(y-1 < 0) snake_pass[3] = 0;
            else if(lab[x][y-1]) snake_pass[3] = 0;
            else {snake_pass[3] = 1; cnt++;}

            if(cnt == 0) continue;
            else pass = GetRandNum(1, cnt);
            for(int j = 0; j < 4; j++)
                if(snake_pass[j] == 1) {
                    pass--;
                    if(pass == 0) {pass = j; break;}
                }

            switch(pass) {
                case 0: x--; break;
                case 1: y++; break;
                case 2: x++; break;
                case 3: y--; break;
            }

            rand = GetRandNum(1, 100);
            //empty seat - 0.5
            if(rand <= 50) lab[x][y] = 1;
            //portal - 0.2 (0.5)
            else if(rand <= 70) {
                if(portal == false) {portal = true; lab[x][y] = 4;}
                else {
                    rand = GetRandNum(0, 1);
                    if(rand == 0) lab[x][y] = 4;
                    else lab[x][y] = 1;
                }
            }
            //chest - 0.1 (0.2)
            else if(rand <= 80) {
                if(chest == false) {chest = true; lab[x][y] = 5;}
                else {
                    rand = GetRandNum(0, 4);
                    if(rand == 0) lab[x][y] = 5;
                    else lab[x][y] = 1;
                }
            }
            //dragon - 0.05 (0)
            else if(rand <= 85) {
                if(dragon == false) {dragon = true; lab[x][y] = 6;}
                else lab[x][y] = 1;
            }
            //end - 0.15 (portal - 0.5 (0.1))
            else {
                if(portal == false) maxi = 1;
                else maxi = 9;
                rand = GetRandNum(0, maxi);
                if(rand == 0) lab[x][y] = 4;
                    else lab[x][y] = 1;
            }
        }
        for(;;) {
            x = GetRandNum(0, n-1);
            y = GetRandNum(0, n-1);
            if(lab[x][y] != 0) continue;
            else break;
        }
    }

    for(int k = 0; k < n*n/24 - 1; k++) {
        for(;;) {
            x = GetRandNum(0, n-1);
            y = GetRandNum(0, n-1);
            if(lab[x][y] != 0) continue;
            else break;
        }
        lab[x][y] = 1;
        portal = false;
        for(int i = 0; i < n; i++) {
            int snake_pass[4], cnt = 0;
            //up
            if(x-1 < 0) snake_pass[0] = 0;
            else if(lab[x-1][y] != 0) snake_pass[0] = 0;
            else {snake_pass[0] = 1; cnt++;}
            //right
            if(y+1 >= n) snake_pass[1] = 0;
            else if(lab[x][y+1] != 0) snake_pass[1] = 0;
            else {snake_pass[1] = 1; cnt++;}
            //down
            if(x+1 >= n) snake_pass[2] = 0;
            else if(lab[x+1][y] != 0) snake_pass[2] = 0;
            else {snake_pass[2] = 1; cnt++;}
            //left
            if(y-1 < 0) snake_pass[3] = 0;
            else if(lab[x][y-1]) snake_pass[3] = 0;
            else {snake_pass[3] = 1; cnt++;}

            if(cnt == 0) continue;
            else pass = GetRandNum(1, cnt);
            for(int j = 0; j < 4; j++)
                if(snake_pass[j] == 1) {
                    pass--;
                    if(pass == 0) {pass = j; break;}
                }

            switch(pass) {
                case 0: x--; break;
                case 1: y++; break;
                case 2: x++; break;
                case 3: y--; break;
            }

            rand = GetRandNum(1, 100);
            //empty seat - 0.5
            if(rand <= 50) lab[x][y] = 1;
            //portal - 0.2 (0.5)
            else if(rand <= 70) {
                if(portal == false) {portal = true; lab[x][y] = 4;}
                else {
                    rand = GetRandNum(0, 1);
                    if(rand == 0) lab[x][y] = 4;
                    else lab[x][y] = 1;
                }
            }
            //chest - 0.1 (0.2)
            else if(rand <= 80) {
                if(chest == false) {chest = true; lab[x][y] = 5;}
                else {
                    rand = GetRandNum(0, 4);
                    if(rand == 0) lab[x][y] = 5;
                    else lab[x][y] = 1;
                }
            }
            //dragon - 0.05 (0)
            else if(rand <= 85) {
                if(dragon == false) {dragon = true; lab[x][y] = 6;}
                else lab[x][y] = 1;
            }
            //end - 0.15 (portal - 0.5 (0.1))
            else {
                if(portal == false) maxi = 1;
                else maxi = 9;
                rand = GetRandNum(0, maxi);
                if(rand == 0) lab[x][y] = 4;
                    else lab[x][y] = 1;
                break;
            }
        }
    }
    /*
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            switch(lab[i][j]) {
                case 0: cout << "■ "; break;
                case 1: cout << "_ "; break;
                case 2: cout << "> "; break;
                case 3: cout << "< "; break;
                case 4: cout << "* "; break;
                case 5: cout << "# "; break;
                case 6: cout << "& "; break;
            }
        }
        cout << endl;
    } */
    int *ptr_lab = (int *)malloc(sizeof(lab));
    memcpy(ptr_lab, lab, sizeof(lab));
    return ptr_lab;
}

vector<struct portals> PortLink(string settings, int *ptr_lab) {
    int n = settings[0];
    struct portals portinfo;
    vector<struct portals> plink(0);
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            if(*(ptr_lab + i*n + j) == 4)
                plink.push_back(portinfo = {i, j});
    return plink;
}

void Library(string plact, string plares, struct player_info *plainfo) {

}

/*
void PlayProcess(struct player_info plainfo, int *ptr_lab, int n, vector<struct portals> plink) {
    int lab[n][n], plamove = 1, plx, ply;
    string command;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++) {
            lab[i][j] = *(ptr_lab + n*i + j);
            if(lab[i][j] == 2)
                plx = i, ply = j;
        }

    if(plainfo.level == 0)
        for(;;) {
            cout << "Пройти обучение?" << endl;
            cout << "1) yes\n2) no" << endl;
            cout << ">>> ";
            cin >> command;
            cout << endl;
            if(command == "1") {
                cout << "Для передвижения по лабиринту используйте клавиши:" << endl;
                cout << "     ↑\n     w\n ← a   d →\n     s\n     ↓" << endl << endl;
                break;
            }
            else if(command == "2") break;
            else cout << "CommandError" << endl << endl;
        }

    //Library("entry", "entry", *plainfo);
    for(;; plamove++) {
        cout << ">>> ";
        cin >> command;
        cout << endl;
        if(command == "pause") {
            for(;;) {

            }
            plamove--;
            continue;
        }
        else if(command == "help") {
            //Library("help", "help", *plainfo);
        }
        else if(command == "w") {
            if(plx-1 < 0 || lab[plx-1][ply] == 0) {}
                //Library("forward", "wall", *plainfo);
            else {
                plx--;
                //Library("forward", "void", *plainfo);
            }
            continue;
        }
        else if(command == "d") {
            if(ply+1 >= n || lab[plx][ply+1] == 0)
                Library("right", "wall", *plainfo);
            else {
                ply++;
                Library("right", "void", *plainfo);
            }
            continue;
        }
        else if(command == "s") {
            if(plx+1 >= n || lab[plx+1][ply] == 0)
                Library("down", "wall", *plainfo);
            else {
                plx++;
                Library("down", "void", *plainfo);
            }
            continue;
        }
        else if(command == "a") {
            if(ply-1 < 0 || lab[plx][ply-1] == 0)
                Library("left", "wall", *plainfo);
            else {
                ply--;
                Library("left", "void", *plainfo);
            }
            continue;
        }
        else if(command == "din") {
            if(plainfo.dynamite > 0 || plainfo.devel_mode == true) {
                for(;;) {
                    Library("din", "void", *plainfo);
                    cout << ">>> ";
                    cin >> command;
                    cout << endl;
                    if(command == "w") {

                    }
                    else if(command == "d") {

                    }
                    else if(command == "s") {

                    }
                    else if(command == "a") {

                    }
                    else if(command == "x") {
                        Library("xact", "void", palinfo);
                        plamove--;
                        break;
                    }
                    else cout << "CommandError" << endl << endl;
                }
            }
            else
            continue;
        }
        else {
            cout << "CommandError" << endl << endl;
            plamove--;
            continue;
        }
    }


}
*/

int main() {
    srand(time(NULL));
    struct player_info plainfo;
    vector<struct player_info> lst(0);
    ifstream imyfile("player_list.txt");
    for(;!imyfile.eof();) {
        imyfile >> plainfo.name >> plainfo.level >> plainfo.devel_mode >> plainfo.money >> plainfo.fake_money >> plainfo.dynamite;
        lst.push_back(plainfo);
    }
    imyfile.close();
    string command, settings;
    int planum = -1, k;
    cout << "labirint[0.3]" << endl << endl;

    for(;;) {
        cout << "1) start\n2) player list\n3) help\n4) quit" << endl;
        cout << ">>> ";
        cin >> command;
        cout << endl;
        if(command == "4") break;
        if(command == "devel_mode") {
            if(planum != -1) {
                if(lst[planum].devel_mode == true) {
                    lst[planum].devel_mode = false;
                    cout << '#' << lst[planum].name << " отключил 'devel_mode'" << endl << endl;
                }
                else lst[planum].devel_mode = Pass();
                if(lst[planum].devel_mode == true)
                    cout << '#' << lst[planum].name << " включил 'devel_mode'" << endl << endl;
            }
            else cout << "Сначала выберите игрока в 'player list'" << endl << endl;
            continue;
        }
        if(command != "1" && command != "2" && command != "3") {
            cout << "CommandError" << endl << endl;
            continue;
        }
        switch(stoi(command)) {
            case 1:
                if(planum == -1) {
                    cout << "Сначала выберите игрока в 'player list'" << endl << endl;
                    continue;
                }
                settings = "nxy";
                for(;;) {
                    menu_start:
                    cout << "1) start\n2) continue\n3) settings\n4) back" << endl;
                    cout << ">>> ";
                    cin >> command;
                    cout << endl;
                    if(command == "4") break;
                    if(command != "1" && command != "2" && command != "3") {
                        cout << "CommandError" << endl << endl;
                        continue;
                    }
                    switch(stoi(command)) {
                        case 1:
                            for(;;) {
                                if(settings[0] != 'n') {
                                    if(settings[1] != 'x') {
                                        if(settings[2] != 'y')
                                            for(;;) {
                                                cout << lst[planum].name << ", приступить к генерации лабиринта?" << endl;
                                                cout << "1) yes\n2) back" << endl;
                                                cout << ">>> ";
                                                cin >> command;
                                                cout << endl;
                                                if(command == "1") {
                                                    cout << "Генерация лабиринта..." << endl << endl;
                                                    int *ptr_lab = LabGener(settings);
                                                    cout << "Линк порталов..." << endl << endl;
                                                    vector<struct portals> plink = PortLink(settings, ptr_lab);
                                                    cout << "Запуск игры..." << endl << endl;
                                                    //PlayProcess(lst[planum], ptr_lab, settings[0], plink);
                                                }
                                                else if(command == "2") goto menu_start;
                                                else cout << "CommandError" << endl << endl;
                                            }
                                        else
                                            for(;;) {
                                            cout << "Введите клетку спавна в лабиринте по координате 'y':" << endl;
                                            cout << ">>> ";
                                            cin >> command;
                                            cout << endl;
                                            for(int i = 0; i < command.size(); i++) {
                                                if(command[i] < '0' || command[i] > '9') {
                                                    command[0] = '0';
                                                    break;
                                                }
                                            }
                                            if(command[0] == '0') {
                                                cout << "CommandError" << endl << endl;
                                                continue;
                                            }
                                            if(stoi(command) > settings[0]) {
                                                cout << "Данное значение больше размера лабиринта" << endl << endl;
                                                continue;
                                            }
                                            cout << endl;
                                            settings[2] = stoi(command)-1;
                                            break;
                                        }
                                    }
                                    else
                                        for(;;) {
                                            cout << "Введите клетку спавна в лабиринте по координате 'x':" << endl;
                                            cout << ">>> ";
                                            cin >> command;
                                            cout << endl;
                                            for(int i = 0; i < command.size(); i++) {
                                                if(command[i] < '0' || command[i] > '9') {
                                                    command[0] = '0';
                                                    break;
                                                }
                                            }
                                            if(command[0] == '0') {
                                                cout << "CommandError" << endl << endl;
                                                continue;
                                            }
                                            if(stoi(command) > settings[0]) {
                                                cout << "Данное значение больше размера лабиринта" << endl << endl;
                                                continue;
                                            }
                                            cout << endl;
                                            settings[1] = stoi(command)-1;
                                            break;
                                        }
                                }
                                else {
                                    if(lst[planum].level == 0) settings[0] = 5;
                                    else if(lst[planum].level == 1) settings[0] = 6;
                                    else if(lst[planum].level <= 3) settings[0] = 8;
                                    else if(lst[planum].level <= 6) settings[0] = 10;
                                    else settings[0] = 12;
                                }
                            }
                        case 2:

                        continue;
                        case 3:

                        continue;
                    }
                }
                continue;

            case 2:
                for(;;) {
                    cout << "1) choose\n2) create\n3) delete\n4) back" << endl;
                    cout << ">>> ";
                    cin >> command;
                    cout << endl;
                    k = -1;
                    if(command == "4") break;
                    if(command != "1" && command != "2" && command != "3" && command != "4") {
                        cout << "CommandError" << endl << endl;
                        continue;
                    }
                    switch(stoi(command)) {
                        case 1:
                            if(lst.size() == 0) {
                                cout << "В 'player list' нет игроков" << endl << endl;
                                continue;
                            }
                            cout << "Введите имя пользователя:" << endl;
                            cout << ">>> ";
                            cin >> command;
                            cout << endl;
                            for(int i = 0; i < lst.size(); i++)
                                if(lst[i].name == command) {k = i; break;}
                            if(k != -1) {
                                planum = k;
                                cout << "Пользователь " << command << " успешно выбран" << endl << endl;
                            }
                            else cout << "Такого пользователя не существует" << endl << endl;
                            continue;
                        case 2:
                            cout << "Введите имя пользователя:" << endl;
                            cout << ">>> ";
                            cin >> command;
                            cout << endl;
                            for(int i = 0; i < lst.size(); i++)
                                if(lst[i].name == command) {k = i; break;}
                            if(k != -1) {
                                cout << "Такой пользователь уже существует" << endl << endl;
                                cout << "Выбрать данного пользователя?" << endl;
                                cout << "1) cancel\n2) yes" << endl;
                                cout << ">>> ";
                                cin >> command;
                                cout << endl;
                                if(command == "2") {
                                    planum = k;
                                    cout << "Пользователь " << lst[planum].name << " успешно выбран" << endl << endl;
                                }
                            }
                            else {
                                plainfo = {command, 0, false, 0, 0, 0};
                                lst.push_back(plainfo);
                                planum = lst.size()-1;
                                cout << "Пользователь " << command << " успешно добавлен и выбран" << endl << endl;
                            }
                            continue;
                        case 3:
                            if(lst.size() == 0) {
                                cout << "В 'player list' нет игроков" << endl << endl;
                                continue;
                            }
                            cout << "Введите имя пользователя:" << endl;
                            cout << ">>> ";
                            cin >> command;
                            cout << endl;
                            for(int i = 0; i < lst.size(); i++)
                                if(lst[i].name == command) {k = i; break;}
                            if(k != -1) {
                                lst.erase(lst.begin() + k);
                                if(planum == k) planum = -1;
                                if(planum > k) planum--;
                                cout << "Игрок " << command << " успешно удалён" << endl << endl;
                            }
                            else cout << "Такого пользователя не существует" << endl << endl;
                            continue;
                    }
                }
                continue;

            case 3:
                Help();
                break;
        }
    }

    ofstream omyfile("player_list.txt");
    for(int i = 0; i < lst.size(); i++)
        omyfile << lst[i].name << " " << lst[i].level << " " << lst[i].devel_mode << " " << lst[i].money << " " << lst[i].fake_money << " " << lst[i].dynamite << endl;
    omyfile.close();
    return 0;
}
