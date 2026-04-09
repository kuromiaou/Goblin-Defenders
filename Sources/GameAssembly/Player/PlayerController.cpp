#include "PlayerController.hpp"
#include <algorithm>
#include <glm/geometric.hpp>
#include "GameAssembly/Enemy/EnemyQuery.hpp"
#include "GameAssembly/Player/Player.hpp"

void PlayerController::Update(float dt)
{
	Move();
	Attack(dt);
}

void PlayerController::Start()
{
    if (m_Owner && !m_Owner->HasComponent<Player>()) {
        m_Owner->AddComponent<Player>();
    }
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

void PlayerController::Attack(float dt)
{
    if (m_AttackCooldown > 0.0f)
        m_AttackCooldown = std::max(0.0f, m_AttackCooldown - dt);

    if (!Input::IsMouseButtonPressed(Termina::MouseButton::Left)) return;
    if (m_AttackCooldown > 0.0f) return;
    if (!m_Owner || !m_Transform || !m_Owner->HasComponent<Player>()) return;

    auto& player = m_Owner->GetComponent<Player>();
    const int damage = player.getCurrentATK();
    const float range = static_cast<float>(player.getATKRange());
    const glm::vec3 playerPos = m_Transform->GetPosition();

    auto enemies = GetAllLiveEnemies(m_Owner->GetParentWorld());
    Termina::Actor* target = nullptr;
    Enemy* targetEnemy = nullptr;
    float bestDistance = range + 0.001f;

    for (auto& [actor, enemy] : enemies)
    {
        if (!actor || !actor->HasComponent<Termina::Transform>()) continue;
        const float d = glm::distance(playerPos, actor->GetComponent<Termina::Transform>().GetPosition());
        if (d <= range && d < bestDistance) {
            bestDistance = d;
            target = actor;
            targetEnemy = enemy;
        }
    }

    if (target && targetEnemy)
        targetEnemy->takeDamage(damage, player.getCurrentDamageType());

    const float atkPerSec = player.getATKSPD();
    m_AttackCooldown = (atkPerSec > 0.0f) ? (1.0f / atkPerSec) : 1.0f;
}
