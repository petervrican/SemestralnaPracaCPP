#pragma once
#include <vector>
#include "UdajeRiadku.h"

class Algoritmus {
public:
	template<typename Iterator, typename Predicate, typename FillFunc>
	void algoritmus(const Iterator& bIterator, const Iterator& eIterator, Predicate predicate, FillFunc fillFunc, ds::amt::ImplicitSequence<UdajeRiadku>* vystup) {
		for (auto itr = bIterator; itr != eIterator; ++itr) {
			if (predicate(*itr)) {
				fillFunc(*itr, vystup);
			}
		}
	}
};