#include "stdafx.h"
#include "SpriteEffect.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"

SpriteEffect::SpriteEffect(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

void SpriteEffect::Init()
{
	SpriteGo::Init();
	// Reset() Init() 생각
	effectAnim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Effects/AttackEffect.csv"));
	effectAnim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Effects/HitEffect.csv"));
	effectAnim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Effects/DieEffect.csv"));

	effectAnim.SetTarget(&sprite);
	SetOrigin(Origins::MC);
}

void SpriteEffect::Reset()
{
	SpriteGo::Reset();
	timer = 0.f;
	sprite.setColor({ 255, 255, 255, 255 });
}

void SpriteEffect::Update(float dt)
{
	FadeEffectNRemove(dt);
}

void SpriteEffect::FadeEffectNRemove(float dt)
{
	timer += dt;
	// AttackEffect 몬스터 맞는 이펙트
	// HitEffect 플레이어 맞는 이펙트
	if (!isEffect)
	{
		effectAnim.Play(animId);
	}
	isEffect = true;
	effectAnim.Update(dt);
	Utils::SetOrigin(sprite, Origins::MC);

	if (timer > duration)
	{
		timer = 0.f;
		// 삭제
		if (pool != nullptr)
		{
			SCENE_MGR.GetCurrScene()->RemoveGo(this);
			pool->Return(this);
		}
		else
		{
			SetActive(false);
		}
	}
}
