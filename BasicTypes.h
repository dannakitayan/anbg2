#pragma once
#include <string>
#include <cstdint> 
#include <vector>
#include <unordered_map>
#include <memory>
#include <glad/glad.h>
#include <glm.hpp>

using ushort = std::uint16_t; //0~65535;
using short16 = std::int16_t;
using uint = uint32_t;
using byte = std::uint8_t; //0~255;
using string = std::string;
using int32 = int32_t;

template <typename T>
using upointer = std::unique_ptr<T>;

template <typename T>
using dictionary = std::unordered_map<string, T>;

template <typename T>
using vector = std::vector<T>;