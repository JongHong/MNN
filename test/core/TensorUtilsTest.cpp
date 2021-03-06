//
//  TensorUtilsTest.cpp
//  MNNTests
//
//  Created by MNN on 2019/01/16.
//  Copyright © 2018, Alibaba Group Holding Limited
//

#include "MNNTestSuite.h"
#include "TensorUtils.hpp"

using namespace MNN;

class TensorUtilsTest : public MNNTestCase {
public:
    virtual ~TensorUtilsTest() = default;
    virtual bool run() {
        // copy
        {
            Tensor src(3, Tensor::TENSORFLOW);
            src.setLength(0, 1);
            src.setLength(1, 2);
            src.setLength(2, 3);
            src.setStride(0, 4);
            src.setStride(1, 5);
            src.setStride(2, 6);

            Tensor dst1(4, Tensor::CAFFE);
            TensorUtils::copyShape(&src, &dst1);
            MNNTEST_ASSERT(dst1.dimensions() == 3);
            MNNTEST_ASSERT(dst1.length(0) == 1);
            MNNTEST_ASSERT(dst1.length(1) == 2);
            MNNTEST_ASSERT(dst1.length(2) == 3);
            MNNTEST_ASSERT(dst1.stride(0) == 4);
            MNNTEST_ASSERT(dst1.stride(1) == 5);
            MNNTEST_ASSERT(dst1.stride(2) == 6);
            MNNTEST_ASSERT(dst1.getDimensionType() == Tensor::CAFFE);

            Tensor dst2(4, Tensor::CAFFE);
            TensorUtils::copyShape(&src, &dst2, true);
            MNNTEST_ASSERT(dst2.dimensions() == 3);
            MNNTEST_ASSERT(dst2.length(0) == 1);
            MNNTEST_ASSERT(dst2.length(1) == 2);
            MNNTEST_ASSERT(dst2.length(2) == 3);
            MNNTEST_ASSERT(dst2.stride(0) == 4);
            MNNTEST_ASSERT(dst2.stride(1) == 5);
            MNNTEST_ASSERT(dst2.stride(2) == 6);
            MNNTEST_ASSERT(dst2.getDimensionType() == Tensor::TENSORFLOW);
        }

        // layout
        {
            Tensor tensor(3, Tensor::TENSORFLOW);
            tensor.setLength(0, 1);
            tensor.setLength(1, 2);
            tensor.setLength(2, 3);
            tensor.setStride(0, 4);
            tensor.setStride(1, 5);
            tensor.setStride(2, 6);
            TensorUtils::setLinearLayout(&tensor);
            MNNTEST_ASSERT(tensor.stride(0) == 2 * 3);
            MNNTEST_ASSERT(tensor.stride(1) == 3);
            MNNTEST_ASSERT(tensor.stride(2) == 1);

            Tensor reorder(3, Tensor::TENSORFLOW);
            reorder.setLength(0, 1);
            reorder.setLength(1, 2);
            reorder.setLength(2, 3);
            reorder.setStride(0, 4);
            reorder.setStride(1, 5);
            reorder.setStride(2, 6);
            reorder.buffer().dim[2].flags = Tensor::REORDER_4;
            TensorUtils::setLinearLayout(&reorder);
            MNNTEST_ASSERT(reorder.stride(0) == 2 * ((3 + 3) / 4 * 4));
            MNNTEST_ASSERT(reorder.stride(1) == (3 + 3) / 4 * 4);
            MNNTEST_ASSERT(reorder.stride(2) == 1);
            reorder.buffer().dim[1].flags = Tensor::REORDER_8;
            TensorUtils::setLinearLayout(&reorder);
            MNNTEST_ASSERT(reorder.stride(0) == ((2 + 7) / 8 * 8) * ((3 + 3) / 4 * 4));
            MNNTEST_ASSERT(reorder.stride(1) == (3 + 3) / 4 * 4);
            MNNTEST_ASSERT(reorder.stride(2) == 1);
        }
        return true;
    }
};
MNNTestSuiteRegister(TensorUtilsTest, "core/tensor_utils");
