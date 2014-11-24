/*
* Copyright (C) 2012-2014, <Aeihu.z, aeihu.z@gmail.com>.
*
* Game Scenario Maker is a free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* Version 2(GPLv2) as published by the Free Software Foundation.
*/

#include "CMessageBox.h"

using namespace std;

CMessageBox::CMessageBox()
{
    _frames.SetMaxFrames(1);
    _frames.SetFrameRate(10);
    _frames.SetCurrentFrame(0);
    _frames._Type = CAnimation::Oscillate;
    _childrenList.push_back(&_frames);
    _frames.SetFlag(FLAG_SCALE);
    _isPaused = false;
}

CMessageBox* CMessageBox::Create(const char* filename)
{
    CMessageBox* __msgbox = new CMessageBox();
    if (__msgbox->LoadConfigFile(filename)){
        __msgbox->SetClassName("msgbox");
        __msgbox->SetPath(filename);
        __msgbox->SetLayerOrder(4);
        return __msgbox;
    }
    
    delete __msgbox;
    return NULL;
}

void CMessageBox::SetFont(sf::Font& font)
{
    CTextProcessing::SetFont(font);
    _speakerName.setFont(font);
}

bool CMessageBox::SetProperty(Object json)
{
    CTextProcessing::SetRowWidth(json.get<Number>("MSG_WIDTH"));

//=================Init cursor==================================
    if (!_frames.LoadImg(json.get<String>("CURSOR_PATH").c_str()))
        return false;

    _frames.SetWidth(json.get<Number>("CURSOR_WIDTH"));
    _frames.SetHeight(json.get<Number>("CURSOR_HEIGHT"));
    _frames.SetMaxFrames(json.get<Number>("CURSOR_MAX_FRAMES"));
    _frames.SetFrameRate(json.get<Number>("CURSOR_FRAME_RATE"));

    _speakerNameOffset.x = json.get<Number>("SPEAKER_OFFSET_X");
    _speakerNameOffset.y = json.get<Number>("SPEAKER_OFFSET_Y");
    _msgOffset.x = json.get<Number>("MSG_OFFSET_X");
    _msgOffset.y = json.get<Number>("MSG_OFFSET_Y");

    CFont* __fnt = NULL;
    CObject* __obj = json.has<Number>("MSG_FONT") ? 
        CResourceControl::_ResourceManager.GetObject("Font:"+json.get<String>("MSG_FONT")):
        CResourceControl::_ResourceManager.GetObject("Font:__main");
   
    if (__obj != NULL){
        __fnt = static_cast<CFont*>(__obj);
        SetFont(__fnt->GetFont());
    }
    else
        return false;

    return CBox::SetProperty(json);
}

bool CMessageBox::CheckList(Object json)
{
    bool result = CBox::CheckList(json);

    if (!json.has<Number>("MSG_OFFSET_X")){
        cout << "can't find value of MSG_OFFSET_X." << endl;
        result = false;
    }

    if (!json.has<Number>("MSG_OFFSET_Y")){
        cout << "can't find value of MSG_OFFSET_X." << endl;
        result = false;
    }

    if (!json.has<Number>("MSG_WIDTH")){
        cout << "can't find value of MSG_WIDTH." << endl;
        result = false;
    }

    //if (!json.has<Number>("MSG_APPEND") < 1){
    //    cout << "can't find value of MSG_APPEND." << endl;
    //    result = false;
    //}

    if (!json.has<String>("CURSOR_PATH")){
        cout << "can't find value of CURSOR_PATH." << endl;
        result = false;
    }

    if (!json.has<Number>("CURSOR_WIDTH")){
        cout << "can't find value of CURSOR_CURSOR_WIDTH." << endl;
        result = false;
    }

    if (!json.has<Number>("CURSOR_HEIGHT")){
        cout << "can't find value of CURSOR_CURSOR_HEIGHT." << endl;
        result = false;
    }

    if (!json.has<Number>("CURSOR_FRAME_RATE")){
        cout << "can't find value of CURSOR_FRAME_RATE." << endl;
        result = false;
    }

    if (!json.has<Number>("CURSOR_MAX_FRAMES")){
        cout << "can't find value of CURSOR_MAX_FRAMES." << endl;
        result = false;
    }

    if (!json.has<Number>("SPEAKER_OFFSET_X")){
        cout << "can't find value of SPEAKER_OFFSET_X." << endl;
        result = false;
    }

    if (!json.has<Number>("SPEAKER_OFFSET_Y")){
        cout << "can't find value of SPEAKER_OFFSET_Y." << endl;
        result = false;
    }

    return result;
}

bool CMessageBox::OnSubLoop()
{
   // bool __result = CBox::OnLoop();

    _speakerName.setPosition(CBox::GetPosition() + _speakerNameOffset);

    CTextProcessing::SetPosition(
        CBox::GetPosition().x + _msgOffset.x, 
        CBox::GetPosition().y + _msgOffset.y);

    if (IsTextAllShown() && !GetText().empty()){
        _frames.SetPosition(CTextProcessing::GetLastCharacterPos().x+5.0f,  
            CTextProcessing::GetLastCharacterPos().y);

        _frames.SetAlpha(255);
    }
    else
        _frames.SetAlpha(0);

    //__result = __result && _frames.OnLoop();
    CTextProcessing::OnLoop();
    return _isPaused || !IsTextAllShown();// || __result;
}

void CMessageBox::OnSubRender(sf::RenderWindow* Surf_Dest)
{
    Surf_Dest->draw(_speakerName);
    CTextProcessing::OnRender(Surf_Dest);
    _frames.OnRender(Surf_Dest);
}

void CMessageBox::SetSpeakerName(string name)
{
    CTextFunction::SetString(_speakerName, name.c_str());
}

void CMessageBox::SetText(string msg)
{
    _isPaused = true;
    CTextProcessing::SetText(msg);
}

bool CMessageBox::OnLButtonDown(int x, int y)
{
    if (IsTextAllShown())
        _isPaused = false;
    else
        Skip();

    return true;
}

bool CMessageBox::OnLButtonUp(int x, int y)
{
    return true;
}

bool CMessageBox::OnRButtonDown(int x, int y)
{
    return true;
}

bool CMessageBox::OnRButtonUp(int x, int y)
{
    return true;
}
