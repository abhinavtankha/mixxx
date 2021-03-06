/***************************************************************************
                          soundsource.h  -  description
                             -------------------
    begin                : Wed Feb 20 2002
    copyright            : (C) 2002 by Tue and Ken Haste Andersen
    email                :
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H

#include <QImage>
#include <QString>

#include <taglib/tfile.h>
#include <taglib/apetag.h>
#include <taglib/id3v2tag.h>
#include <taglib/xiphcomment.h>
#include <taglib/mp4tag.h>

#include "util/types.h"
#include "util/defs.h"

#define MIXXX_SOUNDSOURCE_API_VERSION 6
/** @note SoundSource API Version history:
           1 - Mixxx 1.8.0 Beta 2
           2 - Mixxx 1.9.0 Pre (added key code)
           3 - Mixxx 1.10.0 Pre (added freeing function for extensions)
           4 - Mixxx 1.11.0 Pre (added composer field to SoundSource)
           5 - Mixxx 1.12.0 Pre (added album artist and grouping fields to SoundSource)
           6 - Mixxx 1.13.0 (added cover art suppport)
  */

/** Getter function to be declared by all SoundSource plugins */
namespace Mixxx {
    class SoundSource;
}
typedef Mixxx::SoundSource* (*getSoundSourceFunc)(QString filename);
typedef char** (*getSupportedFileExtensionsFunc)();
typedef int (*getSoundSourceAPIVersionFunc)();
/* New in version 3 */
typedef void (*freeFileExtensionsFunc)(char** exts);


/*
  Base class for sound sources.
*/
namespace Mixxx
{
class SoundSource
{
public:
    SoundSource(QString qFilename);
    virtual ~SoundSource();
    virtual Result open() = 0;
    virtual long seek(long) = 0;
    virtual unsigned read(unsigned long size, const SAMPLE*) = 0;
    virtual long unsigned length() = 0;
    static float str2bpm( QString sBpm );
    virtual Result parseHeader() = 0;

    // Returns the first cover art image embedded within the file (if any).
    virtual QImage parseCoverArt() = 0;

    //static QList<QString> supportedFileExtensions(); //CRAP can't do this!
    /** Return a list of cue points stored in the file */
    virtual QList<long> *getCuePoints();
    /** Returns filename */
    virtual QString getFilename();
    /** Return artist name */
    virtual QString getArtist();
    /** Return track title */
    virtual QString getTitle();
    virtual QString getAlbum();
    virtual QString getAlbumArtist();
    virtual QString getType();
    virtual QString getComment();
    virtual QString getYear();
    virtual QString getGenre();
    virtual QString getComposer();
    virtual QString getGrouping();
    virtual QString getTrackNumber();
    virtual float getReplayGain();
    virtual QString getKey();
    virtual float getBPM();
    virtual int getDuration();
    virtual int getBitrate();
    virtual unsigned int getSampleRate();
    virtual int getChannels();

    virtual void setArtist(QString);
    virtual void setTitle(QString);
    virtual void setAlbum(QString);
    virtual void setAlbumArtist(QString);
    virtual void setType(QString);
    virtual void setComment(QString);
    virtual void setYear(QString);
    virtual void setGenre(QString);
    virtual void setComposer(QString);
    virtual void setGrouping(QString);
    virtual void setTrackNumber(QString);
    virtual void setReplayGain(float);
    virtual void setKey(QString);
    virtual void setBPM(float);
    virtual void setDuration(int);
    virtual void setBitrate(int);
    virtual void setSampleRate(unsigned int);
    virtual void setChannels(int);

protected:
    // Automatically collects generic data from a TagLib File: title, artist,
    // album, comment, genre, year, tracknumber, duration, bitrate, samplerate,
    // and channels.
    bool processTaglibFile(TagLib::File& f);
    bool processID3v2Tag(TagLib::ID3v2::Tag* id3v2);
    bool processAPETag(TagLib::APE::Tag* ape);
    bool processXiphComment(TagLib::Ogg::XiphComment* xiph);
    bool processMP4Tag(TagLib::MP4::Tag* mp4);
    void processBpmString(QString tagName, QString sBpm);
    void parseReplayGainString(QString sReplayGain);

    // In order to avoid processing images when it's not
    // needed (TIO building), we must process it separately.
    QImage getCoverInID3v2Tag(TagLib::ID3v2::Tag* id3v2);
    QImage getCoverInAPETag(TagLib::APE::Tag* ape);
    QImage getCoverInXiphComment(TagLib::Ogg::XiphComment* xiph);
    QImage getCoverInMP4Tag(TagLib::MP4::Tag* mp4);

    // Taglib strings can be NULL and using it could cause some segfaults,
    // so in this case it will return a QString()
    QString toQString(TagLib::String tstring) const;

    /** File name */
    QString m_qFilename;

    QString m_sArtist;
    QString m_sTitle;
    QString m_sAlbum;
    QString m_sAlbumArtist;
    QString m_sType;
    QString m_sComment;
    QString m_sYear;
    QString m_sGenre;
    QString m_sComposer;
    QString m_sGrouping;
    QString m_sTrackNumber;
    float m_fReplayGain;
    QString m_sKey;
    float m_fBPM;
    int m_iDuration;
    int m_iBitrate;
    /** Sample rate of the file */
    unsigned int m_iSampleRate;
    int m_iChannels;
    //Dontcha be forgettin' to initialize these variables.... arr

    static const bool s_bDebugMetadata;
};
} //namespace Mixxx

#endif
