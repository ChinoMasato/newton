// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"
#include "CircleComponent.h"
#include "Asteroid.h"

Ship::Ship(Game* game)
	:Actor(game)
	,mLaserCooldown(0.0f)
	,mInvisibleTime(0.0f)
{
	// Create a sprite component
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// Create an input component and set keys/speed
	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);

	// Create a circle component (for collision)
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);

}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;

	if (mInvisibleTime > 0) {
		mInvisibleTime -= deltaTime;
		SetPosition(Vector2(1024 / 2, 768 / 2));
		SetRotation(0);
	}
	if (mInvisibleTime <= 0) {
		SetVisible(true);
	}
	// Do we intersect with an asteroid?
	for (auto ast : GetGame()->GetAsteroids())
	{
		if (Intersect(*mCircle, *(ast->GetCircle())))
		{
			// The first asteroid we intersect with,
			// set ourselves and the asteroid to dead
			SetVisible(false);
			mInvisibleTime = 1.0f;


			break;
		}
	}
}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		// Create a laser and set its position/rotation to mine
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// Reset laser cooldown (half second)
		mLaserCooldown = 0.5f;
	}
}
