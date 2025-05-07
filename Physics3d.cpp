#include"Physics3d.h"
#include"Math.h"

void simulateSpringMovement(Spring& spring, Ball* ball, const float& deltaTime) {

	Vector3 diff = ball->pos - spring.anchor;
	float length = Length(diff);
	if (length != 0.0f) {
		Vector3 direction = Normalize(diff);
		Vector3 restPosition = spring.anchor + Multiply(direction, spring.naturalLength);
		Vector3 displacement = Multiply((ball->pos - restPosition), length);
		Vector3 restoringForce = Multiply(displacement, -spring.stiffness);
		// 減衰抵抗を計算する
		Vector3 dampingForce = Multiply(ball->velocity, - spring.dampingCoefficient);
		// 減衰抵抗も加味した物体にかかる力を決定する
		Vector3 force = restoringForce + dampingForce;
		ball->acceleration = Divide(force, ball->mass);
	}

	// ボールの座標と速度を更新
	ball->velocity += Multiply(ball->acceleration, deltaTime);
	ball->pos += Multiply(ball->velocity, deltaTime);
}
