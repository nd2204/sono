#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

constexpr Vec2 gVec2Zero(0, 0);
constexpr Vec2 gVector2Right(1, 0);
constexpr Vec2 gVector2Up(0, 1);
constexpr Vec2 gVector2Left(-1, 0);
constexpr Vec2 gVector2Down(0, -1);

constexpr Vec3 gVector3Zero(0, 0, 0);
constexpr Vec3 gVector3Right(1, 0, 0);
constexpr Vec3 gVector3Up(0, 1, 0);
constexpr Vec3 gVector3Forward(0, 0, -1);
constexpr Vec3 gVector3Left(-1, 0, 0);
constexpr Vec3 gVector3Down(0, -1, 0);
constexpr Vec3 gVector3Back(0, 0, 1);

constexpr Vec4 gVector4Zero(0, 0, 0, 0);

const Vec2 &VecBase<2, f32>::Up = gVector2Up;
const Vec2 &VecBase<2, f32>::Down = gVector2Down;
const Vec2 &VecBase<2, f32>::Left = gVector2Left;
const Vec2 &VecBase<2, f32>::Right = gVector2Right;
const Vec2 &VecBase<2, f32>::Zero = gVec2Zero;

const Vec3 &VecBase<3, f32>::Up = gVector3Up;
const Vec3 &VecBase<3, f32>::Down = gVector3Down;
const Vec3 &VecBase<3, f32>::Left = gVector3Left;
const Vec3 &VecBase<3, f32>::Right = gVector3Right;
const Vec3 &VecBase<3, f32>::Forward = gVector3Forward;
const Vec3 &VecBase<3, f32>::Back = gVector3Back;
const Vec3 &VecBase<3, f32>::Zero = gVector3Zero;

const Vec4 &VecBase<4, f32>::Zero = gVector4Zero;
