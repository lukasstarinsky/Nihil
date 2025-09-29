#pragma once

#include "Math/Math.hpp"

enum class CameraProjection
{
    Orthographic = 0,
    Perspective
};

class Camera
{
public:
    Camera() = default;
    Camera(CameraProjection projection, const Vec3f& position, const Vec3f& lookAt, const Vec3f& up, f32 fovDegrees, f32 aspectRatio);

    auto GetProjectionMatrix() const -> const Mat4f&;
    auto GetViewMatrix() const -> const Mat4f&;
    void SetAspectRatio(f32 aspectRatio);

    auto Front() const -> const Vec3f&;
    auto Right() const -> const Vec3f&;

    void Translate(const Vec3f& translation);
    void Rotate(f32 pitch, f32 yaw);
private:
    Mat4f mProjectionMatrix {};
    Mat4f mViewMatrix {};

    Vec3f mPosition {};
    Vec3f mLookAt {};
    Vec3f mUp {};
    Vec3f mRight {};

    f32 mPitch {};
    f32 mYaw {};
    f32 mFovDegrees {};
};