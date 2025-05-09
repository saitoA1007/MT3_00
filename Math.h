#pragma once
#include"Matrix4x4.h"
#include"Vector3.h"
#include"Geometry.h"

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
// スカラー倍
Vector3 Multiply(const Vector3& v1, const float& a);
// Vec3とfloatの割り
Vector3 Divide(const Vector3& v1, const float& a);

// ベクトルの長さを求める
float Length(const Vector3& v);

// ベクトルの正規化
Vector3 Normalize(const Vector3& v);

// 内積
float Dot(const Vector3& v1, const Vector3& v2);
// 外積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// 最大値
Vector3 Max(Vector3 pos1, Vector3 pos2);
// 最小値
Vector3 Min(Vector3 pos1, Vector3 pos2);

// 反射ベクトル
Vector3 Reflect(const Vector3& input, const Vector3& normal);

// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);
// 最近接点
Vector3 ClosestPoint(const Vector3& point,const Segment& segment);

// 引数のベクトルに垂直なベクトルを求める
Vector3 Perpendicular(const Vector3& vector);

// 線形補間
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

// ベジェ曲線上の点を求める
Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t);

// Catmull-romスプライン上の点を求める
Vector3 CatmullRom(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

// 4x4行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
// 4x4行列の減算
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
// 4x4行列の積
Matrix4x4 Multiply(const Matrix4x4& matrix1, const Matrix4x4& matrix2);
// 4x4逆行列の計算
Matrix4x4 Inverse(const Matrix4x4& matrix);
// 4x4行列の転置行列
Matrix4x4 Transpose(const Matrix4x4& m);
// 4x4単位行列
Matrix4x4 MakeIdentity4x4();

// 4xx4のX軸の回転行列を作成
Matrix4x4 MakeRotateXMatrix(const float& theta);

// 4x4のY軸の回転行列を作成
Matrix4x4 MakeRotateYMatrix(const float& theta);

// 4x4のZ軸の回転行列を作成
Matrix4x4 MakeRotateZMatrix(const float& theta);

// 4x4の拡縮行列の作成
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 4x4の平行移動行列の作成
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// 4x4のSRTによるアフィン変換行列の作成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3 translate);

// (3+1)次元座標系をデカルト座標系に変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// 透視投影行列の作成
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

// 正射影行列の作成
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

// ビューポート行列の作成
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minD, float maxD);
