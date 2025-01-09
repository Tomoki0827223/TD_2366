#pragma once
#include <KamataEngine.h>
#include <assert.h>
#include <cmath>
#include <stdio.h>
using namespace KamataEngine;

Matrix4x4 MatrixMultipry(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Matrix4x4 MakeRotateZMatrix(float angle);

Matrix4x4 CreateRotationY(float angle);

Vector3 Transform(const Vector3& vec, const Matrix4x4& mat);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Vector3 Normalize(const Vector3& v);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
// viewport
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

// 透視投影行列
Matrix4x4 MakePerspectiveMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

Matrix4x4 Inverse(const Matrix4x4& m);