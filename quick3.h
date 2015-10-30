#ifndef QUICK3_H
#define QUICK3_H

#include <iterator>
#include <algorithm>
#include <iostream>
#include "ethread.h"

template<class iter>
void median(iter a, iter b, iter c)			//basic mostly optimized median of 3
{	//	sort order is 132
	if (*b < *a)
		iter_swap(a, b);
	if (*c < *a)
		iter_swap(a, c);
	if (*b < *c)
		iter_swap(b, c);
}

template<class iter>
void speed_median(iter a, iter b, iter c)	//optimal Mo3, don't attempt to read
{	//	 sort order is 132
	if (*a <= *c)
	{
		if (*c <= *b) {}		//acb
		else
		{
			if (*a <= *b)		//abc
			{
				auto tmp = *c;
				*c = *b;
				*b = tmp;
			}
			else				//bac
			{
				auto tmp = *a;
				*a = *b;
				*b = *c;
				*c = tmp;
			}
		}
	}
	else
	{
		if (*c <= *b)
		{
			if (*a <= *b)		//cab
			{
				auto tmp = *a;
				*a = *c;
				*c = tmp;
			}
			else				//cba
			{
				auto tmp = *a;
				*a = *c;
				*c = *b;
				*b = tmp;
			}
		}
		else					//bca
		{
			auto tmp = *a;
			*a = *b;
			*b = tmp;
		}
	}
}

template<class iter>
void median(iter a, iter b, iter c, iter d)	//mostly optimized Mo4
{	//	sort order is 2143
	if (*d < *b)
		iter_swap(d, b);
	if (*c < *a)
		iter_swap(c, a);
	if (*a < *b)
		iter_swap(a, b);
	if (*c < *d)
		iter_swap(c, d);
	if (*d < *a)
		iter_swap(d, a);
}

template<class iter>
void quick_impl(iter first, iter last);

template<class iter>
void quick(iter first, iter last)
{
	quick_impl(first, last-1);
}

template<class iter>
void quick_impl(iter first, iter last)
{
	if (last - first <= 0)
		return;
	median(first, first+(last-first)/2, last);
	iter i = first-1, j = last, pv = last;
	while (true)
	{
		while (*(++i) < *pv);
		while (*pv < *(--j) and j != first);
		if (i >= j)
			break;
		iter_swap(i, j);
	}
	iter_swap(i, last);
	quick_impl(first, i-1);
	quick_impl(i+1, last);
}

template<class iter>
void quick3_impl(iter first, iter last);

template<class iter>
void quick3_threaded_impl(iter first, iter last, int depth);

template<class iter>
void quick3(iter first, iter last)
{
	int order = log2(last - first) - 12;
	if (order > 0)
		quick3_threaded_impl(first, last-1, std::min(order, 12));
	else
		quick3_impl(first, last-1);
}

template<class iter>
void quick3_impl(iter first, iter last)
{
	//insertion sort on short lists
	if (last - first < 17)
	{
		for (iter i = first+1; i <= last; i++)
		{
			auto val = *i;
			iter j = i-1;
			for (; j >= first and *j > val; j--)
				*(j+1) = *j;
			*(j+1) = val;
		}
		return;
	}
	
	//set median of these three values to last (pivot value)
	speed_median(first, first+(last-first)/2, last);
	
	iter i = first-1, j = last, p = first-1, q = last, pv = last;
	//partition array around pivot
	while (true)
	{
		while (*(++i) < *pv);
		while (*pv < *(--j));
		if (i >= j)
			break;
		iter_swap(i, j);
		
		//set aside equal elements
		if (*i == *pv)
			iter_swap(i, ++p);
		if (*j == *pv)
			iter_swap(j, --q);
	}
	//put pivot into correct location
	iter_swap(i, pv);
	
	//add equal-to-pivot elements into middle partition
	j = i - 1;
	i = i + 1;
	for (iter k = first; k <= p; k++, j--)
		iter_swap(j, k);
	for (iter k = last-1; k >= q; k--, i++)
		iter_swap(i, k);
	
	//recursive call on first and third partition
	if (j-first > 0)
		quick3_impl(first, j);
	if (last-i > 0)
		quick3_impl(i, last);
}

template<class iter>
void quick3_threaded_impl(iter first, iter last, int depth)
{
	//insertion sort on short lists
	if (last - first < 17)
	{
		for (iter i = first+1; i <= last; i++)
		{
			auto val = *i;
			iter j = i-1;
			for (; j >= first and *j > val; j--)
				*(j+1) = *j;
			*(j+1) = val;
		}
		return;
	}
	
	//set median of these three values to last (pivot value)
	speed_median(first, first+(last-first)/2, last);
	
	iter i = first-1, j = last, p = first-1, q = last, pv = last;
	//partition array around pivot
	while (true)
	{
		while (*(++i) < *pv);
		while (*pv < *(--j));
		if (i >= j)
			break;
		iter_swap(i, j);
		
		//set aside equal elements
		if (*i == *pv)
			iter_swap(i, ++p);
		if (*j == *pv)
			iter_swap(j, --q);
	}
	//put pivot into correct location
	iter_swap(i, pv);
	
	//add equal-to-pivot elements into middle partition
	j = i - 1;
	i = i + 1;
	for (iter k = first; k <= p; k++, j--)
		iter_swap(j, k);
	for (iter k = last-1; k >= q; k--, i++)
		iter_swap(i, k);
	
	//recursive call on first and third partition
	thread* t = NULL;
	if (j-first > 0)
	{
		if (depth > 0)
			t = new thread(quick3_threaded_impl<iter>, first, j, depth - 1);
		else
			quick3_impl(first, j);
	}
	if (last-i > 0)
	{
		if (depth > 0)
			quick3_threaded_impl(i, last, depth - 1);
		else
			quick3_impl(i, last);
	}
	if (t)
		delete t;
}

//dual pivot quicksort, which is broken and I still can't figure out why.
//that's probably ok, though. It seems to be much slower than quick3

template<class iter>
void dquick_impl(iter first, iter last);

template<class iter>
void dquick(iter first, iter last)
{
	dquick_impl(first, last-1);
}

template<class iter>
void dquick_impl(iter first, iter last)
{
	//insertion sort on short lists
	if (last - first < 17)
	{
		for (iter i = first+1; i <= last; i++)
		{
			auto val = *i;
			iter j = i-1;
			for (; j >= first and *j > val; j--)
				*(j+1) = *j;
			*(j+1) = val;
		}
		return;
	}
	
	int dist = (last-first)/3;
	median(first, first+dist, last-dist, last);
	
	bool diffpivots = first != last;
	iter i = first, j = last, p = first, q = last, p1 = first, p2 = last;
	while (true)
	{
		while (*(++i) < *p1);
		while (*p2 < *(--j) and j != first);
		if (i >= j)
			break;
		iter_swap(i, j);
		if (*i >= *p1)
			iter_swap(i, ++p);
		if (*j <= *p2)
			iter_swap(j, --q);
	}
	auto A = *p1; auto B = *p2;
	if (*i <= *p2)
	//std::cout << *i << ' ' << *p2 << ' ' << *j <<'\n';
	iter_swap(i-1, p1);
	iter_swap(i, p2);
	
	j = i - 2;
	i = i + 1;
	for (iter k = first+1; k <= p; k++, j--)
		iter_swap(j, k);
	for (iter k = last-1; k >= q; k--, i++)
		iter_swap(i, k);
	
	// for (iter x = first; x != j+1; x++)
		// if (*x >= A)
			// std::cout << x-first << ' ' << j-first << " bad\n";
	// for (iter x = j+1; x != i; x++)
		// if (*x < A or *x > B)
			// std::cout << x-first << ' ' << i-first << " bad\n";
	// for (iter x = i; x != last+1; x++)
		// if (*x <= B)
			// std::cout << x-first << ' ' << last-first << " bad\n";
	
	
	if (j-first > 0)
		dquick_impl(first, j);
	if (last-i > 0)
		dquick_impl(i, last);
	if ((i-1)-(j+1) > 0)
		quick3_impl(j+1, i-1);
}

#endif