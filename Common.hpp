#pragma once

template <typename ...Fs>
struct overloaded: Fs... {
	using Fs::operator()...;
};

