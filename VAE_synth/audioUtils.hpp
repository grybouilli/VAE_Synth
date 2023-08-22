#pragma once

#include <cstddef>  // define size_t
#include <vector>

void copy_sample_into(std::vector<float>& input, float*& output, int frames, int current_frame, int max_sample);

