/*
* Copyright (C) 2012-2015, <Aeihu.z, aeihu.z@gmail.com>.
*
* Game Scenario Maker is a free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* Version 2(GPLv2) as published by the Free Software Foundation.
*/

#ifndef _CSOUNDBANK_H_
    #define _CSOUNDBANK_H_

#include <SFML/Audio.hpp>
#include <map>
#include <list>
#include <string>
#include <iostream>
#include "../Common/CCommon.h"

using namespace std;
//==============================================================================

class CSoundBank {
    private:
        friend class        CResourceManager;

        class CVoiceStream : public sf::SoundStream
        {
            public:
                string                  _Name;

                CVoiceStream(){ _Name="", _isSilence = true; }
                void Load(const sf::SoundBuffer& buffer);
                bool IsSilence() const;
            private:
                bool                    _isSilence;
                vector<sf::Int16>       _m_samples;
                size_t                  _m_currentSample;

                virtual bool onGetData(Chunk& data);
                virtual void onSeek(sf::Time timeOffset);
        };

        list<CVoiceStream*>                             _voicePool;
        list<sf::Sound>                                 _soundPool;
        sf::Music                                       _bgm;
        map<string, sf::SoundBuffer>                    _voiceList;
        map<string, sf::SoundBuffer>                    _seList;

        int AddBuffer(map<string, sf::SoundBuffer>& list, string name, const char* FileName);
        bool DelBuffer(map<string, sf::SoundBuffer>& list, string name);
    public:
        enum EVoiceFlag{
            VOICE_STOPPED = 1,
            VOICE_SILENCE,
            VOICE_PLAYING,
        };

        //static CSoundBank                               _SoundControl;
        CSoundBank();

        bool OnInit();
        void OnLoop();
        void OnCleanup();

        int AddSE(string name, const char* FileName) ;
        bool PlaySE(string name);
        bool DeleteSE(string name);

        int AddVoice(string name, const char* FileName) ;
        bool PlayVoice(string name, bool isSameChannel=true);
        bool DeleteVoice(string name);
        bool GetVoiceStatus(string name);
        char IsVoiceSilence(string name);

        bool OnLoadBGM(const char* FileName);
        sf::Sound::Status GetBgmStatus();
        void PlayBgm();
        void PauseBgm();
};

//==============================================================================

#endif
