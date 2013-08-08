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
#ifndef O3DGC_DV_ENCODE_PARAMS_H
#define O3DGC_DV_ENCODE_PARAMS_H

#include "o3dgcCommon.h"

namespace o3dgc
{
    class DVEncodeParams
    {
    public:
        //! Constructor.
                                    DVEncodeParams(void)
                                    {
                                        memset(this, 0, sizeof(DVEncodeParams));
                                        m_num               = 0;
                                        m_dim               = 0;
                                        m_stride            = 0;
                                        m_quantBits         = 10;
                                        m_max               = 0;
                                        m_min               = 0;
                                        m_vectors           = 0;
                                        m_streamTypeMode    = O3DGC_STREAM_TYPE_ASCII;
                                        m_encodeMode        = O3DGC_DYNAMIC_VECTOR_LIFT;
                                    };
        //! Destructor.
                                    ~DVEncodeParams(void) {};

        unsigned long               GetNVector()       const { return m_num;}
        unsigned long               GetDimVector()     const { return m_dim;}
        unsigned long               GetStride()        const { return m_stride;}
        unsigned long               GetQuantBits()     const { return m_quantBits;}

        const Real * const          GetMin()       const { return m_min;}
        const Real * const          GetMax()       const { return m_max;}
        Real                        GetMin (unsigned long j) const { return m_min[j];}
        Real                        GetMax (unsigned long j) const { return m_max[j];}

        const Real * const          GetVectors()       const { return m_vectors;}

        O3DGCStreamType             GetStreamType()    const { return m_streamTypeMode;}
        O3DGCDVEncodingMode         GetEncodeMode()    const { return m_encodeMode;}

        void                        SetNVector     (unsigned long num      ) { m_num       = num      ;}
        void                        SetDimVector   (unsigned long dim      ) { m_dim       = dim      ;}
        void                        SetStride      (unsigned long stride   ) { m_stride    = stride   ;}
        void                        SetQuantBits   (unsigned long quantBits) { m_quantBits = quantBits;}

        void                        SetMin    (Real * const min    ) { m_min     = min    ;}
        void                        SetMax    (Real * const max    ) { m_max     = max    ;}
        void                        SetVectors(Real * const vectors) { m_vectors = vectors;}

        void                        SetStreamType(O3DGCStreamType     streamTypeMode) { m_streamTypeMode = streamTypeMode;}
        void                        SetEncodeMode(O3DGCDVEncodingMode encodeMode    ) { m_encodeMode     = encodeMode    ;}

        void                        ComputeMinMax(O3DGCSC3DMCQuantizationMode quantMode)
                                    {
                                        assert( m_max && m_min && m_vectors && m_stride && m_dim && m_num);
                                        ComputeVectorMinMax(m_vectors, m_num , m_dim, m_stride, m_min , m_max , quantMode);
                                    }

    private:
        unsigned long               m_num;
        unsigned long               m_dim;
        unsigned long               m_stride;
        unsigned long               m_quantBits;
        Real *                      m_max;
        Real *                      m_min;
        Real *                      m_vectors;
        O3DGCStreamType             m_streamTypeMode;
        O3DGCDVEncodingMode         m_encodeMode;
    };
}
#endif // O3DGC_DV_ENCODE_PARAMS_H

