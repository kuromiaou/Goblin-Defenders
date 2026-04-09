#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <Termina/Physics/Components/Rigidbody.hpp>

using namespace TerminaScript;

class PlayerController : public TerminaScript::ScriptableComponent
{
public:

	PlayerController() = default;
	PlayerController(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void Update(float dt)override;
	void Start()override;

	void Move();
	void Attack(float dt);

private:
	Termina::Rigidbody rb;
	float m_MoveSpeed = 5.0f;
	float m_AttackCooldown = 0.0f;
};
