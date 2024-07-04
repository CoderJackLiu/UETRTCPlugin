#include "TRTCMediaSource.h"

FTRTCMediaSource::FTRTCMediaSource(liteav::ue::TRTCCloud* InTRTCInstance)
	: TRTCInstance(InTRTCInstance)
{
}

FTimespan FTRTCMediaSource::GetDuration() const
{
	//todo liuqi 是否支持播放本地视频？
	// if (Media == nullptr)
	// {
	// 	return FTimespan::Zero();
	// }
	//
	// int64 Duration = FVlc::MediaGetDuration(Media);
	//
	// if (Duration < 0)
	// {
	// 	return FTimespan::Zero();
	// }

	return FTimespan(FTimespan(100.f * ETimespan::TicksPerMillisecond));
}

void FTRTCMediaSource::Close()
{
	Data.Reset();
	CurrentUrl.Reset();
}

int FTRTCMediaSource::HandleMediaOpen(void* Opaque, void** OutData, uint64* OutSize)
{
	auto Reader = static_cast<FTRTCMediaSource*>(Opaque);

	if ((Reader == nullptr) || !Reader->Data.IsValid())
	{
		return 0;
	}

	*OutSize = Reader->Data->TotalSize();

	return 0;
}

SSIZE_T FTRTCMediaSource::HandleMediaRead(void* Opaque, void* Buffer, SIZE_T Length)
{
	auto Reader = static_cast<FTRTCMediaSource*>(Opaque);

	if (Reader == nullptr)
	{
		return -1;
	}

	TSharedPtr<FArchive, ESPMode::ThreadSafe> Data = Reader->Data;

	if (!Reader->Data.IsValid())
	{
		return -1;
	}

	SIZE_T DataSize = (SIZE_T)Data->TotalSize();
	SIZE_T BytesToRead = FMath::Min(Length, DataSize);
	SIZE_T DataPosition = Reader->Data->Tell();

	if ((DataSize - BytesToRead) < DataPosition)
	{
		BytesToRead = DataSize - DataPosition;
	}

	if (BytesToRead > 0)
	{
		Data->Serialize(Buffer, BytesToRead);
	}

	return (SSIZE_T)BytesToRead;
}

int FTRTCMediaSource::HandleMediaSeek(void* Opaque, uint64 Offset)
{
	auto Reader = static_cast<FTRTCMediaSource*>(Opaque);

	if (Reader == nullptr)
	{
		return -1;
	}

	TSharedPtr<FArchive, ESPMode::ThreadSafe> Data = Reader->Data;

	if (!Reader->Data.IsValid())
	{
		return -1;
	}

	if ((uint64)Data->TotalSize() <= Offset)
	{
		return -1;
	}

	Reader->Data->Seek(Offset);

	return 0;
}

void FTRTCMediaSource::HandleMediaClose(void* Opaque)
{
	auto Reader = static_cast<FTRTCMediaSource*>(Opaque);

	if (Reader != nullptr)
	{
		Reader->Data->Seek(0);
	}
}
