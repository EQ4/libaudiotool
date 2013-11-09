#pragma once

#include "FFTProxy.h"
#include "../InputManagerBase.h"
#include "../copystyle/InputOLA.h"
#include "window/HannWindow.h"
#include "window/BartlettWindow.h"
#include "window/RectWindow.h"
// Templates, never again
template <typename data_type>
class FFTInputProxy : public FFTProxy<data_type>, public InputManagerBase<data_type>
{
		using InputManagerBase<data_type>::channels;
		using InputManagerBase<data_type>::frames;

		using FFTProxy<data_type>::fft;

	private:
		Input_p<data_type> inputImpl = nullptr;
		Window_p<data_type> window = nullptr;

	public:
		FFTInputProxy(InputManagerBase<data_type>* input,
					  FFT_p<data_type> fftmanager,
					  const Parameters<data_type>& cfg):
			FFTProxy<data_type>(fftmanager, cfg),
			InputManagerBase<data_type>(nullptr, cfg),
			inputImpl(input),
			window(new BartlettWindow<data_type>(cfg))
		{
		}

		virtual ~FFTInputProxy() = default;

		virtual Audio_p getNextBuffer() final override
		{
			// 1. On get le buffer.
			Audio_p tmp = inputImpl->getNextBuffer();
			if(tmp == nullptr) return tmp;
			auto& inbuff = static_cast<CData<data_type>*>(tmp.get())->_data;

			// 2. On fenêtre
			for(auto& channel : inbuff)
				window->apply(channel, inputImpl->copyHandler->frameIncrement());

			// 3. On copie dans le buffer de la fft
			std::copy(inbuff.begin(), inbuff.end(), fft->input().begin());

			// 3. fft
			fft->forward();

			// 4. On crée un autre buffer pour le spectre
			auto outbuff = new CData<typename FFTProxy<data_type>::complex_type>;

			// 5. On move fft.spectrum
			outbuff->_data = std::move(fft->spectrum());

			// 6. On retourne
			return Audio_p(outbuff);
		}
};
