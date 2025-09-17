#pragma once
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <memory>

namespace mystl {

template <typename T>
class vector {
public:
  // 类型别名定义
  using value_type = T;           // 元素类型
  using size_type = size_t;       // 大小类型
  using reference = T&;           // 引用类型
  using const_reference = const T&; // 常量引用类型
  using iterator = T*;            // 迭代器类型
  using const_iterator = const T*; // 常量迭代器类型

  // 默认构造函数
  vector() : data_(nullptr), size_(0), capacity_(0) {}

  // 填充构造函数
  vector(size_type n, const T& value = T()) {
    allocate_and_fill(n, value);
  }

  // 拷贝构造函数
  vector(const vector& other) {
    allocate_and_copy(other.data_, other.size_);
  }

  // 移动构造函数
  vector(vector&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  // 列表初始化构造函数
  vector(std::initializer_list<T> ilist) {
    allocate_and_copy(ilist.begin(), ilist.size());
  }

  // 析构函数
  ~vector() {
    clear();
    deallocate();
  }

  // 拷贝赋值运算符
  vector& operator=(const vector& other) {
    if (this != &other) {
      clear();
      deallocate();
      allocate_and_copy(other.data_, other.size_);
    }
    return *this;
  }

  // 移动赋值运算符
  vector& operator=(vector&& other) noexcept {
    if (this != &other) {
      clear();
      deallocate();
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

  // 下标运算符
  reference operator[](size_type pos) { return data_[pos]; }
  const_reference operator[](size_type pos) const { return data_[pos]; }

  // 安全的下标访问
  reference at(size_type pos) {
    if (pos >= size_) throw std::out_of_range("vector::at");
    return data_[pos];
  }
  const_reference at(size_type pos) const {
    if (pos >= size_) throw std::out_of_range("vector::at");
    return data_[pos];
  }

  // 访问第一个和最后一个元素
  reference front() { return data_[0]; }
  const_reference front() const { return data_[0]; }
  reference back() { return data_[size_ - 1]; }
  const_reference back() const { return data_[size_ - 1]; }

  // 迭代器
  iterator begin() { return data_; }
  const_iterator begin() const { return data_; }
  iterator end() { return data_ + size_; }
  const_iterator end() const { return data_ + size_; }

  // 检查是否为空
  bool empty() const { return size_ == 0; }
  // 获取当前大小
  size_type size() const { return size_; }
  // 获取容量
  size_type capacity() const { return capacity_; }

  // 预分配内存
  void reserve(size_type new_cap) {
    if (new_cap > capacity_) {
      reallocate(new_cap);
    }
  }

  // 调整大小
  void resize(size_type count, const T& value = T()) {
    if (count < size_) {
      for (size_type i = count; i < size_; ++i)
        data_[i].~T();
    } else if (count > size_) {
      reserve(count);
      for (size_type i = size_; i < count; ++i)
        new (data_ + i) T(value);
    }
    size_ = count;
  }

  // 添加元素到末尾
  void push_back(const T& value) {
    if (size_ == capacity_) reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    new (data_ + size_) T(value);
    ++size_;
  }

  // 添加元素到末尾（移动语义）
  void push_back(T&& value) {
    if (size_ == capacity_) reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    new (data_ + size_) T(std::move(value));
    ++size_;
  }

  // 移除最后一个元素
  void pop_back() {
    if (size_ > 0) {
      data_[size_ - 1].~T();
      --size_;
    }
  }

  // 清空所有元素
  void clear() {
    for (size_type i = 0; i < size_; ++i)
      data_[i].~T();
    size_ = 0;
  }

private:
  T* data_;            // 指向动态数组的指针
  size_type size_;     // 当前元素数量
  size_type capacity_; // 当前分配的内存容量

  // 分配并填充内存
  void allocate_and_fill(size_type n, const T& value) {
    data_ = static_cast<T*>(::operator new(n * sizeof(T)));
    for (size_type i = 0; i < n; ++i)
      new (data_ + i) T(value);
    size_ = capacity_ = n;
  }

  // 分配并拷贝内存
  template <typename InputIt>
  void allocate_and_copy(InputIt first, size_type n) {
    data_ = static_cast<T*>(::operator new(n * sizeof(T)));
    size_type i = 0;
    try {
      for (; i < n; ++i)
        new (data_ + i) T(*(first + i));
    } catch (...) {
      for (size_type j = 0; j < i; ++j)
        data_[j].~T();
      ::operator delete(data_);
      throw;
    }
    size_ = capacity_ = n;
  }

  // 释放内存
  void deallocate() {
    if (data_) ::operator delete(data_);
    data_ = nullptr;
    capacity_ = 0;
  }

  // 重新分配内存
  void reallocate(size_type new_cap) {
    T* new_data = static_cast<T*>(::operator new(new_cap * sizeof(T)));
    for (size_type i = 0; i < size_; ++i)
      new (new_data + i) T(std::move(data_[i]));
    for (size_type i = 0; i < size_; ++i)
      data_[i].~T();
    ::operator delete(data_);
    data_ = new_data;
    capacity_ = new_cap;
  }
};

} // namespace mystl