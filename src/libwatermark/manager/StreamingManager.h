#pragma once
#include <memory>
#include "ManagerBase.h"
#include "../Data.h"
#include "../stream_io/StreamingOutputInterface.h"

/**
 * @brief Main class.
 *
 */
template<typename data_type>
class StreamingManager: public ManagerBase
{
	public:
		StreamingManager(Input_p i,
						 std::shared_ptr<StreamingOutputInterface<data_type>> o,
						 Parameters<data_type> cfg):
			ManagerBase(i, o),
			conf{cfg}
		{
		}

		int generate(void* outputBuffer )
		{
			float **out = static_cast<float **>(outputBuffer);

			Audio_p buffer = m_input->getNextBuffer();
			auto audio = getAudio<data_type>(buffer); // Should be stereo.

			std::copy(audio[0].begin(),
					  audio[0].end(),
					  out[0]);
			std::copy(audio[1].begin(),
					  audio[1].end(),
					  out[1]);

			return 0;
		}

		virtual void execute() override
		{
			static_cast<StreamingOutputInterface<data_type>*>(output().get())->startStream(std::bind(&StreamingManager::generate, this, std::placeholders::_1));
		}

	private:
		Parameters<data_type>& conf;
};
