#pragma once
#include <vector>
using namespace std; 


class Cache
{
public:
	template<class T>
	static void Add(vector<T*> * item_list, T * t, unsigned long size_limit)
	{
		if (Find(item_list,t)) return;
		if (std::find(item_list->begin(), item_list->end(), t) != item_list->end())
		{
			return;
		}
		if (item_list->size()>=size_limit)
		{
			delete (*item_list)[0];
			item_list->erase(item_list->begin());
			item_list->push_back(t);

		}
		else
		{
			item_list->push_back(t);
		}
	}
	template<class T>
	static T * Find(vector <T*> * item_list,T* t)
	{
		for (auto item:*item_list)
		{
			if (*t==*item)
			{
				return item;
			}
		}
		return 0;
	}
};