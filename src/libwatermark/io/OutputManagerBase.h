#pragma once
#include "IOManagerBase.h"
#include "copystyle/OutputSimple.h"
// Note : les output DOIVENT faire un delete du buffer qu'elles prennent en entrée.
// Ou std::move ?
class OutputManagerBase :  public IOManagerBase
{
	public:
		OutputManagerBase(const Parameters& cfg):
			IOManagerBase(cfg),
			_copy(new OutputSimple(cfg))
		{

		}

		OutputManagerBase(Output* copy, const Parameters& cfg):
			IOManagerBase(cfg),
			_copy(copy)
		{

		}

		virtual ~OutputManagerBase() = default;

		// Copie de buffer vers notre buffer interne
		virtual void writeNextBuffer(IData* buffer)
		{
			if(_pos < _baseData.size())
			{
				CData<data_type>* b = dynamic_cast<CData<data_type>*>(buffer);
				_copy->copy(b->_data.begin(), _baseData.begin(), _pos, conf.bufferSize, _baseData.size());
			}

			_pos += _copy->frameIncrement();
		}

	private:
		std::shared_ptr<Output> _copy = nullptr;
};
