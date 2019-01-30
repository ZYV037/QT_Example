#include <stdio.h>
#include<stdlib.h>
#include <string>
#include <iostream>
#include <QProcess>

#pragma comment( linker, “/subsystem:windows /entry:mainCRTStartup” )

//"C:\Program Files\TortoiseSVN\bin\svn_wrapper.exe" diff System/WMPS/DIS/UI/DisEventVariationUI/DisEventVariationUI.cpp
//C:\WtgeoProducts: "C:\Program Files\TortoiseSVN\bin\svn_wrapper.exe" diff --internal-diff System/WMPS/App/DIS/DIS.pro
int main(int argc, char *argv[])
{
    if(argc < 3)
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

    std::string cmd1 = argv[1];
    if( cmd1 != "diff" )
    {
        return -1;
    }

    if(argc == 3)
    {
        std::string cmd = "TortoiseProc.exe /command:diff /path:";
        cmd += argv[2];
        return QProcess::execute(cmd.c_str());
    }

    std::string cmd2 = argv[2];
    if( cmd2 == "--internal-diff" )
    {
        std::string cmd = "TortoiseProc.exe /command:diff /path:";
        cmd += argv[3];
        return QProcess::execute(cmd.c_str());
    }

//    int res = system(cmd.c_str());

//    std::cout << cmd << std::endl;

    return -1;
}
