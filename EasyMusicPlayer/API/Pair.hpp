#pragma once

template<class Key, class Val>
class Pair
{
public:
	Pair(Key _key = Key(), Val _val = Val())
	{
		key = _key; val = _val;
	}
	/*====================*/
	Val& value(void)
	{
		return val;
	}
	/*====================*/
	friend bool operator<(const Pair& a, const Pair& b)
	{
		return a.key < b.key;
	}
	friend bool operator==(const Pair& a, const Pair& b)
	{
		return a.key == b.key;
	}
private:
	Key key; Val val;
};