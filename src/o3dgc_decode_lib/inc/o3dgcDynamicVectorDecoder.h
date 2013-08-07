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


#pragma once
#ifndef O3DGC_DYNAMIC_VECTOR_DECODER_H
#define O3DGC_DYNAMIC_VECTOR_DECODER_H

#include "o3dgcCommon.h"
#include "o3dgcBinaryStream.h"

namespace o3dgc
{
    //! 
    class DynamicVectorDecoder
    {
    public:    
        //! Constructor.
                                    DynamicVectorDecoder(void);
        //! Destructor.
                                    ~DynamicVectorDecoder(void);
        //! 
        O3DGCErrorCode              Decode(const Real * const vectors,
                                           const long number,
                                           const long dim,
                                           const long stride,
                                           BinaryStream & bstream);
        O3DGCSC3DMCStreamType       GetStreamType() const { return m_streamType; }
        void                        SetStreamType(O3DGCSC3DMCStreamType streamType) { m_streamType = streamType; }

        private:

        long                        m_maxNumVectors;
        long                        m_numVectors;
        long                        m_dimVectors;
        long *                      m_quantVectors;
        O3DGCSC3DMCStreamType       m_streamType;
    };
}
#endif // O3DGC_DYNAMIC_VECTOR_DECODER_H

