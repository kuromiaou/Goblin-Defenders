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
    glm::vec3 direction(0.0f);

    if (Input::IsKeyHeld(Termina::Key::W))     direction += glm::vec3(-1, 0, -1);
    if (Input::IsKeyHeld(Termina::Key::S))     direction += glm::vec3(1, 0, 1);
    if (Input::IsKeyHeld(Termina::Key::A))     direction += glm::vec3(-1, 0, 1);
    if (Input::IsKeyHeld(Termina::Key::D))     direction += glm::vec3(1, 0, -1);

    if (glm::length(direction) > 0.0f)
        direction = glm::normalize(direction);

    glm::vec3 vel = rb.GetLinearVelocity();
    vel.x = direction.x * m_MoveSpeed;
    vel.z = direction.z * m_MoveSpeed;
    rb.SetLinearVelocity(vel);
}