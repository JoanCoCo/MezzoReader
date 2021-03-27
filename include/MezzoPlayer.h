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
    /// Set of buffers to store the base notes.
    ALuint buffers[7];
    /// Source of the sounds.
    ALuint source;
    /// Environment of the player.
    ALuint environment;

    /// Listener position.
    static const ALfloat listenerPosition[3];
    /// Listener velocity.
    static const ALfloat listenerVelocity[3];
    /// Listener orientation.
    static const ALfloat listenerOrientation[6];

    /// Source position.
    static const ALfloat sourcePosition[3];
    /// Source velocity.
    static const ALfloat sourceVelocity[3];

    /// Size of the sound being loaded.
    ALsizei sizeAL;
    /// Frequency of the sound being loaded.
    ALsizei freqAL;
    /// Format of the sound being loaded.
    ALenum formatAL;
    /// Data of the sound being loaded.
    ALvoid *dataAL;

    /// Bool to indicate if there was some error while creating the player.
    bool errorFree;
    /// Error causing the bad creation of the player.
    ALenum error;

    /**
     * Loads the sound descrived in a WAV file to the selected buffer.
     *
     * @param buffer index in the array of buffers of the buffer where we
     *      want to load the sound.
     * @param file path to the file to be loaded.
     */
    void load_buffer_from_wav_file(int buffer, ALbyte* file);

    /**
     * Return the path to the audio file necessary to play a given tone.
     *
     * @param tone integer representing the tone we would like to play.
     */
    string get_tone_audio_file(int tone);

public:
    /**
     * Constructor of the class MezzoPlayer.
     */
    MezzoPlayer();

    /**
     * Deinitializer of the class MezzPlayer.
     */
    ~MezzoPlayer();

    /**
     * Play the given note if the player has been correctly
     * initialized.
     * 
     * @param note Note to be played.
     */
    void play(Note note);

    /**
     * Stop the sound reproduction if it's playing and the player 
     * has been correctly initialized.
     */
    void stop();

    /**
     * Says whenever or not the player is currently playing a note
     * sound.
     *
     * @return bool indicating if it is playing or not. If the player 
     *      hasn't been correctly initialized it will return always false.
     */
    bool is_playing();

    /**
     * Says whenever or not the player has been correctly created.
     *
     * @return bool indicating if the player was correctly created.
     */
    bool has_been_correctly_initialized();

    /**
     * Gets the error string in case the player wasn't correctly created or
     * the empty string otherwise.
     *
     * @return string with the error description. Returns an empty string in
     *      case of no error.
     */
    string get_error();
};

#endif