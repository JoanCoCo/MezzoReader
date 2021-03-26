/**
 * MezzoPlayer.h
 *  
 * SMII 2020/2021
 * Joan Colom Colom
 *  
 * Description of the class MezzoPlayer.
 */

#ifndef MEZZOPLAYER
#define MEZZOPLAYER

#if __APPLE__
    #include <OpenAL/alut.h>
#elif
    #include <AL/alut.h>
#endif
#include <iostream>
#include "Note.h"

using namespace std;

class MezzoPlayer
{
private:
    ALuint buffers[7];
    ALuint source;
    ALuint environment;

    static const ALfloat listenerPosition[3];
    static const ALfloat listenerVelocity[3];
    static const ALfloat listenerOrientation[6];

    static const ALfloat sourcePosition[3];
    static const ALfloat sourceVelocity[3];

    ALsizei sizeAL;
    ALsizei freqAL;
    ALenum formatAL;
    ALvoid *dataAL;
    bool errorFree;

    void LoadBufferFromWAVFile(int buffer, ALbyte* file);
    string GetToneAudioFile(int tone);

public:
    MezzoPlayer();
    ~MezzoPlayer();
    void Play(Note note);
    void Stop();
    bool IsPlaying();
    void WaitToFinishPlaying();
};

#endif