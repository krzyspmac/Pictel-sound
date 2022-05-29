//
//  pictel_sound_file.cpp
//  Pictel-sound
//
//  Created by krzysp on 28/05/2022.
//

#include "player.hpp"

using namespace PictelSound;

Player::Player(DecoderI *decoder, SystemAudioI *systemAudio)
:   __PlayerI(decoder, systemAudio)
,   m_decoder(decoder)
,   m_systemAudio(systemAudio)
{
}

Player::~Player()
{
    Close();
}

bool Player::Open()
{
    if (!m_decoder->Open())
    {   return false;
    }

    m_systemAudio->SetDecoder(m_decoder);
    m_systemAudio->PrepareToPlay();
    return true;
}

void Player::Play()
{
    m_systemAudio->Play();
}

void Player::Pause()
{
    m_systemAudio->Pause();
}

void Player::Stop()
{
    m_systemAudio->Stop();
}

void Player::Close()
{
    if (m_systemAudio != nullptr)
    {   m_systemAudio->Stop();
        m_systemAudio->Free();
        delete m_systemAudio;
    }

    if (m_decoder != nullptr)
    {   m_decoder->Close();
        delete m_decoder;
        m_decoder = NULL;
    }
}

void Player::SetVolume(double value)
{
    m_systemAudio->SetVolume(value);
}

void Player::SetLoops(bool loops)
{
    m_systemAudio->SetLoops(loops);
}

PlayerState Player::GetState()
{
    return PLAYER_STOPPED;
}
