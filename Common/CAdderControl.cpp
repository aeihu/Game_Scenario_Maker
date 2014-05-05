/*
* Copyright (C) 2012-2014, <Aeihu.z, aeihu.z@gmail.com>.
*
* Game Scenario Maker is a free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* Version 2(GPLv2) as published by the Free Software Foundation.
*/

#include "CAdder.h"

bool CAdderControl::IsAlreadyExists(int* pval)
{
    for (list<CfAdder>::iterator it=_fAdderList.begin() ; it != _fAdderList.end();it++){
        if ((*it)._pInt == pval)
            return true;
    }

    return false;
}

bool CAdderControl::IsAlreadyExists(float* pval)
{
    for (list<CfAdder>::iterator it=_fAdderList.begin() ; it != _fAdderList.end();it++){
        if ((*it)._pFloat == pval)
            return true;
    }

    return false;
}

void CAdderControl::Delete(int* pval, bool isGoFinish)
{
    for (list<CfAdder>::iterator it=_fAdderList.begin() ; it != _fAdderList.end();){
        if ((*it)._pInt == pval){
            if (isGoFinish)
                *((*it)._pInt) = static_cast<int>((*it)._goal);

            it=_fAdderList.erase(it);
            if (it == _fAdderList.end())
                break;
        }
        else{
            it++;
        }
    }
}

void CAdderControl::Delete(float* pval, bool isGoFinish)
{
    for (list<CfAdder>::iterator it=_fAdderList.begin() ; it != _fAdderList.end();){
        if ((*it)._pFloat == pval){
            if (isGoFinish)
                *((*it)._pFloat) = (*it)._goal;

            it=_fAdderList.erase(it);
            if (it == _fAdderList.end())
                break;
        }
        else{
            it++;
        }
    }
}

void CAdderControl::Clear()
{
    //_iAdderList.clear();
    _fAdderList.clear();
}

unsigned int CAdderControl::Count()
{
    return _fAdderList.size();
}

int CAdderControl::OnLoop()
{
    unsigned long __time = CCommon::_Common.GetTicks();
    int __result = 0;

    //for (list<CiAdder>::iterator it=_iAdderList.begin() ; it != _iAdderList.end();){
    //    if ((*it).Exec(__time)){
    //        it=_iAdderList.erase(it);
    //        if (it == _iAdderList.end())
    //            break;
    //    }
    //    else{
    //        if ((*it).IsPause())
    //            __result++;

    //        it++;
    //    }
    //}

    for (list<CfAdder>::iterator it=_fAdderList.begin() ; it != _fAdderList.end();){
        if ((*it).Exec(__time)){
            it=_fAdderList.erase(it);
            if (it == _fAdderList.end())
                break;
        }
        else{
            if ((*it).IsPause())
                __result++;

            it++;
        }
    }

    return __result;
}

void CAdderControl::Insert(char type, float goal, bool pause, int* val, float val1, float val2, float val3, float val4, bool isGoFinish)
{
    if (IsAlreadyExists(val))
        Delete(val,isGoFinish);
        
    _fAdderList.push_back(CfAdder(type, goal, pause, val, val1, val2, val3, val4));
}

void CAdderControl::Insert(char type, float goal, bool pause, float* val, float val1, float val2, float val3, float val4, bool isGoFinish)
{
    if (IsAlreadyExists(val))
        Delete(val,isGoFinish);

    _fAdderList.push_back(CfAdder(type, goal, pause, val, val1, val2, val3, val4));
}
        