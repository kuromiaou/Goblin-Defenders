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

private:
	Termina::Rigidbody rb;
	float m_MoveSpeed = 5.0f;
};