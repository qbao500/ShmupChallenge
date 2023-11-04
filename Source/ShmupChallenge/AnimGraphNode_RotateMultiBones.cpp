// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimGraphNode_RotateMultiBones.h"

#include "ShmupFunctionLibrary.h"
#include "Animation/AnimInstanceProxy.h"

FAnimNode_RotateMultiBones::FAnimNode_RotateMultiBones()
{
	
}

void FAnimNode_RotateMultiBones::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	ComponentPose.Initialize(Context);

	OwningActor = Context.AnimInstanceProxy->GetSkelMeshComponent()->GetOwner();

	const UWorld* World = Context.GetAnimBlueprint()->GetWorld();
	if (!World) return;

	bWorldIsGame = World->WorldType == EWorldType::Game;
}

void FAnimNode_RotateMultiBones::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
{
	InitializeBoneReferences(Context.AnimInstanceProxy->GetRequiredBones());
	ComponentPose.CacheBones(Context);
}

void FAnimNode_RotateMultiBones::Update_AnyThread(const FAnimationUpdateContext& Context)
{
	ComponentPose.Update(Context);
}

void FAnimNode_RotateMultiBones::EvaluateComponentSpace_AnyThread(FComponentSpacePoseContext& Output)
{
	// Evaluate the input
	ComponentPose.EvaluateComponentSpace(Output);

	const FBoneContainer& BoneContainer = Output.Pose.GetPose().GetBoneContainer();
	for (FBoneBlendWeight& BoneInfo : TargetBones)
	{
		FBoneReference& BoneToModify = BoneInfo.Bone;

		const FCompactPoseBoneIndex CompactPoseBoneToModify = BoneToModify.GetCompactPoseIndex(BoneContainer);
		FTransform NewBoneTM = Output.Pose.GetComponentSpaceTransform(CompactPoseBoneToModify);
		const FTransform ComponentTransform = Output.AnimInstanceProxy->GetComponentTransform();

		if (RotationMode != BMM_Ignore)
		{
			// Convert to Bone Space.
			FAnimationRuntime::ConvertCSTransformToBoneSpace(ComponentTransform, Output.Pose, NewBoneTM, CompactPoseBoneToModify, RotationSpace);

			const FQuat BoneQuat(Rotation);
			if (RotationMode == BMM_Additive)
			{	
				NewBoneTM.SetRotation(BoneQuat * NewBoneTM.GetRotation() * BoneInfo.Weight);
			}
			else
			{
				NewBoneTM.SetRotation(BoneQuat * BoneInfo.Weight);
			}

			// Convert back to Component Space.
			FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, Output.Pose, NewBoneTM, CompactPoseBoneToModify, RotationSpace);
		}
	}
	
}

// This is just a check to make sure that the bones are present
void FAnimNode_RotateMultiBones::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	for (FBoneBlendWeight& BoneWeight : TargetBones)
	{
		if (BoneWeight.Bone.Initialize(RequiredBones))
		{
			SFL::PrintString(BoneWeight.Bone.BoneName.ToString() + " is initialized");
		}
		else
		{
			SFL::PrintWarning(BoneWeight.Bone.BoneName.ToString() + " is NOT found");
		}
	}
}

FText UAnimGraphNode_RotateMultiBones::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FString Result = GetDescriptiveCompiledName();
	Result += (TitleType == ENodeTitleType::ListView) ? "" : "\n";
	return FText::FromString(Result);
}

FLinearColor UAnimGraphNode_RotateMultiBones::GetNodeTitleColor() const
{
	return FLinearColor(0,12,12,1);
}

FString UAnimGraphNode_RotateMultiBones::GetNodeCategory() const
{
	return TEXT("My Anim Nodes");
}

FText UAnimGraphNode_RotateMultiBones::GetControllerDescription() const
{
	return FText::FromString("Rotate Multi Bones");
}


