#include "bas/builtins.hpp"
#include <unordered_map>
#include <memory>
#include <string>

#include "raylib.h"

using namespace bas;

void register_builtins_audio(FunctionRegistry& R) {
  struct AudioState {
    bool inited = false;
    std::unordered_map<long long, std::unique_ptr<Sound>> sounds; // id -> Sound
    long long nextId = 1;
  };
  static AudioState audio;

  auto ensureAudio = [](){
    if (!audio.inited) {
      InitAudioDevice();
      audio.inited = true;
    }
  };

  // LOADSOUND(path) -> id
  R.add("LOADSOUND", NativeFn{"LOADSOUND", 1, [ensureAudio](const std::vector<Value>& a){
    ensureAudio();
    std::string path = a[0].as_string();
    auto snd = std::make_unique<Sound>(LoadSound(path.c_str()));
    long long id = audio.nextId++;
    audio.sounds[id] = std::move(snd);
    return Value::from_int(id);
  }});

  // PLAYSOUND(id)
  R.add("PLAYSOUND", NativeFn{"PLAYSOUND", 1, [ensureAudio](const std::vector<Value>& a){
    ensureAudio();
    long long id = a[0].as_int();
    auto it = audio.sounds.find(id);
    if (it != audio.sounds.end() && it->second) {
      PlaySound(*it->second);
    }
    return Value::nil();
  }});

  // STOPSOUND(id)
  R.add("STOPSOUND", NativeFn{"STOPSOUND", 1, [ensureAudio](const std::vector<Value>& a){
    ensureAudio();
    long long id = a[0].as_int();
    auto it = audio.sounds.find(id);
    if (it != audio.sounds.end() && it->second) {
      ::StopSound(*it->second);
    }
    return Value::nil();
  }});

  // UNLOADSOUND(id)
  R.add("UNLOADSOUND", NativeFn{"UNLOADSOUND", 1, [](const std::vector<Value>& a){
    long long id = a[0].as_int();
    auto it = audio.sounds.find(id);
    if (it != audio.sounds.end() && it->second) {
      ::UnloadSound(*it->second);
      audio.sounds.erase(it);
    }
    return Value::nil();
  }});

  // CLOSEAUDIO(): unload all sounds and close the audio device
  R.add("CLOSEAUDIO", NativeFn{"CLOSEAUDIO", 0, [](const std::vector<Value>&){
    for (auto it = audio.sounds.begin(); it != audio.sounds.end(); ++it) {
      if (it->second) ::UnloadSound(*it->second);
    }
    audio.sounds.clear();
    if (audio.inited) { CloseAudioDevice(); audio.inited = false; }
    return Value::nil();
  }});
}
