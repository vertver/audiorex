#include "hardware.h"
#include <locale>
#include <codecvt>
#include <thread>

namespace arex {
	static ma_pcm_rb g_rb;

	static void hardware_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
	{
		auto pHardwarePtr = (audio_hardware*)pDevice->pUserData;
		ma_uint32 pcmFramesAvailableInRB;
		ma_uint32 pcmFramesProcessed = 0;
		ma_uint8* pRunningOutput = (ma_uint8*)pOutput;

		while (pcmFramesProcessed < frameCount) {
			ma_uint32 framesRemaining = frameCount - pcmFramesProcessed;

			pcmFramesAvailableInRB = ma_pcm_rb_available_read(&g_rb);
			if (pcmFramesAvailableInRB > 0) {
				ma_uint32 framesToRead = (framesRemaining < pcmFramesAvailableInRB) ? framesRemaining : pcmFramesAvailableInRB;
				void* pReadBuffer;

				ma_pcm_rb_acquire_read(&g_rb, &framesToRead, &pReadBuffer);
				memcpy(pRunningOutput, pReadBuffer, framesToRead * ma_get_bytes_per_frame(pDevice->playback.format, pDevice->playback.channels));
				ma_pcm_rb_commit_read(&g_rb, framesToRead, pReadBuffer);

				pRunningOutput += framesToRead * ma_get_bytes_per_frame(pDevice->playback.format, pDevice->playback.channels);
				pcmFramesProcessed += framesToRead;
			}
			else {
				ma_uint32 framesToWrite = PCM_FRAME_CHUNK_SIZE;
				void* pWriteBuffer;

				ma_pcm_rb_reset(&g_rb);
				ma_pcm_rb_acquire_write(&g_rb, &framesToWrite, &pWriteBuffer);
				pHardwarePtr->callback(pDevice, pWriteBuffer, NULL, framesToWrite);
				ma_pcm_rb_commit_write(&g_rb, framesToWrite, pWriteBuffer);
			}
		}

	}

	void audio_hardware::callback(
		ma_device* pDevice,
		void* pOutput, 
		const void* pInput, 
		ma_uint32 frameCount
	)
	{
		ringbuffer_callback->process(pOutput, const_cast<void*>(pInput), frameCount);
	}

	std::string
	convert_uuid_to_string(
		ma_device_id& device_id
	) 
	{
		// #TODO:
		std::string ret("no uuid");
		return ret;
	}

	bool
	audio_hardware::initialize_device()
	{
		ma_pcm_rb_init(DEVICE_FORMAT, DEVICE_CHANNELS, PCM_FRAME_CHUNK_SIZE, NULL, NULL, &g_rb);
		deviceConfig = ma_device_config_init(ma_device_type_playback);
		deviceConfig.playback.format = DEVICE_FORMAT;
		deviceConfig.playback.channels = DEVICE_CHANNELS;
		deviceConfig.sampleRate = DEVICE_SAMPLE_RATE;
		deviceConfig.dataCallback = hardware_callback;
		deviceConfig.pUserData = NULL;

		if (ma_device_init(&context, &deviceConfig, &device) != MA_SUCCESS) {
			ma_pcm_rb_uninit(&g_rb);
			return false;
		}

		if (ma_device_start(&device) != MA_SUCCESS) {
			ma_pcm_rb_uninit(&g_rb);
			ma_device_uninit(&device);
			return false;
		}

		return true;
	}

	bool
	audio_hardware::initialize_device(
		int device_id
	)
	{
		return false;
	}

	bool
	audio_hardware::initialize_device(
		std::string device_uuid
	)
	{
		return false;
	}

	bool
	audio_hardware::destroy_device()
	{	
		ma_device_uninit(&device);
		ma_pcm_rb_uninit(&g_rb);
		return true;
	}

	std::list<audio_device_description>& 
	audio_hardware::device_list()
	{
		ma_result result;
		ma_device_info* pPlaybackDeviceInfos;
		ma_uint32 playbackDeviceCount;
		ma_device_info* pCaptureDeviceInfos;
		ma_uint32 captureDeviceCount;
		ma_uint32 iDevice;

		result = ma_context_get_devices(&context, &pPlaybackDeviceInfos, &playbackDeviceCount, &pCaptureDeviceInfos, &captureDeviceCount);
		if (result == MA_SUCCESS) {
			hardware_device_list.clear();
			for (size_t i = 0; i < playbackDeviceCount; i++) {
				auto format = audio_device_format(PCM_FRAME_CHUNK_SIZE, DEVICE_SAMPLE_RATE, 32, DEVICE_CHANNELS);
				auto device_info = audio_device_description(i, convert_uuid_to_string(pPlaybackDeviceInfos[i].id), std::string(pPlaybackDeviceInfos[i].name), format);
				hardware_device_list.push_back(device_info);
			}
		}

		return hardware_device_list;
	}
};
