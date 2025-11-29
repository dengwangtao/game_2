#pragma once

#include "comm/singleton.h"
#include "SDL3/SDL.h"
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "comm/log_def.h"
#include <string>
#include <unordered_map>
#include <sstream>
#include "core/game.h"


// TupleIndex 结构体模板，递归查找类型 T 在类型包 Ts... 中的索引
template<typename T, typename... Ts>
struct TupleIndex;

// 基本情况：如果 T 是 Ts 中的第一个类型，则返回 0
template<typename T, typename... Ts>
struct TupleIndex<T, T, Ts...> {
    constexpr static std::size_t value = 0;
};

// 递归情况：如果 T 不是第一个类型，则递归查找后续类型，索引加 1
template<typename T, typename U, typename... Ts>
struct TupleIndex<T, U, Ts...> {
    constexpr static std::size_t value = 1 + TupleIndex<T, Ts...>::value;
};

// 如果找不到 T，返回 -1，表示 T 不在类型包 Ts... 中
template<typename T>
struct TupleIndex<T> {
    constexpr static std::size_t value = -1;
};

template<typename T, typename... Ts>
constexpr std::size_t TupleIndex_v = TupleIndex<T, Ts...>::value;



// 资源类型特征结构体，包含加载器和删除器
template<typename T>
struct ResourceTraits;

template<>
struct ResourceTraits<SDL_Texture> {
    static SDL_Texture* load(const std::string& filename)
    {
        return IMG_LoadTexture(G_GAME.get_renderer(), filename.c_str());
    }

    static void release(SDL_Texture* texture)
    {
        SDL_DestroyTexture(texture);
    }
};

template<>
struct ResourceTraits<Mix_Chunk> {
    static Mix_Chunk* load(const std::string& filename)
    {
        return Mix_LoadWAV(filename.c_str());
    }

    static void release(Mix_Chunk* chunk)
    {
        Mix_FreeChunk(chunk);
    }
};


template<>
struct ResourceTraits<Mix_Music> {
    static Mix_Music* load(const std::string& filename)
    {
        return Mix_LoadMUS(filename.c_str());
    }

    static void release(Mix_Music* music)
    {
        Mix_FreeMusic(music);
    }
};



template<>
struct ResourceTraits<TTF_Font> {
    static TTF_Font* load(const std::string& filename, s32 size)
    {
        return TTF_OpenFont(filename.c_str(), size);
    }

    static void release(TTF_Font* font)
    {
        TTF_CloseFont(font);
    }
};


// 资源管理器
template<typename... Ts>
class ResourceManager : public Singleton<ResourceManager<Ts...>>
{
public:
    ~ResourceManager()
    {
        std::apply([this](auto&&... resource_map)
        {
            (clearResources(resource_map), ...); // 展开调用
        }, resources_);
    }

    // 加载资源：只允许在资源类型列表中
    template<typename T, typename... Args>
    std::enable_if_t<TupleIndex_v<T, Ts...> != -1, T*>
    loadResource(const std::string& filename, Args&&... args)
    {
        // 拼接 filename 和 args 到一个新的字符串
        std::ostringstream oss;
        oss << filename << ";";
        (void)(oss << ... << std::forward<Args>(args));

        std::string key = oss.str();  // 得到拼接后的字符串

        auto& map = std::get<TupleIndex_v<T, Ts...>>(resources_);
        auto it = map.find(key);
        if (it != map.end())
        {
            return it->second;
        }
        LOG_INFO("Loading resource: {}", key);
        T* resource = ResourceTraits<T>::load(filename, std::forward<Args>(args)...);
        if (resource == nullptr)
        {
            LOG_ERROR("Failed to load resource: {}", key);
            return nullptr;
        }
        map[key] = resource;
        return resource;
    }

private:
    // 清理资源
    template<typename T>
    void clearResources(std::unordered_map<std::string, T*>& resource_map)
    {
        for (auto& [filename, resource] : resource_map)
        {
            ResourceTraits<T>::release(resource);
            LOG_INFO("Destroying resource: {}", filename);
        }
    }

private:
    std::tuple<std::unordered_map<std::string, Ts*>...> resources_;
};

#define G_RESOURCE_MGR \
    ResourceManager<SDL_Texture, Mix_Chunk, TTF_Font, Mix_Music>::Instance()