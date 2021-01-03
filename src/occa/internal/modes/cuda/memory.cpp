#include <occa/internal/modes/cuda/memory.hpp>
#include <occa/internal/modes/cuda/device.hpp>
#include <occa/internal/modes/cuda/utils.hpp>

namespace occa {
  namespace cuda {
    memory::memory(modeDevice_t *modeDevice_,
                   udim_t size_,
                   const occa::json &properties_) :
        occa::modeMemory_t(modeDevice_, size_, properties_),
        cuPtr(reinterpret_cast<CUdeviceptr&>(ptr)),
        mappedPtr(NULL),
        isUnified(false) {}

    memory::~memory() {
      if (isOrigin) {
        if (mappedPtr) {
          OCCA_CUDA_DESTRUCTOR_ERROR(
            "Device: mappedFree()",
            cuMemFreeHost(mappedPtr)
          );
        } else if (cuPtr) {
          cuMemFree(cuPtr);
        }
      }
      cuPtr = 0;
      mappedPtr = NULL;
      size = 0;
    }

    CUstream& memory::getCuStream() const {
      return ((device*) modeDevice)->getCuStream();
    }

    void* memory::getKernelArgPtr() const {
      return (void*) &cuPtr;
    }

    modeMemory_t* memory::addOffset(const dim_t offset) {
      memory *m = new memory(modeDevice,
                             size - offset,
                             properties);
      m->cuPtr = cuPtr + offset;
      if (mappedPtr) {
        m->mappedPtr = mappedPtr + offset;
      }
      m->isUnified = isUnified;
      return m;
    }

    void* memory::getPtr(const occa::json &props) {
      if (props.get("mapped", false)) {
        return mappedPtr;
      }
      return ptr;
    }

    void memory::copyFrom(const void *src,
                          const udim_t bytes,
                          const udim_t offset,
                          const occa::json &props) {
      const bool async = props.get("async", false);

      if (!async) {
        OCCA_CUDA_ERROR("Memory: Copy From",
                        cuMemcpyHtoD(cuPtr + offset,
                                     src,
                                     bytes));
      } else {
        OCCA_CUDA_ERROR("Memory: Async Copy From",
                        cuMemcpyHtoDAsync(cuPtr + offset,
                                          src,
                                          bytes,
                                          getCuStream()));
      }
    }

    void memory::copyFrom(const modeMemory_t *src,
                          const udim_t bytes,
                          const udim_t destOffset,
                          const udim_t srcOffset,
                          const occa::json &props) {
      const bool async = props.get("async", false);

      if (!async) {
        OCCA_CUDA_ERROR("Memory: Copy From",
                        cuMemcpyDtoD(cuPtr + destOffset,
                                     ((memory*) src)->cuPtr + srcOffset,
                                     bytes));
      } else {
        OCCA_CUDA_ERROR("Memory: Async Copy From",
                        cuMemcpyDtoDAsync(cuPtr + destOffset,
                                          ((memory*) src)->cuPtr + srcOffset,
                                          bytes,
                                          getCuStream()));
      }
    }

    void memory::copyTo(void *dest,
                        const udim_t bytes,
                        const udim_t offset,
                        const occa::json &props) const {
      const bool async = props.get("async", false);

      if (!async) {
        OCCA_CUDA_ERROR("Memory: Copy From",
                        cuMemcpyDtoH(dest,
                                     cuPtr + offset,
                                     bytes));
      } else {
        OCCA_CUDA_ERROR("Memory: Async Copy From",
                        cuMemcpyDtoHAsync(dest,
                                          cuPtr + offset,
                                          bytes,
                                          getCuStream()));
      }
    }

    void memory::detach() {
      cuPtr = 0;
      size = 0;
    }
  }
}
