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

//MD3;
using float32 = float;
using vec_t = GLfloat;
using vec3_t = vec_t[3];

//MD0;
using md0_vertex = float32[11];
using md0_indexes = int32[3];

template <typename T>
using upointer = std::unique_ptr<T>;

template <typename T>
using dictionary = std::unordered_map<string, T>;

template <typename T>
using vector = std::vector<T>;