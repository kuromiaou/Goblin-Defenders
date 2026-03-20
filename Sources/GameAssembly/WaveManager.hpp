//#pragma once
//#include <Termina/Scripting/API/ScriptingAPI.hpp>
//
//
//#include "Goblin.hpp"
//#include "Hobgoblin.hpp"
//#include "Magicien.hpp"
//#include "Shaman.hpp"
//#include "GoblinRapace.hpp"
//#include <vector>
//#include <memory>
//
//// ============================================================
//// WAVE MANAGER
//// Gère le spawn et le déroulement des vagues d'ennemis
//// Vagues fixes + endless au-delà
//// Scaling HP : +10% par vague
//// ============================================================
//
//enum class WaveState { WAITING, IN_PROGRESS, COMPLETED };
//
//struct WaveConfig {
//    int wave_number;
//    std::vector<std::pair<std::string, int>> spawns; // { type_ennemi, quantité }
//};
//
//class WaveManager : public TerminaScript::ScriptableComponent {
//private:
//    int        current_wave;
//    int        max_fixed_waves;  // ex : 10 vagues fixes, puis endless
//    WaveState  state;
//    bool       is_endless;
//
//    //std::vector<std::shared_ptr<Enemy>> active_enemies;
//
//public:
//    WaveManager(Termina::Actor* owner)
//        : TerminaScript::ScriptableComponent(owner),
//          current_wave(0),
//          max_fixed_waves(10),
//          state(WaveState::WAITING),
//          is_endless(false)
//    {}
//
//    // --- Getters ---
//    int       getCurrentWave()   const { return current_wave; }
//    WaveState getState()         const { return state; }
//    bool      isEndless()        const { return is_endless; }
//    bool      isInProgress()     const { return state == WaveState::IN_PROGRESS; }
//    bool      allEnemiesDead()   const {
//        //for (const auto& e : active_enemies)
//        //    if (e && e->isAlive()) return false;
//        //return true;
//    }
//
//    //const std::vector<std::shared_ptr<Enemy>>& getActiveEnemies() const {
//    //    return active_enemies;
//    //}
//
//    // --- Lancement de la vague (déclenché par le joueur) ---
//    void startWave() {
//        if (state == WaveState::IN_PROGRESS) return;
//        current_wave++;
//        state      = WaveState::IN_PROGRESS;
//        is_endless = (current_wave > max_fixed_waves);
//        //active_enemies.clear();
//        spawnWave(current_wave);
//    }
//
//    // --- Fin de vague ---
//    void update() {
//        if (state != WaveState::IN_PROGRESS) return;
//        if (allEnemiesDead()) {
//            state = WaveState::COMPLETED;
//        }
//    }
//
//    void readyForNext() {
//        if (state == WaveState::COMPLETED)
//            state = WaveState::WAITING;
//    }
//
//    // --- Spawn ---
//    void spawnWave(int wave) {
//        // Vague de base : composition croissante selon la vague
//        // Semaine 1 : 1 vague simple (à étendre)
//        spawnGoblinHorde(wave);
//
//        if (wave >= 2) spawnEnemy<Hobgoblin>(wave, 1);
//        if (wave >= 3) spawnEnemy<Magicien>(wave, 1);
//        if (wave >= 4) spawnEnemy<Shaman>(wave, 1);
//        if (wave >= 5) spawnEnemy<GoblinRapace>(wave, 1);
//    }
//
//    void spawnGoblinHorde(int wave) {
//        int count = Goblin::getHordeSize();
//        for (int i = 0; i < count; i++) {
//            spawnEnemy<Goblin>(wave, 1);
//        }
//    }
//
//private:
//    template<typename T>
//    void spawnEnemy(int wave, int count) {
//        for (int i = 0; i < count; i++) {
//            auto enemy = std::make_shared<T>();
//            enemy->init(wave);
//            //active_enemies.push_back(enemy);
//        }
//    }
//};
