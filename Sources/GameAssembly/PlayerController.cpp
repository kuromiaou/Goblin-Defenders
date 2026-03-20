#include "PlayerController.hpp"

void PlayerController::Update(float dt)
{
	Move();
}

void PlayerController::Start()
{
	rb = m_Owner->GetComponent<Termina::Rigidbody>();
}

void PlayerController::Move()
{
	if (Input::IsKeyPressed(Termina::Key::Space))
		rb.AddImpulse(glm::vec3(-1, 0, -1));
	if (Input::IsKeyPressed(Termina::Key::S))
		rb.AddImpulse(glm::vec3(1, 0, 1));
	if (Input::IsKeyPressed(Termina::Key::A))
		rb.AddImpulse(glm::vec3(-1, 0, 1));
	if (Input::IsKeyPressed(Termina::Key::D))
		rb.AddImpulse(glm::vec3(1, 0, -1));
}