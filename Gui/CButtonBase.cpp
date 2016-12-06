/*
* Copyright (C) 2012-2015, <Aeihu.z, aeihu.z@gmail.com>.
*
* YOYO is a free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* Version 3(GPLv3) as published by the Free Software Foundation.
*/

#include "CButtonBase.h"
#include "../Sound/CSoundBank.h"

CButtonBase::CButtonBase(float x, float y, int w, int h, int maxframes, int framerate):CSequenceOfSprite(x,y,0,0,w,h)
{
    SetMaxFrames(maxframes);
    SetFrameRate(framerate);
    SetCurrentFrame(0);
    _Type = CAnimation::Backward;
    _isMouseDown = false;
    _isMouseOver = false;
    _seNameOfMouseOver = CCommon::_Common.MOUSEOVER_SE;
    _seNameOfMouseDown = CCommon::_Common.MOUSEDOWN_SE;
    SetPosition(x, y);
}

bool CButtonBase::OnLButtonDown(int x, int y)
{
    if (_isShowed)
        if (_isMouseOver){
            _isMouseDown = true;
            SetCurrentImageFrame(GetMaxFrames());
            SetCurrentFrame(GetMaxFrames()-1);
            CResourceControl::_ResourceManager._SoundControl.PlaySE(
                _seNameOfMouseDown, CCommon::_Common.SE_VOLUME, false);
            return true;
        }

    return false;
}

bool CButtonBase::OnLButtonUp(int x, int y)
{
    if (_isShowed){
        if(_isMouseOver){
            _isMouseDown = false;
            SetCurrentImageFrame(GetMaxFrames()-1);
            Exec();
            return true;
        }
        else{
            SetCurrentImageFrame(GetCurrentFrame());
        }
    }

    return false;
}

void CButtonBase::OnLoop()
{
    if (!_isMouseDown){
        CSequenceOfSprite::OnLoop();
    }
}

bool CButtonBase::IsMouseOver() const
{
    return _isMouseOver;
}

bool CButtonBase::IsMouseDown() const
{
    return _isMouseDown;
}

bool CButtonBase::OnMouseMove(int x, int y)
{
    if (_isShowed){
        if(( x > GetPosition().x ) && ( x < GetPosition().x + GetWidth()) && 
            ( y > GetPosition().y ) && ( y < GetPosition().y + GetHeight())){
            if (!_isMouseOver)
            {
                _isMouseOver = true;
                TurnOn(false);
            }

            _Type = CAnimation::Forward;
            sf::FloatRect d = _sprite.getLocalBounds();
            sf::FloatRect f = _sprite.getGlobalBounds();
            return true;
        }
        else{
            _isMouseOver = false;
            _isMouseDown = false;
            _Type = CAnimation::Backward;
            TurnOn(false);
        }
    }
    return false;
}

//================================
//property:
//* TILESET_PATH,
//* WIDTH,
//* HEIGHT,
//* MAX_FRAMES,
//* FRAME_RATE,
//* X,
//* Y,
//? ORDER,
//================================
bool CButtonBase::CheckList(Object json) 
{
    bool result = true;
    if (!json.has<String>("TILESET_PATH")){
        cout << "can't find value of TILESET_PATH." << endl;
        result = false;
    }

    if (!json.has<Number>("WIDTH")){
        cout << "can't find value of WIDTH." << endl;
        result = false;
    }

    if (!json.has<Number>("HEIGHT")){
        cout << "can't find value of HEIGHT." << endl;
        result = false;
    }

    if (!json.has<Number>("MAX_FRAMES")){
        cout << "can't find value of MAX_FRAMES." << endl;
        result = false;
    }

    if (!json.has<Number>("FRAME_RATE")){
        cout << "can't find value of FRAME_RATE." << endl;
        result = false;
    }

    if (!json.has<Number>("X")){
        cout << "can't find value of X." << endl;
        result = false;
    }

    if (!json.has<Number>("Y")){
        cout << "can't find value of Y." << endl;
        result = false;
    }

    return result;
}

bool CButtonBase::SetProperty(Object json, bool isLoad)
{    
    SetWidth(json.get<Number>("WIDTH"));
    SetHeight(json.get<Number>("HEIGHT"));

    if (isLoad)
        if (!LoadImg(json.get<String>("TILESET_PATH").c_str()))
            return false;

    SetMaxFrames(json.get<Number>("MAX_FRAMES"));
    SetFrameRate(json.get<Number>("FRAME_RATE"));
    SetPosition(json.get<Number>("X"), json.get<Number>("Y"));

    return true;
}
