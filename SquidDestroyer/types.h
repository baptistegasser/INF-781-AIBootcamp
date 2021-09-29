#pragma once

#include "Globals.h"

#include <vector>
#include <functional>

// Axial coordinate representation for hexgonale cells
struct Pos {
	int q, r;

	bool operator==(const Pos& other) const {
		return q == other.q && r == other.r;
	}

	bool operator!=(const Pos& other) const {
		return !(*this == other);
	}

	Pos operator+(const Pos& rhs) const
	{
		return { q + rhs.q, r + rhs.r};
	}

	Pos operator-(const Pos& rhs) const
	{
		return { q - rhs.q, r - rhs.r };
	}
};

using PosList = std::vector<Pos>;
using ConstPosRef = const Pos&;

// An Hexagonale cell representation
struct HexCell {
	Pos pos;
	EHexCellType type;

	HexCell()
		: HexCell{ { 0, 0 }, EHexCellType::Forbidden }
	{}
	HexCell(ConstPosRef pos, const EHexCellType type)
		: pos{ pos },
		type{ type }
	{}

	bool operator==(const HexCell& other) const {
		return pos == other.pos;
	}
	bool operator!=(const HexCell& other) const {
		return !(*this == other);
	}
};

using HexCellList = std::vector<HexCell>;
using ConstHexCellRef = const HexCell&;

// Tired of writing std::vector<std::vector<Class>> ?
// Introducing Vector2D !
template <typename T>
class vector2D : public std::vector<std::vector<T>> {
public:
	using ForEachCallBack = std::function<void(const T&)>;

	void for_each(ForEachCallBack callback) const noexcept {
		auto nestedCallback = [&callback](const std::vector<T>& v) {
			std::for_each(
				v.begin(),
				v.end(),
				callback
			);
		};
		
		std::for_each(
			begin(),
			end(),
			nestedCallback
		);
	}
};

template <typename T, typename R, typename Iterator>
std::vector<R> map(const Iterator& begin, const Iterator& end, std::function<R(T)> mapper)
{
	std::vector<R> result;

	for (auto it = begin; it != end; ++it) {
		result.push_back(mapper(*it));
	}

	return result;
}

template <typename T, typename R, typename Iterator>
std::vector<R>& map2D(const Iterator& begin, const Iterator& end, std::function<R(T)> mapper)
{
	std::vector<R> result;

	for (auto it = begin; it != end; ++it) {
		for (auto it2 = (*it).begin(); it2 != (*it).end(); ++it2) {
			result.push_back(mapper(*it2));
		}
	}

	return result;
}

// TODO move to utils.h
template <typename Iterator, typename Predicate, typename Func>
void for_each_if(Iterator begin, Iterator end, Predicate predicate, Func fn)
{
	for (; begin != end; ++begin) {
		if (predicate(*begin)) {
			fn(*begin);
		}
	}
}

template <typename Iterator, typename Func>
void for_map(Iterator begin, Iterator end, Func fn)
{
	for (; begin != end; ++begin) {
		for (auto val : *begin) {
			fn(val);
		}
	}
}

template <typename Iterator, typename Predicate, typename Func>
void for_map_if(Iterator begin, Iterator end, Predicate predicate, Func fn)
{
	for (; begin != end; ++begin) {
		for (auto val : *begin) {
			if (predicate(val)) fn(val);
		}
	}
}