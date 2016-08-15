#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include <memory>

struct bsc_buffer_t {

    bsc_buffer_t();

    template <typename type_t>
    void emit(const type_t in) {
        emit(&in, sizeof(type_t));
    }

    template <typename type_t>
    void patch(uint32_t offset, const type_t in) {
        patch(offset, &in, sizeof(type_t));
    }

    uint32_t pos() const {
        return offset_;
    }

    uint32_t size() const {
        return offset_;
    }

    void clear();

    const uint8_t * get() const {
        return alloc_.data_.get();
    }

protected:

    struct alloc_t {

        std::unique_ptr<uint8_t> data_;
        uint32_t size_;
        
        alloc_t();
        void release();
        void resize(uint32_t size);

    } alloc_;

    uint32_t offset_;

    void reserve(uint32_t size);

    void emit(const void * src, uint32_t size);
    void patch(uint32_t offset, const void * src, uint32_t size);
};

struct bsc_reader_t {

    bsc_reader_t(bsc_buffer_t &buf)
        : buffer_(buf)
        , offset_(0)
    {
    }

    template <typename type_t>
    void read(type_t & out) {
        read(&out, sizeof(type_t));
    }

    void seek(uint32_t offset);

protected:
    void read(void * dst, uint32_t size);

    bsc_buffer_t & buffer_;
    uint32_t offset_;
};