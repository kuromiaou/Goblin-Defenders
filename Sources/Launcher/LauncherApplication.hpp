#pragma once

#include <Termina/Core/Application.hpp>

class LauncherApplication : public Termina::Application
{
public:
    LauncherApplication();
    ~LauncherApplication() = default;

    void OnPreUpdate(float dt) override;
};
