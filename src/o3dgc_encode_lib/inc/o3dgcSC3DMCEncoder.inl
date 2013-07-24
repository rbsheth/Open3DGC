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
#ifndef O3DGC_SC3DMC_ENCODER_INL
#define O3DGC_SC3DMC_ENCODER_INL


#include "o3dgcArithmeticCodec.h"
#include "o3dgcTimer.h"
#include "o3dgcVector.h"
#include "o3dgcBinaryStream.h"

//#define DEBUG_VERBOSE

namespace o3dgc
{
#ifdef DEBUG_VERBOSE
        FILE * g_fileDebugSC3DMCEnc = NULL;
#endif //DEBUG_VERBOSE

    template <class T>
    O3DGCErrorCode SC3DMCEncoder<T>::Encode(const SC3DMCEncodeParams & params, 
                                            const IndexedFaceSet<T> & ifs, 
                                            BinaryStream & bstream)
    {
        // Encode header
        unsigned long start = bstream.GetSize();
        EncodeHeader(params, ifs, bstream);
        // Encode payload
        EncodePayload(params, ifs, bstream);
        bstream.WriteUInt32(O3DGC_BINARY_STREAM_NUM_SYMBOLS_UINT32, bstream.GetSize() - start, m_streamType);
        return O3DGC_OK;
    }
    template <class T>
    O3DGCErrorCode SC3DMCEncoder<T>::EncodeHeader(const SC3DMCEncodeParams & params, 
                                               const IndexedFaceSet<T> & ifs, 
                                               BinaryStream & bstream)
    {
        m_streamType = params.GetStreamType();
        bstream.WriteUInt32(O3DGC_SC3DMC_START_CODE, m_streamType);
        bstream.WriteUInt32(0, m_streamType); // to be filled later

        bstream.WriteUChar(O3DGC_SC3DMC_TFAN, m_streamType);
        bstream.WriteFloat32((float)ifs.GetCreaseAngle(), m_streamType);
          
        unsigned char mask = 0;
        bool markerBit0 = false;
        bool markerBit1 = false;
        bool markerBit2 = false;
        bool markerBit3 = false;

        mask += (ifs.GetCCW()                  );
        mask += (ifs.GetSolid()            << 1);
        mask += (ifs.GetConvex()           << 2);
        mask += (ifs.GetIsTriangularMesh() << 3);
        mask += (markerBit0                << 4);
        mask += (markerBit1                << 5);
        mask += (markerBit2                << 6);
        mask += (markerBit3                << 7);

        bstream.WriteUChar(mask, m_streamType);

        bstream.WriteUInt32(ifs.GetNCoord(), m_streamType);
        bstream.WriteUInt32(ifs.GetNNormal(), m_streamType);
        bstream.WriteUInt32(ifs.GetNColor(), m_streamType);
        bstream.WriteUInt32(ifs.GetNTexCoord(), m_streamType);
        bstream.WriteUInt32(ifs.GetNumFloatAttributes(), m_streamType);
        bstream.WriteUInt32(ifs.GetNumIntAttributes(), m_streamType);

        if (ifs.GetNCoord() > 0)
        {
            bstream.WriteUInt32(ifs.GetNCoordIndex(), m_streamType);
            for(int j=0 ; j<3 ; ++j)
            {
                bstream.WriteFloat32((float) ifs.GetCoordMin(j), m_streamType);
                bstream.WriteFloat32((float) ifs.GetCoordMax(j), m_streamType);
            }            
            bstream.WriteUChar((unsigned char) params.GetCoordQuantBits(), m_streamType);
        }
        if (ifs.GetNNormal() > 0)
        {
            bstream.WriteUInt32(0, m_streamType);
             for(int j=0 ; j<3 ; ++j)
            {
                bstream.WriteFloat32((float) ifs.GetNormalMin(j), m_streamType);
                bstream.WriteFloat32((float) ifs.GetNormalMax(j), m_streamType);
            }
            bstream.WriteUChar(true, m_streamType); //(unsigned char) ifs.GetNormalPerVertex()
            bstream.WriteUChar((unsigned char) params.GetNormalQuantBits(), m_streamType);
        }
        if (ifs.GetNColor() > 0)
        {
            bstream.WriteUInt32(0, m_streamType);
             for(int j=0 ; j<3 ; ++j)
            {
                bstream.WriteFloat32((float) ifs.GetColorMin(j), m_streamType);
                bstream.WriteFloat32((float) ifs.GetColorMax(j), m_streamType);
            }
            bstream.WriteUChar(true, m_streamType); // (unsigned char) ifs.GetColorPerVertex()
            bstream.WriteUChar((unsigned char) params.GetColorQuantBits(), m_streamType);
        }
        if (ifs.GetNTexCoord() > 0)
        {
            bstream.WriteUInt32(0, m_streamType);
             for(int j=0 ; j<2 ; ++j)
            {
                bstream.WriteFloat32((float) ifs.GetTexCoordMin(j), m_streamType);
                bstream.WriteFloat32((float) ifs.GetTexCoordMax(j), m_streamType);
            }
            bstream.WriteUChar((unsigned char) params.GetTexCoordQuantBits(), m_streamType);
        }
        for(unsigned long a = 0; a < ifs.GetNumFloatAttributes(); ++a)
        {
            bstream.WriteUInt32(ifs.GetNFloatAttribute(a), m_streamType);
            if (ifs.GetNFloatAttribute(a) > 0)
            {
                assert(ifs.GetFloatAttributeDim(a) < (unsigned long) O3DGC_MAX_UCHAR8);
                bstream.WriteUInt32(0, m_streamType);
                unsigned char d = (unsigned char) ifs.GetFloatAttributeDim(a);
                bstream.WriteUChar(d, m_streamType);
                for(unsigned char j = 0 ; j < d ; ++j)
                {
                    bstream.WriteFloat32((float) ifs.GetFloatAttributeMin(a, j), m_streamType);
                    bstream.WriteFloat32((float) ifs.GetFloatAttributeMax(a, j), m_streamType);
                }
                bstream.WriteUChar(true, m_streamType); //(unsigned char) ifs.GetFloatAttributePerVertex(a)
                bstream.WriteUChar((unsigned char) params.GetFloatAttributeQuantBits(a), m_streamType);
            }
        }
        for(unsigned long a = 0; a < ifs.GetNumIntAttributes(); ++a)
        {
            bstream.WriteUInt32(ifs.GetNIntAttribute(a), m_streamType);
            if (ifs.GetNIntAttribute(a) > 0)
            {
                assert(ifs.GetFloatAttributeDim(a) < (unsigned long) O3DGC_MAX_UCHAR8);
                bstream.WriteUInt32(0, m_streamType);
                bstream.WriteUChar((unsigned char) ifs.GetIntAttributeDim(a), m_streamType);
                bstream.WriteUChar(true, m_streamType); // (unsigned char) ifs.GetIntAttributePerVertex(a)
            }
        }    
        return O3DGC_OK;
    }    
    template <class T>
    O3DGCErrorCode SC3DMCEncoder<T>::QuantizeFloatArray(const Real * const floatArray, 
                                                   unsigned long numFloatArray,
                                                   unsigned long dimFloatArray,
                                                   const Real * const minFloatArray,
                                                   const Real * const maxFloatArray,
                                                   unsigned long nQBits)
    {
        const unsigned long size = numFloatArray * dimFloatArray;
        Real delta[O3DGC_SC3DMC_MAX_DIM_FLOAT_ATTRIBUTES];
        Real r;
        for(unsigned long d = 0; d < dimFloatArray; d++)
        {
            r = maxFloatArray[d] - minFloatArray[d];
            if (r > 0.0f)
            {
                delta[d] = (float)((1 << nQBits) - 1) / r;
            }
            else
            {
                delta[d] = 1.0f;
            }
        }        
        if (m_quantFloatArraySize < size)
        {
            delete [] m_quantFloatArray;
            m_quantFloatArraySize = size;
            m_quantFloatArray     = new long [size];
        }                                  
        for(unsigned long v = 0; v < numFloatArray; ++v)
        {
            for(unsigned long d = 0; d < dimFloatArray; ++d)
            {
                m_quantFloatArray[v * dimFloatArray + d] = (long)((floatArray[v * dimFloatArray + d]-minFloatArray[d]) * delta[d] + 0.5f);
            }
        }
        return O3DGC_OK;
    }
    inline void EncodeIntACEGC(long predResidual, 
                               Arithmetic_Codec & ace,
                               Adaptive_Data_Model & mModelValues,
                               Static_Bit_Model & bModel0,
                               Adaptive_Bit_Model & bModel1,
                               const unsigned long M)
    {
        unsigned long uiValue;
        if (predResidual < 0)
        {
            uiValue = (unsigned long) (1 - (2 * predResidual));
        }
        else
        {
            uiValue = (unsigned long) (2 * predResidual);
        }
        if (uiValue < M) 
        {
            ace.encode(uiValue, mModelValues);
        }
        else 
        {
            ace.encode(M, mModelValues);
            ace.ExpGolombEncode(uiValue-M, 0, bModel0, bModel1);
        }
    }
    inline void EncodeUIntACEGC(long predResidual, 
                                Arithmetic_Codec & ace,
                                Adaptive_Data_Model & mModelValues,
                                Static_Bit_Model & bModel0,
                                Adaptive_Bit_Model & bModel1,
                                const unsigned long M)
    {
        unsigned long uiValue = (unsigned long) predResidual;
        if (uiValue < M) 
        {
            ace.encode(uiValue, mModelValues);
        }
        else 
        {
            ace.encode(M, mModelValues);
            ace.ExpGolombEncode(uiValue-M, 0, bModel0, bModel1);
        }
    }
    template <class T>
    O3DGCErrorCode SC3DMCEncoder<T>::EncodeFloatArray(const Real * const floatArray, 
                                                   unsigned long numFloatArray,
                                                   unsigned long dimFloatArray,
                                                   const Real * const minFloatArray,
                                                   const Real * const maxFloatArray,
                                                   unsigned long nQBits,
                                                   const IndexedFaceSet<T> & ifs,
                                                   O3DGCSC3DMCPredictionMode predMode,
                                                   BinaryStream & bstream)
    {
        assert(dimFloatArray <  O3DGC_SC3DMC_MAX_DIM_FLOAT_ATTRIBUTES);
        long predResidual, v, uPredResidual;
        unsigned long nPred;
        Arithmetic_Codec ace;
        Static_Bit_Model bModel0;
        Adaptive_Bit_Model bModel1;

        const AdjacencyInfo & v2T         = m_triangleListEncoder.GetVertexToTriangle();
        const long * const    vmap        = m_triangleListEncoder.GetVMap();
        const long * const    tmap        = m_triangleListEncoder.GetTMap();
        const long * const    invVMap     = m_triangleListEncoder.GetInvVMap();
        const T * const       triangles   = ifs.GetCoordIndex();
        const long            nvert       = (long) numFloatArray;
        unsigned long         start       = bstream.GetSize();
        unsigned char         mask        = predMode & 7;
        const unsigned long   M           = O3DGC_SC3DMC_MAX_PREDICTION_SYMBOLS - 1;
        unsigned long         nSymbols    = O3DGC_SC3DMC_MAX_PREDICTION_SYMBOLS;
        unsigned long         nPredictors = O3DGC_SC3DMC_MAX_PREDICTION_NEIGHBORS;
        

        Adaptive_Data_Model mModelValues(M+2);
        Adaptive_Data_Model mModelPreds(O3DGC_SC3DMC_MAX_PREDICTION_NEIGHBORS+1);

        memset(m_freqSymbols, 0, sizeof(unsigned long) * O3DGC_SC3DMC_MAX_PREDICTION_SYMBOLS);
        memset(m_freqPreds  , 0, sizeof(unsigned long) * O3DGC_SC3DMC_MAX_PREDICTION_NEIGHBORS);
        if (m_streamType == O3DGC_SC3DMC_STREAM_TYPE_ASCII)
        {
            mask += (O3DGC_SC3DMC_BINARIZATION_ASCII & 7)<<4;
            m_predictors.Allocate(nvert);
            m_predictors.Clear();
        }
        else
        {
            mask += (O3DGC_SC3DMC_BINARIZATION_AC_EGC & 7)<<4;
            const unsigned int NMAX = numFloatArray * dimFloatArray * 8 + 100;
            if ( m_sizeBufferAC < NMAX )
            {
                delete [] m_bufferAC;
                m_sizeBufferAC = NMAX;
                m_bufferAC     = new unsigned char [m_sizeBufferAC];
            }
            ace.set_buffer(NMAX, m_bufferAC);
            ace.start_encoder();
            ace.ExpGolombEncode(0, 0, bModel0, bModel1);
            ace.ExpGolombEncode(M, 0, bModel0, bModel1);
        }
        bstream.WriteUInt32(0, m_streamType);
        bstream.WriteUChar(mask, m_streamType);
        QuantizeFloatArray(floatArray, numFloatArray, dimFloatArray, minFloatArray, maxFloatArray, nQBits);

#ifdef DEBUG_VERBOSE
        printf("FloatArray (%i, %i)\n", numFloatArray, dimFloatArray);
        fprintf(g_fileDebugSC3DMCEnc, "FloatArray (%i, %i)\n", numFloatArray, dimFloatArray);
#endif //DEBUG_VERBOSE

        for (long vm=0; vm < nvert; ++vm) 
        {
            nPred = 0;
            v     = invVMap[vm];
            assert( v >= 0 && v < nvert);

            if ( v2T.GetNumNeighbors(v) > 0 && 
                 predMode != O3DGC_SC3DMC_NO_PREDICTION)
            {
                int u0 = v2T.Begin(v);
                int u1 = v2T.End(v);
                for (long u = u0; u < u1; u++) 
                {
                    long ta = v2T.GetNeighbor(u);
                    if (predMode == O3DGC_SC3DMC_PARALLELOGRAM_PREDICTION)
                    {
                        long a,b;
                        if ((long) triangles[ta*3] == v)
                        {
                            a = triangles[ta*3 + 1];
                            b = triangles[ta*3 + 2];
                        }
                        else if ((long) triangles[ta*3 + 1] == v)
                        {
                            a = triangles[ta*3 + 0];
                            b = triangles[ta*3 + 2];
                        }
                        else
                        {
                            a = triangles[ta*3 + 0];
                            b = triangles[ta*3 + 1];
                        }
                        if ( vmap[a] < vm && vmap[b] < vm)
                        {
                            int u0 = v2T.Begin(a);
                            int u1 = v2T.End(a);
                            for (long u = u0; u < u1; u++) 
                            {
                                long tb = v2T.GetNeighbor(u);
                                long c = -1;
                                bool foundB = false;
                                for(long k = 0; k < 3; ++k)
                                {
                                    long x = triangles[tb*3 + k];
                                    if (x == b)
                                    {
                                        foundB = true;
                                    }
                                    if (vmap[x] < vm && x != a && x != b)
                                    {
                                        c = x;
                                    }
                                }
                                if (c != -1 && foundB)
                                {
                                    SC3DMCTriplet id = {min(vmap[a], vmap[b]), max(vmap[a], vmap[b]), -vmap[c]-1};
                                    unsigned long p = Insert(id, nPred, m_neighbors);
                                    if (p != 0xFFFFFFFF)
                                    {
                                        for (unsigned long i = 0; i < dimFloatArray; i++) 
                                        {
                                            m_neighbors[p].m_pred[i] = m_quantFloatArray[a*dimFloatArray+i] + 
                                                                       m_quantFloatArray[b*dimFloatArray+i] - 
                                                                       m_quantFloatArray[c*dimFloatArray+i];
                                        } 
                                    }
                                }
                            }
                        }
                    }
                    if ( predMode == O3DGC_SC3DMC_PARALLELOGRAM_PREDICTION ||
                         predMode == O3DGC_SC3DMC_DIFFERENTIAL_PREDICTION )
                    {
                        for(long k = 0; k < 3; ++k)
                        {
                            long w = triangles[ta*3 + k];
                            if ( vmap[w] < vm )
                            {
                                SC3DMCTriplet id = {-1, -1, vmap[w]};
                                unsigned long p = Insert(id, nPred, m_neighbors);
                                if (p != 0xFFFFFFFF)
                                {
                                    for (unsigned long i = 0; i < dimFloatArray; i++) 
                                    {
                                        m_neighbors[p].m_pred[i] = m_quantFloatArray[w*dimFloatArray+i];
                                    } 
                                }
                            }
                        }
                    }        
                }
            }
            if (nPred > 1)
            {
                // find best predictor
                unsigned long bestPred = 0xFFFFFFFF;
                double bestCost = O3DGC_MAX_DOUBLE;
                double cost;
#ifdef DEBUG_VERBOSE
                    printf("\t\t vm %i\n", vm);
                    fprintf(g_fileDebugSC3DMCEnc, "\t\t vm %i\n", vm);
#endif //DEBUG_VERBOSE

                for (unsigned long p = 0; p < nPred; ++p)
                {
#ifdef DEBUG_VERBOSE
                    printf("\t\t pred a = %i b = %i c = %i \n", m_neighbors[p].m_id.m_a, m_neighbors[p].m_id.m_b, m_neighbors[p].m_id.m_c);
                    fprintf(g_fileDebugSC3DMCEnc, "\t\t pred a = %i b = %i c = %i \n", m_neighbors[p].m_id.m_a, m_neighbors[p].m_id.m_b, m_neighbors[p].m_id.m_c);
#endif //DEBUG_VERBOSE
                    cost = -log2((m_freqPreds[p]+1.0) / nPredictors );
                    for (unsigned long i = 0; i < dimFloatArray; ++i) 
                    {
#ifdef DEBUG_VERBOSE
                        printf("\t\t\t %i\n", m_neighbors[p].m_pred[i]);
                        fprintf(g_fileDebugSC3DMCEnc, "\t\t\t %i\n", m_neighbors[p].m_pred[i]);
#endif //DEBUG_VERBOSE

                        predResidual = m_quantFloatArray[v*dimFloatArray+i] - m_neighbors[p].m_pred[i];
                        if (predResidual < 0)
                        {
                            predResidual = 1 - (2 * predResidual);
                        }
                        else
                        {
                            predResidual = 2 * predResidual;
                        }
                        if (predResidual < (long) M) 
                        {
                            cost += -log2((m_freqSymbols[predResidual]+1.0) / nSymbols );
                        }
                        else 
                        {
                            cost += -log2((m_freqSymbols[M] + 1.0) / nSymbols ) + log2((double) (predResidual-M));
                        }
                    }
                    if (cost < bestCost)
                    {
                        bestCost = cost;
                        bestPred = p;
                    }
                }
                if (m_streamType == O3DGC_SC3DMC_STREAM_TYPE_ASCII)
                {
                    m_predictors.PushBack((unsigned char) bestPred);
                }
                else
                {
                    ace.encode(bestPred, mModelPreds);
                }
#ifdef DEBUG_VERBOSE
                    printf("best (%i, %i, %i) \t pos %i\n", m_neighbors[bestPred].m_id.m_a, m_neighbors[bestPred].m_id.m_b, m_neighbors[bestPred].m_id.m_c, bestPred);
                    fprintf(g_fileDebugSC3DMCEnc, "best (%i, %i, %i) \t pos %i\n", m_neighbors[bestPred].m_id.m_a, m_neighbors[bestPred].m_id.m_b, m_neighbors[bestPred].m_id.m_c, bestPred);
#endif //DEBUG_VERBOSE
                // use best predictor
                for (unsigned long i = 0; i < dimFloatArray; ++i) 
                {
                    predResidual  = m_quantFloatArray[v*dimFloatArray+i] - m_neighbors[bestPred].m_pred[i];
                    uPredResidual = (predResidual < 0) ? (1 - (2 * predResidual)) : (2 * predResidual);
                    ++m_freqSymbols[(uPredResidual < (long) M)? uPredResidual : M];

#ifdef DEBUG_VERBOSE
                    printf("%i \t %i \t [%i]\n", vm*dimFloatArray+i, predResidual, m_neighbors[bestPred].m_pred[i]);
                    fprintf(g_fileDebugSC3DMCEnc, "%i \t %i \t [%i]\n", vm*dimFloatArray+i, predResidual, m_neighbors[bestPred].m_pred[i]);
#endif //DEBUG_VERBOSE

                    if (m_streamType == O3DGC_SC3DMC_STREAM_TYPE_ASCII)
                    {
                        bstream.WriteIntASCII(predResidual);
                    }
                    else
                    {
                        EncodeIntACEGC(predResidual, ace, mModelValues, bModel0, bModel1, M);
                    }
                }
                ++m_freqPreds[bestPred];
                nSymbols += dimFloatArray;
                ++nPredictors;
            }
            else if ( vm > 0 && predMode != O3DGC_SC3DMC_NO_PREDICTION)
            {
                long prev = invVMap[vm-1];
                for (unsigned long i = 0; i < dimFloatArray; i++) 
                {
                    predResidual = m_quantFloatArray[v*dimFloatArray+i] - m_quantFloatArray[prev*dimFloatArray+i];
                    if (m_streamType == O3DGC_SC3DMC_STREAM_TYPE_ASCII)
                    {
                        bstream.WriteIntASCII(predResidual);
                    }
                    else
                    {
                        EncodeIntACEGC(predResidual, ace, mModelValues, bModel0, bModel1, M);
                    }
#ifdef DEBUG_VERBOSE
                    printf("%i \t %i\n", vm*dimFloatArray+i, predResidual);
                    fprintf(g_fileDebugSC3DMCEnc, "%i \t %i\n", vm*dimFloatArray+i, predResidual);
#endif //DEBUG_VERBOSE
                }
            }
            else
            {
                for (unsigned long i = 0; i < dimFloatArray; i++) 
                {
                    predResidual = m_quantFloatArray[v*dimFloatArray+i];
                    if (m_streamType == O3DGC_SC3DMC_STREAM_TYPE_ASCII)
                    {
                        bstream.WriteUIntASCII(predResidual);
                    }
                    else
                    {
                        EncodeUIntACEGC(predResidual, ace, mModelValues, bModel0, bModel1, M);
                    }
#ifdef DEBUG_VERBOSE
                    printf("%i \t %i\n", vm*dimFloatArray+i, predResidual);
                    fprintf(g_fileDebugSC3DMCEnc, "%i \t %i\n", vm*dimFloatArray+i, predResidual);
#endif //DEBUG_VERBOSE
                }
            }
        }
        if (m_streamType != O3DGC_SC3DMC_STREAM_TYPE_ASCII)
        {
            unsigned long encodedBytes = ace.stop_encoder();
            for(size_t i = 0; i < encodedBytes; ++i)
            {
                bstream.WriteUChar8Bin(m_bufferAC[i]);
            }
        }
//        printf("*--> %lu \n", bstream.GetSize() - start);
        bstream.WriteUInt32(start, bstream.GetSize() - start, m_streamType);

        if (m_streamType == O3DGC_SC3DMC_STREAM_TYPE_ASCII)
        {
            unsigned long start = bstream.GetSize();
            bstream.WriteUInt32ASCII(0);
            const size_t size       = m_predictors.GetSize();
            for(size_t i = 0; i < size; ++i)
            {
                bstream.WriteUCharASCII(m_predictors[i]);
            }
            bstream.WriteUInt32ASCII(start, bstream.GetSize() - start);
        }
#ifdef DEBUG_VERBOSE
        fflush(g_fileDebugSC3DMCEnc);
#endif //DEBUG_VERBOSE
        return O3DGC_OK;
    }
    template <class T>
    O3DGCErrorCode SC3DMCEncoder<T>::EncodeIntArray(const long * const intArray, 
                                                 unsigned long numIntArray,
                                                 unsigned long dimIntArray,
                                                 O3DGCSC3DMCPredictionMode predMode,
                                                 BinaryStream & bstream)
    {
        const long * const invVMap   = m_triangleListEncoder.GetInvVMap();
        unsigned long start = bstream.GetSize();
        unsigned char mask = predMode & 7;
        Arithmetic_Codec ace;
        Static_Bit_Model bModel0;
        Adaptive_Bit_Model bModel1;
        const unsigned long M = 256;
        Adaptive_Data_Model mModelValues(M+2);

        long minValue = 0;
        const unsigned long size = numIntArray * dimIntArray;
        for(unsigned long i = 0; i < size; ++i)
        {
            if (minValue > intArray[i]) 
            {
                minValue = intArray[i];
            }
        }
        if (m_streamType == O3DGC_SC3DMC_STREAM_TYPE_ASCII)
        {
            mask += (O3DGC_SC3DMC_BINARIZATION_ASCII & 7)<<4;
        }
        else
        {
            mask += (O3DGC_SC3DMC_BINARIZATION_AC_EGC & 7)<<4;
            const unsigned int NMAX = numIntArray * dimIntArray * 8 + 100;
            if ( m_sizeBufferAC < NMAX )
            {
                delete [] m_bufferAC;
                m_sizeBufferAC = NMAX;
                m_bufferAC     = new unsigned char [m_sizeBufferAC];
            }
            ace.set_buffer(NMAX, m_bufferAC);
            ace.start_encoder();
            ace.ExpGolombEncode(0, 0, bModel0, bModel1);
            ace.ExpGolombEncode(M, 0, bModel0, bModel1);
        }
        bstream.WriteUInt32(0, m_streamType);
        bstream.WriteUChar(mask, m_streamType);
        bstream.WriteUInt32(minValue + O3DGC_MAX_LONG, m_streamType);

        long v;
        if (m_streamType == O3DGC_SC3DMC_STREAM_TYPE_ASCII)
        {
            for (unsigned long vm=0; vm < numIntArray; ++vm) 
            {
                v = invVMap[vm];
                assert( v >= 0 && v < (long) numIntArray);
                for (unsigned long i = 0; i < dimIntArray; i++) 
                {
                    bstream.WriteUIntASCII(intArray[v*dimIntArray+i] - minValue);
                }
            }
        }
        else
        {
            for (unsigned long vm=0; vm < numIntArray; ++vm) 
            {
                v = invVMap[vm];
                assert( v >= 0 && v < (long) numIntArray);
                for (unsigned long i = 0; i < dimIntArray; i++) 
                {
                    EncodeUIntACEGC(intArray[v*dimIntArray+i] - minValue, ace, mModelValues, bModel0, bModel1, M);
                }
            }
            unsigned long encodedBytes = ace.stop_encoder();
            for(size_t i = 0; i < encodedBytes; ++i)
            {
                bstream.WriteUChar8Bin(m_bufferAC[i]);
            }
        }
        bstream.WriteUInt32(start, bstream.GetSize() - start, m_streamType);
        return O3DGC_OK;
    }
    template <class T>
    O3DGCErrorCode SC3DMCEncoder<T>::EncodePayload(const SC3DMCEncodeParams & params, 
                                                   const IndexedFaceSet<T> & ifs, 
                                                   BinaryStream & bstream)
    {
#ifdef DEBUG_VERBOSE
        g_fileDebugSC3DMCEnc = fopen("tfans_enc_main.txt", "w");
#endif //DEBUG_VERBOSE

        // encode triangle list        
        m_triangleListEncoder.SetStreamType(params.GetStreamType());
        m_stats.m_streamSizeCoordIndex = bstream.GetSize();
        Timer timer;
        timer.Tic();
        m_triangleListEncoder.Encode(ifs.GetCoordIndex(), ifs.GetNCoordIndex(), ifs.GetNCoord(), bstream);
        timer.Toc();
        m_stats.m_timeCoordIndex       = timer.GetElapsedTime();
        m_stats.m_streamSizeCoordIndex = bstream.GetSize() - m_stats.m_streamSizeCoordIndex;

        // encode coord
        m_stats.m_streamSizeCoord = bstream.GetSize();
        timer.Tic();
        if (ifs.GetNCoord() > 0)
        {
            EncodeFloatArray(ifs.GetCoord(), ifs.GetNCoord(), 3, ifs.GetCoordMin(), ifs.GetCoordMax(), 
                                params.GetCoordQuantBits(), ifs, params.GetCoordPredMode(), bstream);
        }
        timer.Toc();
        m_stats.m_timeCoord       = timer.GetElapsedTime();
        m_stats.m_streamSizeCoord = bstream.GetSize() - m_stats.m_streamSizeCoord;


        // encode Normal
        m_stats.m_streamSizeNormal = bstream.GetSize();
        timer.Tic();
        if (ifs.GetNNormal() > 0)
        {
            EncodeFloatArray(ifs.GetNormal(), ifs.GetNNormal(), 3, ifs.GetNormalMin(), ifs.GetNormalMax(), 
                                params.GetNormalQuantBits(), ifs, params.GetNormalPredMode(), bstream);
        }
        timer.Toc();
        m_stats.m_timeNormal       = timer.GetElapsedTime();
        m_stats.m_streamSizeNormal = bstream.GetSize() - m_stats.m_streamSizeNormal;


        // encode Color
        m_stats.m_streamSizeColor = bstream.GetSize();
        timer.Tic();
        if (ifs.GetNColor() > 0)
        {
            EncodeFloatArray(ifs.GetColor(), ifs.GetNColor(), 3, ifs.GetColorMin(), ifs.GetColorMax(), 
                                params.GetColorQuantBits(), ifs, params.GetColorPredMode(), bstream);
        }
        timer.Toc();
        m_stats.m_timeColor       = timer.GetElapsedTime();
        m_stats.m_streamSizeColor = bstream.GetSize() - m_stats.m_streamSizeColor;

        // encode TexCoord
        m_stats.m_streamSizeTexCoord = bstream.GetSize();
        timer.Tic();
        if (ifs.GetNTexCoord() > 0)
        {
            EncodeFloatArray(ifs.GetTexCoord(), ifs.GetNTexCoord(), 2, ifs.GetTexCoordMin(), ifs.GetTexCoordMax(), 
                                params.GetTexCoordQuantBits(), ifs, params.GetTexCoordPredMode(), bstream);
        }
        timer.Toc();
        m_stats.m_timeTexCoord       = timer.GetElapsedTime();
        m_stats.m_streamSizeTexCoord = bstream.GetSize() - m_stats.m_streamSizeTexCoord;

        m_stats.m_streamSizeFloatAttribute = bstream.GetSize();
        timer.Tic();
        for(unsigned long a = 0; a < ifs.GetNumFloatAttributes(); ++a)
        {
            EncodeFloatArray(ifs.GetFloatAttribute(a), ifs.GetNFloatAttribute(a), ifs.GetFloatAttributeDim(a), ifs.GetFloatAttributeMin(a), ifs.GetFloatAttributeMax(a), 
                                params.GetFloatAttributeQuantBits(a), ifs, params.GetFloatAttributePredMode(a), bstream);
        }
        timer.Toc();
        m_stats.m_timeFloatAttribute       = timer.GetElapsedTime();
        m_stats.m_streamSizeFloatAttribute = bstream.GetSize() - m_stats.m_streamSizeFloatAttribute;

        m_stats.m_streamSizeIntAttribute = bstream.GetSize();
        timer.Tic();        
        for(unsigned long a = 0; a < ifs.GetNumIntAttributes(); ++a)
        {
            EncodeIntArray(ifs.GetIntAttribute(a), ifs.GetNIntAttribute(a), ifs.GetIntAttributeDim(a), params.GetIntAttributePredMode(a), bstream);
        }
        timer.Toc();
        m_stats.m_timeIntAttribute       = timer.GetElapsedTime();
        m_stats.m_streamSizeIntAttribute = bstream.GetSize() - m_stats.m_streamSizeIntAttribute;
#ifdef DEBUG_VERBOSE
        fclose(g_fileDebugSC3DMCEnc);
#endif //DEBUG_VERBOSE
        return O3DGC_OK;
    }
}
#endif // O3DGC_SC3DMC_ENCODER_INL


