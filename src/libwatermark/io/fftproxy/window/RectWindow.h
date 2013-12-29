#pragma once

#include "WindowBase.h"

// Ne fait rien, fenêtre rectangulaire
template<typename data_type>
class RectWindow: public WindowBase<data_type>
{
		using size_type = typename Parameters<data_type>::size_type;
	public:
		RectWindow():
			WindowBase<data_type>({1, 2})
		{
		}

		virtual void apply(std::vector<data_type>&, const size_type) final override
		{
		}
};
