#ifndef   SSL_PATCHES_HEADER
#define   SSL_PATCHES_HEADER

namespace patches {

constexpr unsigned char original_bytes[6] {
    0x29, 0x1D, 0x00, 0x3C, 0x3F, 0x00 // sub dword [health_ptr], ebx
};

constexpr unsigned char hack_bytes[6] {
    0x90, // nop
    0x90, // nop
    0x90, // nop
    0x90, // nop
    0x90, // nop
    0x90
};

} // namespace patches

#endif // SSL_PATCHES_HEADER
