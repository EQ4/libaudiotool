#ifndef SSWENCODE_H
#define SSWENCODE_H

#include <cstdlib>
#include <vector>

#include "SpectralWatermarkBase.h"

// Algorithme d'encodage en SSW
template <typename data_type>
class SSWEncode : public SpectralWatermarkBase<data_type>
{
    public:
        SSWEncode(const Parameters<data_type>& configuration, 
		  std::vector<int> & PNSequence,
		  std::vector<int> & freqWinIndexes,
		  double watermarkAmp):
		SpectralWatermarkBase<data_type>(configuration),
		_PNSequence(PNSequence), _freqWinIndexes(freqWinIndexes),
		_watermarkAmp(watermarkAmp)
	{
        }

        // La seule méthode importante est celle-ci.
        // data : les données audio. Ici ce sera un spectre.
        virtual void operator()(Audio_p& data, WatermarkData& watermark)  override
        {

            // Recopier cette ligne
            auto& spectrum = static_cast<CData<typename SpectralWatermarkBase<data_type>::complex_type>*>(data.get())->_data;

	    for (int i = 0; i < _PNSequence.size(); i++)
	    {
		    spectrum[freqWinIndexes[i]] += _watermarkAmp * (double) _PNSequence[i];
	    }    

        }

        virtual void onFFTSizeUpdate() override
        {
		
        }

        virtual void onDataUpdate() override
        {
		
        }

    private :
	double _watermarkAmp;
	std::vector<int> _freqWinIndexes;
	std::vector<int> _PNSequence;

};

#endif // SSWENCODE_H
