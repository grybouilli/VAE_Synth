#pragma once

#include <vector>
#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

namespace python = boost::python;
namespace np = boost::python::numpy;

void copy(const np::ndarray& src, std::vector<float>& dest);