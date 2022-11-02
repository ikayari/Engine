#include "stdafx.h"
#include "Slow.h"

bool Slow::Start()
{
    return true;
}
void Slow::Update()
{
    if (m_Endtimer > 0.0f)
    {
        m_timer += g_gameTime->GetFrameDeltaTime();
        if (m_timer >= m_Endtimer)
        {
            EndSlowMotion();
        }
    }
}