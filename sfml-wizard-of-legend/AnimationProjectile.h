#pragma once
#include "Projectile.h"
#include "AnimationController.h"
class AnimationProjectile :
    public Projectile
{
protected:
    AnimationController animation;

public:
    AnimationProjectile(const std::string& textureId = "", const std::string& n = "");
    virtual ~AnimationProjectile() override;

    virtual void Init() override;
    virtual void Release() override;
    virtual void Reset() override;
    virtual void Update(float dt) override;
    virtual void Draw(sf::RenderWindow& window) override;

    void AddClip(const std::string path);
    void Play(const std::string name);
    void PlayQueue(const std::string name);
};

