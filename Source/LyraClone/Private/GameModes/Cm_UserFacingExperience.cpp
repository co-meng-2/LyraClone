


#include "GameModes/Cm_UserFacingExperience.h"

#include "CmCU_SessionSubsystem.h"

UCmCU_HostSessionRequest* UCm_UserFacingExperience::CreateHostRequest() const
{
	const FString ExperienceName = ExperienceId.PrimaryAssetName.ToString();

	// 잠깐 한걸음 깊이 생각해보기:
	// - UCommonSession_HostSessionRequest는 UObject로 생성해놓고, 알아서 GC가 된다:
	// - 해당 객체는 현재 프레임에서 사용하기 때문에, GC에 대한 염려가 필요없다: 만약 다음 프레임이든 추가적인 프레임 상에서 해당 객체를 사용할 경우, Lifetime 관리 필요!
	//   - 그렇지 않으면 dangling 난다!
	UCmCU_HostSessionRequest* Result = NewObject<UCmCU_HostSessionRequest>();
	Result->MapId = MapId;
	Result->ExtraArgs.Add(L"Experience", ExperienceName);

	return Result;
}
