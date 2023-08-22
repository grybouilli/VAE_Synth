#include "numpyToCppUtils.hpp"

void copy(const np::ndarray& src, std::vector<float>& dest)
{
    auto src_size = src.shape(0);

    for (auto idx = 0; idx < src_size; ++idx)
    {
        dest.push_back(python::extract<_Float32>(src[idx])); // 
    }
}