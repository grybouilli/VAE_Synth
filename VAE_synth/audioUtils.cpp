#include "audioUtils.hpp"

void copy_sample_into(std::vector<float>& input, float*& output, int frames, int current_frame, int max_sample)
{
    for(auto s = 0; s < frames; ++s)
    {
        output[s] = input[(current_frame+s)%max_sample];
    }
}
