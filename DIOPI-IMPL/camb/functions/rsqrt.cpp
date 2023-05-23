/**
 * @file
 * @author DeepLink
 * @copyright  (c) 2023, DeepLink.
 */

#include <diopi/functions.h>

#include <vector>

#include "../cnnl_helper.hpp"
#include "../common/common.hpp"

namespace impl {
namespace camb {

static diopiError_t rsqrt(diopiContextHandle_t ctx, DiopiTensor& output, DiopiTensor input) {
    cnnlHandle_t handle = cnnlHandlePool.get(ctx);

    std::vector<DiopiTensor*> pTensors{&input};
    std::set<diopiDtype_t> supportedDtypes{diopi_dtype_float16, diopi_dtype_float32};

    DIOPI_CALL(autoCastTensorType(ctx, pTensors, supportedDtypes));

    DiopiTensor outputTmp = output;
    if (input.dtype() != output.dtype()) {
        outputTmp = requiresTensor(ctx, output.shape(), input.dtype());
    }

    CnnlTensorDesc desc(input, CNNL_LAYOUT_ARRAY);
    DIOPI_CALLCNNL(cnnlRsqrt_v2(handle, CNNL_COMPUTATION_HIGH_PRECISION, desc.get(), input.data(), desc.get(), outputTmp.data()));
    if (outputTmp.dtype() != output.dtype()) {
        DIOPI_CALL(dataTypeCast(ctx, output, outputTmp));
    }
    return diopiSuccess;
}

extern "C" diopiError_t diopiRsqrtInp(diopiContextHandle_t ctx, diopiTensorHandle_t input) {
    DiopiTensor inputTensor(input);
    DIOPI_CALL(rsqrt(ctx, inputTensor, inputTensor));
    return diopiSuccess;
}

extern "C" diopiError_t diopiRsqrt(diopiContextHandle_t ctx, diopiTensorHandle_t out, diopiConstTensorHandle_t input) {
    DiopiTensor inputTensor(input);
    DiopiTensor outputTensor(out);
    DIOPI_CALL(rsqrt(ctx, outputTensor, inputTensor));
    return diopiSuccess;
}

}  // namespace camb
}  // namespace impl
