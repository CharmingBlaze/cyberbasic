#include "bas/custom_cyberbasic.hpp"
#include <algorithm>
#include <chrono>
#include <cstring>

namespace bas {

// Global storage for sprites, animations, and particle systems
static std::map<int, Sprite> sprites;
static std::map<int, SpriteAnimation> animations;
static std::map<int, ParticleSystem> particleSystems;
static std::map<int, bool> soundLoops;
static std::map<int, float> soundVolumes;
static std::map<int, float> soundPitches;
static int nextSpriteId = 1;
static int nextAnimationId = 1;
static int nextParticleSystemId = 1;

// Sprite structure
struct Sprite {
    int textureId;
    int x, y;
    int width, height;
    float speed;
    
    Sprite() : textureId(0), x(0), y(0), width(0), height(0), speed(1.0f) {}
    Sprite(int tid, int px, int py, int w, int h) 
        : textureId(tid), x(px), y(py), width(w), height(h), speed(1.0f) {}
};

// Sprite animation structure
struct SpriteAnimation {
    int textureId;
    int frameWidth, frameHeight;
    int totalFrames;
    int frameDelay;
    int currentFrame;
    int frameCounter;
    bool playing;
    bool paused;
    
    SpriteAnimation() : textureId(0), frameWidth(0), frameHeight(0), totalFrames(0), frameDelay(0),
                       currentFrame(0), frameCounter(0), playing(false), paused(false) {}
    SpriteAnimation(int tid, int fw, int fh, int tf, int fd)
        : textureId(tid), frameWidth(fw), frameHeight(fh), totalFrames(tf), frameDelay(fd),
          currentFrame(0), frameCounter(0), playing(false), paused(false) {}
};

// Particle structure
struct Particle {
    float x, y;
    float vx, vy;
    int life;
    Color color;
    
    Particle(float px, float py, float vvx, float vvy, int l, Color c)
        : x(px), y(py), vx(vvx), vy(vvy), life(l), color(c) {}
};

// Particle system structure
struct ParticleSystem {
    std::vector<Particle> particles;
    int x, y;
    int maxParticles;
    int speed;
    int life;
    Color color;
    
    ParticleSystem() : x(0), y(0), maxParticles(0), speed(0), life(0), color({0, 0, 0, 255}) {}
    ParticleSystem(int px, int py, int count, int s, int l, int r, int g, int b)
        : x(px), y(py), maxParticles(count), speed(s), life(l), color({(unsigned char)r, (unsigned char)g, (unsigned char)b, 255}) {
        // Initialize particles
        for (int i = 0; i < count; i++) {
            float angle = (float)i / count * 2.0f * PI;
            float vx = cos(angle) * speed;
            float vy = sin(angle) * speed;
            particles.emplace_back(x, y, vx, vy, life, color);
        }
    }
};

// Sprite management functions
int CreateSprite(int textureId, int x, int y, int width, int height) {
    int id = nextSpriteId++;
    sprites[id] = Sprite(textureId, x, y, width, height);
    return id;
}

void DrawSprite(int spriteId, int x, int y) {
    auto it = sprites.find(spriteId);
    if (it != sprites.end()) {
        Sprite& sprite = it->second;
        // For now, just draw a colored rectangle representing the sprite
        DrawRectangle(x, y, sprite.width, sprite.height, RED);
    }
}

void MoveSprite(int spriteId, int dx, int dy) {
    auto it = sprites.find(spriteId);
    if (it != sprites.end()) {
        it->second.x += dx;
        it->second.y += dy;
    }
}

void SetSpritePosition(int spriteId, int x, int y) {
    auto it = sprites.find(spriteId);
    if (it != sprites.end()) {
        it->second.x = x;
        it->second.y = y;
    }
}

int GetSpriteX(int spriteId) {
    auto it = sprites.find(spriteId);
    return (it != sprites.end()) ? it->second.x : 0;
}

int GetSpriteY(int spriteId) {
    auto it = sprites.find(spriteId);
    return (it != sprites.end()) ? it->second.y : 0;
}

void SetSpriteSpeed(int spriteId, float speed) {
    auto it = sprites.find(spriteId);
    if (it != sprites.end()) {
        it->second.speed = speed;
    }
}

float GetSpriteSpeed(int spriteId) {
    auto it = sprites.find(spriteId);
    return (it != sprites.end()) ? it->second.speed : 0.0f;
}

// Collision detection functions
bool CheckSpriteCollision(int sprite1Id, int sprite2Id) {
    auto it1 = sprites.find(sprite1Id);
    auto it2 = sprites.find(sprite2Id);
    
    if (it1 != sprites.end() && it2 != sprites.end()) {
        Sprite& s1 = it1->second;
        Sprite& s2 = it2->second;
        
        return (s1.x < s2.x + s2.width && s1.x + s1.width > s2.x &&
                s1.y < s2.y + s2.height && s1.y + s1.height > s2.y);
    }
    return false;
}

bool CheckSpritePointCollision(int spriteId, int x, int y) {
    auto it = sprites.find(spriteId);
    if (it != sprites.end()) {
        Sprite& sprite = it->second;
        return (x >= sprite.x && x < sprite.x + sprite.width &&
                y >= sprite.y && y < sprite.y + sprite.height);
    }
    return false;
}

bool CheckSpriteRectCollision(int spriteId, int x, int y, int width, int height) {
    auto it = sprites.find(spriteId);
    if (it != sprites.end()) {
        Sprite& sprite = it->second;
        return (sprite.x < x + width && sprite.x + sprite.width > x &&
                sprite.y < y + height && sprite.y + sprite.height > y);
    }
    return false;
}

// Animation system functions
int CreateSpriteAnimation(int textureId, int frameWidth, int frameHeight, int totalFrames, int frameDelay) {
    int id = nextAnimationId++;
    animations[id] = SpriteAnimation(textureId, frameWidth, frameHeight, totalFrames, frameDelay);
    return id;
}

void PlaySpriteAnimation(int animationId) {
    auto it = animations.find(animationId);
    if (it != animations.end()) {
        it->second.playing = true;
        it->second.paused = false;
    }
}

void PauseSpriteAnimation(int animationId) {
    auto it = animations.find(animationId);
    if (it != animations.end()) {
        it->second.paused = true;
    }
}

void StopSpriteAnimation(int animationId) {
    auto it = animations.find(animationId);
    if (it != animations.end()) {
        it->second.playing = false;
        it->second.paused = false;
        it->second.currentFrame = 0;
        it->second.frameCounter = 0;
    }
}

void SetSpriteAnimationFrame(int animationId, int frame) {
    auto it = animations.find(animationId);
    if (it != animations.end()) {
        it->second.currentFrame = std::max(0, std::min(frame, it->second.totalFrames - 1));
    }
}

int GetSpriteAnimationFrame(int animationId) {
    auto it = animations.find(animationId);
    return (it != animations.end()) ? it->second.currentFrame : 0;
}

// Particle system functions
int CreateParticleSystem(int x, int y, int count, int speed, int life, int r, int g, int b) {
    int id = nextParticleSystemId++;
    particleSystems[id] = ParticleSystem(x, y, count, speed, life, r, g, b);
    return id;
}

void UpdateParticleSystem(int particleSystemId) {
    auto it = particleSystems.find(particleSystemId);
    if (it != particleSystems.end()) {
        ParticleSystem& ps = it->second;
        
        for (auto& particle : ps.particles) {
            if (particle.life > 0) {
                particle.x += particle.vx;
                particle.y += particle.vy;
                particle.life--;
            }
        }
    }
}

void DrawParticleSystem(int particleSystemId) {
    auto it = particleSystems.find(particleSystemId);
    if (it != particleSystems.end()) {
        ParticleSystem& ps = it->second;
        
        for (const auto& particle : ps.particles) {
            if (particle.life > 0) {
                DrawPixel(particle.x, particle.y, particle.color);
            }
        }
    }
}

void SetParticleSystemPosition(int particleSystemId, int x, int y) {
    auto it = particleSystems.find(particleSystemId);
    if (it != particleSystems.end()) {
        it->second.x = x;
        it->second.y = y;
    }
}

// Enhanced sound system functions
void PlaySoundLoop(int soundId) {
    soundLoops[soundId] = true;
    // Note: Actual looping would need to be implemented in the main game loop
}

void StopSoundLoop(int soundId) {
    soundLoops[soundId] = false;
}

void SetSoundVolume(int soundId, float volume) {
    soundVolumes[soundId] = std::max(0.0f, std::min(1.0f, volume));
}

float GetSoundVolume(int soundId) {
    auto it = soundVolumes.find(soundId);
    return (it != soundVolumes.end()) ? it->second : 1.0f;
}

void SetSoundPitch(int soundId, float pitch) {
    soundPitches[soundId] = std::max(0.5f, std::min(2.0f, pitch));
}

float GetSoundPitch(int soundId) {
    auto it = soundPitches.find(soundId);
    return (it != soundPitches.end()) ? it->second : 1.0f;
}

// Enhanced text system functions
void DrawTextEx(const char* text, int x, int y, int fontSize, int spacing, Color color) {
    (void)spacing; // Suppress unused parameter warning
    // For now, use basic text drawing
    DrawText(text, x, y, fontSize, color);
}

int MeasureText(const char* text, int fontSize) {
    // For now, return approximate width
    return strlen(text) * fontSize / 2;
}

void DrawTextCentered(const char* text, int x, int y, int fontSize, int spacing, Color color) {
    (void)spacing; // Suppress unused parameter warning
    int width = MeasureText(text, fontSize);
    DrawText(text, x - width / 2, y, fontSize, color);
}

// Utility functions
int RandomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

float RandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

float Distance(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

float Angle(int x1, int y1, int x2, int y2) {
    return atan2(y2 - y1, x2 - x1) * 180.0f / PI;
}

float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

float Clamp(float value, float min, float max) {
    return std::max(min, std::min(max, value));
}

} // namespace bas
