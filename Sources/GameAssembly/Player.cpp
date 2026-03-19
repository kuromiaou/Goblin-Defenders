#include "Player.hpp"
#include <ImGui/imgui.h>

void Player::Start()
{
}

void Player::Update(float deltaTime)
{
}

void Player::takeDamage(int dmg, DamageType type)
{
    float mult = 100.0f / (100.0f + static_cast<float>(
        type == DamageType::PHYSIQUE
            ? static_cast<int>(res_physique)
            : static_cast<int>(res_magique)
    ));
    hp = std::max(0, hp - static_cast<int>(dmg * mult));
}

int Player::getCurrentATK() const
{
    return current_damage_type == DamageType::PHYSIQUE ? atk_physique : atk_magique;
}

void Player::switchToPhysique()
{
    current_damage_type = DamageType::PHYSIQUE;
    atk_range           = ATKRangeTier::MELEE;
    atk_spd             = ATKSPDTier::ATKSPD_LOW;
}

void Player::switchToMagique()
{
    current_damage_type = DamageType::MAGIQUE;
    atk_range           = ATKRangeTier::RANGE;
    atk_spd             = ATKSPDTier::ATKSPD_HIGH;
}

bool Player::spendGold(int amount)
{
    if (gold < amount) return false;
    gold -= amount;
    return true;
}

bool Player::spendRSC(int cost)
{
    if (rsc < cost) return false;
    rsc -= cost;
    return true;
}

void Player::Inspect()
{
    ImGui::DragInt("HP",           &hp,           1, 0, hp_max);
    ImGui::DragInt("HP Max",       &hp_max,       1, 0, 9999);
    ImGui::DragInt("ATK Physique", &atk_physique, 1, 0, 9999);
    ImGui::DragInt("ATK Magique",  &atk_magique,  1, 0, 9999);
    ImGui::DragInt("RSC",          &rsc,          1, 0, 9999);
    ImGui::DragInt("Gold",         &gold,         1, 0, 9999);
}

void Player::Serialize(nlohmann::json& out) const
{
    out["hp"]            = hp;
    out["hp_max"]        = hp_max;
    out["atk_physique"]  = atk_physique;
    out["atk_magique"]   = atk_magique;
    out["rsc"]           = rsc;
    out["gold"]          = gold;
}

void Player::Deserialize(const nlohmann::json& in)
{
    if (in.contains("hp"))           hp           = in["hp"];
    if (in.contains("hp_max"))       hp_max       = in["hp_max"];
    if (in.contains("atk_physique")) atk_physique = in["atk_physique"];
    if (in.contains("atk_magique"))  atk_magique  = in["atk_magique"];
    if (in.contains("rsc"))          rsc          = in["rsc"];
    if (in.contains("gold"))         gold         = in["gold"];
}
