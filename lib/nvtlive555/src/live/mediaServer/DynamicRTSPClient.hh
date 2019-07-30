/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**********/
// Copyright (c) 1996-2013, Live Networks, Inc.  All rights reserved
// A subclass of "RTSPServer" that creates "ServerMediaSession"s on demand,
// based on whether or not the specified stream name exists as a file
// Header file

#ifndef _DYNAMIC_RTSP_CLIENT_HH
#define _DYNAMIC_RTSP_CLIENT_HH

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "NvtMediaCliData.hh"

class StreamClientState {
public:
  StreamClientState();
  virtual ~StreamClientState();

public:
  MediaSubsessionIterator* iter;
  MediaSession* session;
  MediaSubsession* subsession;
  TaskToken streamTimerTask;
  double duration;
};

class DynamicRTSPClient: public RTSPClient {
public:
  static DynamicRTSPClient* createNew(UsageEnvironment& env, NvtMediaCliData* pNvtData,
				  int verbosityLevel = 0,
				  char const* applicationName = NULL,
				  portNumBits tunnelOverHTTPPortNum = 0);

protected:
  DynamicRTSPClient(UsageEnvironment& env, NvtMediaCliData* pNvtData, char const* rtspURL,
		int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum);
    // called only by createNew();
  virtual ~DynamicRTSPClient();

private:
  NvtMediaCliData* m_pNvtData;

public:
  NvtMediaCliData* Get_NvtData() {return m_pNvtData;}
  unsigned SendDescribeCommand();
  StreamClientState scs;
};


class DummySink: public MediaSink {
public:
    static DummySink* createNew(UsageEnvironment& env,
        NvtMediaCliData* m_pNvtData,
        MediaSubsession& subsession, // identifies the kind of data that's being received
        char const* streamId = NULL); // identifies the stream itself (optional)

private:
    DummySink(UsageEnvironment& env, NvtMediaCliData* m_pNvtData, MediaSubsession& subsession, char const* streamId);
    // called only by "createNew()"
    virtual ~DummySink();

    static void afterGettingFrame(void* clientData, unsigned frameSize,
        unsigned numTruncatedBytes,
    struct timeval presentationTime,
        unsigned durationInMicroseconds);
    void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
    struct timeval presentationTime, unsigned durationInMicroseconds);

private:
    // redefined virtual functions:
    virtual Boolean continuePlaying();

private:
    u_int8_t* fReceiveBuffer;
    u_int32_t fReceiveBufferSize;
    MediaSubsession& fSubsession;
    char* fStreamId;
    NVT_MEDIA_TYPE m_MediaType;
    NvtMediaCliData* m_pNvtData;
};

// Used to shut down and close a stream (including its "RTSPClient" object):
void shutdownStream(RTSPClient* rtspClient, int exitCode = 1);

#endif
