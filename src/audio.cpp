#include "bas/audio.hpp"
#include "bas/runtime.hpp"
#include <algorithm>
#include <cmath>

namespace bas {

// Global audio system instance
std::unique_ptr<AudioSystem> g_audio_system;

// AudioSystem implementation
AudioSystem::AudioSystem() 
    : next_audio_source_id(0), next_audio_listener_id(0), next_music_track_id(0), next_audio_effect_id(0),
      master_volume(1.0f), music_volume(1.0f), sfx_volume(1.0f), muted(false), paused(false) {
}

AudioSystem::~AudioSystem() = default;

// Audio source management
int AudioSystem::create_audio_source(const std::string& name, AudioSourceType type) {
    auto source = std::make_unique<AudioSource>(next_audio_source_id++, name, type);
    int id = source->id;
    audio_sources.push_back(std::move(source));
    return id;
}

void AudioSystem::remove_audio_source(int source_id) {
    audio_sources.erase(std::remove_if(audio_sources.begin(), audio_sources.end(),
        [source_id](const std::unique_ptr<AudioSource>& source) {
            return source->id == source_id;
        }), audio_sources.end());
}

AudioSource* AudioSystem::get_audio_source(int source_id) {
    auto it = std::find_if(audio_sources.begin(), audio_sources.end(),
        [source_id](const std::unique_ptr<AudioSource>& source) {
            return source->id == source_id;
        });
    return (it != audio_sources.end()) ? it->get() : nullptr;
}

bool AudioSystem::load_audio_file(int source_id, const std::string& file_path) {
    AudioSource* source = get_audio_source(source_id);
    if (source) {
        source->file_path = file_path;
        return true;
    }
    return false;
}

bool AudioSystem::play_audio_source(int source_id) {
    AudioSource* source = get_audio_source(source_id);
    if (source) {
        source->playing = true;
        source->paused = false;
        return true;
    }
    return false;
}

bool AudioSystem::pause_audio_source(int source_id) {
    AudioSource* source = get_audio_source(source_id);
    if (source) {
        source->paused = true;
        return true;
    }
    return false;
}

bool AudioSystem::stop_audio_source(int source_id) {
    AudioSource* source = get_audio_source(source_id);
    if (source) {
        source->playing = false;
        source->paused = false;
        return true;
    }
    return false;
}

void AudioSystem::set_audio_source_volume(int source_id, float volume) {
    AudioSource* source = get_audio_source(source_id);
    if (source) {
        source->volume = std::max(0.0f, std::min(1.0f, volume));
    }
}

void AudioSystem::set_audio_source_pitch(int source_id, float pitch) {
    AudioSource* source = get_audio_source(source_id);
    if (source) {
        source->pitch = std::max(0.1f, std::min(10.0f, pitch));
    }
}

void AudioSystem::set_audio_source_pan(int source_id, float pan) {
    AudioSource* source = get_audio_source(source_id);
    if (source) {
        source->pan = std::max(-1.0f, std::min(1.0f, pan));
    }
}

void AudioSystem::set_audio_source_loop(int source_id, bool loop) {
    AudioSource* source = get_audio_source(source_id);
    if (source) {
        source->loop = loop;
    }
}

void AudioSystem::set_audio_source_position(int source_id, float x, float y, float z) {
    AudioSource* source = get_audio_source(source_id);
    if (source) {
        source->position_x = x;
        source->position_y = y;
        source->position_z = z;
    }
}

void AudioSystem::set_audio_source_velocity(int source_id, float x, float y, float z) {
    AudioSource* source = get_audio_source(source_id);
    if (source) {
        source->velocity_x = x;
        source->velocity_y = y;
        source->velocity_z = z;
    }
}

void AudioSystem::set_audio_source_distance_range(int source_id, float min_distance, float max_distance) {
    AudioSource* source = get_audio_source(source_id);
    if (source) {
        source->min_distance = std::max(0.1f, min_distance);
        source->max_distance = std::max(min_distance, max_distance);
    }
}

void AudioSystem::set_audio_source_rolloff_factor(int source_id, float rolloff_factor) {
    AudioSource* source = get_audio_source(source_id);
    if (source) {
        source->rolloff_factor = std::max(0.0f, rolloff_factor);
    }
}

void AudioSystem::set_audio_source_cone(int source_id, float inner_angle, float outer_angle, float outer_gain) {
    AudioSource* source = get_audio_source(source_id);
    if (source) {
        source->cone_inner_angle = std::max(0.0f, std::min(360.0f, inner_angle));
        source->cone_outer_angle = std::max(inner_angle, std::min(360.0f, outer_angle));
        source->cone_outer_gain = std::max(0.0f, std::min(1.0f, outer_gain));
    }
}

// Audio listener management
int AudioSystem::create_audio_listener(const std::string& name) {
    auto listener = std::make_unique<AudioListener>(next_audio_listener_id++, name);
    int id = listener->id;
    audio_listeners.push_back(std::move(listener));
    return id;
}

void AudioSystem::remove_audio_listener(int listener_id) {
    audio_listeners.erase(std::remove_if(audio_listeners.begin(), audio_listeners.end(),
        [listener_id](const std::unique_ptr<AudioListener>& listener) {
            return listener->id == listener_id;
        }), audio_listeners.end());
}

AudioListener* AudioSystem::get_audio_listener(int listener_id) {
    auto it = std::find_if(audio_listeners.begin(), audio_listeners.end(),
        [listener_id](const std::unique_ptr<AudioListener>& listener) {
            return listener->id == listener_id;
        });
    return (it != audio_listeners.end()) ? it->get() : nullptr;
}

void AudioSystem::set_audio_listener_position(int listener_id, float x, float y, float z) {
    AudioListener* listener = get_audio_listener(listener_id);
    if (listener) {
        listener->position_x = x;
        listener->position_y = y;
        listener->position_z = z;
    }
}

void AudioSystem::set_audio_listener_velocity(int listener_id, float x, float y, float z) {
    AudioListener* listener = get_audio_listener(listener_id);
    if (listener) {
        listener->velocity_x = x;
        listener->velocity_y = y;
        listener->velocity_z = z;
    }
}

void AudioSystem::set_audio_listener_orientation(int listener_id, float forward_x, float forward_y, float forward_z, float up_x, float up_y, float up_z) {
    AudioListener* listener = get_audio_listener(listener_id);
    if (listener) {
        listener->forward_x = forward_x;
        listener->forward_y = forward_y;
        listener->forward_z = forward_z;
        listener->up_x = up_x;
        listener->up_y = up_y;
        listener->up_z = up_z;
    }
}

void AudioSystem::set_audio_listener_volume(int listener_id, float volume) {
    AudioListener* listener = get_audio_listener(listener_id);
    if (listener) {
        listener->volume = std::max(0.0f, std::min(1.0f, volume));
    }
}

void AudioSystem::set_audio_listener_active(int listener_id, bool active) {
    AudioListener* listener = get_audio_listener(listener_id);
    if (listener) {
        listener->active = active;
    }
}

// Music track management
int AudioSystem::create_music_track(const std::string& name, const std::string& file_path) {
    auto track = std::make_unique<MusicTrack>(next_music_track_id++, name, file_path);
    int id = track->id;
    music_tracks.push_back(std::move(track));
    return id;
}

void AudioSystem::remove_music_track(int track_id) {
    music_tracks.erase(std::remove_if(music_tracks.begin(), music_tracks.end(),
        [track_id](const std::unique_ptr<MusicTrack>& track) {
            return track->id == track_id;
        }), music_tracks.end());
}

MusicTrack* AudioSystem::get_music_track(int track_id) {
    auto it = std::find_if(music_tracks.begin(), music_tracks.end(),
        [track_id](const std::unique_ptr<MusicTrack>& track) {
            return track->id == track_id;
        });
    return (it != music_tracks.end()) ? it->get() : nullptr;
}

bool AudioSystem::play_music_track(int track_id) {
    MusicTrack* track = get_music_track(track_id);
    if (track) {
        track->playing = true;
        track->paused = false;
        return true;
    }
    return false;
}

bool AudioSystem::pause_music_track(int track_id) {
    MusicTrack* track = get_music_track(track_id);
    if (track) {
        track->paused = true;
        return true;
    }
    return false;
}

bool AudioSystem::stop_music_track(int track_id) {
    MusicTrack* track = get_music_track(track_id);
    if (track) {
        track->playing = false;
        track->paused = false;
        return true;
    }
    return false;
}

void AudioSystem::set_music_track_volume(int track_id, float volume) {
    MusicTrack* track = get_music_track(track_id);
    if (track) {
        track->volume = std::max(0.0f, std::min(1.0f, volume));
    }
}

void AudioSystem::set_music_track_pitch(int track_id, float pitch) {
    MusicTrack* track = get_music_track(track_id);
    if (track) {
        track->pitch = std::max(0.1f, std::min(10.0f, pitch));
    }
}

void AudioSystem::set_music_track_loop(int track_id, bool loop) {
    MusicTrack* track = get_music_track(track_id);
    if (track) {
        track->loop = loop;
    }
}

void AudioSystem::set_music_track_fade_times(int track_id, float fade_in, float fade_out) {
    MusicTrack* track = get_music_track(track_id);
    if (track) {
        track->fade_in_time = std::max(0.0f, fade_in);
        track->fade_out_time = std::max(0.0f, fade_out);
    }
}

void AudioSystem::crossfade_music_tracks(int from_track_id, int to_track_id, float crossfade_time) {
    MusicTrack* from_track = get_music_track(from_track_id);
    MusicTrack* to_track = get_music_track(to_track_id);
    
    if (from_track && to_track) {
        from_track->fade_out_time = crossfade_time;
        to_track->fade_in_time = crossfade_time;
        stop_music_track(from_track_id);
        play_music_track(to_track_id);
    }
}

// Audio effect management
int AudioSystem::create_audio_effect(const std::string& name, AudioEffectType type) {
    auto effect = std::make_unique<AudioEffect>(next_audio_effect_id++, name, type);
    int id = effect->id;
    
    // Set default parameters based on type
    switch (type) {
        case AudioEffectType::REVERB:
            effect->parameters["room_size"] = 0.5f;
            effect->parameters["damping"] = 0.5f;
            effect->parameters["wet_level"] = 0.3f;
            effect->parameters["dry_level"] = 0.7f;
            break;
        case AudioEffectType::ECHO:
            effect->parameters["delay"] = 0.1f;
            effect->parameters["decay"] = 0.5f;
            effect->parameters["feedback"] = 0.3f;
            break;
        case AudioEffectType::CHORUS:
            effect->parameters["rate"] = 1.0f;
            effect->parameters["depth"] = 0.5f;
            effect->parameters["mix"] = 0.5f;
            break;
        case AudioEffectType::DISTORTION:
            effect->parameters["gain"] = 1.0f;
            effect->parameters["tone"] = 0.5f;
            effect->parameters["level"] = 0.5f;
            break;
        case AudioEffectType::LOW_PASS:
            effect->parameters["cutoff"] = 1000.0f;
            effect->parameters["resonance"] = 1.0f;
            break;
        case AudioEffectType::HIGH_PASS:
            effect->parameters["cutoff"] = 1000.0f;
            effect->parameters["resonance"] = 1.0f;
            break;
        case AudioEffectType::BAND_PASS:
            effect->parameters["center"] = 1000.0f;
            effect->parameters["bandwidth"] = 100.0f;
            break;
        case AudioEffectType::COMPRESSOR:
            effect->parameters["threshold"] = -20.0f;
            effect->parameters["ratio"] = 4.0f;
            effect->parameters["attack"] = 0.01f;
            effect->parameters["release"] = 0.1f;
            break;
        case AudioEffectType::LIMITER:
            effect->parameters["threshold"] = -3.0f;
            effect->parameters["release"] = 0.1f;
            break;
        default:
            break;
    }
    
    audio_effects.push_back(std::move(effect));
    return id;
}

void AudioSystem::remove_audio_effect(int effect_id) {
    audio_effects.erase(std::remove_if(audio_effects.begin(), audio_effects.end(),
        [effect_id](const std::unique_ptr<AudioEffect>& effect) {
            return effect->id == effect_id;
        }), audio_effects.end());
}

AudioEffect* AudioSystem::get_audio_effect(int effect_id) {
    auto it = std::find_if(audio_effects.begin(), audio_effects.end(),
        [effect_id](const std::unique_ptr<AudioEffect>& effect) {
            return effect->id == effect_id;
        });
    return (it != audio_effects.end()) ? it->get() : nullptr;
}

void AudioSystem::set_audio_effect_active(int effect_id, bool active) {
    AudioEffect* effect = get_audio_effect(effect_id);
    if (effect) {
        effect->active = active;
    }
}

void AudioSystem::set_audio_effect_parameter(int effect_id, const std::string& name, float value) {
    AudioEffect* effect = get_audio_effect(effect_id);
    if (effect) {
        effect->parameters[name] = value;
    }
}

void AudioSystem::apply_audio_effect_to_source(int effect_id, int source_id) {
    // This would apply the effect to the audio source
    (void)effect_id; (void)source_id; // Suppress unused parameter warnings
}

void AudioSystem::apply_audio_effect_to_music(int effect_id, int track_id) {
    // This would apply the effect to the music track
    (void)effect_id; (void)track_id; // Suppress unused parameter warnings
}

// System management
void AudioSystem::update() {
    update_audio_sources();
    update_audio_listeners();
    update_music_tracks();
    apply_audio_effects();
}

void AudioSystem::set_master_volume(float volume) {
    master_volume = std::max(0.0f, std::min(1.0f, volume));
}

void AudioSystem::set_music_volume(float volume) {
    music_volume = std::max(0.0f, std::min(1.0f, volume));
}

void AudioSystem::set_sfx_volume(float volume) {
    sfx_volume = std::max(0.0f, std::min(1.0f, volume));
}

void AudioSystem::set_muted(bool muted) {
    this->muted = muted;
}

void AudioSystem::set_paused(bool paused) {
    this->paused = paused;
}

void AudioSystem::stop_all_audio() {
    for (auto& source : audio_sources) {
        source->playing = false;
        source->paused = false;
    }
    for (auto& track : music_tracks) {
        track->playing = false;
        track->paused = false;
    }
}

void AudioSystem::pause_all_audio() {
    for (auto& source : audio_sources) {
        if (source->playing) {
            source->paused = true;
        }
    }
    for (auto& track : music_tracks) {
        if (track->playing) {
            track->paused = true;
        }
    }
}

void AudioSystem::resume_all_audio() {
    for (auto& source : audio_sources) {
        if (source->paused) {
            source->paused = false;
        }
    }
    for (auto& track : music_tracks) {
        if (track->paused) {
            track->paused = false;
        }
    }
}

// Utility functions
int AudioSystem::get_audio_source_count() const {
    return static_cast<int>(audio_sources.size());
}

int AudioSystem::get_audio_listener_count() const {
    return static_cast<int>(audio_listeners.size());
}

int AudioSystem::get_music_track_count() const {
    return static_cast<int>(music_tracks.size());
}

int AudioSystem::get_audio_effect_count() const {
    return static_cast<int>(audio_effects.size());
}

float AudioSystem::get_master_volume() const {
    return master_volume;
}

float AudioSystem::get_music_volume() const {
    return music_volume;
}

float AudioSystem::get_sfx_volume() const {
    return sfx_volume;
}

bool AudioSystem::is_muted() const {
    return muted;
}

bool AudioSystem::is_paused() const {
    return paused;
}

// Pre-built audio sources
int AudioSystem::create_sound_effect(const std::string& name, const std::string& file_path) {
    int id = create_audio_source(name, AudioSourceType::SOUND_EFFECT);
    load_audio_file(id, file_path);
    return id;
}

int AudioSystem::create_ambient_sound(const std::string& name, const std::string& file_path) {
    int id = create_audio_source(name, AudioSourceType::AMBIENT);
    load_audio_file(id, file_path);
    set_audio_source_loop(id, true);
    return id;
}

int AudioSystem::create_voice_line(const std::string& name, const std::string& file_path) {
    int id = create_audio_source(name, AudioSourceType::VOICE);
    load_audio_file(id, file_path);
    return id;
}

// Pre-built audio effects
int AudioSystem::create_reverb_effect(const std::string& name) {
    return create_audio_effect(name, AudioEffectType::REVERB);
}

int AudioSystem::create_echo_effect(const std::string& name) {
    return create_audio_effect(name, AudioEffectType::ECHO);
}

int AudioSystem::create_chorus_effect(const std::string& name) {
    return create_audio_effect(name, AudioEffectType::CHORUS);
}

int AudioSystem::create_distortion_effect(const std::string& name) {
    return create_audio_effect(name, AudioEffectType::DISTORTION);
}

int AudioSystem::create_low_pass_filter(const std::string& name) {
    return create_audio_effect(name, AudioEffectType::LOW_PASS);
}

int AudioSystem::create_high_pass_filter(const std::string& name) {
    return create_audio_effect(name, AudioEffectType::HIGH_PASS);
}

int AudioSystem::create_band_pass_filter(const std::string& name) {
    return create_audio_effect(name, AudioEffectType::BAND_PASS);
}

int AudioSystem::create_compressor_effect(const std::string& name) {
    return create_audio_effect(name, AudioEffectType::COMPRESSOR);
}

int AudioSystem::create_limiter_effect(const std::string& name) {
    return create_audio_effect(name, AudioEffectType::LIMITER);
}

// Private helper methods
void AudioSystem::update_audio_sources() {
    for (auto& source : audio_sources) {
        if (source->playing && !source->paused) {
            // Update 3D audio calculations
            for (auto& listener : audio_listeners) {
                if (listener->active) {
                    calculate_3d_audio(*source, *listener);
                }
            }
        }
    }
}

void AudioSystem::update_audio_listeners() {
    // Update listener states
    for (auto& listener : audio_listeners) {
        if (listener->active) {
            // Update listener properties
        }
    }
}

void AudioSystem::update_music_tracks() {
    for (auto& track : music_tracks) {
        if (track->playing && !track->paused) {
            // Update music track properties
        }
    }
}

void AudioSystem::apply_audio_effects() {
    for (auto& effect : audio_effects) {
        if (effect->active) {
            // Apply audio effects
        }
    }
}

void AudioSystem::calculate_3d_audio(AudioSource& source, const AudioListener& listener) {
    apply_distance_attenuation(source, listener);
    apply_cone_attenuation(source, listener);
    apply_doppler_effect(source, listener);
}

void AudioSystem::apply_distance_attenuation(AudioSource& source, const AudioListener& listener) {
    float distance = calculate_distance(
        source.position_x, source.position_y, source.position_z,
        listener.position_x, listener.position_y, listener.position_z
    );
    
    float attenuation = calculate_attenuation(
        distance, source.min_distance, source.max_distance, source.rolloff_factor
    );
    
    source.volume *= attenuation;
}

void AudioSystem::apply_cone_attenuation(AudioSource& source, const AudioListener& listener) {
    // Calculate direction from source to listener
    float dx = listener.position_x - source.position_x;
    float dy = listener.position_y - source.position_y;
    float dz = listener.position_z - source.position_z;
    
    // Normalize the direction vector
    normalize_vector(dx, dy, dz);
    
    // Calculate dot product with source forward direction
    float dot = dot_product(dx, dy, dz, 0.0f, 0.0f, -1.0f); // Assuming source faces forward
    
    // Apply cone attenuation
    if (dot < std::cos(source.cone_outer_angle * M_PI / 180.0f)) {
        source.volume *= source.cone_outer_gain;
    } else if (dot < std::cos(source.cone_inner_angle * M_PI / 180.0f)) {
        float factor = (dot - std::cos(source.cone_outer_angle * M_PI / 180.0f)) /
                      (std::cos(source.cone_inner_angle * M_PI / 180.0f) - std::cos(source.cone_outer_angle * M_PI / 180.0f));
        source.volume *= (1.0f - factor) * source.cone_outer_gain + factor;
    }
}

void AudioSystem::apply_doppler_effect(AudioSource& source, const AudioListener& listener) {
    float source_velocity = std::sqrt(source.velocity_x * source.velocity_x + 
                                     source.velocity_y * source.velocity_y + 
                                     source.velocity_z * source.velocity_z);
    float listener_velocity = std::sqrt(listener.velocity_x * listener.velocity_x + 
                                       listener.velocity_y * listener.velocity_y + 
                                       listener.velocity_z * listener.velocity_z);
    
    float doppler_shift = calculate_doppler_shift(source_velocity, listener_velocity, 343.0f);
    source.pitch *= doppler_shift;
}

float AudioSystem::calculate_distance(float x1, float y1, float z1, float x2, float y2, float z2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

float AudioSystem::calculate_attenuation(float distance, float min_dist, float max_dist, float rolloff) {
    if (distance <= min_dist) {
        return 1.0f;
    } else if (distance >= max_dist) {
        return 0.0f;
    } else {
        return std::pow(min_dist / distance, rolloff);
    }
}

float AudioSystem::calculate_doppler_shift(float source_velocity, float listener_velocity, float speed_of_sound) {
    return (speed_of_sound + listener_velocity) / (speed_of_sound + source_velocity);
}

void AudioSystem::normalize_vector(float& x, float& y, float& z) {
    float length = std::sqrt(x * x + y * y + z * z);
    if (length > 0.0f) {
        x /= length;
        y /= length;
        z /= length;
    }
}

float AudioSystem::dot_product(float x1, float y1, float z1, float x2, float y2, float z2) {
    return x1 * x2 + y1 * y2 + z1 * z2;
}

// Native function implementations
Value audio_init_system(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_audio_system) {
        g_audio_system = std::make_unique<AudioSystem>();
    }
    return Value::nil();
}

Value audio_create_sound_effect(const std::vector<Value>& args) {
    if (args.size() != 2 || !g_audio_system) return Value::from_int(-1);
    
    std::string name = args[0].as_string();
    std::string file_path = args[1].as_string();
    
    int source_id = g_audio_system->create_sound_effect(name, file_path);
    return Value::from_int(source_id);
}

Value audio_create_music_track(const std::vector<Value>& args) {
    if (args.size() != 2 || !g_audio_system) return Value::from_int(-1);
    
    std::string name = args[0].as_string();
    std::string file_path = args[1].as_string();
    
    int track_id = g_audio_system->create_music_track(name, file_path);
    return Value::from_int(track_id);
}

Value audio_play_sound(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_audio_system) return Value::from_bool(false);
    
    int source_id = args[0].as_int();
    bool success = g_audio_system->play_audio_source(source_id);
    return Value::from_bool(success);
}

Value audio_play_music(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_audio_system) return Value::from_bool(false);
    
    int track_id = args[0].as_int();
    bool success = g_audio_system->play_music_track(track_id);
    return Value::from_bool(success);
}

Value audio_set_master_volume(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_audio_system) return Value::nil();
    
    float volume = static_cast<float>(args[0].as_number());
    g_audio_system->set_master_volume(volume);
    return Value::nil();
}

Value audio_set_music_volume(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_audio_system) return Value::nil();
    
    float volume = static_cast<float>(args[0].as_number());
    g_audio_system->set_music_volume(volume);
    return Value::nil();
}

Value audio_set_sfx_volume(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_audio_system) return Value::nil();
    
    float volume = static_cast<float>(args[0].as_number());
    g_audio_system->set_sfx_volume(volume);
    return Value::nil();
}

Value audio_create_reverb_effect(const std::vector<Value>& args) {
    if (args.size() != 1 || !g_audio_system) return Value::from_int(-1);
    
    std::string name = args[0].as_string();
    int effect_id = g_audio_system->create_reverb_effect(name);
    return Value::from_int(effect_id);
}

Value audio_update_audio_system(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_audio_system) return Value::nil();
    
    g_audio_system->update();
    return Value::nil();
}

Value audio_get_audio_source_count(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_audio_system) return Value::from_int(0);
    
    int count = g_audio_system->get_audio_source_count();
    return Value::from_int(count);
}

Value audio_get_music_track_count(const std::vector<Value>& args) {
    (void)args; // Suppress unused parameter warning
    if (!g_audio_system) return Value::from_int(0);
    
    int count = g_audio_system->get_music_track_count();
    return Value::from_int(count);
}

void register_audio_functions(FunctionRegistry& registry) {
    registry.add("INITAUDIO", NativeFn{"INITAUDIO", 0, audio_init_system});
    registry.add("CREATESOUNDEFFECT", NativeFn{"CREATESOUNDEFFECT", 2, audio_create_sound_effect});
    registry.add("CREATEMUSICTRACK", NativeFn{"CREATEMUSICTRACK", 2, audio_create_music_track});
    registry.add("PLAYSOUND", NativeFn{"PLAYSOUND", 1, audio_play_sound});
    registry.add("PLAYMUSIC", NativeFn{"PLAYMUSIC", 1, audio_play_music});
    registry.add("SETMASTERVOLUME", NativeFn{"SETMASTERVOLUME", 1, audio_set_master_volume});
    registry.add("SETMUSICVOLUME", NativeFn{"SETMUSICVOLUME", 1, audio_set_music_volume});
    registry.add("SETSFXVOLUME", NativeFn{"SETSFXVOLUME", 1, audio_set_sfx_volume});
    registry.add("CREATEREVERBEFFECT", NativeFn{"CREATEREVERBEFFECT", 1, audio_create_reverb_effect});
    registry.add("UPDATEAUDIOSYSTEM", NativeFn{"UPDATEAUDIOSYSTEM", 0, audio_update_audio_system});
    registry.add("GETAUDIOSOURCECOUNT", NativeFn{"GETAUDIOSOURCECOUNT", 0, audio_get_audio_source_count});
    registry.add("GETMUSICTRACKCOUNT", NativeFn{"GETMUSICTRACKCOUNT", 0, audio_get_music_track_count});
}

} // namespace bas
