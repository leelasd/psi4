#ifndef yeti_filler_h
#define yeti_filler_h

#include "class.h"

#include "permutation.hpp"
#include "sort.hpp"
#include "index.hpp"
#include "filler.hpp"
#include "data.hpp"
#include "tile.hpp"

#include "mallocimpl.h"

namespace yeti {


class TileEstimater :
    public smartptr::Countable
{

    private:
        static UnitEstimaterPtr unit_;

    public:
        virtual float max_log(const uli* indices) const = 0;

        static UnitEstimater* get_unit_estimater();

};

class UnitEstimater :
    public TileEstimater
{

    public:
        float max_log(const uli* indices) const;

};

class TileElementComputer :
    public smartptr::Countable
{

    private:
        template <class T>
        bool equals(Tile* tile, const T* data);

        void* buffer_;

    public:
        TileElementComputer();

        virtual ~TileElementComputer();

        /**
            @return A copy of the element computer.  This must return a "thread-safe"
            copy for which calls to compute do not interfere between parent and copy.
        */
        virtual TileElementComputer* copy() const = 0;

        virtual void compute(Tile* tile, double* data);

        virtual void compute(Tile* tile, quad* data);

        virtual void compute(Tile* tile, int* data);

        virtual void compute(Tile* tile, float* data);

        bool equals(Tile* tile, const double* data);

        bool equals(Tile* tile, const int* data);

        bool equals(Tile* tile, const quad* data);

        bool equals(Tile* tile, const float* data);

        virtual TileEstimater* get_estimater(usi depth) const;

        void allocate_buffer(uli maxblocksize);
};

class MemsetElementComputer :
    public TileElementComputer
{

    public:
        void compute(Tile* tile, double* data);

        void compute(Tile* tile, quad* data);

        void compute(Tile* tile, int* data);

        void compute(Tile* tile, float* data);

        TileElementComputer* copy() const;

};

class NullElementComputer :
    public TileElementComputer
{
    public:
        void compute(Tile* tile, double* data);

        void compute(Tile* tile, quad* data);

        void compute(Tile* tile, int* data);

        void compute(Tile* tile, float* data);

        TileElementComputer* copy() const;

};

typedef NullElementComputer DiskReadElementComputer;

class ThreadedTileElementComputer :
    public smartptr::Countable
{

    protected:
        std::vector<TileElementComputerPtr> fillers_;

        usi mindepth_;

    public:
        ThreadedTileElementComputer(const TileElementComputerPtr& comp);

        virtual ~ThreadedTileElementComputer();

        void compute(Tile* tile, Data* data, uli threadnum);

        bool equals(Tile* tile, Data* data);

        void allocate_buffer(uli maxblocksize);

        void operator=(const TileElementComputerPtr& comp);

        TileElementComputer* get_thread_computer(uli threadnum) const;

        TileEstimater* get_estimater(usi depth) const;

        usi mindepth() const;

        void set_mindepth(usi depth);


};

} //end namespace yeti

#endif
