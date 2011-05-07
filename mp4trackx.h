#ifndef _MP4TRACKX
#define _MP4TRACKX

#include "mp4v2wrapper.h"

struct SampleTime {
    uint64_t dts, cts;
    uint32_t ctsOffset;
};

struct FPSRange {
    uint32_t numFrames;
    int fps_num, fps_denom;
};

class MP4TrackX: public mp4v2::impl::MP4Track {
    struct CTSComparator {
	MP4TrackX *owner_;
	CTSComparator(MP4TrackX *owner): owner_(owner) {}
	bool operator()(uint32_t a, uint32_t b)
	{
	    return owner_->CompareByCTS(a, b);
	}
    };

    std::vector<SampleTime> m_sampleTimes;
    std::vector<uint32_t> m_ctsIndex;
    uint32_t m_timeScale;
    uint64_t m_mediaDuration;
    int64_t m_initialDelay;
    bool m_compressDTS;
public:
    MP4TrackX(mp4v2::impl::MP4File &pFile, mp4v2::impl::MP4Atom &pTrackAtom);
    void EnableDTSCompression(bool enable)
    {
	m_compressDTS = enable;
    }
    void SetFPS(FPSRange *fpsRanges, size_t numRanges);
    void SetTimeCodes(double *timeCodes, size_t count, uint32_t timeScale);
    void AdjustTimeCodes();
    void DoEditTimeCodes();
    uint32_t GetTimeScale() const { return m_timeScale; }
    const std::vector<SampleTime> &GetTimeCodes() const
    {
	return m_sampleTimes;
    }
    const std::vector<uint32_t> &GetCTSIndex() const
    {
	return m_ctsIndex;
    }
private:
    bool CompareByCTS(uint32_t a, uint32_t b)
    {
	return m_sampleTimes[a].cts < m_sampleTimes[b].cts;
    }
    void FetchStts();
    void FetchCtts();
    uint32_t CalcTimeScale(FPSRange *begin, const FPSRange *end);
    uint64_t CalcSampleTimes(
	    const FPSRange *begin, const FPSRange *end, uint32_t timeScale);
    void CompressDTS();
    void OffsetCTS(int64_t off);
    int64_t CalcInitialDelay();
    void UpdateStts();
    void UpdateCtts();
    void UpdateElst(int64_t duration, int64_t mediaTime);
};

#endif
