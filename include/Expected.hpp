#pragma once

#include "expected/expected.hpp"


template <class T, class E>
using expected = tl::expected<T, E>;

template <typename E>
using unexpected = tl::unexpected<E>;

using tl::make_unexpected;
