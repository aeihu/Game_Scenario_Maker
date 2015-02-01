/*
* Copyright (C) 2012-2015, <Aeihu.z, aeihu.z@gmail.com>.
*
* Game Scenario Maker is a free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* Version 2(GPLv2) as published by the Free Software Foundation.
*/

#ifndef _CACTIONBY_H_
    #define _CACTIONBY_H_

#include "CActionBaseClass.h"
#include "CCommon.h"

class CActionBy : public CActionBaseClass
{
    protected:
    public:
        CActionBy(float* val, size_t elapsed, float fin, bool restore=false, bool pause=false);

        bool IsPause();
        bool OnLoop(bool cleanup=true);
    private:
        bool        _restore;
        float       _orgVal;
        float       _byVal;
        float*      _val;
        float       _valOfFinish;
        float       _incr;
        bool        _isRunning;
        size_t      _elapsed;
};

#endif
