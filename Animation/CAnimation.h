/*
* Copyright (C) 2012-2014, <Aeihu.z, aeihu.z@gmail.com>.
*
* Game Scenario Maker is a free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* Version 2(GPLv2) as published by the Free Software Foundation.
*/
#ifndef _CANIMATION_H_
  #define _CANIMATION_H_

//==============================================================================
class CAnimation {
    private:
        bool                    _enable;
        int                     _currentFrame;
        int                     _frameInc;
        int                     _frameRate; //Milliseconds
        unsigned long           _oldTime;

        void                    OnOscillate();
        void                    OnALoop();
        void                    OnForWard();
        void                    OnBackWard();
    public:
        typedef enum{
            Oscillate,
            Loop,
            Forward,
            Backward
        } eAnimationType;

        eAnimationType          _Type;
        int                     _MaxFrames;

        CAnimation();
        void OnAnimate(unsigned long time);
        void SetFrameRate(int rate);
        void SetCurrentFrame(int frame);
        int GetCurrentFrame() const;
        bool GetEnable() const;
        void SetFrameInc(int inc);
        void TurnOn();
        void TurnOff(int frame = 0);
};

//==============================================================================

#endif
