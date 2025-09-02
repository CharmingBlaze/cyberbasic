#pragma once

#include "raylib.h"
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <random>

namespace bas {

// Forward declarations
struct Sprite;
struct SpriteAnimation;
struct ParticleSystem;

// Sprite management
int CreateSprite(int textureId, int x, int y, int width, int height);
void DrawSprite(int spriteId, int x, int y);
void MoveSprite(int spriteId, int dx, int dy);
void SetSpritePosition(int spriteId, int x, int y);
int GetSpriteX(int spriteId);
int GetSpriteY(int spriteId);
void SetSpriteSpeed(int spriteId, float speed);
float GetSpriteSpeed(int spriteId);

// Collision detection
bool CheckSpriteCollision(int sprite1Id, int sprite2Id);
bool CheckSpritePointCollision(int spriteId, int x, int y);
bool CheckSpriteRectCollision(int spriteId, int x, int y, int width, int height);

// Animation system
int CreateSpriteAnimation(int textureId, int frameWidth, int frameHeight, int totalFrames, int frameDelay);
void PlaySpriteAnimation(int animationId);
void PauseSpriteAnimation(int animationId);
void StopSpriteAnimation(int animationId);
void SetSpriteAnimationFrame(int animationId, int frame);
int GetSpriteAnimationFrame(int animationId);

// Particle system
int CreateParticleSystem(int x, int y, int count, int speed, int life, int r, int g, int b);
void UpdateParticleSystem(int particleSystemId);
void DrawParticleSystem(int particleSystemId);
void SetParticleSystemPosition(int particleSystemId, int x, int y);

// Enhanced sound system
void PlaySoundLoop(int soundId);
void StopSoundLoop(int soundId);
void SetSoundVolume(int soundId, float volume);
float GetSoundVolume(int soundId);
void SetSoundPitch(int soundId, float pitch);
float GetSoundPitch(int soundId);

// Enhanced text system
void DrawTextEx(const char* text, int x, int y, int fontSize, int spacing, Color color);
int MeasureText(const char* text, int fontSize);
void DrawTextCentered(const char* text, int x, int y, int fontSize, int spacing, Color color);

// Utility functions
int RandomInt(int min, int max);
float RandomFloat(float min, float max);
float Distance(int x1, int y1, int x2, int y2);
float Angle(int x1, int y1, int x2, int y2);
float Lerp(float a, float b, float t);
float Clamp(float value, float min, float max);

} // namespace bas
