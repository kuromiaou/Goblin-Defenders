#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>

class Checkpoint : public TerminaScript::ScriptableComponent
{
public:

	Checkpoint() = default;
	Checkpoint(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {};

	void Start() override;
	void Update(float Deltatime) override;

	void Inspect() override;

	int getOrder();

private:
	int m_Order = 0;
	int m_CheckPointMax = 999;
};