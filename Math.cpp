#include"Math.h"
#include<cmath>
#include<cassert>
#include <algorithm> 

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {

	Quaternion result;
	result.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
	result.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
	result.y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
	result.z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;
	return result;
}

Quaternion IdentityQuaternion() {
	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

Quaternion Conjugate(const Quaternion& quaternion) {
	return { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
}

float Norm(const Quaternion& quaternion) {
	return std::sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);
}

Quaternion Normalize(const Quaternion& quaternion) {
	float norm = Norm(quaternion);
	// 0除算を避けるため単位Quaternionを返す
	if (norm == 0.0f) {
		return IdentityQuaternion();
	}
	return { quaternion.x / norm, quaternion.y / norm, quaternion.z / norm, quaternion.w / norm};
}

Quaternion Inverse(const Quaternion& quaternion) {
	float norm = Norm(quaternion);
	// 0除算を避けるため単位Quaternionを返す
	if (norm == 0.0f) {
		return IdentityQuaternion();
	}
	Quaternion conjugate = Conjugate(quaternion);
	float invNorm = 1.0f / (norm * norm);
	return { conjugate.x * invNorm, conjugate.y * invNorm,conjugate.z * invNorm,conjugate.w * invNorm };
}

Vector3 Add(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3 Multiply(const Vector3& v1, const float& a) {
	return Vector3(v1.x * a, v1.y * a, v1.z * a);
}

Vector3 Divide(const Vector3& v1, const float& a) {
	return Vector3(v1.x / a, v1.y / a, v1.z / a);
}

float Length(const Vector3& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Normalize(const Vector3& v) {
	float length = Length(v);
	if (length == 0.0f) {
		return Vector3(0.0f, 0.0f, 0.0f);
	} else {
		return Vector3(v.x / length, v.y / length, v.z / length);
	}
}

float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

Vector3 Max(Vector3 pos1, Vector3 pos2) {
	return Vector3(std::max(pos1.x, pos2.x), std::max(pos1.y, pos2.y), std::max(pos1.z, pos2.z));
}

Vector3 Min(Vector3 pos1, Vector3 pos2) {
	return Vector3(std::min(pos1.x, pos2.x), std::min(pos1.y, pos2.y), std::min(pos1.z, pos2.z));
}

Vector3 Reflect(const Vector3& input, const Vector3& normal) {
	return Subtract(input, Multiply(normal, 2.0f * Dot(input, normal)));
}

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	float dot = Dot(v1, v2);
	float lengthSquared = Dot(v2, v2);
	if (lengthSquared == 0.0f) {
		return Vector3(0.0f, 0.0f, 0.0f);
	} else {
		return Multiply(v2, dot / lengthSquared);
	}
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 segmentVector = segment.diff;
	Vector3 pointToSegmentStart = Subtract(point, segment.origin);
	float t = Dot(pointToSegmentStart, segmentVector) / Dot(segmentVector, segmentVector);
	t = std::max(0.0f, std::min(1.0f, t));
	return Add(segment.origin, Multiply(segmentVector, t));
}

Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y,vector.x,0.0f };
	}
	return { 0.0f,-vector.z,vector.y };
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return Vector3{
		v1.x + (v2.x - v1.x) * t,
		v1.y + (v2.y - v1.y) * t,
		v1.z + (v2.z - v1.z) * t
	};
}

Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t) {
	Vector3 p0p1 = Lerp(p0, p1, t);
	Vector3 p1p2 = Lerp(p1, p2, t);
	Vector3 p = Lerp(p0p1, p1p2, t);
	return p;
}

Vector3 CatmullRom(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
	Vector3 result;
	result.x = 1.0f / 2.0f * (
		(-p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x) * t * t * t +
		(2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * t * t + (-p0.x + p2.x) * t + 2.0f * p1.x);

	result.y = 1.0f / 2.0f * (
		(-p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y) * t * t * t +
		(2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * t * t + (-p0.y + p2.y) * t + 2.0f * p1.y);

	result.z = 1.0f / 2.0f * (
		(-p0.z + 3.0f * p1.z - 3.0f * p2.z + p3.z) * t * t * t +
		(2.0f * p0.z - 5.0f * p1.z + 4.0f * p2.z - p3.z) * t * t + (-p0.z + p2.z) * t + 2.0f * p1.z);

	return result;
}

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 Multiply(const Matrix4x4& matrix1, const Matrix4x4& matrix2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += matrix1.m[i][k] * matrix2.m[k][j];
			}
		}
	}
	return result;
}

Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}

Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 identity = {};
	for (int i = 0; i < 4; ++i) {
		// 対角成分を1に設定
		identity.m[i][i] = 1.0f;
	}
	return identity;
}

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to) {

	// 角
	float cos = Dot(from,to);
	float sin = Length(Cross(from, to));
	float t = 1.0f - cos;

	// 軸
	Vector3 axis = Normalize(Cross(from, to));

	// 真逆の場合
	if (cos <= -0.9999f) {

		if (from.x != 0.0f || from.y != 0.0f) {
			axis = Normalize(Vector3( from.y,-from.x,0.0f));
		} else if (from.x != 0.0f || from.z != 0.0f) {
			axis = Normalize(Vector3( from.z,0.0f,-from.x ));
		}
	}

	Matrix4x4 result = {
		axis.x * axis.x * t + cos, axis.x * axis.y * t + axis.z * sin, axis.x * axis.z * t - axis.y * sin, 0.0f,
		axis.x * axis.y * t - axis.z * sin, axis.y * axis.y * t + cos, axis.y * axis.z * t + axis.x * sin, 0.0f,
		axis.x * axis.z * t + axis.y * sin, axis.y * axis.z * t - axis.x * sin, axis.z * axis.z * t + cos, 0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};
	return result;
}

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {

	float cos = std::cosf(angle);
	float sin = std::sinf(angle);
	float t = 1.0f - cos;

	Matrix4x4 result = {
		axis.x * axis.x * t + cos, axis.x * axis.y * t + axis.z * sin, axis.x * axis.z * t - axis.y * sin, 0.0f,
		axis.x * axis.y * t - axis.z * sin, axis.y * axis.y * t + cos, axis.y * axis.z * t + axis.x * sin, 0.0f,
		axis.x * axis.z * t + axis.y * sin, axis.y * axis.z * t - axis.x * sin, axis.z * axis.z * t + cos, 0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};
	return result;
}

Matrix4x4 MakeRotateXMatrix(const float& theta) {
	Matrix4x4 result = {
		1, 0, 0, 0,
		0, std::cosf(theta), std::sinf(theta), 0,
		0, -std::sinf(theta), std::cosf(theta), 0,
		0, 0, 0, 1
	};
	return result;
}

Matrix4x4 MakeRotateYMatrix(const float& theta) {
	Matrix4x4 result = {
		std::cosf(theta), 0, -std::sinf(theta), 0,
		0, 1, 0, 0,
		std::sinf(theta), 0, std::cosf(theta), 0,
		0, 0, 0, 1
	};
	return result;
}

Matrix4x4 MakeRotateZMatrix(const float& theta) {
	Matrix4x4 result = {
		std::cosf(theta), std::sinf(theta), 0, 0,
		-std::sinf(theta), std::cosf(theta), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = {
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1
	};
	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translate.x, translate.y, translate.z, 1
	};
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3 translate) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));
	Matrix4x4 transformMatrix = Multiply(scaleMatrix, rotateMatrix);
	Matrix4x4 result = transformMatrix;
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;
	return result;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];
	// 同次座標に変換するために 4D ベクトルを使う
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Matrix4x4 Inverse(const Matrix4x4& matrix) {
	Matrix4x4 result;
	float det = matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] +
		matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] +
		matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] +
		matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2] +
		matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] +
		matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] +
		matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] +
		matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] +
		matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] +
		matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1] +
		matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] +
		matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] -
		matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2] -
		matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] -
		matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] -
		matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] -
		matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] -
		matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] -
		matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1] -
		matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] -
		matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] -
		matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] -
		matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] -
		matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1];
	result.m[0][0] = (matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] + matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] + matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] - matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2] - matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1]) / det;
	result.m[0][1] = (matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][2] + matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][3] + matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][1] - matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][3] - matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][1] - matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][2]) / det;
	result.m[0][2] = (matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][3] + matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][1] + matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][2] - matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][2] - matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][3] - matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][1]) / det;
	result.m[0][3] = (matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] + matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] + matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] - matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] - matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2]) / det;
	result.m[1][0] = (matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2] + matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] + matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] - matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] - matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2]) / det;
	result.m[1][1] = (matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][3] + matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][0] + matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][2] - matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][2] - matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][0]) / det;
	result.m[1][2] = (matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][2] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3] + matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][0] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][3] - matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][2]) / det;
	result.m[1][3] = (matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] + matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] + matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] - matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] - matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0]) / det;
	result.m[2][0] = (matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] + matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] + matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] - matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1] - matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0]) / det;
	result.m[2][1] = (matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][1] + matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][3] + matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][1]) / det;
	result.m[2][2] = (matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][3] + matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][0] + matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][1] - matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][1] - matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][3] - matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][0]) / det;
	result.m[2][3] = (matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] + matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] + matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] - matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] - matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] - matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1]) / det;
	result.m[3][0] = (matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1] + matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] + matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] - matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] - matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1]) / det;
	result.m[3][1] = (matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][2] + matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][0] + matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][1] - matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][1] - matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][2] - matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][0]) / det;
	result.m[3][2] = (matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][1] + matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][2] + matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][0] - matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][2] - matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][0] - matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][1]) / det;
	result.m[3][3] = (matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] + matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0]) / det;
	return result;
}


Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	float h = 1 / std::tanf(fovY / 2);
	float w = h / aspectRatio;
	Matrix4x4 result = {
		w, 0, 0, 0,
		0, h, 0, 0,
		0, 0, farClip / (farClip - nearClip), 1,
		0, 0, -nearClip * farClip / (farClip - nearClip), 0
	};
	return result;
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result = {
		2.0f / (right - left), 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (nearClip - farClip), 0.0f,
		(left + right) / (left - right), (top + bottom) / (bottom - top), nearClip / (nearClip - farClip), 1.0f
	};
	return result;
}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minD, float maxD) {
	Matrix4x4 result = {
		width / 2, 0, 0, 0,
		0, -height / 2, 0, 0,
		0, 0, maxD - minD, 0,
		left + width / 2, top + height / 2, minD, 1
	};
	return result;
}
