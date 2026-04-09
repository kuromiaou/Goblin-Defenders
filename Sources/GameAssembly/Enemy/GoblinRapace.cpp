#include "GoblinRapace.hpp"
#include <ImGui/imgui.h>
#include "GameAssembly/Gameplay/GoldPickup.hpp"
#include "GameAssembly/Utils/ActorUtils.hpp"

void GoblinRapace::Start()
{
    initForWave(1);
}

void GoblinRapace::Update(float deltaTime)
{
    tickEffects(deltaTime);
    if (isDead()) {
        handleDeath();
        return;
    }
}

void GoblinRapace::initForWave(int current_wave)
{
    wave          = current_wave;
    float scaling = 1.0f + (wave - 1) * 0.1f;
    hp_max        = static_cast<int>(35 * scaling);
    hp            = hp_max;
    stolen_gold   = 0;
    reached_nexus = false;
}

void GoblinRapace::takeDamage(int dmg, DamageType type)
{
    int res = (type == DamageType::PHYSIQUE)
        ? static_cast<int>(res_physique)
        : static_cast<int>(res_magique);
    res = applyResistanceEffects(res);
    float mult = 100.0f / (100.0f + static_cast<float>(res));
    hp = std::max(0, hp - static_cast<int>(dmg * mult));
}

void GoblinRapace::handleDeath()
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

int GoblinRapace::getGoldValue() const
{
    if (reached_nexus) return 0;
    return static_cast<int>((hp_max * 0.5f + stolen_gold) * 1.2f);
}

void GoblinRapace::Inspect()
{
    ImGui::DragInt("HP",          &hp,          1, 0, hp_max);
    ImGui::DragInt("HP Max",      &hp_max,      1, 0, 9999);
    ImGui::DragInt("Wave",        &wave,        1, 1, 999);
    ImGui::DragInt("Stolen Gold", &stolen_gold, 1, 0, 9999);
    ImGui::Checkbox("Reached Nexus", &reached_nexus);
}

void GoblinRapace::Serialize(nlohmann::json& out) const
{
    out["hp"]           = hp;
    out["hp_max"]       = hp_max;
    out["wave"]         = wave;
    out["stolen_gold"]  = stolen_gold;
    out["reached_nexus"]= reached_nexus;
}

void GoblinRapace::Deserialize(const nlohmann::json& in)
{
    if (in.contains("hp"))            hp            = in["hp"];
    if (in.contains("hp_max"))        hp_max        = in["hp_max"];
    if (in.contains("wave"))          wave          = in["wave"];
    if (in.contains("stolen_gold"))   stolen_gold   = in["stolen_gold"];
    if (in.contains("reached_nexus")) reached_nexus = in["reached_nexus"];
}
