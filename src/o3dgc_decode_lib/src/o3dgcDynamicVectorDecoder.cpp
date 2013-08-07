/*
Copyright (c) 2013 Khaled Mammou - Advanced Micro Devices, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "o3dgcDynamicVectorDecoder.h"

namespace o3dgc
{
    O3DGCErrorCode Update(long * const data, const long size)
    {   
        assert(size > 1);
        const long size1 = size - 1;
        long p = 2;
        data[0] -= data[1] >> 1;
        while(p < size1)
        {
			data[p] -= (data[p-1] + data[p+1]) >> 2;
            p += 2;
		}
		if ( p == size1)
		{
			data[p] -= data[p-1]>>1;
		}
        return O3DGC_OK;
    }
    O3DGCErrorCode Predict(long * const data, const long size)
    {   
        assert(size > 1);
        const long size1 = size - 1;
        long p = 1;
        while(p < size1)
        {
			data[p] += (data[p-1] + data[p+1]) >> 1;
            p += 2;
		}
		if ( p == size1)
		{
			data[p] += data[p-1];
		}
        return O3DGC_OK;
    }
    O3DGCErrorCode Merge(long * const data, const long size)
    {
        assert(size > 1);
        const long h = size >> 2;
        long       a = h-1;
        long       b = h;
    
        while (a > 0)
        {
            for (long i = a; i < b; i += 2)
            {
                swap(data[i], data[i+1]);
            }
            --a;
            ++b;
        }
        return O3DGC_OK;
    }
    DynamicVectorDecoder::DynamicVectorDecoder(void)
    {
        m_maxNumVectors           = 0;
        m_numVectors              = 0;
        m_dimVectors              = 0;
        m_quantVectors            = 0;
        m_streamType              = O3DGC_SC3DMC_STREAM_TYPE_UNKOWN;
    }
    DynamicVectorDecoder::~DynamicVectorDecoder()
    {
        delete [] m_quantVectors;
    }
    
    O3DGCErrorCode DynamicVectorDecoder::Decode(const Real * const vectors,
                                                const long number,
                                                const long dim,
                                                const long stride,
                                                BinaryStream & bstream)
    {
        assert(number > 0);
        assert(dim > 0);
        assert(stride >= dim);
        
        return O3DGC_OK;
    }
}
