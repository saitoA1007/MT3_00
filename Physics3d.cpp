#include"Physics3d.h"
#include<cmath>
#include"Math.h"

void simulateSpringMovement(Spring& spring, Ball& ball, const float& deltaTime) {

	Vector3 diff = ball.pos - spring.anchor;
	float length = Length(diff);
	if (length != 0.0f) {
		Vector3 direction = Normalize(diff);
		Vector3 restPosition = spring.anchor + Multiply(direction, spring.naturalLength);
		Vector3 displacement = Multiply((ball.pos - restPosition), length);
		Vector3 restoringForce = Multiply(displacement, -spring.stiffness);
		// 減衰抵抗を計算する
		Vector3 dampingForce = Multiply(ball.velocity, - spring.dampingCoefficient);
		// 減衰抵抗も加味した物体にかかる力を決定する
		Vector3 force = restoringForce + dampingForce;
		ball.acceleration = Divide(force, ball.mass);
	}

	// ボールの座標と速度を更新
	ball.velocity += Multiply(ball.acceleration, deltaTime);
	ball.pos += Multiply(ball.velocity, deltaTime);
}

void pendulumMotion(Pendulum& pendulum, Vector3& postion, const float& deltaTime) {
	// 各加速度
	pendulum.angularAcceleration = -(9.8f / pendulum.length) * std::sinf(pendulum.angle);
	// 角速度
	pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
	// 角度
	pendulum.angle += pendulum.angularVelocity * deltaTime;

	// 振り子の先端の位置。
	postion.x = pendulum.anchor.x + std::sinf(pendulum.angle) * pendulum.length;
	postion.y = pendulum.anchor.y - std::cosf(pendulum.angle) * pendulum.length;
	postion.z = pendulum.anchor.z;
}

void ConicalPendulumMotion(ConicalPendulum& conicalPendulum, Vector3& position, const float& deltaTime) {

	// 角度(ω)を求める処理
	conicalPendulum.angularVelocity = std::sqrtf(9.8f / (conicalPendulum.length * std::cosf(conicalPendulum.halfApexAngle)));
	conicalPendulum.angle += conicalPendulum.angularVelocity * deltaTime;

	// 回る軌跡の半径と高さを求める
	float radius = std::sinf(conicalPendulum.halfApexAngle) * conicalPendulum.length;
	float height = std::cosf(conicalPendulum.halfApexAngle) * conicalPendulum.length;

	// 先端の位置を求める
	position.x = conicalPendulum.anchor.x + std::cosf(conicalPendulum.angle) * radius;
	position.y = conicalPendulum.anchor.y - height;
	position.z = conicalPendulum.anchor.z - std::sinf(conicalPendulum.angle) * radius;
}