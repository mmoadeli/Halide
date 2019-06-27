#ifndef HALIDE_RUNTIME_BUFFER_WRAPPERS
#define HALIDE_RUNTIME_BUFFER_WRAPPERS
#endif

#include "HalideRuntime.h"
#include "HalideBuffer.h"

#include <math.h>
#include <stdio.h>

#include "constinput.h"

using namespace Halide::Runtime;

const int kSize = 32;

void verify(int result, const Buffer<int32_t> &img) {
    if (result != 0) {
        fprintf(stderr, "Result: %d\n", result);
        exit(-1);
    }
    for (int i = 0; i < kSize; i++) {
        for (int j = 0; j < kSize; j++) {
            for (int c = 0; c < 3; c++) {
                int i1 = i + j + c;
                int i2 = i + j + c + 1;
                int expected = i1 + i2;
                if (img(i, j, c) != expected) {
                    fprintf(stderr, "img[%d, %d, %d] = %d (expected %d)\n", i, j, c, img(i, j, c), expected);
                    exit(-1);
                }
            }
        }
    }
}

int main(int argc, char **argv) {
    int result;

    Buffer<int32_t> input1(kSize, kSize, 3);
    input1.for_each_element([&](int x, int y, int c) { input1(x, y, c) = x + y + c; });

    Buffer<int32_t> input2(kSize, kSize, 3);
    input2.for_each_element([&](int x, int y, int c) { input2(x, y, c) = x + y + c + 1; });

    Buffer<int32_t> output(kSize, kSize, 3);

    // Test calls into the wrappers that accept mutable-ref for buffers
    {
        result = constinput(input1, input2, 0, output);
        verify(result, output);
    }

    // Same, but with refs
    {
        auto &i1_ref = input1;
        auto &i2_ref = input2;
        auto &o1_ref = output;
        result = constinput(i1_ref, i2_ref, 0, o1_ref);
        verify(result, output);
    }

    // Test calls into the wrappers that accept mutable-ref for buffers,
    // with Buffer<const T> for inputs
    {
        Buffer<const int32_t> i1 = input1;
        Buffer<const int32_t> i2 = input2;
        result = constinput(i1, i2, 0, output);
        verify(result, output);
    }

    // Same, but with refs
    {
        Buffer<const int32_t> i1 = input1;
        Buffer<const int32_t> i2 = input2;
        auto &i1_ref = i1;
        auto &i2_ref = i2;
        auto &o1_ref = output;
        result = constinput(i1_ref, i2_ref, 0, o1_ref);
        verify(result, output);
    }

    // This should fail to compile (output cannot be const)
    // {
    //     Buffer<const int32_t> o1 = output;
    //     result = constinput(input1, input2, 0, o1);
    //     verify(result, output);
    // }

    // Same, but with refs
    // {
    //     Buffer<const int32_t> o1 = output;
    //     auto &i1_ref = input1;
    //     auto &i2_ref = input2;
    //     auto &o1_ref = o1;
    //     result = constinput(i1_ref, i2_ref, 0, o1_ref);
    //     verify(result, output);
    // }

    printf("Success!\n");
    return 0;
}
