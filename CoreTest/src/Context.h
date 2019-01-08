#pragma once

#include <memory>

namespace core { class Allegro; }
namespace core { class Display; }

extern std::unique_ptr<core::Allegro> ALLEGRO;
extern std::unique_ptr<core::Display> DISPLAY;

inline auto & allegro() { return *ALLEGRO; }
inline auto & display() { return *DISPLAY; }
