#include "Goblin.hpp"
#include <ImGui/imgui.h>
#include "GameAssembly/Gameplay/GoldPickup.hpp"
#include "GameAssembly/Utils/ActorUtils.hpp"



void Goblin::Start()
{
    initForWave(1);
}

void Goblin::Update(float deltaTime)
{
    tickEffects(deltaTime);
    if (isDead()) {
        handleDeath();
        return;
    }
}

void Goblin::initForWave(int current_wave)
{
    wave = current_wave;
    float scaling = 1.0f + (wave - 1) * 0.1f;
    hp_max = static_cast<int>(static_cast<int>(HPTier::HP_LOW) * scaling);
    hp = hp_max;
    computeGoldValue();
}

void Goblin::takeDamage(int dmg, DamageType type)
{
    int res = (type == DamageType::PHYSIQUE)
        ? static_cast<int>(res_physique)
        : static_cast<int>(res_magique);
    res = applyResistanceEffects(res);
    float mult = 100.0f / (100.0f + static_cast<float>(res));
    hp = std::max(0, hp - static_cast<int>(dmg * mult));
}

void Goblin::handleDeath()
{
    if (death_handled || !m_Owner) return;
    death_handled = true;

    static const TerminaScript::Prefab goldPrefab("Assets/Prefabs/gold pile.trp");
    if (goldPrefab.IsValid() && m_Transform) {
        if (Termina::Actor* drop = Instantiate(goldPrefab)) {
            drop->GetComponent<Termina::Transform>().SetPosition(m_Transform->GetPosition());
            // NOTE: Le composant "Gold Pickup" doit être présent sur le prefab "gold pile.trp" (pas d'AddComponent runtime).
            if (drop->HasComponent<GoldPickup>())
                drop->GetComponent<GoldPickup>().SetGoldAmount(getGoldValue());
        }
    }

    DestroyActorHierarchy(m_Owner);
}

void Goblin::Inspect()
{
    ImGui::DragInt("HP", &hp, 1, 0, hp_max);
    ImGui::DragInt("HP Max", &hp_max, 1, 0, 9999);
    ImGui::DragInt("Wave", &wave, 1, 1, 999);
    ImGui::DragInt("Gold Value", &gold_value, 1, 0, 9999);
}

//Works with Hot Reload
void Goblin::Serialize(nlohmann::json& out) const
{
    out["hp"] = hp;
    out["hp_max"] = hp_max;
    out["wave"] = wave;
    out["gold_value"] = gold_value;
}

void Goblin::Deserialize(const nlohmann::json& in)
{
    if (in.contains("hp"))         hp = in["hp"];
    if (in.contains("hp_max"))     hp_max = in["hp_max"];
    if (in.contains("wave"))       wave = in["wave"];
    if (in.contains("gold_value")) gold_value = in["gold_value"];
}
