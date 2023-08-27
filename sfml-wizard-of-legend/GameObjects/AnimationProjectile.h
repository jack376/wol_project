#pragma once
#include "Projectile.h"
#include "AnimationController.h"
#include "ObjectPool.h"

class Particle;

enum class Type
{
    None,
    Meteor,
};

class AnimationProjectile :
    public Projectile
{
protected:
    AnimationController animation;
    Type type;
    sf::Vector2f startPos;
    sf::Vector2f endPos;
    float collisionDuration = 1.f;
    float collisionTimer = 0.f;

public:
    ObjectPool<AnimationProjectile>* pool = nullptr;
    ObjectPool<Particle>* particlePool = nullptr;

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

    void MeteorFire(const sf::Vector2f start, const sf::Vector2f end, int damage);
    void SetType(Type type) { this->type = type; }

    void SetParticle(sf::Vector2f position, int count);
    void SetParticlePool(ObjectPool<Particle>* pool);
};

