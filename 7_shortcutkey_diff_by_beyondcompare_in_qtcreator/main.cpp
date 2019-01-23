#include <stdio.h>
#include<stdlib.h>
#include <string>
#include <iostream>
#include <QProcess>

#pragma comment( linker, “/subsystem:windows /entry:mainCRTStartup” )

int main(int argc, char *argv[])
{
    if(argc < 4)
    {
        std::string cmd = "Cmd error: " ;
        for(int i = 0; i < argc; ++ i)
        {
            cmd += argv[i];
            cmd += " ";
        }
        std::cout << cmd << std::endl;
        return -1;
    }

    std::string cmd = "TortoiseProc.exe /command:diff /path:";
    cmd += argv[3];
    int res = QProcess::execute(cmd.c_str());
//    int res = system(cmd.c_str());

//    std::cout << cmd << std::endl;

    return res;
}
