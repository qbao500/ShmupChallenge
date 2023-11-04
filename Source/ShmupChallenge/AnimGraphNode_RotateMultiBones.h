// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_Base.h"
#include "BoneControllers/AnimNode_ModifyBone.h"
#include "AnimGraphNode_RotateMultiBones.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FBoneBlendWeight
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FBoneReference Bone;
	UPROPERTY(EditAnywhere)
	float Weight = 1.0f;
};

USTRUCT(BlueprintInternalUseOnly)
struct FAnimNode_RotateMultiBones : public FAnimNode_Base
{
	GENERATED_BODY()

	FAnimNode_RotateMultiBones();
    
	// FAnimNode_Base interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
	virtual void EvaluateComponentSpace_AnyThread(FComponentSpacePoseContext& Output) override;
	// End of FAnimNode_Base interface

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Links")
	FComponentSpacePoseLink ComponentPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skeletal Control")
	TArray<FBoneBlendWeight> TargetBones;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator Rotation = FRotator();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	TEnumAsByte<EBoneModificationMode> RotationMode = BMM_Additive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	TEnumAsByte<EBoneControlSpace> RotationSpace = BCS_WorldSpace;

protected:

	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones);
	
	bool bWorldIsGame = false;
	UPROPERTY()
	AActor* OwningActor = nullptr;
	
};

UCLASS(MinimalAPI)
class UAnimGraphNode_RotateMultiBones : public UAnimGraphNode_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FAnimNode_RotateMultiBones Node;

public:

	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FString GetNodeCategory() const override;
	virtual FText GetControllerDescription() const override;
	// End of UEdGraphNode interface

protected:

};
