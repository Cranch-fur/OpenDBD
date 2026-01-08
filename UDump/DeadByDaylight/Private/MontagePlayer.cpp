#include "MontagePlayer.h"
#include "AnimationMontageDescriptor.h"
#include "UObject/NoExportTypes.h"

class UAnimInstance;
class UAnimMontage;

void UMontagePlayer::Stop(float blendOutTime)
{

}

void UMontagePlayer::SetPlayRate(float PlayRate)
{

}

void UMontagePlayer::SetAnimInstance(UAnimInstance* AnimInstance)
{

}

UAnimMontage* UMontagePlayer::PlayScaledForDuration(FAnimationMontageDescriptor animMontageID, float Duration)
{
	return NULL;
}

UAnimMontage* UMontagePlayer::PlayLoopForDuration(FAnimationMontageDescriptor animMontageID, float Duration, float PlayRate, FName Out)
{
	return NULL;
}

UAnimMontage* UMontagePlayer::Play(FAnimationMontageDescriptor animMontageID, float PlayRate, bool isFollower)
{
	return NULL;
}

void UMontagePlayer::OnMontageEndedInternal(FGuid Guid, UAnimMontage* Montage, bool interrupted)
{

}

void UMontagePlayer::OnMontageBlendingOutInternal(FGuid Guid, UAnimMontage* Montage, bool interrupted)
{

}

void UMontagePlayer::JumpToSectionEnd(FName sectionName)
{

}

void UMontagePlayer::JumpToSection(FName sectionName)
{

}

bool UMontagePlayer::IsPlaying(FAnimationMontageDescriptor montageDescriptor) const
{
	return false;
}

UAnimInstance* UMontagePlayer::GetAnimInstance()
{
	return NULL;
}

UMontagePlayer::UMontagePlayer()
{
	this->_outSectionName = NAME_None;
	this->_timeBeforeOut = -1.000000;
}
