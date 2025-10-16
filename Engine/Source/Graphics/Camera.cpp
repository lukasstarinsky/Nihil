#include "Camera.hpp"

Camera::Camera(CameraProjection projection, const Vec3f& position, const Vec3f& lookAt, const Vec3f& up, f32 fovDegreesOrWidth, f32 aspectRatioOrHeight)
    : mProjection{projection}
    , mPosition{position}
    , mLookAt{lookAt}
    , mUp{up}
    , mFovDegrees{fovDegreesOrWidth}
{
    if (projection == CameraProjection::Orthographic)
    {
        mProjectionMatrix = Mat4f::Orthographic(0, fovDegreesOrWidth, aspectRatioOrHeight, 0, -1.0f, 1.0f);
        mViewMatrix = Mat4f::Identity();
    }
    else
    {
        mProjectionMatrix = Mat4f::Perspective(DEG_TO_RAD(fovDegreesOrWidth), aspectRatioOrHeight, 0.1f, 100.0f);
        mViewMatrix = Mat4f::LookAt(position, lookAt, up);
    }
    mRight = Vec3f::Normalize(Vec3f::Cross(mLookAt, mUp));
}

auto Camera::GetProjectionMatrix() const -> const Mat4f&
{
    return mProjectionMatrix;
}

auto Camera::GetViewMatrix() const -> const Mat4f&
{
    return mViewMatrix;
}

auto Camera::Front() const -> const Vec3f&
{
    return mLookAt;
}

auto Camera::Right() const -> const Vec3f&
{
    return mRight;
}

void Camera::Rotate(f32 pitch, f32 yaw)
{
    if (mProjection == CameraProjection::Orthographic)
        return;

    mPitch += pitch;
    mYaw += yaw;

    mPitch = std::clamp(mPitch, -std::numbers::pi_v<f32> / 2.0f + 0.001f, std::numbers::pi_v<f32> / 2.0f - 0.001f);
    mYaw = std::fmod(mYaw, 2.0f * std::numbers::pi_v<f32>);

    mLookAt = Vec3f::Normalize({
        std::cos(mYaw) * std::cos(mPitch),
        std::sin(mPitch),
        std::sin(mYaw) * std::cos(mPitch)
    });
    mRight = Vec3f::Normalize(Vec3f::Cross(mLookAt, mUp));
    mViewMatrix = Mat4f::LookAt(mPosition, mPosition + mLookAt, mUp);
}

void Camera::Translate(const Vec3f& translation)
{
    mPosition += translation;
    mViewMatrix = Mat4f::LookAt(mPosition, mPosition + mLookAt, mUp);
}

void Camera::OnResize(i32 width, i32 height)
{
    if (mProjection == CameraProjection::Orthographic)
    {
        mProjectionMatrix = Mat4f::Orthographic(0.0f, static_cast<f32>(width), static_cast<f32>(height), 0.0f, -1.0f, 1.0f);
    }
    else
    {
        mProjectionMatrix = Mat4f::Perspective(DEG_TO_RAD(mFovDegrees), static_cast<f32>(width) / static_cast<f32>(height), 0.1f, 100.0f);
    }
}