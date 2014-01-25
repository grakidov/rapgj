// Waves.h: interface for the CWaves class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CWAVES_H_
#define _CWAVES_H_

//#include <windows.h>
#include <stdio.h>
#include <PSSG.h>

#define MAX_NUM_WAVEID			1024

// Speaker Positions:
#define SPEAKER_FRONT_LEFT              0x1
#define SPEAKER_FRONT_RIGHT             0x2
#define SPEAKER_FRONT_CENTER            0x4
#define SPEAKER_LOW_FREQUENCY           0x8
#define SPEAKER_BACK_LEFT               0x10
#define SPEAKER_BACK_RIGHT              0x20
#define SPEAKER_FRONT_LEFT_OF_CENTER    0x40
#define SPEAKER_FRONT_RIGHT_OF_CENTER   0x80
#define SPEAKER_BACK_CENTER             0x100
#define SPEAKER_SIDE_LEFT               0x200
#define SPEAKER_SIDE_RIGHT              0x400
#define SPEAKER_TOP_CENTER              0x800
#define SPEAKER_TOP_FRONT_LEFT          0x1000
#define SPEAKER_TOP_FRONT_CENTER        0x2000
#define SPEAKER_TOP_FRONT_RIGHT         0x4000
#define SPEAKER_TOP_BACK_LEFT           0x8000
#define SPEAKER_TOP_BACK_CENTER         0x10000
#define SPEAKER_TOP_BACK_RIGHT          0x20000

#if !defined(WAVE_FORMAT_EXTENSIBLE)
#define  WAVE_FORMAT_EXTENSIBLE                 0xFFFE
#endif // !defined(WAVE_FORMAT_EXTENSIBLE)

#ifndef WAVE_FORMAT_PCM

/* OLD general waveform format structure (information common to all formats) */
typedef struct waveformat_tag {
    unsigned short	wFormatTag;        /* format type */
    unsigned short	nChannels;         /* number of channels (i.e. mono, stereo, etc.) */
    unsigned int	nSamplesPerSec;    /* sample rate */
    unsigned int	nAvgBytesPerSec;   /* for buffer estimation */
    unsigned short	nBlockAlign;       /* block size of data */
} WAVEFORMAT, *PWAVEFORMAT;

/* flags for wFormatTag field of WAVEFORMAT */
#define WAVE_FORMAT_PCM     1


/* specific waveform format structure for PCM data */
typedef struct pcmwaveformat_tag {
    WAVEFORMAT		wf;
    unsigned short	wBitsPerSample;
} PCMWAVEFORMAT, *PPCMWAVEFORMAT;
#endif /* WAVE_FORMAT_PCM */

enum WAVEFILETYPE
{
	WF_EX  = 1,
	WF_EXT = 2,
	WF_MSF = 3,
};

enum WAVERESULT
{
	WR_OK = 0,
	WR_INVALIDFILENAME					= - 1,
	WR_BADWAVEFILE						= - 2,
	WR_INVALIDPARAM						= - 3,
	WR_INVALIDWAVEID					= - 4,
	WR_NOTSUPPORTEDYET					= - 5,
	WR_WAVEMUSTBEMONO					= - 6,
	WR_WAVEMUSTBEWAVEFORMATPCM			= - 7,
	WR_WAVESMUSTHAVESAMEBITRESOLUTION	= - 8,
	WR_WAVESMUSTHAVESAMEFREQUENCY		= - 9,
	WR_WAVESMUSTHAVESAMEBITRATE			= -10,
	WR_WAVESMUSTHAVESAMEBLOCKALIGNMENT	= -11,
	WR_OFFSETOUTOFDATARANGE				= -12,
	WR_FILEERROR						= -13,
	WR_OUTOFMEMORY						= -14,
	WR_INVALIDSPEAKERPOS				= -15,
	WR_INVALIDWAVEFILETYPE				= -16,
	WR_NOTWAVEFORMATEXTENSIBLEFORMAT	= -17
};

#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_
typedef struct tWAVEFORMATEX
{
    unsigned short	wFormatTag;
    unsigned short	nChannels;
    unsigned int	nSamplesPerSec;
    unsigned int	nAvgBytesPerSec;
    unsigned short	nBlockAlign;
    unsigned short	wBitsPerSample;
    unsigned short	cbSize;
} WAVEFORMATEX;
#endif /* _WAVEFORMATEX_ */

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_

#ifdef __CELLOS_LV2__
typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} GUID;
#endif//__CELLOS_LV2__

typedef struct {
    WAVEFORMATEX    Format;
    union {
        unsigned short wValidBitsPerSample;       /* bits of precision  */
        unsigned short wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
        unsigned short wReserved;                 /* If neither applies, set to zero. */
    } Samples;
    unsigned int	dwChannelMask;      /* which channels are */
                                        /* present in stream  */
    GUID            SubFormat;
} WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
#endif // !_WAVEFORMATEXTENSIBLE_

typedef struct {
	int sampleSize;
	int sampleRate;
	int miscInfo;
	int LoopMarkers[10];
} MSFDATA;

typedef struct {
	int compressionType;
	int channels;
	MSFDATA msData;
} MSFFORMAT;

typedef struct
{
	WAVEFILETYPE	wfType;
	union
	{
		WAVEFORMATEXTENSIBLE wfEXT;		// For non-WAVEFORMATEXTENSIBLE wavefiles, the header is stored in the Format member of wfEXT
		MSFFORMAT wfMSF;
	};
	char			*pData;
	unsigned long	ulDataSize;
	FILE			*pFile;
	unsigned long	ulDataOffset;
} WAVEFILEINFO, *LPWAVEFILEINFO;

typedef int (*PFNALGETENUMVALUE)( const char *szEnumName );
typedef int	WAVEID;

PSSG::PResult ParseFile(const char *szFilename, LPWAVEFILEINFO pWaveInfo);
PSSG::PResult ParseFileFromBuffer(const void *bufferData, unsigned int bufferSize, LPWAVEFILEINFO pWaveInfo);
PSSG::PResult LoadWaveFile(const char *szFilename, WAVEFILEINFO& waveInfo);

void* GetWaveData(WAVEFILEINFO& waveInfo);
unsigned long GetWaveSize(WAVEFILEINFO& waveInfo);
unsigned long GetWaveFrequency(WAVEFILEINFO& waveInfo);
unsigned long GetWaveDataOffset(WAVEFILEINFO& waveInfo);

PSSG::PResult GetWaveALBufferFormat(WAVEFILEINFO& waveInfo, PFNALGETENUMVALUE pfnGetEnumValue, unsigned long *pulFormat);

class CWaves  
{
public:
	CWaves();
	virtual ~CWaves();

	//PSSG::PResult LoadWaveFile(const char *szFilename, WAVEID *WaveID);
	//PSSG::PResult OpenWaveFile(const char *szFilename, WAVEID *WaveID);
	WAVERESULT ReadWaveData(WAVEID WaveID, void *pData, unsigned long ulDataSize, unsigned long *pulBytesWritten);
	WAVERESULT SetWaveDataOffset(WAVEID WaveID, unsigned long ulOffset);
	WAVERESULT GetWaveType(WAVEID WaveID, WAVEFILETYPE *pwfType);
	WAVERESULT GetWaveFormatExHeader(WAVEID WaveID, WAVEFORMATEX *pWFEX);
	WAVERESULT GetWaveFormatExtensibleHeader(WAVEID WaveID, WAVEFORMATEXTENSIBLE *pWFEXT);
	WAVERESULT DeleteWaveFile(WAVEID WaveID);

	char *GetErrorString(WAVERESULT wr, char *szErrorString, unsigned long nSizeOfErrorString);
	bool IsWaveID(WAVEID WaveID);

private:
	//PSSG::PResult ParseFile(const char *szFilename, LPWAVEFILEINFO pWaveInfo);
	WAVEID InsertWaveID(LPWAVEFILEINFO pWaveFileInfo);
	
	LPWAVEFILEINFO	m_WaveIDs[MAX_NUM_WAVEID];
};

#endif // _CWAVES_H_
