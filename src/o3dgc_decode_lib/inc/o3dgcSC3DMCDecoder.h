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
<<<<<<< HEAD
#ifndef O3DGC_SC3DMC_H
#define O3DGC_SC3DMC_H
=======
#ifndef O3DGC_SC3DMC_DECODER_H
#define O3DGC_SC3DMC_DECODER_H
>>>>>>> eed4f212e93f096aa3c1130ba4dadb947a493d98

#include "o3dgcCommon.h"
#include "o3dgcBinaryStream.h"
#include "o3dgcIndexedFaceSet.h"
#include "o3dgcSC3DMCEncodeParams.h"
#include "o3dgcTriangleListDecoder.h"

namespace o3dgc
{    
    //! 
<<<<<<< HEAD
=======
    template <class T>
>>>>>>> eed4f212e93f096aa3c1130ba4dadb947a493d98
    class SC3DMCDecoder
    {
    public:    
        //! Constructor.
                                    SC3DMCDecoder(void)
                                    {
                                        m_iterator            = 0;
                                        m_streamSize          = 0;
                                        m_quantFloatArray     = 0;
                                        m_quantFloatArraySize = 0;
                                        m_streamType        = O3DGC_SC3DMC_STREAM_TYPE_UNKOWN;
                                    };
        //! Destructor.
                                    ~SC3DMCDecoder(void)
                                    {
                                        delete [] m_quantFloatArray;
                                    }
        //!
<<<<<<< HEAD
        O3DGCErrorCode              DecodeHeader(IndexedFaceSet & ifs,
                                                 const BinaryStream & bstream);
        //!                         
        O3DGCErrorCode              DecodePlayload(IndexedFaceSet & ifs,
                                           const BinaryStream & bstream);
=======
        O3DGCErrorCode              DecodeHeader(IndexedFaceSet<T> & ifs,
                                                 const BinaryStream & bstream);
        //!                         
        O3DGCErrorCode              DecodePlayload(IndexedFaceSet<T> & ifs,
                                                  const BinaryStream & bstream);
>>>>>>> eed4f212e93f096aa3c1130ba4dadb947a493d98
                                    
    private:                        
        O3DGCErrorCode              DecodeFloatArray(Real * const floatArray,
                                                     unsigned long numfloatArraySize,
                                                     unsigned long dimfloatArraySize,
                                                     const Real * const minfloatArray,
                                                     const Real * const maxfloatArray,
                                                     unsigned long nQBits,
<<<<<<< HEAD
                                                     const IndexedFaceSet & ifs,
=======
                                                     const IndexedFaceSet<T> & ifs,
>>>>>>> eed4f212e93f096aa3c1130ba4dadb947a493d98
                                                     O3DGCSC3DMCPredictionMode predMode,
                                                     const BinaryStream & bstream);
        O3DGCErrorCode              IQuantizeFloatArray(Real * const floatArray,
                                                       unsigned long numfloatArraySize,
                                                       unsigned long dimfloatArraySize,
                                                       const Real * const minfloatArray,
                                                       const Real * const maxfloatArray,
                                                       unsigned long nQBits);
        O3DGCErrorCode              DecodeIntArray(long * const intArray, 
                                                 unsigned long numIntArraySize,
                                                 unsigned long dimIntArraySize,
                                                 const BinaryStream & bstream);

        unsigned long               m_iterator;
        unsigned long               m_streamSize;
        SC3DMCEncodeParams          m_params;
<<<<<<< HEAD
        TriangleListDecoder         m_triangleListDecoder;
        long *                      m_quantFloatArray;
        unsigned long               m_quantFloatArraySize;
        O3DGCSC3DMCStreamType         m_streamType;
    };


}
#endif // O3DGC_SC3DMC_H
=======
        TriangleListDecoder<T>      m_triangleListDecoder;
        long *                      m_quantFloatArray;
        unsigned long               m_quantFloatArraySize;
        O3DGCSC3DMCStreamType       m_streamType;
    };
}
#include "o3dgcSC3DMCDecoder.inl"    // template implementation
#endif // O3DGC_SC3DMC_DECODER_H
>>>>>>> eed4f212e93f096aa3c1130ba4dadb947a493d98

