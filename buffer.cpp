#include <cassert>

#include "builder.h"
#include "opcodes.h"

bsc_buffer_t::bsc_buffer_t()
    : offset_(0)
    , alloc_()
{
}

void bsc_buffer_t::reserve(uint32_t size) {

    const uint32_t new_size = (offset_+size>alloc_.size_*2) 
                              ? offset_+size
                              : alloc_.size_*2;

    if (offset_+size>alloc_.size_) {
        alloc_.resize(new_size);
    }
}

void bsc_buffer_t::emit(const void * src,
                        uint32_t size) {

    reserve(size);
    assert((offset_+size)<=alloc_.size_);
    memcpy(alloc_.data_.get()+offset_, src, size);
    offset_ += size;
}

void bsc_buffer_t::patch(uint32_t offset,
                         const void * src,
                         uint32_t size) {

    assert((offset+size)<=offset_);
    memcpy(alloc_.data_.get()+offset, src, size);
}

void bsc_buffer_t::clear() {
    offset_ = 0;
    alloc_.release();
}

bsc_buffer_t::alloc_t::alloc_t()
    : size_(0)
    , data_()
{
}

void bsc_buffer_t::alloc_t::release() {
    data_.reset();
    size_ = 0;
}

void bsc_buffer_t::alloc_t::resize(uint32_t size) {
    // temp new buffer
    std::unique_ptr<uint8_t> ptr(new uint8_t[size]);
    // if we have old data
    if (data_.get()) {
        // copy over old data
        int copy_size = (size<size_) ? size : size_;
        memcpy(ptr.get(), data_.get(), copy_size);
    }
    // replace the old data
    size_ = size;
    data_.reset(ptr.release());
}

void bsc_reader_t::seek(uint32_t offset) {
    assert(offset<buffer_.size());
    offset_ = offset;
}

void bsc_reader_t::read(void * dst, uint32_t size) {
    assert(offset_+size<=buffer_.size());
    memcpy(dst, buffer_.get()+offset_, size);
    offset_ += size;
}
