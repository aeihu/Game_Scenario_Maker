/*
* Copyright (C) 2012-2015, <Aeihu.z, aeihu.z@gmail.com>.
*
* Game Scenario Maker is a free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* Version 2(GPLv2) as published by the Free Software Foundation.
*/

#include "CResourceControl.h"
#include "../Parser/CParser.h"

CResourceControl  CResourceControl::_ResourceManager;

CResourceControl::CResourceControl():_threadOfLoading(&CResourceControl::LoadAsset, this)
{
    _EffectObjCtrlEnable = _DrawableObjCtrlEnable = false;
    _script.reset();
    _fileNameOfScript = "";
}

string CResourceControl::GetNameInFilename(string filename)
{
    size_t __last_x_pos = filename.find_last_of('/');
    size_t __last_s_pos = filename.find_last_of('*');
    __last_x_pos = __last_x_pos == string::npos ? 0 : __last_x_pos;
    __last_s_pos = __last_s_pos == string::npos ? 0 : __last_s_pos;
    string __result = "";

    if (__last_s_pos > __last_x_pos)
        __result = filename.substr(__last_s_pos+1);
    else
        __result = filename.substr(__last_x_pos+1);
    
    size_t __last_p_pos = __result.find_last_of('.');
    if (__last_p_pos != string::npos)
        __result = __result.substr(0, __last_p_pos);

    return __result;
}

bool CResourceControl::CheckOut(Object& json, string colName, string objTypeName)
{
    if (!json.has<Array>(colName))
        return false;

    Array __array = json.get<Array>(colName);
    
    for (size_t i=0; i<__array.size(); i++){
        if (__array.get<Object>(i).get<bool>("isdelete")){
            if (objTypeName == "Font"){
                _ObjectControl.DelObject(__array.get<Object>(i).get<String>("name"));
            }
            else if (objTypeName == "Voice"){
                _SoundControl.DeleteVoice(__array.get<Object>(i).get<String>("name"));
            }
            else if (objTypeName == "Se"){
                _SoundControl.DeleteSE(__array.get<Object>(i).get<String>("name"));
            }
            else if (objTypeName == "Camera"){
                _CameraControl.DelCamera(__array.get<Object>(i).get<String>("name"));
            }
            else if (objTypeName == "Music"){
                _SoundControl.DelBgm(__array.get<Object>(i).get<String>("name"));
            }
            else if (objTypeName == "Text"){
                _DrawableObjectControl.DelDrawableObject(__array.get<Object>(i).get<String>("name"));
            }
            else{
                _DrawableObjectControl.DelDrawableObject(__array.get<Object>(i).get<String>("name"));
            }
        }
    }

    return true;
    
}

//
// result: 
//      1: normal
//      0: column is 0
//      -1: can't find column
//      -2: failed to create asset
//
char CResourceControl::CheckIn(Object& json, string colName, string objTypeName)
{
    if (!json.has<Array>(colName))
        return -1;

    Array& __array = json.get<Array>(colName);

    string __path;
    
    for (size_t i=0; i<__array.size(); i++){
        __path = __array.get<Object>(i).get<String>("path");
        string& __assetName = __array.get<Object>(i).get<String>("name");

        cout << "CResourceControl::CheckIn():Loading '" << __assetName << "':'" << __path << "'" << endl;

        if (objTypeName == "Font"){
            if (!_ObjectControl.AddObject(__assetName,
                objTypeName,
                __path))
                return -2;
            
            __assetName = objTypeName + ":" + __assetName;
        }
        else if (objTypeName == "Voice"){
            if (_SoundControl.AddVoice(__assetName,
                __path) != 0)
                return -2;
        }
        else if (objTypeName == "Se"){
            if (_SoundControl.AddSE(__assetName,
                __path) != 0)
                return -2;
        }
        else if (objTypeName == "Camera"){
            if (!_CameraControl.AddCamera(__assetName,
                __path))
                return -2;
        }
        else if (objTypeName == "LoadingImg"){
            objTypeName = "Img";
            if (!_EffectObjectControl.AddDrawableObject(
                __assetName, 
                objTypeName, 
                __path))
                return -2;

            __assetName = objTypeName + ":" + __assetName;
        }
        else if (objTypeName == "Music"){
            if (!_SoundControl.AddBgm(
                __assetName, 
                __path))
                return -2;
        }
        else if (objTypeName == "Text"){
            if (!_DrawableObjectControl.AddDrawableObject(
                __assetName, 
                objTypeName, 
                __path))
                return -2;

            __assetName = objTypeName + ":" + __assetName;
        }
        else{
            if (!_DrawableObjectControl.AddDrawableObject(
                __assetName, 
                objTypeName, 
                __path))
                return -2;

            __assetName = objTypeName + ":" + __assetName;
        }
    }

    if (__array.size() < 1)
        return 0;

    return 1;
}

bool CResourceControl::OnInit(string filename, sf::RenderTarget* display)
{
    if (!_CameraControl.OnInit(display))
        return false;

    if (!_SoundControl.OnInit())
        return false;

    if (!OnInit(filename))
        return false;

    return true;
}
        
bool CResourceControl::JsonProcess(Object& src, Object& des, string colName)
{
    if (src.has<Array>(colName)){
        Array __array = src.get<Array>(colName);
        Array __arrayResult;

        if (colName == "text"){
            for (size_t i=0; i<__array.size(); i++){
                if (__array.get<Object>(i).has<String>("name")
                    &&
                    __array.get<Object>(i).has<String>("font")){

                    Object __obj;
                    __obj << "name" << __array.get<Object>(i).get<String>("name");
                    __obj << "path" << __array.get<Object>(i).get<String>("font");
                    __obj << "isdelete" << true;
                    __arrayResult << __obj;
                }
            }
        }
        else{
            for (size_t i=0; i<__array.size(); i++){
                Object __obj;
                __obj << "name" << GetNameInFilename(__array.get<String>(i));
                __obj << "path" << __array.get<String>(i);
                __obj << "isdelete" << true;
                __arrayResult << __obj;
            }
        }

        des << colName << __arrayResult;
        return true;
    }

    return false;
}

bool CResourceControl::LoadJson(Object& obj, string filename)
{
    Object __json;

    if (!__json.parse(Cio::LoadTxtFile(filename)))
        return false;

    JsonProcess(__json, obj, "font");
    JsonProcess(__json, obj, "se");
    JsonProcess(__json, obj, "image_for_loading");
    JsonProcess(__json, obj, "image_for_effect");
    JsonProcess(__json, obj, "messagebox");
    JsonProcess(__json, obj, "button");
    JsonProcess(__json, obj, "camera");
    JsonProcess(__json, obj, "character");
    JsonProcess(__json, obj, "background");
    JsonProcess(__json, obj, "cg");
    JsonProcess(__json, obj, "voice");
    JsonProcess(__json, obj, "music");
    JsonProcess(__json, obj, "text");

    if (__json.has<String>("main_script"))
        obj << "main_script" << __json.get<String>("main_script");

    if (__json.has<Array>("script"))
        obj << "script" << __json.get<Array>("script");
    
    return true;
}

bool CResourceControl::OnInit(string filename)
{
    if (!LoadJson(_gameBaiscAsset, filename))
        return false;
    
    if (CheckIn(_gameBaiscAsset, "font", "Font") < 1) return false;
    if (CheckIn(_gameBaiscAsset, "text", "Text") < 0) return false;
    if (CheckIn(_gameBaiscAsset, "se", "Se") < 0) return false;
    if (CheckIn(_gameBaiscAsset, "image_for_loading", "LoadingImg") < 1) return false;
    if (CheckIn(_gameBaiscAsset, "image_for_effect", "Img") < 0) return false;
    if (CheckIn(_gameBaiscAsset, "messagebox", "MessageBox") < 1) return false;
    if (CheckIn(_gameBaiscAsset, "button", "Button") < 0) return false;
    if (CheckIn(_gameBaiscAsset, "camera", "Camera") < 0) return false;
    //if (CheckIn(_gameBaiscAsset, "music", "Music") < 0) return false;

    return LoadScript(_gameBaiscAsset.get<String>("main_script"));
}

void CResourceControl::BeginLoadProcess()
{
    _threadOfLoading.launch();
}

void CResourceControl::EndLoadProcess()
{
    _EffectObjCtrlEnable = false;
}

void CResourceControl::Compare(Object& src, Object& des, string colName)
{
    if (src.has<Array>(colName) && des.has<Array>(colName)){
        for (size_t i=0; i < src.get<Array>(colName).size(); i++){
            for (size_t j=0; j < des.get<Array>(colName).size(); j++){
                if (src.get<Array>(colName).get<Object>(i).get<String>("name")
                    == 
                    des.get<Array>(colName).get<Object>(j).get<String>("name")){
                    
                    bool& __isDelete = des.get<Array>(colName).get<Object>(j).get<bool>("isdelete");
                    __isDelete = false;
                }
            }
        }
    }
}

void CResourceControl::LoadAsset()
{
    CParser::_Parser.Pause();
    _DrawableObjCtrlEnable = false;

    Object __obj;
    if (LoadJson(__obj, _fileNameOfScript)){
        if (!_script.empty()){
            Compare(__obj, _script, "character");
            Compare(__obj, _script, "background");
            Compare(__obj, _script, "cg");
            Compare(__obj, _script, "button");
            Compare(__obj, _script, "se");
            Compare(__obj, _script, "voice");
            Compare(__obj, _script, "camera");
            Compare(__obj, _script, "music");
            
            CheckOut(_script, "text", "Text");
            CheckOut(_script, "character","CharacterLayer");
            CheckOut(_script, "background","Background");
            CheckOut(_script, "cg","Img");
            CheckOut(_script, "button","Button");
            CheckOut(_script, "se", "Se");
            CheckOut(_script, "voice", "Voice");
            CheckOut(_script, "camera", "Camera");
            CheckOut(_script, "music", "Music");
            _script.reset();
        }
        
        _script = __obj;
        _script << "filename" << _fileNameOfScript;

        CheckIn(_script, "text", "Text");
        CheckIn(_script, "character","CharacterLayer");
        CheckIn(_script, "background","Background");
        CheckIn(_script, "cg","Img");
        CheckIn(_script, "button","Button");
        CheckIn(_script, "se", "Se");
        CheckIn(_script, "voice", "Voice");
        CheckIn(_script, "camera", "Camera");
        CheckIn(_script, "music", "Music");

        if (_script.has<Array>("script")){
            CParser::_Parser.InsertCmd(_script.get<Array>("script"));
        }
    }

    CDrawableClass* __img = _EffectObjectControl.GetDrawableObject(_nameOfLoadingImg);
    if (__img != NULL){
        CSequenceOfAction* __seq = new CSequenceOfAction();
        __seq->AddAction(__img->CreateActionOfAlphaTo(500,0,false,true));
        __seq->AddAction(new CClassFuncOfAction<CResourceControl>(this, &CResourceControl::EndLoadProcess));
        _ActionControl.AddAction(__seq);
    }
    
    _DrawableObjCtrlEnable = true;
    CParser::_Parser.Continue();
}

bool CResourceControl::LoadScript(string filename)
{
    _fileNameOfScript = filename;
    if (_gameBaiscAsset.has<Array>("image_for_loading")){
        Array __imgs = _gameBaiscAsset.get<Array>("image_for_loading");
        _nameOfLoadingImg = __imgs.get<Object>(std::rand() % __imgs.size()).get<String>("name");
        CDrawableClass* __img = _EffectObjectControl.GetDrawableObject(_nameOfLoadingImg);

        if (__img != NULL){
            CSequenceOfAction* __seq = new CSequenceOfAction();
            _EffectObjCtrlEnable = true;
            __seq->AddAction(__img->CreateActionOfAlphaTo(500,255,false,true));
            __seq->AddAction(new CClassFuncOfAction<CResourceControl>(this, &CResourceControl::BeginLoadProcess));
            _ActionControl.AddAction(__seq);
            __img->SetLayerOrder(120);
            //_EffectObjectControl.SetDrawableObjectLayerOrder(_nameOfLoadingImg, 120);
        }
    }

    return true;
}

void CResourceControl::OnLoop()
{
    _CameraControl.OnLoop();

    if (_DrawableObjCtrlEnable)
        _DrawableObjectControl.OnLoop();

    if (_EffectObjCtrlEnable)
        _EffectObjectControl.OnLoop();

    _SoundControl.OnLoop();
    
    _ActionControl.OnLoop();

    if (_ActionControl.IsPause())
        return;
    
    CParser::_Parser.OnLoop();
}

void CResourceControl::OnRender(sf::RenderTarget* Surf_Dest)
{
    if (_DrawableObjCtrlEnable)
        _DrawableObjectControl.OnRender(Surf_Dest);
    
    if (_EffectObjCtrlEnable)
        _EffectObjectControl.OnRender(Surf_Dest);
}

void CResourceControl::OnCleanup()
{
    _DrawableObjectControl.OnCleanup();
    _EffectObjectControl.OnCleanup();
}

void CResourceControl::OnSaveData()
{
    //ofstream __savefile("./userdata/1.txt");
    //Object __json;
    //__json.reset();

    //if(!__savefile){
    //    cout << "error" << endl;
    //    return;
    //}

    //for (size_t i=0; i<_drawableObjectList.size(); i++){
    //    cout << "_(:3J Z)_" <<endl;
    //    __savefile << "name=" << _drawableObjectList[i].first <<endl;
    //    _drawableObjectList[i].second->OnSaveData(__json);
    //    cout << "(:3[____]" <<endl;
    //}

    //__savefile.close();
}

void CResourceControl::OnLoadData()
{

}
