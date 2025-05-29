#pragma once
#include"Vector3.h"
#include"Matrix4x4.h"
#include"Math.h"

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

void QuaternionScreenPrintf(int x, int y, const Quaternion& quaternion, const char* label);