//
// C++ Interface: config
//

#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

#include "pluginloader.h"
#include "metadata/tagengine.h"
#include "conversionoptionsmanager.h"
#include "codecoptimizations.h"

#include <QDomDocument>

#define ConfigUpdateDelay 100

class Logger;


/**
	@author Daniel Faust <hessijames@gmail.com>
*/
class Config : public QObject
{
    Q_OBJECT
public:
    struct CodecData
    {
        QString codecName;
        QStringList encoders;
        QStringList decoders;
        QStringList replaygain;
    };

    struct ProfileData
    {
        QString fileName;
        QString profileName;
        QString pluginName;
        QString codecName;
        QDomDocument data;
    };

    struct Data
    {
        struct General
        {
            int startTab;
            int lastTab;
            QString defaultProfile;
            QString lastProfile; // may be obsolete
            QString defaultFormat;
            QString lastFormat;
            int lastOutputDirectoryMode;
            QString specifyOutputDirectory;
            QString metaDataOutputDirectory;
            QString copyStructureOutputDirectory;
            QStringList lastMetaDataOutputDirectoryPaths;
            QStringList lastNormalOutputDirectoryPaths;
//             int priority;
            bool waitForAlbumGain;
            bool useVFATNames;
            bool writeLogFiles;
            enum ConflictHandling
            {
                NewFileName = 0,
                Skip = 1,
                Overwrite = 2
            } conflictHandling;
            int numFiles;
            int numReplayGainFiles;
//             bool executeUserScript;
//             bool showToolBar;
//             int outputFilePermissions;
            QStringList actionMenuConvertMimeTypes;
            QStringList actionMenuReplayGainMimeTypes;
            enum ReplayGainGrouping
            {
                AlbumDirectory = 0,
                Album = 1,
                Directory = 2
            } replayGainGrouping;
            QString preferredOggVorbisExtension;
            QString preferredVorbisCommentCommentTag;
            QString preferredVorbisCommentTrackTotalTag;
            QString preferredVorbisCommentDiscTotalTag;
        } general;

        struct Advanced
        {
            bool useSharedMemoryForTempFiles;
            int maxSizeForSharedMemoryTempFiles; // maximum file size for storing in shared memory [MiB]
            int sharedMemorySize; // the size of the tmpfs [MiB]
            bool usePipes;
        } advanced;

        struct CoverArt
        {
            int writeCovers;
            int writeCoverName;
            QString writeCoverDefaultName;
        } coverArt;

        struct Backends
        {
            QStringList rippers;
            QList<CodecData> codecs;
            QStringList filters;
            QStringList enabledFilters;
        } backends;

        struct BackendOptimizationsIgnoreList
        {
            QList<CodecOptimizations::Optimization> optimizationList;
        } backendOptimizationsIgnoreList;

        struct App
        {
            int configVersion;
        } app;

        QMap<QString,ConversionOptions*> profiles;

    } data;

    Config( Logger *_logger, QObject *parent );

    ~Config();

    void load();
    void save();

    /// returns a list of all working custom profiles
    QStringList customProfiles();

    /// Check if new backends got installed and the backend settings can be optimized
    QList<CodecOptimizations::Optimization> getOptimizations( bool includeIgnored = false );

    PluginLoader *pluginLoader() { return pPluginLoader; }
    TagEngine *tagEngine() { return pTagEngine; }
    ConversionOptionsManager *conversionOptionsManager() { return pConversionOptionsManager; }

public slots:
    /// Optimize backend settings according to the user input
    void doOptimizations( const QList<CodecOptimizations::Optimization>& optimizationList );

signals:
    /// connected to logger
    void updateWriteLogFilesSetting( bool writeLogFiles );

private:
    Logger *logger;

    PluginLoader *pPluginLoader;
    TagEngine *pTagEngine;
    ConversionOptionsManager *pConversionOptionsManager;

    void writeServiceMenu();
};

#endif
