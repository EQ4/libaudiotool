#pragma once


#include "InputManagerBase.h"

template <typename data_type>
class LoopInputProxy : public InputManagerBase<data_type>
{
		using InputManagerBase<data_type>::channels;
		using InputManagerBase<data_type>::frames;

	private:
		Input_p inputImpl = nullptr;

	public:
		LoopInputProxy(Input_p input,
					   Parameters<data_type>& cfg):
			InputManagerBase<data_type>(nullptr, cfg),
			inputImpl(input)
		{
		}

		virtual ~LoopInputProxy() = default;

		virtual Audio_p getNextBuffer() final override
		{
			static int loop_count = 10;
			auto impl = static_cast<InputManagerBase<data_type>*>(inputImpl.get());
			Audio_p tmp = inputImpl->getNextBuffer();

			// General case
			if(impl->pos() < impl->frames()) return tmp;

			if(loop_count-- > 0)
			{
				// If we have to loop
				impl->reset();

				for(auto i = 0U; i < impl->channels(); ++i)
				{
					std::cerr << impl->copyHandler->lastIncrement() << std::endl;
					std::copy(impl->v(i).begin(),
							  impl->v(i).begin() + impl->copyHandler->frameIncrement() - impl->copyHandler->lastIncrement(),
							  getAudio<data_type>(tmp)[i].begin() + impl->copyHandler->lastIncrement());

					impl->pos() += impl->copyHandler->frameIncrement() - impl->copyHandler->lastIncrement();
				}
			}

			return tmp;
		}
};
