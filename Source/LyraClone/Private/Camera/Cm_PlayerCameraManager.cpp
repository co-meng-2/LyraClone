#include "Camera/Cm_PlayerCameraManager.h"

ACm_PlayerCameraManager::ACm_PlayerCameraManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultFOV = CM_CAMERA_DEFAULT_FOV;
	ViewPitchMin = CM_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = CM_CAMERA_DEFAULT_PITCH_MAX;
}
