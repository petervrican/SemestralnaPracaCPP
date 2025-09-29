#pragma once
#include <libds/adt/table.h>
#include <libds/amt/implicit_sequence.h>

namespace ds::amt {

    template<typename DataType>
    class ImplicitSequenceComparable : 
        public ImplicitSequence<DataType>
    {
    public:
        using BlockType = MemoryBlock<DataType>;

        bool equals(const AMT& other) const
        {
            if (this == &other)
            {
                return true;
            }

            if (this->size() != other.size())
            {
                return false;
            }

            const ImplicitSequenceComparable<DataType>* otherImplicitSequenceComparable = dynamic_cast<const ImplicitSequenceComparable<DataType>*>(&other);
            if (otherImplicitSequenceComparable == nullptr)
            {
                return false;
            }

            BlockType* myCurrent = this->accessFirst();
            BlockType* otherCurrent = otherImplicitSequenceComparable->accessFirst();

            while (myCurrent != nullptr)
            {
                if (!(myCurrent->data_ == otherCurrent->data_))
                {
                    return false;
                }
                else
                {
                    myCurrent = this->accessNext(*myCurrent);
                    otherCurrent = otherImplicitSequenceComparable->accessNext(*otherCurrent);
                }
            }

            return true;
        }

        bool operator==(const ImplicitSequenceComparable& other) const
        {
            return this->equals(other);
        }

        bool operator!=(const ImplicitSequenceComparable& other) const
        {
            return !(*this == other);
        }
    };
}

namespace ds::adt {
    template <typename K, typename T>
    class HashTableMyAddition :
        public HashTable<K, ds::amt::ImplicitSequenceComparable<T>*>
    {
    public:
        using Sequence = ds::amt::ImplicitSequenceComparable<T>;

        void insertValue(const K& key, const T& data) {
            Sequence** sequencePtr;

            if (this->tryFind(key, sequencePtr)) {
                (*sequencePtr)->insertLast().data_ = data;
            }
            else {
                Sequence* newSeq = new Sequence();
                newSeq->insertLast().data_ = data;
                this->insert(key, newSeq);
            }
        }
    };

}