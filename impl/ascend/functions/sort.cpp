/**
 * @file
 * @author DeepLink
 * @copyright  (c) 2023, DeepLink.
 */

#include <diopi/functions.h>

#include "../common/acloprunner.hpp"

namespace impl {
namespace ascend {
extern "C" {
DIOPI_API diopiError_t diopiSort(diopiContextHandle_t ctx, diopiTensorHandle_t values, diopiTensorHandle_t indices, diopiConstTensorHandle_t input, int64_t dim,
                                 bool descending, const bool* stable) {
    AclOpRunner<1, 2>("Sort", ctx).addInput(input).setAttr("axis", dim).setAttr("descending", descending).addOutput(values).addOutput(indices).run();
    return diopiSuccess;
}
}

}  // namespace ascend
}  // namespace impl
