#if defined(DOJA)

#define SOUND_DATA            MediaSound
#define SOUND_PLAYER          AudioPresenter
#define GET_SOUND_PLAYER()    AudioPresenter.getAudioPresenter();
#define SOUND_PLAYER_SET(p,s) p.setSound(s)

/**
*   JPHONE Java アプリのサウンド再生ルーチン
*/
#elif defined(JPHONE)

#define SOUND_DATA            Phrase
#define SOUND_PLAYER          PhraseTrack
#define GET_SOUND_PLAYER()    PhrasePlayer.getPlayer().getTrack()
#define SOUND_PLAYER_SET(p,s) p.setPhrase(s)

#else
#error

#endif
