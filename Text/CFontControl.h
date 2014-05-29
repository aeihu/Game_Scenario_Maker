/*
* Copyright (C) 2012-2014, <Aeihu.z, aeihu.z@gmail.com>.
*
* Game Scenario Maker is a free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* Version 2(GPLv2) as published by the Free Software Foundation.
*/

#ifndef _CFONTCONTROL_H_
    #define _CFONTCONTROL_H_

#include <string>
#include "../Common/CObjectControl.h"
#include "CFont.h"
#include <map>

using namespace std;

class CFontControl : public CObjectControl
{
    private:
        friend class CResourceManager;
        friend bool Cmd_AddMessageBox(vector<string> args);
        friend bool Cmd_Message(vector<string> args);
        
        map<string, CFont>          _fontList;

        CObject* GetObject(string name);
    public:
        bool AddCFont(string name, const char* filename);
        bool DelCFont(string name);

        void OnCleanup();
};

#endif