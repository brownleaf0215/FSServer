#pragma once

constexpr std::size_t ce_sqrt(std::size_t x, std::size_t y = 0)
{
	if (x == 0)
		return 0;

	if (x == 1)
		return y;
	++y;

	return ce_sqrt(x >> 1, y);
}

constexpr std::size_t ce_2_square(std::size_t x)
{
	if (x > 0)
		return 2 * ce_2_square(x - 1);
	return 1;
}

// x = sizeof(T)일 때, 64, 128, 256, 512, ... 구하는 메크로.
#define POOL_MEMSIZE(x) (ce_2_square( ( (x+7)/64 == 0 ? 0 : ce_sqrt((x+7)/64) + 1) + 6) )

// pair 에서 키를 찾아, 값을 리턴한다. (compile time)
constexpr int32_t ce_find_in_pair(const std::pair<std::size_t, int32_t> pairs[], std::size_t key, std::size_t count)
{
	--count;

	if (key == pairs[count].first)
		return pairs[count].second;

	if (count == 0)
		return -1;

	return ce_find_in_pair(pairs, key, count);
}
