/*
 * Copyright (c) 2006, Creative Labs Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided
 * that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice, this list of conditions and
 * 	     the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions
 * 	     and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *     * Neither the name of Creative Labs Inc. nor the names of its contributors may be used to endorse or
 * 	     promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "PSSGWaves.h"
//#include <ks.h>
//#include <ksmedia.h>
#include <string.h>
#include <errno.h>
#include <PSSGFileStream.h>

#pragma pack(push, 4)

typedef struct
{
	char			szRIFF[4];
	union
	{
		unsigned long	ulRIFFSize;
		int				msfCompressionType;
	};
	union
	{
		char	szWAVE[4];
		int		msfChannels;
	};
} WAVEFILEHEADER;

typedef struct
{
	char			szChunkName[4];
	unsigned long	ulChunkSize;
} RIFFCHUNK;

typedef struct
{
	unsigned short	usFormatTag;
	unsigned short	usChannels;
	unsigned long	ulSamplesPerSec;
	unsigned long	ulAvgBytesPerSec;
	unsigned short	usBlockAlign;
	unsigned short	usBitsPerSample;
	unsigned short	usSize;
	unsigned short  usReserved;
	unsigned long	ulChannelMask;
    GUID            guidSubFormat;
} WAVEFMT;

#pragma pack(pop)

#if _MSC_VER <= 1310 

// Wrap around the deprecated functions for VS2003 support
int fopen_s( FILE** pFile, const char *filename, const char *mode )
{
    *pFile = fopen( filename, mode );
    
    return *pFile ? 0 : EBADF;
}

int strncpy_s( char *strDest, size_t numberOfElements, const char *strSource, size_t count )
{
    strncpy( strDest, strSource, count );

    return 0;
}

#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWaves::CWaves()
{
	memset(&m_WaveIDs, 0, sizeof(m_WaveIDs));
}

CWaves::~CWaves()
{
	long lLoop;

	for (lLoop = 0; lLoop < MAX_NUM_WAVEID; lLoop++)
	{
		if (m_WaveIDs[lLoop])
		{
			if (m_WaveIDs[lLoop]->pData)
				delete m_WaveIDs[lLoop]->pData;

			if (m_WaveIDs[lLoop]->pFile)
				fclose(m_WaveIDs[lLoop]->pFile);

			delete m_WaveIDs[lLoop];
			m_WaveIDs[lLoop] = 0;
		}
	}
}


PSSG::PResult LoadWaveFile(const char *szFilename, WAVEFILEINFO& waveInfo)
{
	PSSG::PResult result = PSSG::PE_RESULT_OUT_OF_MEMORY;

	result = ParseFile(szFilename, &waveInfo);
	if(result == PSSG::PE_RESULT_NO_ERROR)
	{
		// Allocate memory for sample data
		unsigned long dataSize = GetWaveSize(waveInfo);
		waveInfo.pData = PSSG_ALLOCATE(char, dataSize);
		if(waveInfo.pData)
		{
			// Seek to start of audio data
			unsigned long dataOffset = GetWaveDataOffset(waveInfo);
			fseek(waveInfo.pFile, PSSG_INV_SWAP4(dataOffset), SEEK_SET);

			// Read Sample Data
			unsigned long dataSize = GetWaveSize(waveInfo);
			if (fread(waveInfo.pData, 1, dataOffset, waveInfo.pFile) != dataSize)
			{
				PSSG_FREE(waveInfo.pData);
				waveInfo.pData = NULL;
				result = PSSG::PE_RESULT_INSUFFICIENT_DATA;
			}
		}
		else
		{
			result = PSSG::PE_RESULT_OUT_OF_MEMORY;
		}

		fclose(waveInfo.pFile);
		waveInfo.pFile = 0;
	}

	return result;
}

WAVERESULT CWaves::ReadWaveData(WAVEID WaveID, void *pData, unsigned long ulDataSize, unsigned long *pulBytesWritten)
{
	LPWAVEFILEINFO pWaveInfo;
	WAVERESULT wr = WR_BADWAVEFILE;

	if (!pData || !pulBytesWritten || ulDataSize == 0)
		return WR_INVALIDPARAM;

	if (IsWaveID(WaveID))
	{
		pWaveInfo = m_WaveIDs[WaveID];
		if (pWaveInfo->pFile)
		{
			unsigned long ulOffset = ftell(pWaveInfo->pFile);

			if ((ulOffset - pWaveInfo->ulDataOffset + ulDataSize) > pWaveInfo->ulDataSize)
				ulDataSize = pWaveInfo->ulDataSize - (ulOffset - pWaveInfo->ulDataOffset);

			*pulBytesWritten = (unsigned long)fread(pData, 1, ulDataSize, pWaveInfo->pFile);

			wr = WR_OK;
		}
	}
	else
	{
		wr = WR_INVALIDWAVEID;
	}

	return wr;
}

WAVERESULT CWaves::SetWaveDataOffset(WAVEID WaveID, unsigned long ulOffset)
{
	LPWAVEFILEINFO pWaveInfo;
	WAVERESULT wr = WR_INVALIDPARAM;

	if (IsWaveID(WaveID))
	{
		pWaveInfo = m_WaveIDs[WaveID];
		if (pWaveInfo->pFile)
		{
			// Seek into audio data
			fseek(pWaveInfo->pFile, pWaveInfo->ulDataOffset + ulOffset, SEEK_SET);
			wr = WR_OK;
		}
	}
	else
	{
		wr = WR_INVALIDWAVEID;
	}

	return wr;
}

unsigned long GetWaveDataOffset(WAVEFILEINFO& waveInfo)
{
	return waveInfo.ulDataOffset;
}

PSSG::PResult ParseFile(const char *szFilename, LPWAVEFILEINFO pWaveInfo)
{
	WAVEFILEHEADER	waveFileHeader;
	RIFFCHUNK		riffChunk;
	WAVEFMT			waveFmt;
	//WAVERESULT		wr = WR_BADWAVEFILE;

	if (!szFilename || !pWaveInfo)
		return PSSG::PE_RESULT_NULL_POINTER_ARGUMENT;

	memset(pWaveInfo, 0, sizeof(WAVEFILEINFO));

	// Open the wave file for reading
	fopen_s(&pWaveInfo->pFile, szFilename, "rb");

	if (pWaveInfo->pFile)
	{
		// Read Wave file header
		fread(&waveFileHeader, 1, sizeof(WAVEFILEHEADER), pWaveInfo->pFile);
		if(!PSSG_STRNICMP(waveFileHeader.szRIFF, "MSF", 3))
		{
			pWaveInfo->wfType = WF_MSF;
			pWaveInfo->wfMSF.compressionType = waveFileHeader.msfCompressionType;
			pWaveInfo->wfMSF.channels = waveFileHeader.msfChannels;

			if(fread(&pWaveInfo->wfMSF.msData, 1, sizeof(MSFDATA), pWaveInfo->pFile) != sizeof(MSFDATA))
				return PSSG::PE_RESULT_CORRUPT_DATA_SOURCE;
			pWaveInfo->ulDataOffset = ftell(pWaveInfo->pFile);
		}
		else if (!PSSG_STRNICMP(waveFileHeader.szRIFF, "RIFF", 4) && !PSSG_STRNICMP(waveFileHeader.szWAVE, "WAVE", 4))
		{
			while (fread(&riffChunk, 1, sizeof(RIFFCHUNK), pWaveInfo->pFile) == sizeof(RIFFCHUNK))
			{
				if (!PSSG_STRNICMP(riffChunk.szChunkName, "fmt ", 4))
				{
					if (PSSG_INV_SWAP4(riffChunk.ulChunkSize) <= sizeof(WAVEFMT))
					{
						fread(&waveFmt, 1, PSSG_INV_SWAP4(riffChunk.ulChunkSize), pWaveInfo->pFile);
					
						// Determine if this is a WAVEFORMATEX or WAVEFORMATEXTENSIBLE wave file
						if (PSSG_INV_SWAP2(waveFmt.usFormatTag) == WAVE_FORMAT_PCM)
						{
							pWaveInfo->wfType = WF_EX;
							memcpy(&pWaveInfo->wfEXT.Format, &waveFmt, sizeof(PCMWAVEFORMAT));
						}
						else if (PSSG_INV_SWAP2(waveFmt.usFormatTag) == WAVE_FORMAT_EXTENSIBLE)
						{
							pWaveInfo->wfType = WF_EXT;
							memcpy(&pWaveInfo->wfEXT, &waveFmt, sizeof(WAVEFORMATEXTENSIBLE));
						}
					}
					else
					{
						fseek(pWaveInfo->pFile, PSSG_INV_SWAP4(riffChunk.ulChunkSize), SEEK_CUR);
					}
				}
				else if (!PSSG_STRNICMP(riffChunk.szChunkName, "data", 4))
				{
					pWaveInfo->ulDataSize = riffChunk.ulChunkSize;
					pWaveInfo->ulDataOffset = ftell(pWaveInfo->pFile);

					fseek(pWaveInfo->pFile, PSSG_INV_SWAP4(riffChunk.ulChunkSize), SEEK_CUR);
				}
				else
				{
					fseek(pWaveInfo->pFile, PSSG_INV_SWAP4(riffChunk.ulChunkSize), SEEK_CUR);
				}

				// Ensure that we are correctly aligned for next chunk
				if (riffChunk.ulChunkSize & 1)
				{
					fseek(pWaveInfo->pFile, 1, SEEK_CUR);
				}
			}

			if (pWaveInfo->ulDataSize && pWaveInfo->ulDataOffset && ((pWaveInfo->wfType == WF_EX) || (pWaveInfo->wfType == WF_EXT)))
				return PSSG::PE_RESULT_NO_ERROR;
			else
			{
				fclose(pWaveInfo->pFile);
				pWaveInfo->pFile = NULL;
			}
		}
	}
	else
	{
		return PSSG::PE_RESULT_FILE_DOES_NOT_EXIST;
	}

	return PSSG::PE_RESULT_CORRUPT_DATA_SOURCE;
}

PSSG::PResult ParseFileFromBuffer(const void *bufferData, unsigned int bufferSize, LPWAVEFILEINFO pWaveInfo)
{
	WAVEFILEHEADER	waveFileHeader;
	RIFFCHUNK		riffChunk;
	WAVEFMT			waveFmt;

	const char* buffer = (const char*)bufferData;
	if (!buffer || !pWaveInfo)
		return PSSG::PE_RESULT_NULL_POINTER_ARGUMENT;

	memset(pWaveInfo, 0, sizeof(WAVEFILEINFO));
	pWaveInfo->pFile = NULL;

	const char* curPtr = buffer;
	unsigned int remainingBufferSize = bufferSize;
	
	// Read Wave file header
	memcpy(&waveFileHeader,curPtr,sizeof(WAVEFILEHEADER));
	curPtr += sizeof(WAVEFILEHEADER);
	remainingBufferSize -= sizeof(WAVEFILEHEADER);
	if(!PSSG_STRNICMP(waveFileHeader.szRIFF, "MSF", 3))
	{
		pWaveInfo->wfType = WF_MSF;
		pWaveInfo->wfMSF.compressionType = waveFileHeader.msfCompressionType;
		pWaveInfo->wfMSF.channels = waveFileHeader.msfChannels;

		if(remainingBufferSize < sizeof(MSFDATA))
			return PSSG::PE_RESULT_CORRUPT_DATA_SOURCE;

		memcpy(&pWaveInfo->wfMSF.msData,curPtr,sizeof(MSFDATA));
		curPtr += sizeof(MSFDATA);
		remainingBufferSize -= sizeof(MSFDATA);

		pWaveInfo->ulDataOffset = (unsigned long)(curPtr - buffer);
		pWaveInfo->pData = const_cast<char*>(curPtr);
	}
	else if(!PSSG_STRNICMP(waveFileHeader.szRIFF, "RIFF", 4) && !PSSG_STRNICMP(waveFileHeader.szWAVE, "WAVE", 4))
	{
		while(remainingBufferSize >= sizeof(RIFFCHUNK))
		{
			memcpy(&riffChunk, curPtr, sizeof(RIFFCHUNK));
			curPtr += sizeof(RIFFCHUNK);
			remainingBufferSize -= sizeof(RIFFCHUNK);

			if(!PSSG_STRNICMP(riffChunk.szChunkName, "fmt ", 4))
			{
				if(PSSG_INV_SWAP4(riffChunk.ulChunkSize) <= sizeof(WAVEFMT))
				{
					memcpy(&waveFmt, curPtr, PSSG_INV_SWAP4(riffChunk.ulChunkSize));
					curPtr += PSSG_INV_SWAP4(riffChunk.ulChunkSize);
					remainingBufferSize -= PSSG_INV_SWAP4(riffChunk.ulChunkSize);
				
					if (PSSG_INV_SWAP2(waveFmt.usFormatTag) == WAVE_FORMAT_PCM)
					{
						pWaveInfo->wfType = WF_EX;
						memcpy(&pWaveInfo->wfEXT.Format, &waveFmt, sizeof(PCMWAVEFORMAT));
					}
					else if (PSSG_INV_SWAP2(waveFmt.usFormatTag) == WAVE_FORMAT_EXTENSIBLE)
					{
						pWaveInfo->wfType = WF_EXT;
						memcpy(&pWaveInfo->wfEXT, &waveFmt, sizeof(WAVEFORMATEXTENSIBLE));
					}
				}
				else
				{
					curPtr += PSSG_INV_SWAP4(riffChunk.ulChunkSize);
					remainingBufferSize -= PSSG_INV_SWAP4(riffChunk.ulChunkSize);
				}
			}
			else if (!PSSG_STRNICMP(riffChunk.szChunkName, "data", 4))
			{
				pWaveInfo->ulDataSize = riffChunk.ulChunkSize;
				pWaveInfo->ulDataOffset = (unsigned long)(curPtr - buffer);
				pWaveInfo->pData = const_cast<char*>(curPtr);

				curPtr += PSSG_INV_SWAP4(riffChunk.ulChunkSize);
				remainingBufferSize -= PSSG_INV_SWAP4(riffChunk.ulChunkSize);
			}
			else
			{
				curPtr += PSSG_INV_SWAP4(riffChunk.ulChunkSize);
				remainingBufferSize -= PSSG_INV_SWAP4(riffChunk.ulChunkSize);
			}

			// Ensure that we are correctly aligned for next chunk
			if (riffChunk.ulChunkSize & 1)
			{
				//fseek(pWaveInfo->pFile, 1, SEEK_CUR);
				curPtr += 1;
				remainingBufferSize -= 1;
			}
		}

		if (pWaveInfo->ulDataSize && pWaveInfo->ulDataOffset && ((pWaveInfo->wfType == WF_EX) || (pWaveInfo->wfType == WF_EXT)))
			return PSSG::PE_RESULT_NO_ERROR;
	}

	return PSSG::PE_RESULT_CORRUPT_DATA_SOURCE;
}


WAVERESULT CWaves::DeleteWaveFile(WAVEID WaveID)
{
	WAVERESULT wr = WR_OK;

	if (IsWaveID(WaveID))
	{
		if (m_WaveIDs[WaveID]->pData)
			delete m_WaveIDs[WaveID]->pData;

		if (m_WaveIDs[WaveID]->pFile)
			fclose(m_WaveIDs[WaveID]->pFile);

		delete m_WaveIDs[WaveID];
		m_WaveIDs[WaveID] = 0;
	}
	else
	{
		wr = WR_INVALIDWAVEID;
	}

	return wr;
}


WAVERESULT CWaves::GetWaveType(WAVEID WaveID, WAVEFILETYPE *wfType)
{
	if (!IsWaveID(WaveID))
		return WR_INVALIDWAVEID;

	if (!wfType)
		return WR_INVALIDPARAM;

	*wfType = m_WaveIDs[WaveID]->wfType;

	return WR_OK;
}


WAVERESULT CWaves::GetWaveFormatExHeader(WAVEID WaveID, WAVEFORMATEX *wfex)
{
	if (!IsWaveID(WaveID))
		return WR_INVALIDWAVEID;

	if (!wfex)
		return WR_INVALIDPARAM;

	memcpy(wfex, &(m_WaveIDs[WaveID]->wfEXT.Format), sizeof(WAVEFORMATEX));

	return WR_OK;
}


WAVERESULT CWaves::GetWaveFormatExtensibleHeader(WAVEID WaveID, WAVEFORMATEXTENSIBLE *wfext)
{
	if (!IsWaveID(WaveID))
		return WR_INVALIDWAVEID;

	if (m_WaveIDs[WaveID]->wfType != WF_EXT)
		return WR_NOTWAVEFORMATEXTENSIBLEFORMAT;

	if (!wfext)
		return WR_INVALIDPARAM;

	memcpy(wfext, &(m_WaveIDs[WaveID]->wfEXT), sizeof(WAVEFORMATEXTENSIBLE));

	return WR_OK;
}

void* GetWaveData(WAVEFILEINFO& waveInfo)
{
	return waveInfo.pData;
}

unsigned long GetWaveSize(WAVEFILEINFO& waveInfo)
{
	if(waveInfo.wfType == WF_MSF)
		return PSSG_SWAP4(waveInfo.wfMSF.msData.sampleSize);
	else
		return PSSG_INV_SWAP4(waveInfo.ulDataSize);
}


unsigned long GetWaveFrequency(WAVEFILEINFO& waveInfo)
{
	if(waveInfo.wfType == WF_MSF)
		return PSSG_SWAP4(waveInfo.wfMSF.msData.sampleRate);
	else
		return PSSG_INV_SWAP4(waveInfo.wfEXT.Format.nSamplesPerSec);
}


PSSG::PResult GetWaveALBufferFormat(WAVEFILEINFO& waveInfo, PFNALGETENUMVALUE pfnGetEnumValue, unsigned long *pulFormat)
{
	PSSG::PResult result = PSSG::PE_RESULT_NO_ERROR;

	if(pfnGetEnumValue && pulFormat)
	{
		*pulFormat = 0;

		if(waveInfo.wfType == WF_MSF)
		{
			switch(PSSG_SWAP2(waveInfo.wfMSF.channels))
			{
			case 1:
				*pulFormat = pfnGetEnumValue("AL_FORMAT_MONO16");
				break;
			case 2:
				*pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO16");
				break;
			case 6:
				*pulFormat = pfnGetEnumValue("AL_FORMAT_51CHN16");
				break;
			case 8:
				*pulFormat = pfnGetEnumValue("AL_FORMAT_71CHN16");
				break;
			}
		}
		else if (waveInfo.wfType == WF_EX)
		{
			if (PSSG_INV_SWAP2(waveInfo.wfEXT.Format.nChannels) == 1)
			{
				switch (PSSG_INV_SWAP2(waveInfo.wfEXT.Format.wBitsPerSample))
				{
				case 4:
					*pulFormat = pfnGetEnumValue("AL_FORMAT_MONO_IMA4");
					break;
				case 8:
					*pulFormat = pfnGetEnumValue("AL_FORMAT_MONO8");
					break;
				case 16:
					*pulFormat = pfnGetEnumValue("AL_FORMAT_MONO16");
					break;
				}
			}
			else if (PSSG_INV_SWAP2(waveInfo.wfEXT.Format.nChannels) == 2)
			{
				switch (PSSG_INV_SWAP2(waveInfo.wfEXT.Format.wBitsPerSample))
				{
				case 4:
					*pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO_IMA4");
					break;
				case 8:
					*pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO8");
					break;
				case 16:
					*pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO16");
					break;
				}
			}
			else if ((PSSG_INV_SWAP2(waveInfo.wfEXT.Format.nChannels) == 4) && (PSSG_INV_SWAP2(waveInfo.wfEXT.Format.wBitsPerSample) == 16))
				*pulFormat = pfnGetEnumValue("AL_FORMAT_QUAD16");
		}
		else if (waveInfo.wfType == WF_EXT)
		{
			if ((PSSG_INV_SWAP2(waveInfo.wfEXT.Format.nChannels) == 1) &&
				((PSSG_INV_SWAP4(waveInfo.wfEXT.dwChannelMask) == SPEAKER_FRONT_CENTER) ||
				 (PSSG_INV_SWAP4(waveInfo.wfEXT.dwChannelMask) == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT)) ||
				 (PSSG_INV_SWAP4(waveInfo.wfEXT.dwChannelMask) == 0)))
			{
				switch (PSSG_INV_SWAP2(waveInfo.wfEXT.Format.wBitsPerSample))
				{
				case 4:
					*pulFormat = pfnGetEnumValue("AL_FORMAT_MONO_IMA4");
					break;
				case 8:
					*pulFormat = pfnGetEnumValue("AL_FORMAT_MONO8");
					break;
				case 16:
					*pulFormat = pfnGetEnumValue("AL_FORMAT_MONO16");
					break;
				}
			}
			else if ((PSSG_INV_SWAP2(waveInfo.wfEXT.Format.nChannels) == 2) && (PSSG_INV_SWAP4(waveInfo.wfEXT.dwChannelMask) == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT)))
			{
				switch (waveInfo.wfEXT.Format.wBitsPerSample)
				{
				case 4:
					*pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO_IMA4");
					break;
				case 8:
					*pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO8");
					break;
				case 16:
					*pulFormat = pfnGetEnumValue("AL_FORMAT_STEREO16");
					break;
				}
			}
			else if ((PSSG_INV_SWAP2(waveInfo.wfEXT.Format.nChannels) == 2) && (PSSG_INV_SWAP2(waveInfo.wfEXT.Format.wBitsPerSample) == 16) && (PSSG_INV_SWAP4(waveInfo.wfEXT.dwChannelMask) == (SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
				*pulFormat =  pfnGetEnumValue("AL_FORMAT_REAR16");
			else if ((PSSG_INV_SWAP2(waveInfo.wfEXT.Format.nChannels) == 4) && (PSSG_INV_SWAP2(waveInfo.wfEXT.Format.wBitsPerSample) == 16) && (PSSG_INV_SWAP4(waveInfo.wfEXT.dwChannelMask) == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
				*pulFormat = pfnGetEnumValue("AL_FORMAT_QUAD16");
			else if ((PSSG_INV_SWAP2(waveInfo.wfEXT.Format.nChannels) == 6) && (PSSG_INV_SWAP2(waveInfo.wfEXT.Format.wBitsPerSample) == 16) && (PSSG_INV_SWAP4(waveInfo.wfEXT.dwChannelMask) == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_FRONT_CENTER|SPEAKER_LOW_FREQUENCY|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT)))
				*pulFormat = pfnGetEnumValue("AL_FORMAT_51CHN16");
			else if ((PSSG_INV_SWAP2(waveInfo.wfEXT.Format.nChannels) == 7) && (PSSG_INV_SWAP2(waveInfo.wfEXT.Format.wBitsPerSample) == 16) && (PSSG_INV_SWAP4(waveInfo.wfEXT.dwChannelMask) == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_FRONT_CENTER|SPEAKER_LOW_FREQUENCY|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT|SPEAKER_BACK_CENTER)))
				*pulFormat = pfnGetEnumValue("AL_FORMAT_61CHN16");
			else if ((PSSG_INV_SWAP2(waveInfo.wfEXT.Format.nChannels) == 8) && (PSSG_INV_SWAP2(waveInfo.wfEXT.Format.wBitsPerSample) == 16) && (PSSG_INV_SWAP4(waveInfo.wfEXT.dwChannelMask) == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_FRONT_CENTER|SPEAKER_LOW_FREQUENCY|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT|SPEAKER_SIDE_LEFT|SPEAKER_SIDE_RIGHT)))
				*pulFormat = pfnGetEnumValue("AL_FORMAT_71CHN16");
		}

		if (*pulFormat == 0)
			result = PSSG::PE_RESULT_CORRUPT_DATA_SOURCE;
	}
	else
	{
		result = PSSG::PE_RESULT_NULL_POINTER_ARGUMENT;
	}

	return result;
}


bool CWaves::IsWaveID(WAVEID WaveID)
{
	bool bReturn = false;

	if ((WaveID >= 0) && (WaveID < MAX_NUM_WAVEID))
	{
		if (m_WaveIDs[WaveID])
			bReturn = true;
	}

	return bReturn;
}


char *CWaves::GetErrorString(WAVERESULT wr, char *szErrorString, unsigned long nSizeOfErrorString)
{	
	switch (wr)
	{
		case WR_OK:
			strncpy_s(szErrorString, nSizeOfErrorString, "Success\n", nSizeOfErrorString-1);
			break;

		case WR_INVALIDFILENAME:
			strncpy_s(szErrorString, nSizeOfErrorString, "Invalid file name or file does not exist\n", nSizeOfErrorString-1);
			break;

		case WR_BADWAVEFILE:
			strncpy_s(szErrorString, nSizeOfErrorString, "Invalid Wave file\n", nSizeOfErrorString-1);
			break;

		case WR_INVALIDPARAM:
			strncpy_s(szErrorString, nSizeOfErrorString, "Invalid parameter passed to function\n", nSizeOfErrorString-1);
			break;

		case WR_FILEERROR:
			strncpy_s(szErrorString, nSizeOfErrorString, "File I/O error\n", nSizeOfErrorString-1);
			break;

		case WR_INVALIDWAVEID:
			strncpy_s(szErrorString, nSizeOfErrorString, "Invalid WAVEID\n", nSizeOfErrorString-1);
			break;

		case WR_NOTSUPPORTEDYET:
			strncpy_s(szErrorString, nSizeOfErrorString, "Function not supported yet\n", nSizeOfErrorString-1);
			break;

		case WR_WAVEMUSTBEMONO:
			strncpy_s(szErrorString, nSizeOfErrorString, "Input wave files must be mono\n", nSizeOfErrorString-1);
			break;

		case WR_WAVEMUSTBEWAVEFORMATPCM:
			strncpy_s(szErrorString, nSizeOfErrorString, "Input wave files must be in Wave Format PCM\n", nSizeOfErrorString-1);
			break;
		
		case WR_WAVESMUSTHAVESAMEBITRESOLUTION:
			strncpy_s(szErrorString, nSizeOfErrorString, "Input wave files must have the same Bit Resolution\n", nSizeOfErrorString-1);
			break;

		case WR_WAVESMUSTHAVESAMEFREQUENCY:
			strncpy_s(szErrorString, nSizeOfErrorString, "Input wave files must have the same Frequency\n", nSizeOfErrorString-1);
			break;

		case WR_WAVESMUSTHAVESAMEBITRATE:
			strncpy_s(szErrorString, nSizeOfErrorString, "Input wave files must have the same Bit Rate\n", nSizeOfErrorString-1);
			break;

		case WR_WAVESMUSTHAVESAMEBLOCKALIGNMENT:
			strncpy_s(szErrorString, nSizeOfErrorString, "Input wave files must have the same Block Alignment\n", nSizeOfErrorString-1);
			break;

		case WR_OFFSETOUTOFDATARANGE:
			strncpy_s(szErrorString, nSizeOfErrorString, "Wave files Offset is not within audio data\n", nSizeOfErrorString-1);
			break;

		case WR_INVALIDSPEAKERPOS:
			strncpy_s(szErrorString, nSizeOfErrorString, "Invalid Speaker Destinations\n", nSizeOfErrorString-1);
			break;

		case WR_OUTOFMEMORY:
			strncpy_s(szErrorString, nSizeOfErrorString, "Out of memory\n", nSizeOfErrorString-1);
			break;

		case WR_INVALIDWAVEFILETYPE:
			strncpy_s(szErrorString, nSizeOfErrorString, "Invalid Wave File Type\n", nSizeOfErrorString-1);
			break;

		case WR_NOTWAVEFORMATEXTENSIBLEFORMAT:
			strncpy_s(szErrorString, nSizeOfErrorString, "Wave file is not in WAVEFORMATEXTENSIBLE format\n", nSizeOfErrorString-1);
			break;

		default:
			strncpy_s(szErrorString, nSizeOfErrorString, "Undefined error\n", nSizeOfErrorString-1);
	}
	szErrorString[nSizeOfErrorString-1] = '\0';
	return szErrorString;
}