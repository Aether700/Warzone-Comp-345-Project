#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "MapLoader.h"

namespace WZ
{
    using std::string;
    using std::cout;
    using std::cin;

    MapLoader::MapLoader() {
        string file_content{ NULL }, map_name = menu_loader();
        in(map_name);
        in.open(map_name);
        if (!in) {
            cout << "Unable to open file";
            exit(1);
        }
        while (std::getline(in, file_content)) {

        }
    }
    bool MapLoader::parserFunction(string s) {
        //  crunching the file content
        // calling the Map constructor and passing the relevant data
        // returning false if the file is no map type of data is corrupt
    }


    string MapLoader::menu_loader() {
        int map_to_play, map = 0;
        string map_list[100];
        std::ifstream local_dir;
        // local_dir.open(current directory) ; code for local_dir to open and read all the .map and .txt files from the current folder
        // while(local_dir >> keep_reading)
        //      map_list[map] = keep_reading; 
        //      map++;
        while (true) {
            for (short i = 0; i < (sizeof(map_list) / sizeof(map_list[0])); i++) {
                cout << map_list[i];
            }
            cout << "Please select the map by entering the number in front of it: ";
            cin >> map_to_play;
            if (map < 0 || map_to_play > map) {
                cout << "Out of bound exception.\nRetry\n";
                continue;
            }
        }
        return map_list[map_to_play];
    }
}
